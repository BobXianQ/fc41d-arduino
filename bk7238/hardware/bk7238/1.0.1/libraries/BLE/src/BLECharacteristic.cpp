#include <string.h>
#include <stdlib.h>
#include "BLECharacteristic.h"
#include "BLEService.h"
#include "ql_hal.h"
#include "bk_hal_ble.h"

BLECharacteristic::BLECharacteristic(std::string uuid,uint8_t service_hander,uint32_t properties) 
{
    uint8_t uuid_buff[16];

    cccd_data_len = 0;

	if (uuid.length() == 4 || uuid.length() == 32) 
	{
        hexstr2bin(uuid.data(), uuid_buff, uuid.length()/2);
	}

    cccd_handle = bk_add_characteristic(service_hander,uuid_buff,uuid.length()/2,properties);
    service_id = service_hander;
}

void BLECharacteristic::addDescriptor() 
{
    return;
} 

void BLECharacteristic::setValue(uint8_t* data, size_t length) 
{
    uint16_t data_len = length>sizeof(cccd_data)?sizeof(cccd_data):length;

    memcpy(cccd_data,data,data_len);   
    cccd_data_len = data_len;   
} 

void BLECharacteristic::notify() 
{
    if(cccd_data_len)
        bk_ble_send_ntf_value(cccd_data_len, cccd_data,service_id,cccd_handle);
}

void BLECharacteristic::indicate() 
{
    if(cccd_data_len)
        bk_ble_send_ind_value(cccd_data_len, cccd_data,service_id,cccd_handle);
}
