

#ifndef _BLECHARACTERISTIC_H_
#define _BLECHARACTERISTIC_H_

#include "ql_hal.h"
#include <string>

#define MAX_CHAR_VAULE_SIZE    512
class BLEService;

class BLECharacteristic {
public:
	BLECharacteristic(std::string uuid,uint8_t service_hander,uint32_t properties);
	void           addDescriptor();
	void indicate();
	void notify();
	void setValue(uint8_t* data, size_t size);

	static const uint32_t PROPERTY_READ      		= 1<<0;
	static const uint32_t PROPERTY_WRITE     		= 1<<1;
	static const uint32_t PROPERTY_NOTIFY    		= 1<<2;
	static const uint32_t PROPERTY_BROADCAST 		= 1<<3;
	static const uint32_t PROPERTY_INDICATE  		= 1<<4;
	static const uint32_t PROPERTY_WRITE_NO_RSP  	= 1<<5;

private:
	uint8_t  					service_id;
	uint16_t                    cccd_handle;
	uint8_t 					cccd_data[MAX_CHAR_VAULE_SIZE];
	uint16_t 					cccd_data_len;
}; 

#endif /* _BLECHARACTERISTIC_H_ */
