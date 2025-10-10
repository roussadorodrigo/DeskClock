#ifndef NTP_MANAGER_H
#define NTP_MANAGER_H

#include <WiFi.h>
#include "time.h"

bool NTPManager_Init(const char* ssidParameter, const char* passwordParameter, const char* ntpServer, long gmtOffset, int dayLightOffset);

bool NTPManager_ConnectWifi(const char* ssid, const char* password);

tm* NTPManager_NTPGetTime(const long _gmtOffset, const int _daylightOffset, const char* _ntpServer);

bool NTPManager_DisconnectWifi();

#endif