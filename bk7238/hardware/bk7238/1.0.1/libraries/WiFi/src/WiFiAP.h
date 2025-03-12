#ifndef WIFIAP_H_
#define WIFIAP_H_

#include "WiFiType.h"
#include "WiFiGeneric.h"

class WiFiAPClass
{
public:
    bool softAP(const char* ssid, const char* passphrase = NULL, int channel = 1, int ssid_hidden = 0, int max_connection = 4, bool ftm_responder = false);
    bool softAP(const String& ssid, const String& passphrase = emptyString, int channel = 1, int ssid_hidden = 0, int max_connection = 4, bool ftm_responder = false) {
       return softAP(ssid.c_str(), passphrase.c_str(), channel, ssid_hidden, max_connection, ftm_responder);
    }

    bool softAPConfig(IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dhcp_lease_start = (uint32_t) 0);
    bool softAPdisconnect(bool wifioff = false);
    uint8_t softAPgetStationNum();
    IPAddress softAPIP();
    IPAddress softAPBroadcastIP();
    IPAddress softAPNetworkID();
    IPAddress softAPSubnetMask();
    uint8_t softAPSubnetCIDR();
    const char * softAPgetHostname();
    bool softAPsetHostname(const char * hostname);
    uint8_t* softAPmacAddress(uint8_t* mac);
    String softAPmacAddress(void);
    String softAPSSID(void) const;
};

#endif /* WIFIAP_H_*/
