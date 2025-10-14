#include "NTPManager.h"

#define DEBUG 1
#define WIFI_CONNECTIONS_MAX_ATTEMPS 20

bool NTPManager_Init(const char* ssidParameter, const char* passwordParameter, const char* ntpServer, long gmtOffset, int dayLightOffset){

  #ifdef DEBUG
    Serial.print("Insert the ssid: ");
    const char* ssid = Serial.readStringUntil('\n').c_str();

    Serial.print("Insert the password: ");
    const char* password = Serial.readStringUntil('\n').c_str();
  #endif

  return NTPManager_ConnectWifi(ssidParameter, passwordParameter);
  
}

static bool NTPManager_ConnectWifi(const char* ssid, const char* password){
  int wifi_attempts_counter = WIFI_CONNECTIONS_MAX_ATTEMPS; 

  #ifdef DEBUG
    Serial.print("Connecting to WiFi...");
  #endif

  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED && wifi_attempts_counter > 0){
    vTaskDelay(pdMS_TO_TICKS(500));
    wifi_attempts_counter--;
  }

  if(WiFi.status() != WL_CONNECTED){
    #ifdef DEBUG
      Serial.print("Error: couldn't connect to the WiFi network");
    #endif

    return false;
  }

  #ifdef DEBUG
    Serial.print("Connected to WiFi network: ");
    Serial.print(ssid);
    Serial.print("IP Adress: ");
    Serial.print(WiFi.localIP());
  #endif

  return true;
}


tm* NTPManager_NTPGetTime(const long _gmtOffset, const int _daylightOffset, const char*_ntpServer){

  #ifdef DEBUG
    Serial.println("Synchronizing with NTP server...");
  #endif

  configTime(_gmtOffset, _daylightOffset, _ntpServer);

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Error: couldn't connect to RTC");
    return NULL;
  }

  #ifdef DEBUG
    Serial.print("NTP Time: ");
    Serial.print(timeinfo.tm_mday);
    Serial.print("/");
    Serial.print(timeinfo.tm_mon + 1);
    Serial.print("/");
    Serial.print(timeinfo.tm_year + 1900);
    Serial.print(" - ");
    Serial.print(timeinfo.tm_hour);
    Serial.print(":");
    Serial.print(timeinfo.tm_min);
    Serial.print(":");
    Serial.print(timeinfo.tm_sec);
  #endif

  return &timeinfo;
}

bool NTPManager_DisconnectWifi(){
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}
