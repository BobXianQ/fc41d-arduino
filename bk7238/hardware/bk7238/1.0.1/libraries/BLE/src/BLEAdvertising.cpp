#include "BLEAdvertising.h"

static void arduino_ble_cmd_cb(ble_cmd_t cmd, ble_cmd_param_t *param)
{
	bk_printf("cmd:%d idx:%d status:%d\r\n", cmd, param->cmd_idx, param->status);
}

void BLEAdvertising::setMinInterval(uint16_t mininterval) 
{
	advParams.interval_min = mininterval;
}

void BLEAdvertising::setMaxInterval(uint16_t maxinterval) 
{
	advParams.interval_max = maxinterval;
}

void BLEAdvertising::setAdvertisementData(uint8_t *adv_data,uint8_t len) 
{
	if(actv_idx == 0xff)
	{
		actv_idx = app_ble_get_idle_actv_idx_handle();
		bk_ble_create_advertising(actv_idx, 7, advParams.interval_min, advParams.interval_max, arduino_ble_cmd_cb);
	}
	rtos_delay_milliseconds(100);
	bk_ble_set_adv_data(actv_idx, adv_data, len, arduino_ble_cmd_cb);
	rtos_delay_milliseconds(100);
}


void BLEAdvertising::setScanResponseData(uint8_t *adv_rsp_data,uint8_t len) 
{
	if(actv_idx == 0xff)
	{
		actv_idx = app_ble_get_idle_actv_idx_handle();
		bk_ble_create_advertising(actv_idx, 7, advParams.interval_min, advParams.interval_max, arduino_ble_cmd_cb);
	}

	bk_ble_set_scan_rsp_data(actv_idx, adv_rsp_data, len, arduino_ble_cmd_cb);
	rtos_delay_milliseconds(100);
} 


void BLEAdvertising::start() 
{
	bk_ble_start_advertising(actv_idx, 0, arduino_ble_cmd_cb);
	rtos_delay_milliseconds(100);
} 

void BLEAdvertising::stop() 
{
	bk_ble_stop_advertising(actv_idx, arduino_ble_cmd_cb);
	rtos_delay_milliseconds(100);
} 