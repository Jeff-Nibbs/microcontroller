#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_AHTX0.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64          // set to 32 if your OLED is 128x32
#define OLED_ADDR 0x3C            // change to 0x3D if needed
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_AHTX0 aht;

const unsigned long UPDATE_MS = 1000;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    while (1) { } // display failed
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  if (!aht.begin()) {
    // show simple error
    display.setTextSize(2);
    display.setCursor(0, 0); display.println(F("AHT30"));
    display.setTextSize(1);
    display.setCursor(0, 24); display.println(F("not found"));
    display.display();
    while (1) { }
  }
}

void loop() {
  static unsigned long last = 0;
  if (millis() - last < UPDATE_MS) return;
  last = millis();

  sensors_event_t hum, temp;
  aht.getEvent(&hum, &temp);   // reads humidity + temperature

  float tC = temp.temperature;
  float RH  = hum.relative_humidity;
  if (isnan(tC) || isnan(RH)) return;
  
  float T = (tC * 9.0 / 5.0) + 32.0;

  float HI = -42.379 + 2.04901523 * T + 10.14333127 * RH
            - 0.22475541 * T * RH - 0.00683783 * T * T
            - 0.05481717 * RH * RH + 0.00122874 * T * T * RH
            + 0.00085282 * T * RH * RH - 0.00000199 * T * T * RH * RH;

  display.clearDisplay();

  // Title line
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(F("Home Temperature"));
  display.drawLine(0, 12, SCREEN_WIDTH, 12, SSD1306_WHITE);

  // Temperature big
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.print(F("Temperature: "));
  display.print(T, 1); display.print((char)247); display.print(F("F"));

  // Humidity big
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print(F("Humidity: "));
  display.print(RH, 1); display.print(F("%"));

  // Heat Index
  display.setTextSize(1);
  display.setCursor(0, 50);
  display.print(F("Heat Index: "));
  display.print(HI, 1); display.print((char)247); display.print(F("F"));

  display.display();
}
