#ifndef RTC_MANAGER_H
#define RTC_MANAGER_H

#include <Arduino.h>
#include <Wire.h>
#include <RTClib.h>

class RTCManager {
public:
  RTCManager(int sdaPin = 5, int sclPin = 6);
  bool begin();
  void adjust(DateTime dateTime);
  DateTime now();
  float getTemperature();
  void printCurrentTime();
  void printTemperature();
  String getFormattedTime();

private:
  RTC_DS3231 rtc;
  int _sdaPin, _sclPin;
  const char* daysOfTheWeek[7] = {
    "Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"
  };
};

#endif
