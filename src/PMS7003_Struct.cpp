#ifdef PMS7003_STRUCT

#include <Arduino.h>

int PM_SP_UG_1_0, PM_SP_UG_2_5, PM_SP_UG_10_0;
int PM_AE_UG_1_0, PM_AE_UG_2_5, PM_AE_UG_10_0;

#define PMS Serial2

bool pms_process() ;

void setup() {
  Serial.begin(115200);
  PMS.begin(9600, SERIAL_8N1, 27, -1);
  PMS.setTimeout(100);

  Serial.println("PM2.5 Sensor");
}

void loop() {
  if (pms_process()) {
    Serial.print("PM2.5: ");
    Serial.println(PM_AE_UG_2_5);
  }

  delay(10);
}

typedef struct {
  uint8_t start1;
  uint8_t start2;
  uint16_t frame_length_swap;
  uint16_t data_swap[13];
  uint16_t check_swap;
} PMS7003_Packet_t;

uint16_t swap(uint16_t in) {
  return ((in >> 8) & 0xFF) | ((in & 0xFF) << 8);
}

bool pms_process() {
  PMS7003_Packet_t packet;
  memset(&packet, 0, sizeof(packet));
  int n = PMS.readBytes((uint8_t*) &packet, sizeof(packet));
  if (n != sizeof(packet)) {
    return false;
  }
  
  // Check start character
  if ((packet.start1 != 0x42) || (packet.start2 != 0x4d)) {
    Serial.println("invaild start character");
    return false;
  }

  // Check frame length
  if (swap(packet.frame_length_swap) != ((2 * 13) + 2)) { // Datasheet page.13
    Serial.println("invaild frame length");
    return false;
  }

  // Checksum
  uint16_t check = 0;
  for (uint8_t i=0;i<(sizeof(packet) - 2);i++) {
    check += ((uint8_t*)(&packet))[i];
  }
  if (check != swap(packet.check_swap)) {
    Serial.println("invaild check word");
    return false;
  }

  PM_SP_UG_1_0 = swap(packet.data_swap[0]);
  PM_SP_UG_2_5 = swap(packet.data_swap[1]);
  PM_SP_UG_10_0 = swap(packet.data_swap[2]);

  PM_AE_UG_1_0 = swap(packet.data_swap[3]);
  PM_AE_UG_2_5 = swap(packet.data_swap[4]);
  PM_AE_UG_10_0 = swap(packet.data_swap[5]);

  return true;
}

#endif
