/*
 * File:   Motors.cpp
 * Author: yova
 *
 * Created on 26 de enero de 2016, 06:15 PM
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
        printf("Wrong slave address for the Digital IO Module...\n");
        return;
    }
    slave_address = 0x10 | _add; // - Direccion por default
    for (uint8_t i = 0; i < 15; i++) {
      wBuf[i] = 0xFF;
    }

    bcm_init();
    leftMotor = MOTORS_MOTOR2_CONN;
    rightMotor = MOTORS_MOTOR1_CONN;

    motorsPWM(0,0);
    motorsControl(MOTORS_STOP,MOTORS_STOP);
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
void Motors::motor1Control(uint8_t control){
    // selectModule();
    wBuf[0] = (uint8_t) MOTORS_M1_CONTROL; // - Direccion del registro
    wBuf[1] = (uint8_t) control; // - Contenido del registro
    i2cWrite(2);
}

/**
 * Establece el PWM del motor A.
 * @param pwm => Valor de PWM en un rango de 0-100, con un digito decimal,
 * con lo que se obtienen 1000 velocidades.
 */
void Motors::motor1PWM(float pwm){
    // selectModule();

    realMotor1PWM = constrainPWM(pwm);

    wBuf[0] = (uint8_t) MOTORS_M1_PWM;
    wBuf[1] = (uint8_t)realMotor1PWM;
    wBuf[2] = (uint8_t)(realMotor1PWM >> 8);

    i2cWrite(3);
}

/**
 * Establece el sentido de giro y permite detener el motor B.
 * @param control => 0 --> Stop, 1 --> counter_clockwise, 2 --> clockwise.
 */
void Motors::motor2Control(uint8_t control){
    // selectModule();
    wBuf[0] = (uint8_t) MOTORS_M2_CONTROL; // - Direccion del registro
    wBuf[1] = (uint8_t) control; // - Contenido del registro
    i2cWrite(2);
}

/**
 * Establece el PWM del motor B.
 * @param pwm => Valor de PWM en un rango de 0-100, con un digito decimal,
 * con lo que se obtienen 1000 velocidades.
 */
void Motors::motor2PWM(float pwm){
    // selectModule();

    realMotor2PWM = constrainPWM(pwm);

    wBuf[0] = (uint8_t) MOTORS_M2_PWM;
    wBuf[1] = (uint8_t)realMotor2PWM;
    wBuf[2] = (uint8_t)(realMotor2PWM >> 8);

    i2cWrite(3);
}

void Motors::motorsControl(uint8_t m1Control, uint8_t m2Control){
    // selectModule();

    wBuf[0] = (uint8_t) MOTORS_M1_CONTROL; // - Direccion del registro
    wBuf[1] = (uint8_t) m1Control; // - Contenido del registro
    wBuf[2] = (uint8_t) m2Control; // - Contenido del registro
    i2cWrite(3);
}

void Motors::motorsPWM(float m1PWM, float m2PWM){
    // selectModule();

    realMotor1PWM = constrainPWM(m1PWM);
    realMotor2PWM = constrainPWM(m2PWM);

    wBuf[0] = (uint8_t) MOTORS_M1_PWM;
    wBuf[1] = (uint8_t)realMotor1PWM;
    wBuf[2] = (uint8_t)(realMotor1PWM >> 8);
    wBuf[3] = (uint8_t)(realMotor2PWM);
    wBuf[4] = (uint8_t)(realMotor2PWM >> 8);

    i2cWrite(5);
}

/**
 * Motor A is the Left motor on the robot, Motor B is the Right one
 * @param maPWM
 * @param mbPWM
 */
void Motors::drivePWM(float m1PWM, float m2PWM){
    // We stablish a negative relationship with the PWM of the LEFT motor, this way
    // we obtain a proper behavior for a robot:
    // - Positive PWM => forward motion (CCW)
    // - negative PWM => backward motion (CW)
    motorsPWM(-m1PWM, m2PWM);
}

void Motors::i2cWrite(uint8_t byteCount){
  // for (uint8_t i = 0; i < byteCount; i++) {
  //   wBuf[i+2] = writeBuf[i];
  // }
  // wBuf[byteCount+2] = ';';
  selectModule();
  bcm2835_i2c_write(wBuf, byteCount);
  // printf("i2cWrite: %s, byteCount: %d\n", wBuf, byteCount);
  pause();
}

void Motors::pause(){
    std::this_thread::sleep_for(std::chrono::microseconds(1000));
    //  acknowledge: "m,ok;"
    // uint8_t result = bcm2835_i2c_read(ackRes, 5);
    // printf("[MotorModule] => Response: %s\n", ackRes);
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
    bcm2835_i2c_setSlaveAddress(slave_address);
    bcm2835_i2c_write(wBuf, 2);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    bcm2835_i2c_write(wBuf, 2);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void Motors::bcm_end(){
    bcm2835_i2c_end();
    bcm2835_close();
}

void Motors::release(){
    motorsControl(MOTORS_STOP,MOTORS_STOP);
    bcm_end();
    printf("[MotorsModule] => Released\n");
}
