#include "NTPManager.h"

NTPManager::NTPManager(const char* ntpServer, long gmtOffset, int daylightOffset)
  : _ntpServer(ntpServer), _gmtOffset(gmtOffset), _daylightOffset(daylightOffset) {}

bool NTPManager::connectWiFi(String ssid, String password) {
  Serial.print("Conectando ao WiFi");
  WiFi.begin(ssid.c_str(), password.c_str());
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi conectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    return true;
  } else {
    Serial.println("\nFalha ao conectar WiFi.");
    return false;
  }
}

bool NTPManager::syncRTC(RTC_DS3231& rtc) {
  Serial.println("Sincronizando com servidor NTP...");
  configTime(_gmtOffset, _daylightOffset, _ntpServer);
  
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("ERRO: Falha ao obter hora do NTP");
    return false;
  }

  rtc.adjust(DateTime(timeinfo.tm_year + 1900,
                      timeinfo.tm_mon + 1,
                      timeinfo.tm_mday,
                      timeinfo.tm_hour,
                      timeinfo.tm_min,
                      timeinfo.tm_sec));

  Serial.println("RTC sincronizado com sucesso!");
  Serial.printf("Hora NTP: %02d/%02d/%04d %02d:%02d:%02d\n",
                timeinfo.tm_mday,
                timeinfo.tm_mon + 1,
                timeinfo.tm_year + 1900,
                timeinfo.tm_hour,
                timeinfo.tm_min,
                timeinfo.tm_sec);
  return true;
}

void NTPManager::disconnectWiFi() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void NTPManager::getWiFiCredentials(String& ssid, String& password) {
  Serial.println("\n--- Configuração WiFi ---");
  Serial.println("Digite o SSID da rede WiFi:");
  while (Serial.available() == 0) delay(100);
  ssid = Serial.readStringUntil('\n');
  ssid.trim();

  while (Serial.available() > 0) Serial.read();
  Serial.println("Digite a senha da rede WiFi:");
  while (Serial.available() == 0) delay(100);
  password = Serial.readStringUntil('\n');
  password.trim();

  while (Serial.available() > 0) Serial.read();
  Serial.println("--- Configuração concluída ---\n");
}
