#ifndef _BLEADVERTISING_H_
#define _BLEADVERTISING_H_

#include "ql_hal.h"
#include <vector>
#include "ble_api_5_x.h"


class BLEAdvertising {
public:
	BLEAdvertising(){
		advParams.interval_max=160;
		advParams.interval_min=160;
		actv_idx = 0xff;
	}
	
	void start();
	void stop();
	void setMaxInterval(uint16_t maxinterval);
	void setMinInterval(uint16_t mininterval);
	void setAdvertisementData(uint8_t *adv_data,uint8_t len);
	void setScanResponseData(uint8_t *adv_rsp_data,uint8_t len);
private:
	struct adv_param	 advParams;
	uint8_t 			 actv_idx;
};

#endif /* _BLEADVERTISING_H_ */
