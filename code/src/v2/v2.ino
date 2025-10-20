#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WebServer.h>
#include <time.h>
#include <EEPROM.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Pinos I2C do XIAO ESP32-S3
#define SDA_PIN 5
#define SCL_PIN 6

// EEPROM
#define EEPROM_SIZE 512
#define SSID_ADDR 0
#define SSID_SIZE 32
#define PASSWORD_ADDR 32
#define PASSWORD_SIZE 64

// WiFi
char ssid[SSID_SIZE] = "";
char password[PASSWORD_SIZE] = "";
const char* apSSID = "ESP32-Config";
const char* apPassword = "12345678";

// NTP
const char* ntpServer = "pool.ntp.org";

// Web Server
WebServer server(80);

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

void displayWifi(int x, int y) {
  // Símbolo de WiFi pixelizado 10x10 pixeis
  display.fillRect(x + 2, y, 6, 1, SSD1306_WHITE);
  display.fillRect(x + 1, y + 1, 1, 1, SSD1306_WHITE);
  display.fillRect(x + 8, y + 1, 1, 1, SSD1306_WHITE);
  display.fillRect(x + 2, y + 2, 6, 1, SSD1306_WHITE);
  display.fillRect(x, y + 3, 1, 1, SSD1306_WHITE);
  display.fillRect(x + 9, y + 3, 1, 1, SSD1306_WHITE);
  display.fillRect(x + 2, y + 4, 6, 1, SSD1306_WHITE);
  display.fillRect(x + 1, y + 5, 1, 1, SSD1306_WHITE);
  display.fillRect(x + 8, y + 5, 1, 1, SSD1306_WHITE);
  display.fillRect(x + 2, y + 6, 6, 1, SSD1306_WHITE);
  display.fillRect(x + 4, y + 8, 2, 2, SSD1306_WHITE);
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

void loadCredentials() {
  EEPROM.readBytes(SSID_ADDR, (uint8_t*) ssid, SSID_SIZE);
  EEPROM.readBytes(PASSWORD_ADDR, (uint8_t*) password, PASSWORD_SIZE);
}

void saveCredentials() {
  EEPROM.writeBytes(SSID_ADDR, (uint8_t*) ssid, SSID_SIZE);
  EEPROM.writeBytes(PASSWORD_ADDR, (uint8_t*) password, PASSWORD_SIZE);
  EEPROM.commit();
}

void handleRoot() {
  String html = R"(
    <!DOCTYPE html>
    <html>
    <head>
      <meta charset="utf-8">
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <title>Configuração WiFi</title>
      <style>
        body { font-family: Arial; text-align: center; padding: 20px; background: #f0f0f0; }
        .container { background: white; border-radius: 10px; padding: 30px; max-width: 500px; margin: 0 auto; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }
        h1 { color: #333; }
        input { width: 100%; padding: 10px; margin: 10px 0; border: 1px solid #ddd; border-radius: 5px; box-sizing: border-box; font-size: 16px; }
        button { width: 100%; padding: 12px; background: #007bff; color: white; border: none; border-radius: 5px; cursor: pointer; font-size: 16px; font-weight: bold; }
        button:hover { background: #0056b3; }
        .info { color: #666; margin-top: 20px; font-size: 14px; }
      </style>
    </head>
    <body>
      <div class="container">
        <h1>⚙️ Configuração WiFi</h1>
        <form action="/config" method="POST">
          <label>SSID (Nome da Rede):</label>
          <input type="text" name="ssid" required placeholder="Nome da rede WiFi">
          
          <label>Password (Senha):</label>
          <input type="password" name="password" required placeholder="Senha da rede WiFi">
          
          <button type="submit">Guardar e Conectar</button>
        </form>
        <div class="info">
          <p>Após submeter, o dispositivo tentará conectar à rede WiFi</p>
        </div>
      </div>
    </body>
    </html>
  )";
  server.send(200, "text/html", html);
}

void handleConfig() {
  if (server.method() == HTTP_POST) {
    if (server.hasArg("ssid") && server.hasArg("password")) {
      String newSSID = server.arg("ssid");
      String newPassword = server.arg("password");
      
      // Limpa arrays
      memset(ssid, 0, SSID_SIZE);
      memset(password, 0, PASSWORD_SIZE);
      
      // Copia novos valores
      newSSID.toCharArray(ssid, SSID_SIZE);
      newPassword.toCharArray(password, PASSWORD_SIZE);
      
      // Guarda
      saveCredentials();
      
      String response = R"(
        <!DOCTYPE html>
        <html>
        <head>
          <meta charset="utf-8">
          <title>Sucesso</title>
          <style>
            body { font-family: Arial; text-align: center; padding: 50px; background: #f0f0f0; }
            .container { background: white; border-radius: 10px; padding: 30px; max-width: 500px; margin: 0 auto; }
            .success { color: green; font-size: 24px; }
          </style>
        </head>
        <body>
          <div class="container">
            <div class="success">✓ Credenciais guardadas!</div>
            <p>O dispositivo vai reiniciar e conectar à rede...</p>
          </div>
        </body>
        </html>
      )";
      
      server.send(200, "text/html", response);
      delay(1000);
      ESP.restart();
    }
  }
}

void startAPMode() {
  Serial.println("Iniciando Modo Access Point...");
  
  clearDisplayScreen();
  display.setTextSize(2);
  display.setCursor(5, 10);
  display.println("AP Mode");
  display.setTextSize(1);
  display.setCursor(0, 40);
  display.println("WiFi: ESP32-Config");
  display.setCursor(0, 50);
  display.println("Pass: 12345678");
  updateDisplay();
  
  WiFi.mode(WIFI_AP);
  WiFi.softAP(apSSID, apPassword);
  
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP: ");
  Serial.println(IP);
  
  server.on("/", handleRoot);
  server.on("/config", handleConfig);
  server.begin();
  
  // Aguarda até 5 minutos por configuração
  unsigned long startTime = millis();
  while (millis() - startTime < 300000) {
    server.handleClient();
    delay(100);
  }
  
  server.stop();
  WiFi.mode(WIFI_STA);
}

void initWiFi() {
  if (strlen(ssid) == 0 || strlen(password) == 0) {
    startAPMode();
  }
  
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
    startAPMode();
  }
}

void initNTP() {
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

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  EEPROM.begin(EEPROM_SIZE);
  initDisplay();
  loadCredentials();
  
  initWiFi();
  initNTP();
  
  clearDisplayScreen();
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
  displayWifi(115, 5);
  
  updateDisplay();
  
  delay(1000);
}