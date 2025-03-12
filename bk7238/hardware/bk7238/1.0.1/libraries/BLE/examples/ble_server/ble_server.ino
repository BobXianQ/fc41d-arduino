#include <BLEDEV.h>

uint8_t adv_data[31];
uint8_t adv_rsp_data[31];

#define SERVICE_UUID1         "ff33"
#define SERVICE_UUID2         "4fafc2011fb5459e8fccc5c9c331914b"

#define CHARACTERISTIC_UUID1 "FF11"
#define CHARACTERISTIC_UUID2 "FF22"
#define CHARACTERISTIC_UUID3 "ff11c2011fb5459e8fccc5c9c33191ff"
bool deviceConnected = false;
bool oldDeviceConnected = false;

BLECharacteristic* pCharacteristic1;
BLECharacteristic* pCharacteristic2;
BLECharacteristic* pCharacteristic3;
BLEAdvertising *pAdvertising;

uint32_t value = 0;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect() {
      deviceConnected = true;
      Serial.println("onConnect...");

    };

    void onDisconnect() {
      deviceConnected = false;
      Serial.println("onDisconnect...");
    }

    void onwrite(uint16_t prf_id,uint16_t att_idx,uint8_t* data,uint16_t data_len) {
        Serial.printf("prf_id:%d,att_idx:%d,data_len:%d,data[0]:%d\r\n",prf_id,att_idx,data_len,data[0]);
    }

    void onread(uint16_t prf_id,uint16_t att_idx,uint8_t* data,uint16_t *data_len) {
      Serial.printf("prf_id:%d,att_idx:%d\r\n",prf_id,att_idx);
      data[0]=0xff;
      data[1]=0xee;
      data[2]=0xdd;
      *data_len =3;
    }

};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  BLEDEV::init();
  BLEServer* pServer = BLEDEV::createServer();
  pAdvertising = BLEDEV::getAdvertising();
  BLEService *pService1 = pServer->createService(SERVICE_UUID1);
  BLEService *pService2 = pServer->createService(SERVICE_UUID2);
  pServer->setCallbacks(new MyServerCallbacks());

  pCharacteristic1 = pService1->createCharacteristic(
                                                      CHARACTERISTIC_UUID1,
                                                      BLECharacteristic::PROPERTY_READ   |
                                                      BLECharacteristic::PROPERTY_WRITE  |
                                                      BLECharacteristic::PROPERTY_NOTIFY 
                                                    );

  pCharacteristic2 = pService1->createCharacteristic(
                                                      CHARACTERISTIC_UUID2,
                                                      BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_INDICATE
                                                    );

  pCharacteristic3 = pService2->createCharacteristic(
                                                      CHARACTERISTIC_UUID3,
                                                      BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ
                                                    );                                                                   
  pService1->start();
  pService2->start();

	adv_data[0] = 0x0A;
	adv_data[1] = 0x09;
	memcpy(&adv_data[2], "7238_BLE", 9);
  pAdvertising->setAdvertisementData((uint8_t*)&adv_data,0xb);

	adv_rsp_data[0] = 0x06;
	adv_rsp_data[1] = 0x08;
	memcpy(&adv_rsp_data[2], "7238", 5);
  pAdvertising->setScanResponseData((uint8_t*)&adv_rsp_data,7);

  pAdvertising->start();
}

void loop() {
  // put your main code here, to run repeatedly:
    if (deviceConnected) {
        delay(500);
        pCharacteristic1->setValue((uint8_t*)&value, 4);
        pCharacteristic1->notify();
        value++;
        delay(500); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
        pCharacteristic2->setValue((uint8_t*)&value, 4);
        pCharacteristic2->indicate();
        delay(500);
    }

    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pAdvertising->start();// restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }

    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        oldDeviceConnected = deviceConnected;
    }

}
