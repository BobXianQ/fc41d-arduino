#ifndef _BK_BLESERVICE_H_
#define _BK_BLESERVICE_H_

#include "BLECharacteristic.h"
#include "ql_hal.h"
#include <string>
class BLEServer;

class BLEService {
public:
	BLECharacteristic* createCharacteristic(const char* uuid, uint32_t properties);
	void               start();


private:
	BLEService(std::string uuid);
	uint16_t svc_hander;
	friend class BLEServer;
}; 



#endif /* _BLESERVICE_H_ */
