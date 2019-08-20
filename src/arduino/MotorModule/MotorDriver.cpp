#include "MotorDriver.h"

MotorDriver::MotorDriver(){
}

void MotorDriver::begin() {
  // These pins are used to control the direction of rotation for the motors,
  // Clockwise, Counterclockwise and Stop
  pinMode(MOTOR1_DIR, OUTPUT);
  pinMode(MOTOR2_DIR, OUTPUT);
  motor1Stop();
  motor2Stop();

  // Initialization of the PWM pins and Timer 1 to manage PWM signal
  pinMode(DISABLE, OUTPUT);
  pinMode(MOTOR1_PWM_PIN, OUTPUT);
  pinMode(MOTOR2_PWM_PIN, OUTPUT);
  digitalWrite(DISABLE, LOW); // StandBy pin to High

  // // - Non inverted mode, OC1A and OC1B as PWM output
	// // - Fast Mode, top in ICR1
	TCCR1A = 0xA2;
	// // - [Fast PWM,Top ICR1, non inverted mode],[no prescaling]
	TCCR1B = 0x19;
	TCCR1C = 0x00;
  // It should be considered that the crystal frequency is 16MHz
  // The maximum value for the counter is relative to a time period of 5x10^-5 s,
  // which is the period of the pwm signal with a frequency of 20kHz.
  // The PWM values will be in the range of 0 - 800, where 800 => 100 %
  ICR1 = 800;
  // Initial PWM of 0%
  OCR1A = 0;
  OCR1B = 0;

  pwmRatio = 800.0f/1000.0f;
}

// for simplicity the rotation and speed of the motors are controlled using
// PWM values in a range of (-1000) - 1000, with negative values the motors
// rotate in Counterclockwise, with positive values the motors rotate Clockwise
// and a PWM value of cero the PWM signal will be LOW
void MotorDriver::motorPWM(int16_t pwm, uint8_t motorIdx) {
  switch (motorIdx) {
    case 0:
      motor1PWM(pwm);
      break;
    case 1:
      motor2PWM(pwm);
      break;
    default:
      break;
  }
}

void MotorDriver::motor1PWM(int16_t pwm) {
  int16_t realPWM = pwmRatio * pwm;
  if (realPWM < 0) { // - Negative PWM => Counterclockwise
    motor1CounterClockwise();
      OCR1A = (uint16_t)(-realPWM);
  } else {
    motor1Clockwise();
    OCR1A = (uint16_t) realPWM;
  }
}

void MotorDriver::motor2PWM(int16_t pwm) {
  int16_t realPWM = pwmRatio * pwm;
  if (realPWM < 0) { // - Negative PWM => Counterclockwise
    motor2CounterClockwise();
    OCR1B = (uint16_t)(-realPWM);
  } else {
    motor2Clockwise();
    OCR1B = (uint16_t) realPWM;
  }
}

/**********************
BASIC CONTROL FUNCTIONS
 **********************/
void MotorDriver::motor1Control(uint8_t control){
  switch (control) {
    case 0x00:
      motor1PWM(0);
      motor1Stop();
      break;
    case 0x01:
      motor1CounterClockwise();
      break;
    case 0x02:
      motor1Clockwise();
      break;
  }
}

void MotorDriver::motor1Clockwise(){
  digitalWrite(MOTOR1_DIR, HIGH);
}

void MotorDriver::motor1CounterClockwise(){
  digitalWrite(MOTOR1_DIR, LOW);
}

void MotorDriver::motor1Stop(){
  OCR1A = 0;
}

void MotorDriver::motor2Control(uint8_t control){
  switch (control) {
    case 0x00:
      motor2PWM(0);
      motor2Stop();
      break;
    case 0x01:
      motor2CounterClockwise();
      break;
    case 0x02:
      motor2Clockwise();
      break;
  }
}

void MotorDriver::motor2Clockwise(){
  digitalWrite(MOTOR2_DIR, HIGH);
}

void MotorDriver::motor2CounterClockwise(){
  digitalWrite(MOTOR2_DIR, LOW);
}

void MotorDriver::motor2Stop(){
  OCR1A = 0;
}
