#ifdef PCF8574_READ

#include <Arduino.h>
#include <Wire.h>

#define PCF8574_ADDR (0x20)

int PCF8574_Read() {
  int data = -1;
  int n = Wire.requestFrom(PCF8574_ADDR, 1); // Send Start + Address + Get Data 1 byte + Stop
  if (n == 1) {
    data = Wire.read();
  } else {
    Serial.println("read error !");
  }

  return data;
}

void setup() {
  Serial.begin(115200);

  Wire.begin();
}

void loop() {
  int data = PCF8574_Read();
  Serial.printf("data = %d\n", bitRead(data, 0));
  delay(100);
}

#endif
