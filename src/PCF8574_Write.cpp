#ifdef PCF8574_WRITE

#include <Arduino.h>
#include <Wire.h>

#define PCF8574_ADDR (0x20)

bool PCF8574_Write(uint8_t data) {
  Wire.beginTransmission(PCF8574_ADDR); // Send Start + Address
  Wire.write(data); // Send DATA
  uint8_t code = Wire.endTransmission(); // Send Stop
  if (code != 0) { 
    Serial.printf("write error code %d\n", code);
    return false;
  }  
  return true;
}

void setup() {
  Serial.begin(115200);

  Wire.begin();
}

void loop1() {
  PCF8574_Write(0x0F);
  delay(500);
  PCF8574_Write(0xF0);
  delay(500);
}

void loop() {
  static uint8_t data = 0x01;
  PCF8574_Write(~data);
  data = data == 0x80 ? 0x01 : data << 1;
  delay(200);
}

#endif
