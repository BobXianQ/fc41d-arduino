#include <Arduino.h>
#include "WiFi.h"
#include "WiFiGeneric.h"
#include "WiFiScan.h"

extern "C" {
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <ql_hal.h>
#include <lwip/ip_addr.h>
#include "lwip/err.h"
}

bool WiFiScanClass::_scanAsync = false;
uint32_t WiFiScanClass::_scanStarted = 0;
uint32_t WiFiScanClass::_scanTimeout = 10000;
uint16_t WiFiScanClass::_scanCount = 0;
void* WiFiScanClass::_scanResult = 0;

void WiFiScanClass::_scanDone()
{
    bk_wifi_scan_get_ap_num(&(WiFiScanClass::_scanCount));
    if(WiFiScanClass::_scanCount) {
        WiFiScanClass::_scanResult = new wifi_ap_record_t[WiFiScanClass::_scanCount];
        if(!WiFiScanClass::_scanResult || bk_wifi_scan_get_ap_records(&(WiFiScanClass::_scanCount), (wifi_ap_record_t*)_scanResult) != BK_OK) {
            WiFiScanClass::_scanCount = 0;
        }
    }
    WiFiScanClass::_scanStarted=0; 
    WiFiGenericClass::setStatusBits(WIFI_SCAN_DONE_BIT);
    WiFiGenericClass::clearStatusBits(WIFI_SCANNING_BIT);
}

int16_t WiFiScanClass::scanComplete()
{
    if (WiFiScanClass::_scanStarted && (millis()-WiFiScanClass::_scanStarted) > WiFiScanClass::_scanTimeout) { //Check is scan was started and if the delay expired, return WIFI_SCAN_FAILED in this case 
    	WiFiGenericClass::clearStatusBits(WIFI_SCANNING_BIT);
	return WIFI_SCAN_FAILED;
    }

    if(WiFiGenericClass::getStatusBits() & WIFI_SCAN_DONE_BIT) {
        return WiFiScanClass::_scanCount;
    }

    if(WiFiGenericClass::getStatusBits() & WIFI_SCANNING_BIT) {
        return WIFI_SCAN_RUNNING;
    }

    return WIFI_SCAN_FAILED;
}

void WiFiScanClass::scanDelete()
{
    WiFiGenericClass::clearStatusBits(WIFI_SCAN_DONE_BIT);
    if(WiFiScanClass::_scanResult) {
        delete[] reinterpret_cast<wifi_ap_record_t*>(WiFiScanClass::_scanResult);
        WiFiScanClass::_scanResult = 0;
        WiFiScanClass::_scanCount = 0;
    }
}

int16_t WiFiScanClass::scanNetworks(bool async, bool show_hidden, bool passive, uint32_t max_ms_per_chan, uint8_t channel, const char * ssid, const uint8_t * bssid)
{
    if(WiFiGenericClass::getStatusBits() & WIFI_SCANNING_BIT) {
        return WIFI_SCAN_RUNNING;
    }

    WiFiScanClass::_scanTimeout = max_ms_per_chan * 20;
    WiFiScanClass::_scanAsync = async;

    WiFi.enableSTA(true);

    scanDelete();

    if(bk_wifi_scan_start() == BK_OK) {
        _scanStarted = millis();
        if (!_scanStarted) { //Prevent 0 from millis overflow
	    ++_scanStarted;
	}

    WiFiGenericClass::clearStatusBits(WIFI_SCAN_DONE_BIT);
    WiFiGenericClass::setStatusBits(WIFI_SCANNING_BIT);

    if(WiFiScanClass::_scanAsync) {
        return WIFI_SCAN_RUNNING;
    }

    if(WiFiGenericClass::waitStatusBits(WIFI_SCAN_DONE_BIT, 10000)){
        return (int16_t) WiFiScanClass::_scanCount;
    }
    }
    
    return WIFI_SCAN_FAILED;
}



void * WiFiScanClass::_getScanInfoByIndex(int i)
{
    if(!WiFiScanClass::_scanResult || (size_t) i >= WiFiScanClass::_scanCount) {
        return 0;
    }
    return reinterpret_cast<wifi_ap_record_t*>(WiFiScanClass::_scanResult) + i;
}


bool WiFiScanClass::getNetworkInfo(uint8_t i, String &ssid, uint8_t &encType, int32_t &rssi, uint8_t* &bssid, int32_t &channel)
{
    wifi_ap_record_t* it = reinterpret_cast<wifi_ap_record_t*>(_getScanInfoByIndex(i));
    if(!it) {
        return false;
    }
    ssid = (const char*) it->ssid;
    encType = it->authmode;
    rssi = it->rssi;
    bssid = it->bssid;
    channel = it->channel;
    return true;
}

String WiFiScanClass::SSID(uint8_t i)
{
    wifi_ap_record_t* it = reinterpret_cast<wifi_ap_record_t*>(_getScanInfoByIndex(i));
    if(!it) {
        return String();
    }
    return String(reinterpret_cast<const char*>(it->ssid));
}


wifi_auth_mode_t WiFiScanClass::encryptionType(uint8_t i)
{
    wifi_ap_record_t* it = reinterpret_cast<wifi_ap_record_t*>(_getScanInfoByIndex(i));
    if(!it) {
        return WIFI_AUTH_OPEN;
    }
    return it->authmode;
}

int32_t WiFiScanClass::RSSI(uint8_t i)
{
    wifi_ap_record_t* it = reinterpret_cast<wifi_ap_record_t*>(_getScanInfoByIndex(i));
    if(!it) {
        return 0;
    }
    return (int32_t)it->rssi;
}


uint8_t * WiFiScanClass::BSSID(uint8_t i)
{
    wifi_ap_record_t* it = reinterpret_cast<wifi_ap_record_t*>(_getScanInfoByIndex(i));
    if(!it) {
        return 0;
    }
    return it->bssid;
}

String WiFiScanClass::BSSIDstr(uint8_t i)
{
    char mac[18] = { 0 };
    wifi_ap_record_t* it = reinterpret_cast<wifi_ap_record_t*>(_getScanInfoByIndex(i));
    if(!it) {
        return String();
    }
    sprintf(mac, "%02X:%02X:%02X:%02X:%02X:%02X", it->bssid[0], it->bssid[1], it->bssid[2], it->bssid[3], it->bssid[4], it->bssid[5]);
    return String(mac);
}

int32_t WiFiScanClass::channel(uint8_t i)
{
    wifi_ap_record_t* it = reinterpret_cast<wifi_ap_record_t*>(_getScanInfoByIndex(i));
    if(!it) {
        return 0;
    }
    return it->channel;
}

