#ifndef SETTINGS
#define SETTINGS
#include <avr/eeprom.h>

typedef struct {
  int maxRPM; // Max RPM ouput
  int cpr; // Encoder steps per revolutions
  int gear; // Gearbox relation 1:30
  float kp;
  float kd;
  float ki;
} settings_type;

void readSettings(settings_type *settingsPtr) {
  Serial.println(F("--- Welcome to Motors Setup CLI ---\nTo execute a function enter the request as follows (no spaces, only numbers and commas):\n\"#func,#param1,#param2...\" and ENTER"));
  Serial.println(F("For example, to execute updateSettingValues: 1,300,44,30,1,2,0.001"));
  Serial.println(F("--> MAKE SURE you connect the motor using the proper pinout in the connectors\n"));
  Serial.println(F("\n1: updateSettingsValues(maxRPM,cpr,gear,kp,kd,ki) => output RPM, encoders Counts Per Revolution, Gearbox output"));
  Serial.println(F("2: resetValues() => updateSettingsValues(300,44,30,1,2,0.001)"));
  Serial.println(F("3: setMotorSpeed(rpmSpeed)"));
  eeprom_read_block((void*)settingsPtr, (void*)0, sizeof(*settingsPtr));
  Serial.println(
    "Settings: \nRPM:" + String(settingsPtr->maxRPM, DEC) + "\n" +
    "cpr:" + String(settingsPtr->cpr, DEC) + "\n" +
    "gear:" + String(settingsPtr->gear, DEC) + "\n" +
    "kp:" + String(settingsPtr->kp, DEC) + "\n" +
    "kd:" + String(settingsPtr->kd, DEC) + "\n" +
    "ki:" + String(settingsPtr->ki, DEC) + "\n"
  );
}

void updateSettings(settings_type *settings) {
  eeprom_write_block((const void*)settings, (void*)0, sizeof(*settings));
  Serial.println(F("Settings Update => Success!"));
}

// void updateSettingValues(
//   int maxRPM,
//   int cpr,
//   int gear,
//   float kp,
//   float kd,
//   float ki
// ) {
//   settings.maxRPM = maxRPM;
//   settings.cpr = cpr;
//   settings.gear = gear;
//   settings.kp = kp;
//   settings.kd = kd;
//   settings.ki = ki;
//   eeprom_write_block((const void*)&settings, (void*)0, sizeof(settings));
//   Serial.println("Settings Update => Success!");
// }

void resetMemValues(settings_type *settingsPtr) {
  settingsPtr->maxRPM = 300;
  settingsPtr->cpr = 44;
  settingsPtr->gear = 30;
  settingsPtr->kp = 1;
  settingsPtr->kd = 2;
  settingsPtr->ki = 0.001;
  eeprom_write_block((const void*)settingsPtr, (void*)0, sizeof(*settingsPtr));
  Serial.println(F("Settings Update => Success!"));
}

#endif // SETTINGS