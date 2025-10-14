#ifndef NTP_MANAGER_H
#define NTP_MANAGER_H

#include <WiFi.h>
#include "time.h"

/**
 * @brief Function that initializes the NTP so it can be used
 * 
 * @param ssidParameter Network's SSID
 * @param passwordParameter Network's password
 * @param ntpServer 
 * @param gmtOffset 
 * @param dayLightOffset 
 * @return true If initialization successful
 * @return false If initialization error
 */
bool NTPManager_Init(const char* ssidParameter, const char* passwordParameter, const char* ntpServer, long gmtOffset, int dayLightOffset);


/**
 * @brief Function that connects to the wifi network
 * 
 * @param ssid Network's SSID
 * @param password Network's password
 * @return true If connection successful
 * @return false If connection error
 * 
 * @note Note that this is a static function (used by NTPManager_Init)
 */
static bool NTPManager_ConnectWifi(const char* ssid, const char* password);


/**
 * @brief Function that retrieves the current time
 * 
 * @param _gmtOffset 
 * @param _daylightOffset 
 * @param _ntpServer 
 * @return tm* Pointer to the time structure that contains the current time
 */
tm* NTPManager_NTPGetTime(const long _gmtOffset, const int _daylightOffset, const char* _ntpServer);


/**
 * @brief Function that closes the open WiFi connection
 * 
 * @return true If disconnect success
 * @return false If disconnect error
 */
bool NTPManager_DisconnectWifi();

#endif