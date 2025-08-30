// Sender_ESP32C6_AHT30_ESPNow_FIXED.ino
#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <esp_wifi.h>   // <-- required for esp_wifi_set_channel / set_promiscuous

// ----------- I2C pins -----------
#define SDA_PIN 5
#define SCL_PIN 6

// Match both sides
const uint8_t ESPNOW_CHANNEL = 1;

// Replace with your receiver's MAC
uint8_t peerMac[] = { 0xF0, 0xF5, 0xBD, 0x09, 0xD8, 0x1C }; // F0:F5:BD:09:D8:1C

Adafruit_AHTX0 aht;

typedef struct {
  float tempC;
  float hum;
  uint32_t ms;
} SensorPacket;

void setup() {
  Serial.begin(115200);
  delay(200);

  // I2C + sensor
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!aht.begin()) {
    Serial.println("AHT30 not found! (addr 0x38) Check wiring.");
    while (1) delay(10);
  }
  Serial.println("AHT30 OK.");

  // WiFi Station, lock channel BEFORE ESP-NOW
  WiFi.mode(WIFI_STA);

  // Either of these patterns is fine; pick ONE:

  // (A) simplest: set channel directly
  esp_wifi_set_channel(ESPNOW_CHANNEL, WIFI_SECOND_CHAN_NONE);

  // // (B) traditional: briefly enable promiscuous while changing channel
  // esp_wifi_set_promiscuous(true);
  // esp_wifi_set_channel(ESPNOW_CHANNEL, WIFI_SECOND_CHAN_NONE);
  // esp_wifi_set_promiscuous(false);

  Serial.print("Sender STA MAC: ");
  Serial.println(WiFi.macAddress());

  // ESP-NOW init
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed!");
    while (1) delay(10);
  }

  // Add peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, peerMac, 6);
  peerInfo.channel = ESPNOW_CHANNEL;
  peerInfo.ifidx = WIFI_IF_STA;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer. Check MAC.");
    while (1) delay(10);
  }

  Serial.println("ESP-NOW sender ready.");
}

void loop() {
  sensors_event_t humEvent, tempEvent;
  aht.getEvent(&humEvent, &tempEvent);

  SensorPacket pkt;
  pkt.tempC = tempEvent.temperature;
  pkt.hum   = humEvent.relative_humidity;
  pkt.ms    = millis();

  esp_err_t result = esp_now_send(peerMac, (uint8_t*)&pkt, sizeof(pkt));

  if (result == ESP_OK) {
    Serial.printf("TX  Temp: %.2f C  Hum: %.2f %%\n", pkt.tempC, pkt.hum);
  } else {
    Serial.printf("Send error: %d\n", (int)result);
  }

  delay(1000);
}
