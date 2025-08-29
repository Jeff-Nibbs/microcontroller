#include <Wire.h>

// AHT30 I2C address
#define AHT30_ADDRESS 0x38

// AHT30 commands
#define AHT30_INIT_CMD 0xBE
#define AHT30_MEASURE_CMD 0xAC
#define AHT30_SOFT_RESET_CMD 0xBA

// I2C pins for ESP32-C6
#define SDA_PIN 5
#define SCL_PIN 6

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Initialize I2C with custom pins
  Wire.begin(SDA_PIN, SCL_PIN);
  
  Serial.println("ESP32-C6 AHT30 Sensor Test");
  
  // Initialize AHT30
  if (initAHT30()) {
    Serial.println("AHT30 initialized successfully");
  } else {
    Serial.println("Failed to initialize AHT30");
  }
}

void loop() {
  float temperature, humidity;
  
  if (readAHT30(&temperature, &humidity)) {
    float tempf = (temperature * 9 / 5) + 32;
    Serial.print("Temperature: ");
    Serial.print(tempf, 2);
    Serial.print("°F, Humidity: ");
    Serial.print(humidity, 2);
    Serial.println("%");
  } else {
    Serial.println("Failed to read from AHT30");
  }
  
  delay(2000); // Read every 2 seconds
}

bool initAHT30() {
  // Soft reset
  Wire.beginTransmission(AHT30_ADDRESS);
  Wire.write(AHT30_SOFT_RESET_CMD);
  Wire.endTransmission();
  delay(20);
  
  // Initialize sensor
  Wire.beginTransmission(AHT30_ADDRESS);
  Wire.write(AHT30_INIT_CMD);
  Wire.write(0x08);
  Wire.write(0x00);
  int result = Wire.endTransmission();
  delay(10);
  
  return (result == 0);
}

bool readAHT30(float* temperature, float* humidity) {
  // Trigger measurement
  Wire.beginTransmission(AHT30_ADDRESS);
  Wire.write(AHT30_MEASURE_CMD);
  Wire.write(0x33);
  Wire.write(0x00);
  int result = Wire.endTransmission();
  
  if (result != 0) {
    return false;
  }
  
  // Wait for measurement to complete
  delay(80);
  
  // Read 6 bytes of data
  Wire.requestFrom(AHT30_ADDRESS, 6);
  
  if (Wire.available() != 6) {
    return false;
  }
  
  uint8_t data[6];
  for (int i = 0; i < 6; i++) {
    data[i] = Wire.read();
  }
  
  // Check if measurement is ready (bit 7 should be 0)
  if (data[0] & 0x80) {
    return false; // Measurement not ready
  }
  
  // Extract humidity data (20 bits)
  uint32_t humidityRaw = ((uint32_t)data[1] << 12) | 
                         ((uint32_t)data[2] << 4) | 
                         ((uint32_t)data[3] >> 4);
  
  // Extract temperature data (20 bits)
  uint32_t temperatureRaw = (((uint32_t)data[3] & 0x0F) << 16) | 
                            ((uint32_t)data[4] << 8) | 
                            (uint32_t)data[5];
  
  // Convert to actual values
  *humidity = (float)humidityRaw * 100.0 / 1048576.0; // 2^20 = 1048576
  *temperature = (float)temperatureRaw * 200.0 / 1048576.0 - 50.0;
  
  return true;
}

// Alternative function to check sensor status
bool checkAHT30Status() {
  Wire.requestFrom(AHT30_ADDRESS, 1);
  
  if (Wire.available()) {
    uint8_t status = Wire.read();
    Serial.print("AHT30 Status: 0x");
    Serial.println(status, HEX);
    return !(status & 0x80); // Return true if not busy
  }
  
  return false;
}