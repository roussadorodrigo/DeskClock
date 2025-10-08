/*********
  DS3231 RTC com sincronização NTP para XIAO ESP32S3
  
  XIAO ESP32S3 Wiring:
  DS3231 SDA -> GPIO 5 (Pin D4)
  DS3231 SCL -> GPIO 6 (Pin D5)
  DS3231 VCC -> 3.3V
  DS3231 GND -> GND
*********/

#include <WiFi.h>
#include "RTClib.h"
#include "time.h"

// Configurações WiFi
const char* ssid = "nome";           // Substitua pelo nome da sua rede
const char* password = "pass";    // Substitua pela senha da sua rede

// Configurações NTP
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;            // Offset GMT em segundos (0 = UTC)
const int daylightOffset_sec = 3600;     // Offset horário de verão (3600 = 1 hora)
// Para Portugal: gmtOffset_sec = 0, daylightOffset_sec = 3600 (verão) ou 0 (inverno)

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n=== Inicializando RTC e WiFi ===");
  
  // Initialize I2C with XIAO ESP32S3 default pins
  Wire.begin(5, 6);
  
  if (!rtc.begin()) {
    Serial.println("ERRO: Não foi possível encontrar o RTC!");
    Serial.flush();
    while (1) delay(10);
  }
  
  Serial.println("RTC encontrado!");
  
  // Conectar ao WiFi
  Serial.print("Conectando ao WiFi");
  WiFi.begin(ssid, password);
  
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
    
    // Sincronizar com NTP
    syncRTCWithNTP();
  } else {
    Serial.println("\nFalha ao conectar WiFi. Usando hora do RTC.");
  }
  
  // Desconectar WiFi para economizar energia (opcional)
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  
  Serial.println("\n=== Sistema pronto ===\n");
}

void syncRTCWithNTP() {
  Serial.println("Sincronizando com servidor NTP...");
  
  // Configurar NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("ERRO: Falha ao obter hora do NTP");
    return;
  }
  
  // Atualizar o RTC com a hora do NTP
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
}

void loop() {
  // Get the current time from the RTC
  DateTime now = rtc.now();
 
  // Getting each time field in individual variables
  // And adding a leading zero when needed
  String yearStr = String(now.year(), DEC);
  String monthStr = (now.month() < 10 ? "0" : "") + String(now.month(), DEC);
  String dayStr = (now.day() < 10 ? "0" : "") + String(now.day(), DEC);
  String hourStr = (now.hour() < 10 ? "0" : "") + String(now.hour(), DEC);
  String minuteStr = (now.minute() < 10 ? "0" : "") + String(now.minute(), DEC);
  String secondStr = (now.second() < 10 ? "0" : "") + String(now.second(), DEC);
  String dayOfWeek = daysOfTheWeek[now.dayOfTheWeek()];
  
  // Complete time string
  String formattedTime = dayOfWeek + ", " + dayStr + "/" + monthStr + "/" + yearStr + " " + hourStr + ":" + minuteStr + ":" + secondStr;
  
  // Print the complete formatted time
  Serial.println(formattedTime);
  
  // Getting temperature
  Serial.print("Temperatura: ");
  Serial.print(rtc.getTemperature());
  Serial.println("ºC");
  Serial.println();
  
  delay(3000);
}