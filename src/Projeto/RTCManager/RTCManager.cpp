#include "RTCManager.h"

RTCManager::RTCManager(int sdaPin, int sclPin)
  : _sdaPin(sdaPin), _sclPin(sclPin) {}

bool RTCManager::begin() {
  Wire.begin(_sdaPin, _sclPin);
  if (!rtc.begin()) {
    Serial.println("ERRO: Não foi possível encontrar o RTC!");
    return false;
  }
  Serial.println("RTC encontrado!");
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
