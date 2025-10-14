#include "RTCManager.h"

#define DEBUG 1

bool RTCManager_Begin(int sda_pin, int scl_pin){
  Wire.begin(sda_pin, scl_pin);
  if (!rtc.begin()){

    #ifdef DEBUG
      Serial.println("Error: Couldn't find the RTC module");
    #endif

    return false;
  }

  #ifdef DEBUG
    Serial.println("RTC found");
  #endif

  return true;
}



void RTCManager::adjust(DateTime dateTime) {
  rtc.adjust(dateTime);
}

DateTime RTCManager::now() {
  return rtc.now();
}

float RTCManager::getTemperature() {
  return rtc.getTemperature();
}

void RTCManager::printCurrentTime() {
  Serial.println(getFormattedTime());
}

void RTCManager::printTemperature() {
  Serial.print("Temperatura: ");
  Serial.print(rtc.getTemperature());
  Serial.println("ºC");
}

String RTCManager::getFormattedTime() {
  DateTime now = rtc.now();
  String formatted = String(daysOfTheWeek[now.dayOfTheWeek()]) + ", " +
                     (now.day() < 10 ? "0" : "") + String(now.day()) + "/" +
                     (now.month() < 10 ? "0" : "") + String(now.month()) + "/" +
                     String(now.year()) + " " +
                     (now.hour() < 10 ? "0" : "") + String(now.hour()) + ":" +
                     (now.minute() < 10 ? "0" : "") + String(now.minute()) + ":" +
                     (now.second() < 10 ? "0" : "") + String(now.second());
  return formatted;
}
