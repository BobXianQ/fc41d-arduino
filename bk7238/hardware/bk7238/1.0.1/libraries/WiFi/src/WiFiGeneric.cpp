#include "Arduino.h"
#include "WiFi.h"
#include "WiFiGeneric.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "projdefs.h"
#include "portmacro.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "event_groups.h"
#include "bk_hal_wifi.h"
#include "bk_err.h"
#include "lwip/ip_addr.h"
#include "lwip/opt.h"
#include "lwip/err.h"
#include "lwip/dns.h"
#include "wlan_ui_pub.h"
#include "rw_msg_pub.h"
#include "lwip/err.h"
#include <vector>
#include "lwip_netif_address.h"
#include "net.h"

typedef struct WiFiEventCbList {
    static wifi_event_id_t current_id;
    wifi_event_id_t id;
    WiFiEventCb cb;
    WiFiEventFuncCb fcb;
    WiFiEventSysCb scb;
    arduino_event_id_t event;

    WiFiEventCbList() : id(current_id++), cb(NULL), fcb(NULL), scb(NULL), event(ARDUINO_EVENT_WIFI_READY) {}
} WiFiEventCbList_t;

static bool _bk_wifi_started = false;
static QueueHandle_t _arduino_event_queue;
static TaskHandle_t _arduino_event_task_handle = NULL;
static EventGroupHandle_t _arduino_event_group = NULL;
static bool lowLevelInitDone = false;
static std::vector<WiFiEventCbList_t> cbEventList;

bool WiFiGenericClass::_sleepEnabled = false;
wifi_event_id_t WiFiEventCbList::current_id = 1;

static void _arduino_event_task(void * arg){
	arduino_event_t *data = NULL;
    for (;;) {
        if(xQueueReceive(_arduino_event_queue, &data, portMAX_DELAY) == pdTRUE){
            WiFiGenericClass::_eventCallback(data);
            free(data);
            data = NULL;
        }
    }
    vTaskDelete(NULL);
    _arduino_event_task_handle = NULL;
}

bk_err_t postArduinoEvent(arduino_event_t *data)
{
	if(data == NULL){
        return BK_FAIL;
	}
	arduino_event_t * event = (arduino_event_t*)malloc(sizeof(arduino_event_t));
	if(event == NULL){
        bk_printf("Arduino Event Malloc Failed!\r\n");
        return BK_FAIL;
	}
	memcpy(event, data, sizeof(arduino_event_t));
    if (xQueueSend(_arduino_event_queue, &event, portMAX_DELAY) != pdPASS) {
        bk_printf("Arduino Event Send Failed!\r\n");
        return BK_FAIL;
    }
    return BK_OK;
}

static void app_demo_sta_rw_event_func(void *new_evt)
{
    arduino_event_t arduino_event;
	arduino_event.event_id = ARDUINO_EVENT_MAX;
    rw_evt_type evt_type = *((rw_evt_type *)new_evt);

    switch(evt_type)
    {
        case RW_EVT_STA_CONNECTED:
            arduino_event.event_id = ARDUINO_EVENT_WIFI_STA_CONNECTED;
            LinkStatusTypeDef link_conf;

            memset(&link_conf, 0, sizeof(link_conf));
            bk_wlan_get_link_status(&link_conf);
            memcpy(arduino_event.event_info.wifi_sta_connected.ssid,link_conf.ssid,strlen((char*)link_conf.ssid));
            memcpy(arduino_event.event_info.wifi_sta_connected.ssid,link_conf.bssid,sizeof(link_conf.bssid));
            
            arduino_event.event_info.wifi_sta_connected.ssid_len = strlen((char*)link_conf.ssid);
            arduino_event.event_info.wifi_sta_connected.channel = link_conf.channel;
            arduino_event.event_info.wifi_sta_connected.authmode = (wifi_auth_mode_t )link_conf.security;
            break;
          
        case RW_EVT_STA_GOT_IP:
            arduino_event.event_id = ARDUINO_EVENT_WIFI_STA_GOT_IP;
            struct wlan_ip_config addr;
            net_get_if_addr(&addr, net_get_sta_handle());

            ip_addr_set_ip4_u32(&arduino_event.event_info.got_ip.ip_info.gw,addr.ipv4.gw);
            ip_addr_set_ip4_u32(&arduino_event.event_info.got_ip.ip_info.ip, addr.ipv4.address);
            ip_addr_set_ip4_u32(&arduino_event.event_info.got_ip.ip_info.netmask, addr.ipv4.netmask);
            ip_addr_set_ip4_u32(&arduino_event.event_info.got_ip.ip_info.dns, addr.ipv4.dns1); 
            break;    

        case RW_EVT_SCAN_DONE:  
            arduino_event.event_id = ARDUINO_EVENT_WIFI_SCAN_DONE;
            break;  

        case RW_EVT_STA_BEACON_LOSE:
        case RW_EVT_STA_NO_AP_FOUND:
        case RW_EVT_STA_PASSWORD_WRONG:
        case RW_EVT_STA_CONNECT_FAILED:
        case RW_EVT_STA_DISCONNECTED:
            arduino_event.event_id = ARDUINO_EVENT_WIFI_STA_DISCONNECTED;
            break;  

        case RW_EVT_AP_CONNECTED:
            arduino_event.event_id = ARDUINO_EVENT_WIFI_AP_STACONNECTED;
            break;

        case RW_EVT_AP_CONNECT_FAILED:    
        case RW_EVT_AP_DISCONNECTED:
            arduino_event.event_id = ARDUINO_EVENT_WIFI_AP_STADISCONNECTED;
            break;

        default:
            break;
    }

    if(arduino_event.event_id < ARDUINO_EVENT_MAX){
		postArduinoEvent(&arduino_event);
	}
}

static bool _start_network_event_task(){
    if(!_arduino_event_group){
        _arduino_event_group = xEventGroupCreate();
        if(!_arduino_event_group){
            bk_printf("Network Event Group Create Failed!");
            return false;
        }
        xEventGroupSetBits(_arduino_event_group, WIFI_DNS_IDLE_BIT);
    }
    if(!_arduino_event_queue){
    	_arduino_event_queue = xQueueCreate(32, sizeof(arduino_event_t*));
        if(!_arduino_event_queue){
            bk_printf("Network Event Queue Create Failed!");
            return false;
        }
    }

    bk_wlan_status_register_cb(app_demo_sta_rw_event_func);

    if(!_arduino_event_task_handle){
        xTaskCreateUniversal(_arduino_event_task, "arduino_events", 4096, NULL, 5, &_arduino_event_task_handle, 1);
        if(!_arduino_event_task_handle){
            bk_printf("Network Event Task Start Failed!");
            return false;
        }
    }

    return true;
}

bool tcpipInit(){
    static bool initialized = false;
    if(!initialized){
        initialized = true;

        if(initialized){
        	initialized = _start_network_event_task();
        } else {
        	bk_printf("netif_init failed!\r\n");
        }
    }
    return initialized;
}

bool wifiLowLevelInit(){
    if(!lowLevelInitDone){
        lowLevelInitDone = true;
        if(!tcpipInit()){
        	lowLevelInitDone = false;
        	return lowLevelInitDone;
        }

        if(lowLevelInitDone){
			arduino_event_t arduino_event;
			arduino_event.event_id = ARDUINO_EVENT_WIFI_READY;
			postArduinoEvent(&arduino_event);
        }
    }
    return lowLevelInitDone;
}

static bool wifiLowLevelDeinit(){
    if(lowLevelInitDone){
    	lowLevelInitDone = !(bk_wifi_deinit() == BK_OK);
    }
    return !lowLevelInitDone;
}

static bool bkWiFiStart(){
    if(_bk_wifi_started){
        return true;
    }
    _bk_wifi_started = true;
    uint8_t err = bk_wifi_start();
    if (err != 0) {
        _bk_wifi_started = false;
        bk_printf("bk_wifi_started %d\r\n", err);
        return _bk_wifi_started;
    }
    return _bk_wifi_started;
}

static bool bkWiFiStop(){
    bk_err_t err;
    if(!_bk_wifi_started){
        return true;
    }
    _bk_wifi_started = false;
    err = bk_wifi_stop();
    if(err){
        bk_printf("Could not stop WiFi! %d\r\n", err);
        _bk_wifi_started = true;
        return false;
    }
    return wifiLowLevelDeinit();
}

int WiFiGenericClass::setStatusBits(int bits){
    if(!_arduino_event_group){
        return 0;
    }
    return xEventGroupSetBits(_arduino_event_group, bits);
}

int WiFiGenericClass::clearStatusBits(int bits){
    if(!_arduino_event_group){
        return 0;
    }
    return xEventGroupClearBits(_arduino_event_group, bits);
}

int WiFiGenericClass::getStatusBits(){
    if(!_arduino_event_group){
        return 0;
    }
    return xEventGroupGetBits(_arduino_event_group);
}

int WiFiGenericClass::waitStatusBits(int bits, uint32_t timeout_ms){
    if(!_arduino_event_group){
        return 0;
    }

    return xEventGroupWaitBits(_arduino_event_group, bits, pdFALSE, pdTRUE, timeout_ms / portTICK_PERIOD_MS ) & bits;
}

wifi_event_id_t WiFiGenericClass::onEvent(WiFiEventCb cbEvent, arduino_event_id_t event)
{
    if(!cbEvent) {
        return 0;
    }
    WiFiEventCbList_t newEventHandler;
    newEventHandler.cb = cbEvent;
    newEventHandler.fcb = NULL;
    newEventHandler.scb = NULL;
    newEventHandler.event = event;
    cbEventList.push_back(newEventHandler);
    return newEventHandler.id;
}

wifi_event_id_t WiFiGenericClass::onEvent(WiFiEventFuncCb cbEvent, arduino_event_id_t event)
{
    if(!cbEvent) {
        return 0;
    }
    WiFiEventCbList_t newEventHandler;
    newEventHandler.cb = NULL;
    newEventHandler.fcb = cbEvent;
    newEventHandler.scb = NULL;
    newEventHandler.event = event;
    cbEventList.push_back(newEventHandler);
    return newEventHandler.id;
}

wifi_event_id_t WiFiGenericClass::onEvent(WiFiEventSysCb cbEvent, arduino_event_id_t event)
{
    if(!cbEvent) {
        return 0;
    }
    WiFiEventCbList_t newEventHandler;
    newEventHandler.cb = NULL;
    newEventHandler.fcb = NULL;
    newEventHandler.scb = cbEvent;
    newEventHandler.event = event;
    cbEventList.push_back(newEventHandler);
    return newEventHandler.id;
}

void WiFiGenericClass::removeEvent(WiFiEventCb cbEvent, arduino_event_id_t event)
{
    if(!cbEvent) {
        return;
    }

    for(uint32_t i = 0; i < cbEventList.size(); i++) {
        WiFiEventCbList_t entry = cbEventList[i];
        if(entry.cb == cbEvent && entry.event == event) {
            cbEventList.erase(cbEventList.begin() + i);
        }
    }
}

void WiFiGenericClass::removeEvent(WiFiEventSysCb cbEvent, arduino_event_id_t event)
{
    if(!cbEvent) {
        return;
    }

    for(uint32_t i = 0; i < cbEventList.size(); i++) {
        WiFiEventCbList_t entry = cbEventList[i];
        if(entry.scb == cbEvent && entry.event == event) {
            cbEventList.erase(cbEventList.begin() + i);
        }
    }
}

void WiFiGenericClass::removeEvent(wifi_event_id_t id)
{
    for(uint32_t i = 0; i < cbEventList.size(); i++) {
        WiFiEventCbList_t entry = cbEventList[i];
        if(entry.id == id) {
            cbEventList.erase(cbEventList.begin() + i);
        }
    }
}

bk_err_t WiFiGenericClass::_eventCallback(arduino_event_t *event)
{
    if(!event) return BK_OK;                                                     
    
    if(event->event_id == ARDUINO_EVENT_WIFI_SCAN_DONE) {
        WiFiScanClass::_scanDone();
    } else if(event->event_id == ARDUINO_EVENT_WIFI_STA_START) {
        WiFiSTAClass::_setStatus(WL_DISCONNECTED);
        setStatusBits(STA_STARTED_BIT);
    } else if(event->event_id == ARDUINO_EVENT_WIFI_STA_STOP) {
        WiFiSTAClass::_setStatus(WL_NO_SHIELD);
        clearStatusBits(STA_STARTED_BIT | STA_CONNECTED_BIT | STA_HAS_IP_BIT | STA_HAS_IP6_BIT);
    } else if(event->event_id == ARDUINO_EVENT_WIFI_STA_CONNECTED) {
        WiFiSTAClass::_setStatus(WL_IDLE_STATUS);
        setStatusBits(STA_CONNECTED_BIT);
    } else if(event->event_id == ARDUINO_EVENT_WIFI_STA_DISCONNECTED) {
        WiFiSTAClass::_setStatus(WL_DISCONNECTED);
        clearStatusBits(STA_CONNECTED_BIT | STA_HAS_IP_BIT | STA_HAS_IP6_BIT);
    } else if(event->event_id == ARDUINO_EVENT_WIFI_STA_GOT_IP) {
        WiFiSTAClass::_setStatus(WL_CONNECTED);
        setStatusBits(STA_HAS_IP_BIT | STA_CONNECTED_BIT);
    } else if(event->event_id == ARDUINO_EVENT_WIFI_AP_START) {
        setStatusBits(AP_STARTED_BIT);
    } else if(event->event_id == ARDUINO_EVENT_WIFI_AP_STOP) {
        clearStatusBits(AP_STARTED_BIT | AP_HAS_CLIENT_BIT);
    } else if(event->event_id == ARDUINO_EVENT_WIFI_AP_STACONNECTED) {
        setStatusBits(AP_HAS_CLIENT_BIT);
    } else if(event->event_id == ARDUINO_EVENT_WIFI_AP_STADISCONNECTED) {
        clearStatusBits(AP_HAS_CLIENT_BIT);
    } 

    for(uint32_t i = 0; i < cbEventList.size(); i++) {
        WiFiEventCbList_t entry = cbEventList[i];
        if(entry.cb || entry.fcb || entry.scb) {
            if(entry.event == (arduino_event_id_t) event->event_id || entry.event == ARDUINO_EVENT_MAX) {
                if(entry.cb) {
                    entry.cb((arduino_event_id_t) event->event_id);
                } else if(entry.fcb) {
                    entry.fcb((arduino_event_id_t) event->event_id, (arduino_event_info_t) event->event_info);
                } else {
                    entry.scb(event);
                }
            }
        }
    }
    return BK_OK;
}

int32_t WiFiGenericClass::channel(void)
{
    if(!lowLevelInitDone){
        return 0;
    }

    return bk_wifi_get_channel();
}

bool WiFiGenericClass::mode(wifi_mode_t m)
{
    wifi_mode_t cm = getMode();
    int err;
    if(cm == m) {
        return true;
    }
    if(!cm && m){
        if(!wifiLowLevelInit()){
            return false;
        }
    } else if(cm && !m){
        return bkWiFiStop();
    }

    err = bk_wifi_set_mode(m);
    if(err){
        bk_printf("Could not set mode! %d", err);
        return false;
    }

    if(!bkWiFiStart()){
        return false;
    }

    return true;
}

wifi_mode_t WiFiGenericClass::getMode()
{
    if(!lowLevelInitDone || !_bk_wifi_started){
        return WIFI_MODE_NULL;
    }
    wifi_mode_t mode;
    if(bk_wifi_get_mode(&mode) != 0){
        bk_printf("WiFi not started");
        return WIFI_MODE_NULL;
    }
    return mode;
}

bool WiFiGenericClass::enableSTA(bool enable)
{
    wifi_mode_t currentMode = getMode();
    bool isEnabled = ((currentMode & WIFI_MODE_STA) != 0);

    if(isEnabled != enable) {
        if(enable) {
            return mode((wifi_mode_t)(currentMode | WIFI_MODE_STA));
        }
        return mode((wifi_mode_t)(currentMode & (~WIFI_MODE_STA)));
    }
    return true;
}

bool WiFiGenericClass::enableAP(bool enable)
{
    wifi_mode_t currentMode = getMode();
    bool isEnabled = ((currentMode & WIFI_MODE_AP) != 0);

    if(isEnabled != enable) {
        if(enable) {
            return mode((wifi_mode_t)(currentMode | WIFI_MODE_AP));
        }
        return mode((wifi_mode_t)(currentMode & (~WIFI_MODE_AP)));
    }
    return true;
}

bool WiFiGenericClass::setSleep(bool enabled)
{
    _sleepEnabled = enabled;
    bk_wifi_set_ps(enabled);
    return true;
}

bool WiFiGenericClass::getSleep()
{
    return _sleepEnabled;
}

static void wifi_dns_found_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg)
{
    if(ipaddr) {
        (*reinterpret_cast<IPAddress*>(callback_arg)) = ipaddr->addr;
    }
    xEventGroupSetBits(_arduino_event_group, WIFI_DNS_DONE_BIT);
}

int WiFiGenericClass::hostByName(const char* aHostname, IPAddress& aResult)
{
    if (!aResult.fromString(aHostname))
    {
        ip_addr_t addr;
        aResult = static_cast<uint32_t>(0);
        waitStatusBits(WIFI_DNS_IDLE_BIT, 16000);
        clearStatusBits(WIFI_DNS_IDLE_BIT | WIFI_DNS_DONE_BIT);
        err_t err = dns_gethostbyname(aHostname, &addr, &wifi_dns_found_callback, &aResult);
        if(err == ERR_OK && addr.addr) {
            aResult = addr.addr;
        } else if(err == ERR_INPROGRESS) {
            waitStatusBits(WIFI_DNS_DONE_BIT, 15000);  
            clearStatusBits(WIFI_DNS_DONE_BIT);
        }
        setStatusBits(WIFI_DNS_IDLE_BIT);
        if((uint32_t)aResult == 0){
            bk_printf("DNS Failed for %s\r\n", aHostname);
        }
    }

    return (uint32_t)aResult != 0;
}

IPAddress WiFiGenericClass::calculateNetworkID(IPAddress ip, IPAddress subnet) {
	IPAddress networkID;

	for (size_t i = 0; i < 4; i++)
		networkID[i] = subnet[i] & ip[i];

	return networkID;
}

IPAddress WiFiGenericClass::calculateBroadcast(IPAddress ip, IPAddress subnet) {
    IPAddress broadcastIp;
    
    for (int i = 0; i < 4; i++)
        broadcastIp[i] = ~subnet[i] | ip[i];

    return broadcastIp;
}

uint8_t WiFiGenericClass::calculateSubnetCIDR(IPAddress subnetMask) {
	uint8_t CIDR = 0;

	for (uint8_t i = 0; i < 4; i++) {
		if (subnetMask[i] == 0x80)  // 128
			CIDR += 1;
		else if (subnetMask[i] == 0xC0)  // 192
			CIDR += 2;
		else if (subnetMask[i] == 0xE0)  // 224
			CIDR += 3;
		else if (subnetMask[i] == 0xF0)  // 242
			CIDR += 4;
		else if (subnetMask[i] == 0xF8)  // 248
			CIDR += 5;
		else if (subnetMask[i] == 0xFC)  // 252
			CIDR += 6;
		else if (subnetMask[i] == 0xFE)  // 254
			CIDR += 7;
		else if (subnetMask[i] == 0xFF)  // 255
			CIDR += 8;
	}

	return CIDR;
}
