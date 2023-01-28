#ifdef PCF8574_WRITE

#include <Arduino.h>
#include <Wire.h>

#define PCF8574_ADDR 0x20

void setup() {
  Wire.begin();
}

void PCF8574_Write(uint8_t data) {
  Wire.beginTransmission(PCF8574_ADDR); // Start + Address
  Wire.write(data); // DATA
  Wire.endTransmission();
}

void loop() {
  PCF8574_Write(0b11111111);
  delay(500);
  PCF8574_Write(0b00000000);
  delay(500);
}

#endif
