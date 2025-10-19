#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <time.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Pinos I2C do XIAO ESP32-S3
#define SDA_PIN 5
#define SCL_PIN 6

// WiFi
const char* ssid = "barreiro";
const char* password = "219622343a";

// NTP
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 0;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void initDisplay() {
  Wire.begin(SDA_PIN, SCL_PIN);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Falha na inicialização do SSD1306"));
    for(;;);
  }
  
  delay(2000);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
}

void initWiFi() {
  Serial.print("Conectando ao WiFi: ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
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
  } else {
    Serial.println("\nFalha na conexão WiFi");
  }
}

void initNTP() {
  // Configura o fuso horário (Portugal é GMT+0 em inverno, GMT+1 em verão)
  // Para Portugal: "WET0WEST,M3.5.0/1,M10.5.0"
  
  configTime(0, 0, ntpServer);
  setenv("TZ", "WET0WEST,M3.5.0/1,M10.5.0", 1);
  tzset();

  Serial.println("Sincronizando hora com NTP...");
  time_t now = time(nullptr);
  int attempts = 0;
  
  while (now < 24 * 3600 && attempts < 20) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
    attempts++;
  }
  
  Serial.println();
  Serial.println("Hora sincronizada!");
}

void displayDate(String date) {
  display.setTextSize(1);
  display.setCursor(0, 5);
  display.println(date);
}

void displayHour(String hour) {
  display.setTextSize(3);
  display.setCursor(25, 25);
  display.println(hour);
}

void displayTemp(String temperature) {
  display.setTextSize(1);
  display.setCursor(50, 55);
  display.println(temperature);
}

void updateDisplay() {
  display.display();
}

void clearDisplayScreen() {
  display.clearDisplay();
}

String getFormattedDate() {
  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);
  
  char buffer[11];
  strftime(buffer, sizeof(buffer), "%d/%m/%Y", timeinfo);
  return String(buffer);
}

String getFormattedTime() {
  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);
  
  char buffer[6];
  strftime(buffer, sizeof(buffer), "%H:%M", timeinfo);
  return String(buffer);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  initDisplay();
  initWiFi();
  initNTP();
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Hora sincronizada!");
  display.display();
  delay(2000);
}

void loop() {
  clearDisplayScreen();
  
  String date = getFormattedDate();
  String time = getFormattedTime();
  String temp = "25 C";
  
  displayDate(date);
  displayHour(time);
  displayTemp(temp);
  
  updateDisplay();
  
  delay(1000);
}