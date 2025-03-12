#include "WiFi.h"
#include "WiFiGeneric.h"
#include "WiFiSTA.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <lwip/ip_addr.h>
#include "lwip/err.h"
#include "lwip/dns.h"
#include "FreeRTOS.h"
#include "event_groups.h"
#include "Arduino.h"

static wl_status_t _sta_status = WL_NO_SHIELD;
static EventGroupHandle_t _sta_status_group = NULL;

void WiFiSTAClass::_setStatus(wl_status_t status)
{
    if(!_sta_status_group){
        _sta_status_group = xEventGroupCreate();
        if(!_sta_status_group){
            bk_printf("STA Status Group Create Failed!");
            _sta_status = status;
            return;
        }
    }
    xEventGroupClearBits(_sta_status_group, 0x00FFFFFF);
    xEventGroupSetBits(_sta_status_group, status);
}

wl_status_t WiFiSTAClass::status()
{
    if(!_sta_status_group){
        return _sta_status;
    }
    return (wl_status_t)xEventGroupClearBits(_sta_status_group, 0);
}

wl_status_t WiFiSTAClass::begin(const char* wpa2_ssid, wpa2_auth_method_t method, const char* wpa2_identity, const char* wpa2_username, const char *wpa2_password, const char* ca_pem, const char* client_crt, const char* client_key, int32_t channel, const uint8_t* bssid, bool connect)
{
    return begin(wpa2_ssid, wpa2_password, channel, bssid, connect);
}

wl_status_t WiFiSTAClass::begin(const char* ssid, const char *passphrase, int32_t channel, const uint8_t* bssid, bool connect)
{
    if(!WiFi.enableSTA(true)) {
        bk_printf("STA enable failed!");
        return WL_CONNECT_FAILED;
    }

    if(!ssid || *ssid == 0x00 || strlen(ssid) > 32) {
        bk_printf("SSID too long or missing!");
        return WL_CONNECT_FAILED;
    }

    if(passphrase && strlen(passphrase) > 64) {
        bk_printf("passphrase too long!");
        return WL_CONNECT_FAILED;
    }

    if(connect){
    	if(bk_wifi_connect(ssid,passphrase) != BK_OK) {
			bk_printf("connect failed!");
			return WL_CONNECT_FAILED;
		}
    }

    return status();
}

wl_status_t WiFiSTAClass::begin(char* ssid, char *passphrase, int32_t channel, const uint8_t* bssid, bool connect)
{
    return begin((const char*) ssid, (const char*) passphrase, channel, bssid, connect);
}

wl_status_t WiFiSTAClass::begin()
{
    return status();
}

bool WiFiSTAClass::reconnect()
{
    return false;
}


bool WiFiSTAClass::disconnect(bool wifioff, bool eraseap)
{

    if(WiFi.getMode() & WIFI_MODE_STA){
        if(bk_wifi_disconnect()==BK_FAIL){
            bk_printf("disconnect failed!\r\n");
            return false;
        }

        if(wifioff) {
             return WiFi.enableSTA(false);
        }
        return true;
    }

    return false;
}


bool WiFiSTAClass::eraseAP(void) {

    return BK_OK;
}


bool WiFiSTAClass::config(IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dns1, IPAddress dns2)
{
    bk_err_t err = BK_OK;
    bk_netif_ip_str_info_t config;

    if(!WiFi.enableSTA(true)) {
        return false;
    }

    strcpy(config.ip,local_ip.toString().c_str());
    strcpy(config.gw,gateway.toString().c_str());
    strcpy(config.netmask,subnet.toString().c_str());
    strcpy(config.dns1,dns1.toString().c_str());
    strcpy(config.dns2,dns2.toString().c_str());

    bk_printf("ip:%s,Gateway: %s,Netmask: %s,dns:%s\r\n", local_ip.toString().c_str(), gateway.toString().c_str(), subnet.toString().c_str(), dns1.toString().c_str());

    bk_netif_set_ip_info(WIFI_MODE_STA,&config);

    return err == BK_OK;
}

bool WiFiSTAClass::isConnected()
{
    return (status() == WL_CONNECTED);
}

uint8_t WiFiSTAClass::waitForConnectResult(unsigned long timeoutLength)
{
    if((WiFiGenericClass::getMode() & WIFI_MODE_STA) == 0) {
        return WL_DISCONNECTED;
    }

    unsigned long start = millis();
    while((!status() || status() >= WL_DISCONNECTED) && (millis() - start) < timeoutLength) {
        delay(100);
    }
    return status();
}

IPAddress WiFiSTAClass::localIP()
{
    bk_netif_ip_info_t ip_info;

    if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
        return IPAddress();
    }

    if(bk_netif_get_ip_info(WIFI_MODE_STA,&ip_info) != BK_OK){
    	bk_printf("Netif Get IP Failed!\r\n");
    	return IPAddress();
    }

    return IPAddress(ip_info.ip.addr);
}

uint8_t* WiFiSTAClass::macAddress(uint8_t* mac)
{
    bk_wifi_get_station_mac_address((char *)mac);

    return mac;
}

String WiFiSTAClass::macAddress(void)
{
    uint8_t mac[6] = { 0 };
    bk_wifi_get_station_mac_address((char *)&mac);
    char macStr[18] = { 0 };
    sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return String(macStr);
}

IPAddress WiFiSTAClass::subnetMask()
{
    bk_netif_ip_info_t ip_info;

    if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
        return IPAddress();
    }

    if(bk_netif_get_ip_info(WIFI_MODE_STA,&ip_info) != BK_OK){
    	bk_printf("Netif Get IP Failed!\r\n");
    	return IPAddress();
    }

    return IPAddress(ip_info.netmask.addr);
}

IPAddress WiFiSTAClass::gatewayIP()
{
    bk_netif_ip_info_t ip_info;

    if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
        return IPAddress();
    }

    if(bk_netif_get_ip_info(WIFI_MODE_STA,&ip_info) != BK_OK){
    	bk_printf("Netif Get IP Failed!\r\n");
    	return IPAddress();
    }

    return IPAddress(ip_info.gw.addr);
}

IPAddress WiFiSTAClass::dnsIP(uint8_t dns_no)
{
    bk_netif_ip_info_t ip_info;

    if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
        return IPAddress();
    }

    if(bk_netif_get_ip_info(WIFI_MODE_STA,&ip_info) != BK_OK){
    	bk_printf("Netif Get IP Failed!\r\n");
    	return IPAddress();
    }

    return IPAddress(ip_info.dns.addr);
}

IPAddress WiFiSTAClass::broadcastIP()
{
    bk_netif_ip_info_t ip_info;

    if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
        return IPAddress();
    }

    if(bk_netif_get_ip_info(WIFI_MODE_STA,&ip_info) != BK_OK){
    	bk_printf("Netif Get IP Failed!\r\n");
    	return IPAddress();
    }

    return WiFiGenericClass::calculateBroadcast(IPAddress(ip_info.gw.addr), IPAddress(ip_info.netmask.addr));
}

IPAddress WiFiSTAClass::networkID()
{
    bk_netif_ip_info_t ip_info;

    if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
        return IPAddress();
    }

    if(bk_netif_get_ip_info(WIFI_MODE_STA,&ip_info) != BK_OK){
    	bk_printf("Netif Get IP Failed!\r\n");
    	return IPAddress();
    }

    return WiFiGenericClass::calculateNetworkID(IPAddress(ip_info.gw.addr), IPAddress(ip_info.netmask.addr));
}

uint8_t WiFiSTAClass::subnetCIDR()
{

    bk_netif_ip_info_t ip_info;

    if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
        return (uint8_t)0;
    }

    if(bk_netif_get_ip_info(WIFI_MODE_STA,&ip_info) != BK_OK){
    	bk_printf("Netif Get IP Failed!\r\n");
    	return IPAddress();
    }

    return WiFiGenericClass::calculateSubnetCIDR(IPAddress(ip_info.netmask.addr));
}

String WiFiSTAClass::SSID() const
{
    wifi_ap_record_t info;

    if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
        return String();
    }
    
    if(bk_wifi_sta_get_ap_info(&info)!=BK_OK) {
        return String();    
    }

    return String(reinterpret_cast<char*>(info.ssid));
}

String WiFiSTAClass::psk() const
{
    wifi_ap_record_t info;
    if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
        return String();
    }

    if(bk_wifi_sta_get_ap_info(&info)!=BK_OK) {
        return String();
    }

    return String(reinterpret_cast<char*>(info.password));
}

uint8_t* WiFiSTAClass::BSSID(void)
{
    static uint8_t bssid[6];
    wifi_ap_record_t info;
    if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
        return NULL;
    }

    if(bk_wifi_sta_get_ap_info(&info)!=BK_OK) {
        return NULL;
    }

    memcpy(bssid, info.bssid, 6);
    return reinterpret_cast<uint8_t*>(bssid);
}

String WiFiSTAClass::BSSIDstr(void)
{
    uint8_t* bssid = BSSID();
    if(!bssid){
        return String();
    }
    char mac[18] = { 0 };
    sprintf(mac, "%02X:%02X:%02X:%02X:%02X:%02X", bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);
    return String(mac);
}

int8_t WiFiSTAClass::RSSI(void)
{
    if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
        return 0;
    }

    wifi_ap_record_t info;

    if(bk_wifi_sta_get_ap_info(&info)!=BK_OK) {
        return 0;
    }

    return info.rssi;
}

