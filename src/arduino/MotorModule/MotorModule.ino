#include <Arduino.h>
#include "MotorDriver.h"
#include <Wire.h>

#define SLAVE_ADDRESS 0x10
#define REG_MAP_SIZE 0x20
#define MAX_BYTES 0x10 // Max of 10 bytes

// #define SERIAL_LOG

uint8_t receivedBytes[MAX_BYTES];
// uint8_t registerMap[REG_MAP_SIZE];
MotorDriver motors;

// int outValue = 0;
String serialLog;

void setup() {
  // #ifdef SERIAL_LOG
  //   Serial.begin(115200);
  //   Serial.println("Motor Module logs:");
  // #endif
  motors.begin();

  // Using pin A2 you can connect up to two motor modules, the default value for
  // A2 is LOW, if the jumper is placed in the two pin connector then the value
  // for A2 is HIGH, this means that the address for the current module will be
  // 0x11
  // Wire.setClock(400000);
  Wire.begin( SLAVE_ADDRESS | digitalRead(A2) );

  Wire.onReceive(receiveHandler);
}

void loop() {

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
  // #ifdef SERIAL_LOG
  //   serialLog = "I2C["+String(byteCount)+"]: ";
  // #endif

  for (uint8_t i = 0; i < byteCount; i++) {
    if ( i < MAX_BYTES ) {
      receivedBytes[i] = Wire.read();
      // #ifdef SERIAL_LOG
      //   serialLog += String(receivedBytes[i],HEX) + " => ";
      // #endif
    } else { // throw away the excess bytes
      Wire.read();
    }
  }

  // #ifdef SERIAL_LOG
  //   Serial.println(serialLog);
  // #endif

  // Function mapping, we are progressively increasing the register address on each
  // received data, with this we will have better control of the function to call
  uint8_t regAdd = receivedBytes[0];
  for (size_t i = 1; i < byteCount; i++) {
    switch (regAdd) {
      case 0x00: // Control function for motor1
        motors.motor1Control(receivedBytes[i]);
        break;
      case 0x01:
        motors.motor2Control(receivedBytes[i]);
        break;
      // case 0x02: // Motor1 PWM[lower byte]
      case 0x03: // Motor1 PWM[upper byte], the pwm will be set only if the two bytes were received
        motors.motor1PWM( ((int16_t) (receivedBytes[i] << 8)) | receivedBytes[i-1] );
        break;
      // case 0x04: // Motor2 PWM[lower byte]
      case 0x05: // Motor2 PWM[upper byte], the pwm will be set only if the two bytes were received
        motors.motor2PWM( ((int16_t) (receivedBytes[i] << 8)) | receivedBytes[i-1] );
        break;
    }
    regAdd++;
  }
}
