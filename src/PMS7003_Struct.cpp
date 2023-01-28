#ifdef PMS7003_STRUCT

#include <Arduino.h>

typedef struct {
  uint8_t start1;
  uint8_t start2;
  uint16_t frame_length_swap;
  uint16_t data_swap[13];
  uint16_t check_swap;
} PMS7003_Packet_t;

uint16_t swap(uint16_t data) {
  return (data >> 8) | (data << 8);
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 4, -1);
  Serial2.setTimeout(100);
}

void loop() {
  PMS7003_Packet_t packet;
  int n = Serial2.readBytes((uint8_t*) &packet, sizeof(packet));
  if (n == sizeof(packet)) {
    if ((packet.start1 == 0x42) && (packet.start2 == 0x4d)) {
      if (swap(packet.frame_length_swap) == 2 * 13 + 2) {
        uint16_t sum = 0;
        for (int i=0;i<30;i++) {
          sum += ((uint8_t*)(&packet))[i];
        }
        if (swap(packet.check_swap) == sum) {
          uint16_t pm1_0 = swap(packet.data_swap[0]);
          uint16_t pm2_5 = swap(packet.data_swap[1]);
          uint16_t pm10_0 = swap(packet.data_swap[2]);

          Serial.printf("PM1.0: %d\tPM2.5: %d\tPM10.0: %d\n", pm1_0, pm2_5, pm10_0);
        } else {
          Serial.println("chack invaild");
        }
      } else {
        Serial.println("frame length invaild");
      }
    } else {
      Serial.println("start packet invaild");
    }
  }
}

#endif
