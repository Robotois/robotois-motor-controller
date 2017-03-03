/* 
 * File:   Motors.h
 * Author: yova
 *
 * Created on 26 de enero de 2016, 06:15 PM
 */

#ifndef MOTORS_H
#define	MOTORS_H

#include <stdint.h>

#define MOTORS_MOTOR1_CONN 0x01
#define MOTORS_MOTOR2_CONN 0x02
#define MOTORS_CLOCKWISE 0x02
#define MOTORS_COUNTER_CLOCKWISE 0x01
#define MOTORS_STOP 0x00

// --- Function Registers
#define MOTORS_M1_CONTROL 0x00 // Register for motor 1 control
#define MOTORS_M2_CONTROL 0x01 // Register for motor 2 control
#define MOTORS_M1_PWM 0x02 // Address of the lower byte for the PWM of motor 1
//#define MOTORS_M1_PWM_BYTE2 0x03 // Address of the upper byte for the PWM of motor 1
#define MOTORS_M2_PWM 0x04 // Address of the lower byte for the PWM of motor 2
//#define MOTORS_M2_PWM_BYTE2 0x05 // Address of the upper byte for the PWM of motor 2

class Motors {
public:
    char rBuf[10]; // - Max length
    char wBuf[10]; // - Max length
    uint8_t slave_address;
        
    Motors(uint8_t _add = 0x00);
    Motors(const Motors& orig);
    virtual ~Motors();
    
    void selectModule();
    
    void motor1Control(uint8_t control);
    void motor2Control(uint8_t control);
    void motorsControl(uint8_t m1Control,uint8_t m2Control);

    void motor1PWM(int16_t pwm);
    void motor2PWM(int16_t pwm);
    void motorsPWM(int16_t m1PWM, int16_t m2PWM);

    void drivePWM(int16_t m1PWM, int16_t m2PWM);

//    void motorsSetup25D();
//    void motor1Setup(float gearBox);
//    void motor1Speed(int16_t rpm);
//    void motor2Setup(float gearBox);

//    void motor2Speed(int16_t rpm);   
//    void motorsPWM(float motor1PWM, float mbPWM);
//    void motorsSpeed(int16_t motor1Speed, int16_t mbSpeed);
//    void driveSpeeds(int16_t motor1Speed,int16_t mbSpeed);
//    void GetEncoderTicks(long *LeftEncodetTicks, long *RightEncoderTicks);
//    void ResetEncoderCounters();

//    void motor1BasicTest();
//    void motorsBasicTest();
//    void motorsBasicSpeedTest();
//    void motor1FullSpeedTest();
//    void motor1PIDResponse();
//
//    void mbBasicTest();
//    void mbFullSpeedTest();
//    void mbPIDResponse();
//
    void configPause();
    void release();
private:
    uint8_t leftMotor;
    uint8_t rightMotor;
//    int16_t motor1PWM, motor2PWM;
    
    void constrain(int16_t *value,int16_t min, int16_t max);
    void bcm_init();
    void bcm_end();

};

#endif	/* MOTORS_H */

