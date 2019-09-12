#include <Arduino.h>
#include <Wire.h>
#include "MotorDriver.h"
#include "frame_validation.h"
#include "PIDs.h"
#include "settings.h"

#define SLAVE_ADDRESS 0x10
// #define REG_MAP_SIZE 0x20
#define MAX_BYTES 10 // Max of 10 bytes
#define frameTail 0xFF

uint8_t receivedBytes[MAX_BYTES];
uint8_t response[MAX_BYTES]; // CRC16 + Tail
bool statusCheck = false;
uint8_t frameSize = 0;
MotorDriver motors;

// String serialLog;
String command;

int pwm = 100, steps = 100;

settings_type settings;

void setup() {
  Serial.begin(115200);
  settings = readSettings();

  motors.begin();
  // Using pin A2 you can connect up to two motor modules, the default value for
  // A2 is LOW, if the jumper is placed in the two pin connector then the value
  // for A2 is HIGH, this means that the address for the current module will be
  // 0x11
  Wire.begin( SLAVE_ADDRESS | digitalRead(A2) );
  pinMode(13, OUTPUT);

  Wire.onReceive(receiveHandler);
  Wire.onRequest(statusRequest);
  response[2] = frameTail;
  response[3] = frameTail;
  pidSetup(&motors, settings);
  // setMotorSpeed(150, 0);
}

void loop() {
  // motors.motor1PWM(250);
  pidControl();
}

/**
 * Get the byte sequence received from the master, in which the first byte corresponds
 * to the function/register address, and the following bytes are the data or params
 * to be written.
 * For the motor module we are considering the data is received in Burst Write Operation:
 * [initialRegisterAddress] -> [Data0] -> [Data1] -> [Data2] ... -> [DataX]
 * @param byteCount
 */
void receiveHandler(int byteCount) {
  if (byteCount < 4) {
    receivedBytes[0] = Wire.read();
    // Serial.println("I2C["+String(byteCount)+"]: " + String(receivedBytes[0], HEX));
    if (receivedBytes[0] == 0x10) {
      statusCheck = true;
    }
    return;
  }
  frameSize = 0;
  // serialLog = "I2C["+String(byteCount)+"]: ";
  for (uint8_t i = 0; i < byteCount; i++) {
    if(i < MAX_BYTES) {
      receivedBytes[i] = Wire.read();
      // serialLog += "-" + String(receivedBytes[i],HEX);
      if (
        i >= 4 &&
        receivedBytes[i] == frameTail &&
        receivedBytes[i-1] == frameTail &&
        frameSize == 0
      ) {
        frameSize = i - 1;
      }
    } else {
      Wire.read();
    }
  }
  // Serial.println(serialLog + " Frame Size:" + frameSize);
  // Serial.println(serialLog);
  if (frameSize == 0) {
    response[0] = 0;
    response[1] = 0;
    return;
  }
  uint16_t crc = validFrame(receivedBytes, frameSize);
  // Serial.println("CRC: " + String(crc, HEX));
  response[0] = (uint8_t) (crc >> 8);
  response[1] = (uint8_t) (crc & 0xFF);
  if (crc == 0) {
    return;
  }

  // Function mapping, we are progressively increasing the register address on each
  // received data, with this we will have better control of the function to call
  uint8_t functionAdd = receivedBytes[0];
  for (size_t i = 1; i < (frameSize - 2); i++) {
    switch (functionAdd) {
      case 0x01: // Control function for motor1
        motors.motor1Control(receivedBytes[i]);
        break;
      case 0x02:
        motors.motor2Control(receivedBytes[i]);
        break;
      // case 0x03: // Motor1 PWM[lower byte]
      case 0x04: // Motor1 PWM[upper byte], the pwm will be set only if the two bytes were received
        disablePid(0);
        motors.motor1PWM( ((int16_t) (receivedBytes[i] << 8)) | receivedBytes[i-1] );
        break;
      // case 0x05: // Motor2 PWM[lower byte]
      case 0x06: // Motor2 PWM[upper byte], the pwm will be set only if the two bytes were received
        disablePid(1);
        motors.motor2PWM( ((int16_t) (receivedBytes[i] << 8)) | receivedBytes[i-1] );
        break;
      // case 0x07: // Motor2 PWM[lower byte]
      case 0x08: // Motor1 speed[upper byte], the pwm will be set only if the two bytes were received
        setMotorSpeed( ((int16_t) (receivedBytes[i] << 8)) | receivedBytes[i-1], 0);
        break;
      // case 0x09: // Motor2 PWM[lower byte]
      case 0x0A: // Motor2 PWM[upper byte], the pwm will be set only if the two bytes were received
        setMotorSpeed( ((int16_t) (receivedBytes[i] << 8)) | receivedBytes[i-1], 1);
        break;
    }
    functionAdd++;
  }
}

void statusRequest() {
  if (statusCheck) {
    // Serial.println("StatusRequest");
    Wire.write(response, 2);
    response[0] = 0;
    response[1] = 0;
    statusCheck = false;
  }
}

void serialEvent() {
  readSerial();
}