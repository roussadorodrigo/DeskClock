#ifndef NTP_MANAGER_H
#define NTP_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include "time.h"
#include "RTClib.h"

class NTPManager {
public:
  NTPManager(const char* ntpServer = "pool.ntp.org", long gmtOffset = 0, int daylightOffset = 3600);
  bool connectWiFi(String ssid, String password);
  bool syncRTC(RTC_DS3231& rtc);
  void disconnectWiFi();
  void getWiFiCredentials(String& ssid, String& password);

private:
  const char* _ntpServer;
  long _gmtOffset;
  int _daylightOffset;
};

#endif
