#ifndef WIFISCAN_H_
#define WIFISCAN_H_

#include "WiFiType.h"
#include "WiFiGeneric.h"

class WiFiScanClass
{

public:

    int16_t scanNetworks(bool async = false, bool show_hidden = false, bool passive = false, uint32_t max_ms_per_chan = 300, uint8_t channel = 0, const char * ssid=nullptr, const uint8_t * bssid=nullptr);
    int16_t scanComplete();
    void scanDelete();
    bool getNetworkInfo(uint8_t networkItem, String &ssid, uint8_t &encryptionType, int32_t &RSSI, uint8_t* &BSSID, int32_t &channel);

    String SSID(uint8_t networkItem);
    wifi_auth_mode_t encryptionType(uint8_t networkItem);
    int32_t RSSI(uint8_t networkItem);
    uint8_t * BSSID(uint8_t networkItem);
    String BSSIDstr(uint8_t networkItem);
    int32_t channel(uint8_t networkItem);
    static void * getScanInfoByIndex(int i) { return _getScanInfoByIndex(i); }; 
    static void _scanDone();

protected:
    static bool _scanAsync;
    static uint32_t _scanStarted;
    static uint32_t _scanTimeout;
    static uint16_t _scanCount;
    static void* _scanResult;
    static void * _getScanInfoByIndex(int i);
};


#endif /* WIFISCAN_H_ */
