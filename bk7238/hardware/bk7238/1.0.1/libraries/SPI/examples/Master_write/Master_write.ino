#include <SPI.h>
void setup() {
  Serial.begin(115200);
  SPI.begin();
}

// the loop function runs over and over again until power down or reset
void loop() {

  //use the SPI buses
  uint8_t data1,data2;
  uint16_t s_data1 = 0x1122;
  uint16_t s_data2 = 0x6677;
  data1 = SPI.transfer(s_data1);
  Serial.printf("s_data1:%02x,data1 %02x\r\n",s_data1,data1);
  delay(1000);
  data2 = SPI.transfer(s_data2);
  Serial.printf("s_data2:%02x,data2 %02x\r\n",s_data2,data2);
  delay(1000);
}
