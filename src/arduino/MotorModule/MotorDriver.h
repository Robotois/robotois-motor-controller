#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include "Arduino.h"
// #include "TimerOne.h"
// Motor1 pins

#define MOTOR1_PWM_PIN 9
#define MOTOR1_DIR 4
// #define MOTOR1_IN2 4

// Motor2 pins

#define MOTOR2_PWM_PIN 10
#define MOTOR2_DIR 6
// #define MOTOR2_IN2 7

#define DISABLE 12

class MotorDriver {
private:
  /* data */

public:
  MotorDriver ();

  float pwmRatio;

  void begin();
  void motor1Control(uint8_t control);
  void motor1Stop();
  void motor1Clockwise();
  void motor1CounterClockwise();

  void motor2Control(uint8_t control);
  void motor2Stop();
  void motor2Clockwise();
  void motor2CounterClockwise();

  void motorPWM(int16_t pwm, uint8_t motorIdx);
  void motor1PWM(int16_t pwm);
  void motor2PWM(int16_t pwm);
};

#endif
