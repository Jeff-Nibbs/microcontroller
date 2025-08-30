// Receiver_ESP32C6_SSD1306_ESPNow_FIXED.ino
#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <esp_wifi.h>   // << needed for esp_wifi_set_* calls
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"

// ----------- I2C pins -----------
#define SDA_PIN 5
#define SCL_PIN 6

// ----------- OLED config --------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET   -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Match both sides
const uint8_t ESPNOW_CHANNEL = 1;

// Data packet
typedef struct {
  float tempC;
  float hum;
  uint32_t ms;
} SensorPacket;

// Shared state (NO volatile on the struct)
SensorPacket latest {NAN, NAN, 0};
volatile bool gotPacket = false;

// Lightweight guard for cross-task access
portMUX_TYPE latestMux = portMUX_INITIALIZER_UNLOCKED;

// Newer ESP-NOW receive callback signature (IDF v5+)
void onReceive(const esp_now_recv_info* info, const uint8_t* data, int len) {
  if (len == sizeof(SensorPacket)) {
    SensorPacket pkt;
    memcpy(&pkt, data, sizeof(pkt));
    portENTER_CRITICAL(&latestMux);
    latest = pkt;               // safe: not volatile, guarded
    gotPacket = true;
    portEXIT_CRITICAL(&latestMux);
  }
}

void setup() {
  Serial.begin(115200);
  delay(200);

  // I2C
  Wire.begin(SDA_PIN, SCL_PIN);

  // OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 not found at 0x3C");
    while (1) delay(10);
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Home Temps");
  display.display();

  // WiFi Station on fixed channel
  WiFi.mode(WIFI_STA);
  // Lock the channel BEFORE starting ESP-NOW
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(ESPNOW_CHANNEL, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);

  // Print MAC so you can add it to the sender
  Serial.print("Receiver STA MAC: ");
  Serial.println(WiFi.macAddress());

  // ESP-NOW init
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed!");
    while (1) delay(10);
  }

  // Register receive callback
  if (esp_now_register_recv_cb(onReceive) != ESP_OK) {
    Serial.println("Register recv cb failed!");
  }
}

void drawValues(float t, float h) {
  // display.clearDisplay();
  // display.setTextSize(1);
  // display.setCursor(0, 0);
  // display.println("ESP-NOW AHT30 Receiver");
  
  float f = ( t * 9 / 5 ) + 32;
  display.fillRect(0, 30, 128, 20, SSD1306_BLACK); // temp row
  display.fillRect(0, 48, 128, 20, SSD1306_BLACK); // humidity row

  display.setTextSize(1);
  display.setCursor(0, 30);
  display.print("Temperature: ");
  if (isnan(t)) display.print("--");
  else          display.print(f, 1);
  display.println("F");

  display.setCursor(0, 48);
  display.print("Humidity: ");
  if (isnan(h)) display.print("--");
  else          display.print(h, 1);
  display.println("%");

  display.display();
}

void loop() {
  static uint32_t lastDraw = 0;

  if (gotPacket) {
    SensorPacket pkt;
    // Safely copy shared struct to a local
    portENTER_CRITICAL(&latestMux);
    pkt = latest;
    gotPacket = false;
    portEXIT_CRITICAL(&latestMux);
    float tempf = (pkt.tempC * 9 / 5) + 32;

    Serial.printf("RX  Temp: %.2f F  Hum: %.2f %%  (ms=%lu)\n",
                  tempf, pkt.hum, (unsigned long)pkt.ms);
    drawValues(pkt.tempC, pkt.hum);
    lastDraw = millis();
  }

  // Periodic refresh
  if (millis() - lastDraw > 2000) {
    SensorPacket snap;
    portENTER_CRITICAL(&latestMux);
    snap = latest;
    portEXIT_CRITICAL(&latestMux);
    drawValues(snap.tempC, snap.hum);
    lastDraw = millis();
  }
}
