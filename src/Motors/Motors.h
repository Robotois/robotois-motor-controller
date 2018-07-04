/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Motors.h
 * Author: yovany
 *
 * Created on June 20, 2018, 9:51 PM
 */

#ifndef MOTORS_H
#define MOTORS_H

#include <cstdint>
#include <cstdlib>

#define MOTORS_MOTOR1_CONN 0x01
#define MOTORS_MOTOR2_CONN 0x02
#define MOTORS_CLOCKWISE 0x02
#define MOTORS_COUNTER_CLOCKWISE 0x01
#define MOTORS_STOP 0x00
#define MOTORS_MAX_USER_SPEED (int16_t)100
#define MOTORS_MAX_SPEED (int16_t)1000

// --- Function Registers
#define MOTORS_M1_CONTROL 0x01 // Register for motor 1 control
#define MOTORS_M2_CONTROL 0x02 // Register for motor 2 control
#define MOTORS_M1_PWM 0x03 // Address of the lower byte for the PWM of motor 1
//#define MOTORS_M1_PWM_BYTE2 0x04 // Address of the upper byte for the PWM of motor 1
#define MOTORS_M2_PWM 0x05 // Address of the lower byte for the PWM of motor 2
//#define MOTORS_M2_PWM_BYTE2 0x06 // Address of the upper byte for the PWM of motor 2
#define MOTORS_STATUS 0x10

#define CRC16 0x8005
#define masterHeader 'M'
#define slaveHeader 'M'
#define frameTail1 ';'
#define frameTail2 '\n'

class Motors {
public:
    char rBuf[15]; // - Max length
    char wBuf[15]; // - Max length
    char ackReg;
    uint8_t slave_address;
    uint16_t crc, ackCRC;

    Motors(uint8_t _add = 0x00);
    Motors(const Motors& orig);
    virtual ~Motors();

    void selectModule();

    void motor1Control(uint8_t control, uint8_t force = 0);
    void motor2Control(uint8_t control, uint8_t force = 0);
    void motorsControl(uint8_t m1Control,uint8_t m2Control, uint8_t force = 0);

    void motor1PWM(float pwm, uint8_t force = 0);
    void motor2PWM(float pwm, uint8_t force = 0);
    void motorsPWM(float m1PWM, float m2PWM, uint8_t force = 0);

    void drivePWM(float m1PWM, float m2PWM, uint8_t force = 0);

    void release();
private:
    // The final integer value for PWM, this is the value that goes to the Motor
    // Module.
    int16_t realMotor1PWM, realMotor2PWM;

    int16_t constrainPWM(float value);
    int16_t constrain(int16_t value, int16_t min, int16_t max);
    void bcm_init();
    void bcm_end();
    void i2cWrite(uint8_t byteCount, uint8_t force = 0);

    uint16_t getCRC16(const uint8_t *frame, uint8_t length);
    uint16_t validFrame(const uint8_t *received, uint8_t length);

    void pause();

};

#endif /* MOTORS_H */
