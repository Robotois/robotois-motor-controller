#ifndef SETTINGS
#define SETTINGS
#include <avr/eeprom.h>

struct settings_type
{
  int maxRPM; // Max RPM ouput
  int cpr; // Encoder steps per revolutions
  int gear; // Gearbox relation 1:30
  float kp;
  float kd;
  float ki;
} settings;

settings_type readSettings() {
  Serial.println("Welcome to Motors Setup CLI\nTo execute a function enter the request as follows: \"#func,#param1,#param2...\" and ENTER");
  Serial.println("MAKE SURE you connect the motor using the proper pinout\n");
  Serial.println("1: settings(maxRPM,cpr,gear,kp,kd,ki) => output RPM, encoders Counts Per Revolution, Gearbox output");
  Serial.println("2: resetValues() => settings(300,44,30,1,2,0.001)");
  Serial.println("3: setMotorSpeed(rpmSpeed)");
  eeprom_read_block((void*)&settings, (void*)0, sizeof(settings));
  Serial.println(
    "Settings: \nRPM:" + String(settings.maxRPM, DEC) + "\n" +
    "cpr:" + String(settings.cpr, DEC) + "\n" +
    "gear:" + String(settings.gear, DEC) + "\n" +
    "kp:" + String(settings.kp, DEC) + "\n" +
    "kd:" + String(settings.kd, DEC) + "\n" +
    "ki:" + String(settings.ki, DEC) + "\n"
  );
  return settings;
}

void updateSettingValues(
  int maxRPM,
  int cpr,
  int gear,
  float kp,
  float kd,
  float ki
) {
  settings.maxRPM = maxRPM;
  settings.cpr = cpr;
  settings.gear = gear;
  settings.kp = kp;
  settings.kd = kd;
  settings.ki = ki;
  eeprom_write_block((const void*)&settings, (void*)0, sizeof(settings));
  Serial.println("Settings Update => Success!");
}

void resetMemValues() {
  updateSettingValues(
    (int) 300,
    (int) 44,
    (int) 30,
    (float) 1,
    (float) 2,
    (float) 0.001
  );
}

void readSerial() {
  if (Serial.available()) {
    command = Serial.readStringUntil('\n');
    Serial.println("Input Params: " + command);
    switch (command[0]) {
      case '2':
        resetMemValues();
        break;
      case '3':
        int speed = command.substring(2).toInt();
        Serial.println("Desired Speed: " + String(speed, DEC));
        setMotorSpeed(speed, 0);
        break;
      default:
        break;
    }
  }
}

#endif // SETTINGS