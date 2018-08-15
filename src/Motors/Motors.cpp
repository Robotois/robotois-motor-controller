/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Motors.cpp
 * Author: yovany
 *
 * Created on June 20, 2018, 9:51 PM
 */

#include <iostream>
#include <cstdint>
#include <cmath>
#include <thread>
#include <chrono>
#include <string.h>

#include "Motors.h"
#include <bcm2835.h>

using namespace std;

Motors::Motors(uint8_t _add) {
    if(_add > 1){
        printf("Wrong slave address for the Motors Module...\n");
        return;
    }
    slave_address = 0x10 | _add; // - Direccion por default
//    for (uint8_t i = 0; i < 15; i++) {
//      wBuf[i] = 0xFF;
//    }

    bcm_init();
//    leftMotor = MOTORS_MOTOR2_CONN;
//    rightMotor = MOTORS_MOTOR1_CONN;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    motorsControl(MOTORS_STOP,MOTORS_STOP, 1);
}

Motors::Motors(const Motors& orig) {
}

Motors::~Motors() {
    bcm_end();
}

/**
 * Reestablece la direccion del esclavo en l puerto i2c. Se debe llamar antes de
 * iniciar una transaccion o serie de transacciones hacia el modulo de motores.
 */
void Motors::selectModule(){
    bcm2835_i2c_setSlaveAddress(slave_address);
}

/**
 * Establece el sentido de giro y permite detener el motor A.
 * @param control => 0 --> Stop, 1 --> counter_clockwise, 2 --> clockwise.
 */
void Motors::motor1Control(uint8_t control, uint8_t force){
    // selectModule();
    wBuf[0] = (uint8_t) MOTORS_M1_CONTROL; // - Direccion del registro
    wBuf[1] = (uint8_t) control; // - Contenido del registro
    i2cWrite(2, force);
}

/**
 * Establece el PWM del motor A.
 * @param pwm => Valor de PWM en un rango de 0-100, con un digito decimal,
 * con lo que se obtienen 1000 velocidades.
 */
void Motors::motor1PWM(float pwm, uint8_t force){
    // selectModule();

    realMotor1PWM = constrainPWM(pwm);

    wBuf[0] = (uint8_t) MOTORS_M1_PWM;
    wBuf[1] = (uint8_t)realMotor1PWM;
    wBuf[2] = (uint8_t)(realMotor1PWM >> 8);

    i2cWrite(3, force);
}

/**
 * Establece el sentido de giro y permite detener el motor B.
 * @param control => 0 --> Stop, 1 --> counter_clockwise, 2 --> clockwise.
 */
void Motors::motor2Control(uint8_t control, uint8_t force){
    // selectModule();
    wBuf[0] = (uint8_t) MOTORS_M2_CONTROL; // - Direccion del registro
    wBuf[1] = (uint8_t) control; // - Contenido del registro
    i2cWrite(2, force);
}

/**
 * Establece el PWM del motor B.
 * @param pwm => Valor de PWM en un rango de 0-100, con un digito decimal,
 * con lo que se obtienen 1000 velocidades.
 */
void Motors::motor2PWM(float pwm, uint8_t force){
    // selectModule();

    realMotor2PWM = constrainPWM(pwm);

    wBuf[0] = (uint8_t) MOTORS_M2_PWM;
    wBuf[1] = (uint8_t)realMotor2PWM;
    wBuf[2] = (uint8_t)(realMotor2PWM >> 8);

    i2cWrite(3, force);
}

void Motors::motorsControl(uint8_t m1Control, uint8_t m2Control, uint8_t force){
    // selectModule();

    wBuf[0] = (uint8_t) MOTORS_M1_CONTROL; // - Direccion del registro
    wBuf[1] = (uint8_t) m1Control; // - Contenido del registro
    wBuf[2] = (uint8_t) m2Control; // - Contenido del registro
    i2cWrite(3);
}

void Motors::motorsPWM(float m1PWM, float m2PWM, uint8_t force){
    // selectModule();

    realMotor1PWM = constrainPWM(m1PWM);
    realMotor2PWM = constrainPWM(m2PWM);

    wBuf[0] = (uint8_t) MOTORS_M1_PWM;
    wBuf[1] = (uint8_t)realMotor1PWM;
    wBuf[2] = (uint8_t)(realMotor1PWM >> 8);
    wBuf[3] = (uint8_t)(realMotor2PWM);
    wBuf[4] = (uint8_t)(realMotor2PWM >> 8);

    i2cWrite(5, force);
}

/**
 * Motor A is the Left motor on the robot, Motor B is the Right one
 * @param maPWM
 * @param mbPWM
 */
void Motors::drivePWM(float m1PWM, float m2PWM, uint8_t force){
    // We stablish a negative relationship with the PWM of the LEFT motor, this way
    // we obtain a proper behavior for a robot:
    // - Positive PWM => forward motion (CCW)
    // - negative PWM => backward motion (CW)
    motorsPWM(-m1PWM, m2PWM, force);
}

void Motors::i2cWrite(uint8_t byteCount, uint8_t force) {
    crc = getCRC16((uint8_t *)wBuf, byteCount);
    wBuf[byteCount] = (uint8_t) (crc >> 8);
    wBuf[byteCount+1] = (uint8_t) (crc & 0xFF);
    wBuf[byteCount+2] = frameTail;
    wBuf[byteCount+3] = frameTail;

//    printf("Data Frame: ");
//    for(uint8_t i = 0; i < byteCount+4; i++) {
//        printf("-%x", wBuf[i]);
//    }
//    printf("\n");

    selectModule();
    bcm2835_i2c_write(wBuf, byteCount+4);
    pause();
    wBuf[0] = MOTORS_STATUS;
    selectModule();
    uint8_t res = bcm2835_i2c_write_read_rs(wBuf, 1, rBuf, 2);

//    printf("Response Frame: [%x] ", res);
//    for(uint8_t i = 0; i < 2; i++) {
//        printf("-%x", rBuf[i]);
//    }
//    printf("\n");

    ackCRC = (uint16_t)(rBuf[0] << 8) | rBuf[1];
//    printf(": %x\n", ackCRC);
    if (force == 1 && res != 0x00 && ackCRC != crc) {
        printf("[Motor Module] => I2C response error\n");
        pause();
        i2cWrite(byteCount, force);
    }
//    else {
//        printf("[Motor Module] => I2C Response OK!\n");
//    }
}

void Motors::pause(){
    std::this_thread::sleep_for(std::chrono::microseconds(3000));
//    std::this_thread::sleep_for(std::chrono::microseconds(500));
}

int16_t Motors::constrainPWM(float value){
    int16_t roundedValue = (int16_t)std::round(value*10.0f);
    roundedValue = constrain(roundedValue, -MOTORS_MAX_SPEED, MOTORS_MAX_SPEED);
    return roundedValue;
}

int16_t Motors::constrain(int16_t value, int16_t min, int16_t max){
    if(value > max){
        return max;
    }

    if(value < min){
        return min;
    }

    return value;
}

void Motors::bcm_init(){
    // - 400kHz aproximadamente...
    uint16_t clk_div = BCM2835_I2C_CLOCK_DIVIDER_2500;
    // - La direccion del esclavo se establece en cada modulo

    if (!bcm2835_init()){
        printf("BCM2835 Error!!...\n");
        exit(1);
    }

    bcm2835_i2c_begin();

    bcm2835_i2c_setClockDivider(clk_div);
//    bcm2835_i2c_setSlaveAddress(slave_address);
//    bcm2835_i2c_write(wBuf, 2);
//    std::this_thread::sleep_for(std::chrono::milliseconds(500));
//    bcm2835_i2c_write(wBuf, 2);
//    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void Motors::bcm_end(){
    bcm2835_i2c_end();
    bcm2835_close();
}

void Motors::release(){
    motorsControl(MOTORS_STOP,MOTORS_STOP, 1);
//    bcm_end();
    printf("[MotorsModule] => Released\n");
}

static const uint16_t wCRCTable[] = {
0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040 };

uint16_t Motors::getCRC16(const uint8_t *frame, uint8_t length) {
    uint8_t nTemp;
    uint16_t wCRCWord = 0x0000;
    uint8_t count = 0;
   while (count < length) {
      nTemp = frame[count] ^ wCRCWord;
      wCRCWord >>= 8;
      wCRCWord ^= wCRCTable[nTemp];
      count++;
   }
   return wCRCWord;
}

uint16_t Motors::validFrame(const uint8_t* received, uint8_t length) {
    uint16_t frameCRC = ((uint16_t) (received[length - 1] << 8)) | received[length - 2];
    uint16_t computedCRC = getCRC16(received, length - 2);
    if (frameCRC == computedCRC) {
            return computedCRC;
    }
    return 0;
}
