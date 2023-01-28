#ifdef PCF8574_READ

#include <Arduino.h>
#include <Wire.h>

#define PCF8574_ADDR 0x20

void setup() {
  Serial.begin(115200);
  Wire.begin();
}

void loop() {
  int i = Wire.requestFrom(PCF8574_ADDR, 1);
  if (i == 1) {
    uint8_t data = Wire.read();
    Serial.printf("data = %02x\n", data);
  } else {
    Serial.println("error, read fail");
  }
  delay(100);
}

#endif
