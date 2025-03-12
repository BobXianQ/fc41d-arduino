#include "BLEService.h"
#include "BLEServer.h"
#include "BLECharacteristic.h"
#include "ql_hal.h"
#include "bk_hal_ble.h"
#include <string.h>

static uint8_t service_num = 0;
BLEService::BLEService(std::string uuid) 
{
	uint8_t svc_uuid[16];
	svc_hander = service_num;
	service_num++;

	if (uuid.length() == 4 || uuid.length() == 32) 
	{
		hexstr2bin(uuid.data(), svc_uuid, uuid.length()/2);
	}

	bk_add_service(svc_hander,svc_uuid,uuid.length()/2);
} 

void BLEService::start() 
{
	bk_start_service(svc_hander);
}

BLECharacteristic* BLEService::createCharacteristic(const char* uuid, uint32_t properties) 
{
	BLECharacteristic* pCharacteristic = new BLECharacteristic(uuid,svc_hander,properties);

	return pCharacteristic;
} 
