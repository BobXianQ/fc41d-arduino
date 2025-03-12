#include "WiFi.h"
#include "WiFiGeneric.h"
#include "WiFiAP.h"

extern "C" {
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <lwip/ip_addr.h>
#include "lwip/err.h"
#include "lwip/dns.h"
}

bool WiFiAPClass::softAP(const char* ssid, const char* passphrase, int channel, int ssid_hidden, int max_connection, bool ftm_responder)
{
    if(!ssid || *ssid == 0) {
        bk_printf("SSID missing!\r\n");
        return false;
    }

    if(passphrase && (strlen(passphrase) > 0 && strlen(passphrase) < 8)) {
        bk_printf("passphrase too short!\r\n");
        return false;
    }

    if(!WiFi.enableAP(true)) {
        bk_printf("enable AP first!\r\n");
        return false;
    }

    if(BK_FAIL==bk_wifi_start_softap(ssid,passphrase))
    {
        bk_printf("start AP fail!\r\n");
        return false;
    }

    return true;
}


String WiFiAPClass::softAPSSID() const
{
    if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
        return String();
    }
    wifi_ap_config_t info;
    if(bk_wifi_get_ap_param(&info)==BK_OK) {
        return String(reinterpret_cast<char*>(info.ssid));
    }
    return String();
}

bool WiFiAPClass::softAPConfig(IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dhcp_lease_start)
{
    bk_err_t err = BK_OK;
    bk_netif_ip_str_info_t config;

    if(!WiFi.enableAP(true)) {
        // enable AP failed
        return false;
    }

    strcpy(config.ip,local_ip.toString().c_str());
    strcpy(config.gw,gateway.toString().c_str());
    strcpy(config.netmask,subnet.toString().c_str());

    bk_printf("ip:%s,Gateway: %s,Netmask: %s,dns:%s\r\n", local_ip.toString().c_str(), gateway.toString().c_str(), subnet.toString().c_str());

    bk_netif_set_ip_info(WIFI_MODE_AP,&config);

    return err == BK_OK;
}


bool WiFiAPClass::softAPdisconnect(bool wifioff)
{
    bool ret;

    if(wifioff) {
        ret = WiFi.enableAP(false) == BK_OK;
    }

    return ret;
}

uint8_t WiFiAPClass::softAPgetStationNum()
{
    wlan_ap_stas_t clients;
    if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
        return 0;
    }
    if(bk_wifi_ap_get_sta_list(&clients) == BK_OK) {
        return clients.num;
    }
    return 0;
}


IPAddress WiFiAPClass::softAPIP()
{
    bk_netif_ip_info_t ip;

    if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
        return IPAddress();
    }

    if(bk_netif_get_ip_info(WIFI_MODE_AP,&ip) != BK_OK){
    	bk_printf("Netif Get IP Failed!\r\n");
    	return IPAddress();
    }

    return IPAddress(ip.ip.addr);
}


IPAddress WiFiAPClass::softAPBroadcastIP()
{
    bk_netif_ip_info_t ip;

    if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
        return IPAddress();
    }

    if(bk_netif_get_ip_info(WIFI_MODE_AP,&ip) != BK_OK){
    	bk_printf("Netif Get IP Failed!\r\n");
    	return IPAddress();
    }

    return WiFiGenericClass::calculateBroadcast(IPAddress(ip.gw.addr), IPAddress(ip.netmask.addr));
}


IPAddress WiFiAPClass::softAPNetworkID()
{
    bk_netif_ip_info_t ip;

    if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
        return IPAddress();
    }

    if(bk_netif_get_ip_info(WIFI_MODE_AP,&ip) != BK_OK){
    	bk_printf("Netif Get IP Failed!\r\n");
    	return IPAddress();
    }

    return WiFiGenericClass::calculateNetworkID(IPAddress(ip.gw.addr), IPAddress(ip.netmask.addr));
}

IPAddress WiFiAPClass::softAPSubnetMask()
{
    bk_netif_ip_info_t ip;

    if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
        return IPAddress();
    }

    if(bk_netif_get_ip_info(WIFI_MODE_AP,&ip) != BK_OK){
    	bk_printf("Netif Get IP Failed!\r\n");
    	return IPAddress();
    }

    return IPAddress(ip.netmask.addr);
}

uint8_t WiFiAPClass::softAPSubnetCIDR()
{
    return WiFiGenericClass::calculateSubnetCIDR(softAPSubnetMask());
}

uint8_t* WiFiAPClass::softAPmacAddress(uint8_t* mac)
{
    if(WiFiGenericClass::getMode() != WIFI_MODE_NULL){
        bk_wifi_get_softap_mac_address((char*)mac);
    }
    return mac;
}


String WiFiAPClass::softAPmacAddress(void)
{
    uint8_t mac[6];
    char macStr[18] = { 0 };
    if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
        return String();
    }

    softAPmacAddress(mac);

    sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return String(macStr);
}


const char * WiFiAPClass::softAPgetHostname()
{
    const char * hostname = NULL;
    if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
        return hostname;
    }

    hostname = bk_wifi_ap_netif_get_hostname();

    return hostname;
}


bool WiFiAPClass::softAPsetHostname(const char * hostname)
{
    if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
        return false;
    }

    bk_wifi_ap_netif_set_hostname((char *)hostname);
    return true;
}

