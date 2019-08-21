// #include <stdint.h>
#ifndef PIDS
#define PIDS
#include "Arduino.h"
#include "MotorDriver.h"

struct settings_t
{
  int maxRPM; // Max RPM ouput
  int cpr; // Encoder steps per revolutions
  int gear; // Gearbox relation 1:30
  float kp;
  float kd;
  float ki;
} settings;

MotorDriver *motorsPtr;

uint8_t encoderPins[2][2] = {
  {2, 3},
  {14, 15},
};
uint8_t motorCount = 2;

int boundValue(int value, int min, int max) {
  if(value < min) {
    return min;
  }
  if(value > max) {
    return max;
  }
  return value;
}

/*
  Motors
*/
const int lookUpTable[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
volatile int encCount[4] = {0, 0, 0, 0}, prevEncCount[4] = {0, 0, 0, 0};
volatile uint8_t prevEnc[4] = {0, 0, 0, 0};
bool pidEnable[2] = {false, false};
uint8_t currentEnc1;
uint8_t currentEnc2;

void encThick(uint8_t currentEnc, uint8_t motorNumber) {
    uint8_t lookAdd = (prevEnc[motorNumber] <<2) | currentEnc;
    prevEnc[motorNumber] = currentEnc;
    encCount[motorNumber] += lookUpTable[lookAdd];
}

void encInterrupt() {
  if (pidEnable[0]) {
    currentEnc1 = digitalRead(encoderPins[0][0]) << 1 | digitalRead(encoderPins[0][1]);
    if (prevEnc[0] != currentEnc1) {
      encThick(currentEnc1, 0);
    }
  }
  if (pidEnable[1]) {
    currentEnc2 = digitalRead(encoderPins[1][0]) << 1 | digitalRead(encoderPins[1][1]);
    if (prevEnc[1] != currentEnc2) {
      encThick(currentEnc2, 1);
    }
  }
}

/*
  PID Functions
*/
volatile uint8_t pidFlag = 0;
float speedRatio = 0;

void pidSetup(MotorDriver *motors) {
  motorsPtr = motors;
  encoderPins[0][0] = 2;
  encoderPins[1][0] = 3;
  encoderPins[0][1] = 14;
  encoderPins[1][1] = 15;
  speedRatio = (float)(settings.cpr * settings.gear) / (6000.0f);
  //set timer2 interrupt at 8kHz
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 8khz increments
  OCR2A = 156;// = (16*10^6) / (8000*8) - 1 (must be <256)
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS21 bit for 1024 prescaler
  TCCR2B |= (1 << CS22 | 1 << CS21 | 1 << CS20);   
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);
  sei();//allow interrupts
}

ISR(TIMER2_COMPA_vect) {//timer1 interrupt 8kHz toggles pin 9
  pidFlag = 1;
}

uint8_t idx = 0;
volatile int encTarget[2] = {0, 0};
float controlPWM;
float currentError, prevError[2] = {0, 0};
float integral[2] = {0, 0};
int currentPWM[2], prevPWM[2] = {0, 0};

void pidControl() {
  encInterrupt();

  if(pidFlag == 0) {
    return;
  }

  for(idx = 0; idx < motorCount; idx++) {
    if (pidEnable[idx]) {
      currentError = encTarget[idx] - encCount[idx];
      integral[idx] += currentError;
      controlPWM = currentError * settings.kp +
        integral[idx] * settings.ki +
        (currentError - prevError[idx]) * settings.kd;

      currentPWM[idx] += 0.5*controlPWM;
      currentPWM[idx] = boundValue(currentPWM[idx], -1000, 1000);

      prevError[idx] = currentError;
      prevEncCount[idx] = encCount[idx];
      // if (idx == 0) {
      //   Serial.println("Count: " + String(encCount[idx], DEC));
      // }
      encCount[0] = 0;
      prevPWM[idx] = currentPWM[idx];
      motorsPtr->motorPWM(currentPWM[idx], idx);
    }
  }
  pidFlag = 0;
}

void setMotorSpeed(int speed, uint8_t motorIdx) {
  int boundSpeed = boundValue(speed, -settings.maxRPM, settings.maxRPM);
  encTarget[motorIdx] = (int) (boundSpeed * speedRatio);
  pidEnable[motorIdx] = true;
  Serial.println("EncTarget: " + String(encTarget[motorIdx], DEC));
}

void disablePid(uint8_t idx) {
  pidEnable[idx] = false;
  encCount[idx] = 0;
}
#endif // PIDS
