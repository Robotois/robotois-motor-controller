/*
 * File:   Motors.cpp
 * Author: yova
 *
 * Created on 26 de enero de 2016, 06:15 PM
 */

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <stdlib.h>
#include "Motors.h"
#include "../../Libraries/Timer/AccurateTiming.h"
#include <bcm2835.h>

using namespace std;

Motors::Motors(uint8_t _add) {
    if(_add > 1){
        printf("Wrong slave address for the Digital IO Module...\n");
        return;
    }
    slave_address = 0x10 | _add; // - Direccion por default
    bcm_init();

    leftMotor = MOTORS_MOTOR2_CONN;
    rightMotor = MOTORS_MOTOR1_CONN;

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
    selectModule();
    wBuf[0] = MOTORS_M1_CONTROL; // - Direccion del registro
    wBuf[1] = control; // - Contenido del registro
    bcm2835_i2c_write(wBuf, 2);
}

/**
 * Establece el PWM del motor A.
 * @param pwm => Valor de PWM en un rango de 0-100, con un digito decimal,
 * con lo que se obtienen 1000 velocidades.
 */
void Motors::motor1PWM(int16_t pwm){
    selectModule();

    constrain(&pwm,-1000,1000);
    wBuf[0] = MOTORS_M1_PWM;
    wBuf[1] = (uint8_t)pwm;
    wBuf[2] = (uint8_t)(pwm >> 8);

    bcm2835_i2c_write(wBuf, 3);
}

/**
 * Establece el sentido de giro y permite detener el motor B.
 * @param control => 0 --> Stop, 1 --> counter_clockwise, 2 --> clockwise.
 */
void Motors::motor2Control(uint8_t control){
    selectModule();
    wBuf[0] = MOTORS_M2_CONTROL; // - Direccion del registro
    wBuf[1] = control; // - Contenido del registro
    bcm2835_i2c_write(wBuf, 2);
}

void Motors::motorsControl(uint8_t m1Control, uint8_t m2Control){
    selectModule();
    wBuf[0] = MOTORS_M1_CONTROL; // - Direccion del registro
    wBuf[1] = m1Control; // - Contenido del registro
    wBuf[2] = m2Control; // - Contenido del registro
    bcm2835_i2c_write(wBuf,3);
}

/**
 * Establece el PWM del motor B.
 * @param pwm => Valor de PWM en un rango de 0-100, con un digito decimal,
 * con lo que se obtienen 1000 velocidades.
 */
void Motors::motor2PWM(int16_t pwm){
    selectModule();

    constrain(&pwm,-1000,1000);
    wBuf[0] = MOTORS_M2_PWM;
    wBuf[1] = (uint8_t)pwm;
    wBuf[2] = (uint8_t)(pwm >> 8);

    bcm2835_i2c_write(wBuf, 3);
}

void Motors::motorsPWM(int16_t m1PWM, int16_t m2PWM){
    selectModule();

    constrain(&m1PWM,-1000,1000);
    constrain(&m2PWM,-1000,1000);

    wBuf[0] = MOTORS_M1_PWM;
    wBuf[1] = (uint8_t)m1PWM;
    wBuf[2] = (uint8_t)(m1PWM >> 8);
    wBuf[3] = (uint8_t)(m2PWM);
    wBuf[4] = (uint8_t)(m2PWM >> 8);

    bcm2835_i2c_write(wBuf, 5);
}

/**
 * Motor A is the Left motor on the robot, Motor B is the Right one
 * @param maPWM
 * @param mbPWM
 */
void Motors::drivePWM(int16_t _leftPWM, int16_t _rightPWM){
    // We stablish a negative relationship with the PWM of the LEFT motor, this way
    // we obtain a proper behavior for a robot:
    // - Positive PWM => forward motion (CCW)
    // - negative PWM => backward motion (CW)
    if(leftMotor == MOTORS_MOTOR1_CONN and rightMotor == MOTORS_MOTOR2_CONN){
        motorsPWM(-_leftPWM, _rightPWM);
    }else{
        if(leftMotor == MOTORS_MOTOR2_CONN and rightMotor == MOTORS_MOTOR1_CONN){
            motorsPWM(_rightPWM,-_leftPWM);
        }else
            printf("MotorModule Error!!... Wrong motor configuration\n");
    }
}

void Motors::configPause(){
    uDelay(20);
}

void Motors::constrain(int16_t* value, int16_t min, int16_t max){
    if(*value > max){
        *value = max;
        return;
    }

    if(*value < min){
        *value = min;
        return;
    }
}

void Motors::bcm_init(){
    // - 400kHz aproximadamente...
    uint16_t clk_div = BCM2835_I2C_CLOCK_DIVIDER_626;
    // - La direccion del esclavo se establece en cada modulo

    if (!bcm2835_init()){
        printf("BCM2835 Error!!...\n");
        exit(1);
    }

    bcm2835_i2c_begin();

    bcm2835_i2c_setClockDivider(clk_div);
}

void Motors::bcm_end(){
    bcm2835_i2c_end();
    bcm2835_close();
}

void Motors::release(){
    motorsControl(MOTORS_STOP,MOTORS_STOP);
    printf("[MotorsModule] => Released\n");
}
