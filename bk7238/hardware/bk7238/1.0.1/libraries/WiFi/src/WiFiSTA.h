#ifndef WIFISTA_H_
#define WIFISTA_H_


#include "WiFiType.h"
#include "WiFiGeneric.h"

typedef enum {
    WPA2_AUTH_TLS = 0,
    WPA2_AUTH_PEAP = 1,
    WPA2_AUTH_TTLS = 2
} wpa2_auth_method_t;

class WiFiSTAClass
{
public:
    wl_status_t begin(const char* wpa2_ssid, wpa2_auth_method_t method, const char* wpa2_identity=NULL, const char* wpa2_username=NULL, const char *wpa2_password=NULL, const char* ca_pem=NULL, const char* client_crt=NULL, const char* client_key=NULL, int32_t channel=0, const uint8_t* bssid=0, bool connect=true);
    wl_status_t begin(const String& wpa2_ssid, wpa2_auth_method_t method, const String& wpa2_identity = (const char*)NULL, const String& wpa2_username = (const char*)NULL, const String& wpa2_password = (const char*)NULL, const String& ca_pem = (const char*)NULL, const String& client_crt = (const char*)NULL, const String& client_key = (const char*)NULL, int32_t channel=0, const uint8_t* bssid=0, bool connect=true) {
        return begin(wpa2_ssid.c_str(), method, wpa2_identity.c_str(), wpa2_username.c_str(), wpa2_password.c_str(), ca_pem.c_str(), client_crt.c_str(), client_key.c_str(), channel, bssid, connect);
    }
    wl_status_t begin(const char* ssid, const char *passphrase = NULL, int32_t channel = 0, const uint8_t* bssid = NULL, bool connect = true);
    wl_status_t begin(const String& ssid, const String& passphrase = (const char*)NULL, int32_t channel = 0, const uint8_t* bssid = NULL, bool connect = true) {
        return begin(ssid.c_str(), passphrase.c_str(), channel, bssid, connect);
    }
    wl_status_t begin(char* ssid, char *passphrase = NULL, int32_t channel = 0, const uint8_t* bssid = NULL, bool connect = true);
    wl_status_t begin();

    bool config(IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dns1 = (uint32_t)0x00000000, IPAddress dns2 = (uint32_t)0x00000000);
    bool reconnect();
    bool disconnect(bool wifioff = false, bool eraseap = false);
    bool eraseAP(void);
    bool isConnected();
    uint8_t waitForConnectResult(unsigned long timeoutLength = 60000);
    IPAddress localIP();
    uint8_t * macAddress(uint8_t* mac);
    String macAddress();
    IPAddress subnetMask();
    IPAddress gatewayIP();
    IPAddress dnsIP(uint8_t dns_no = 0);
    IPAddress broadcastIP();
    IPAddress networkID();
    uint8_t subnetCIDR();
    static wl_status_t status();
    String SSID() const;
    String psk() const;
    String BSSIDstr();
    uint8_t * BSSID();
    int8_t RSSI();
    static void _setStatus(wl_status_t status);
};

#endif /* WIFISTA_H_ */
