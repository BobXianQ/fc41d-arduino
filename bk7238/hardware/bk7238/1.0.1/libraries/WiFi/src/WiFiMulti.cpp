#include <Arduino.h>
#include "WiFiMulti.h"
#include <limits.h>
#include <string.h>
#include <ql_hal.h>

WiFiMulti::WiFiMulti()
{
}

WiFiMulti::~WiFiMulti()
{
    for(uint32_t i = 0; i < APlist.size(); i++) {
        WifiAPlist_t entry = APlist[i];
        if(entry.ssid) {
            free(entry.ssid);
        }
        if(entry.passphrase) {
            free(entry.passphrase);
        }
    }
    APlist.clear();
}

bool WiFiMulti::addAP(const char* ssid, const char *passphrase)
{
    WifiAPlist_t newAP;

    if(!ssid || *ssid == 0x00 || strlen(ssid) > 31) {
        // fail SSID too long or missing!
        bk_printf("[WIFI][APlistAdd] no ssid or ssid too long\r\n");
        return false;
    }

    if(passphrase && strlen(passphrase) > 64) {
        // fail passphrase too long!
        bk_printf("[WIFI][APlistAdd] passphrase too long\r\n");
        return false;
    }

    newAP.ssid = strdup(ssid);

    if(!newAP.ssid) {
        bk_printf("[WIFI][APlistAdd] fail newAP.ssid == 0\r\n");
        return false;
    }

    if(passphrase && *passphrase != 0x00) {
        newAP.passphrase = strdup(passphrase);
        if(!newAP.passphrase) {
            bk_printf("[WIFI][APlistAdd] fail newAP.passphrase == 0\r\n");
            free(newAP.ssid);
            return false;
        }
    } else {
        newAP.passphrase = NULL;
    }

    APlist.push_back(newAP);
    bk_printf("[WIFI][APlistAdd] add SSID: %s\r\n", newAP.ssid);
    return true;
}

uint8_t WiFiMulti::run(uint32_t connectTimeout)
{
    int8_t scanResult;
    uint8_t status = WiFi.status();
    if(status == WL_CONNECTED) {
        for(uint32_t x = 0; x < APlist.size(); x++) {
            if(WiFi.SSID()==APlist[x].ssid) {
                return status;
            }
        }
        bk_printf("WiFi.disconnect\r\n");
        WiFi.disconnect(false,false);
        rtos_delay_milliseconds(10);
        status = WiFi.status();
    }

    scanResult = WiFi.scanNetworks();
    if(scanResult == WIFI_SCAN_RUNNING) {
        // scan is running
        return WL_NO_SSID_AVAIL;
    } else if(scanResult >= 0) {
        // scan done analyze
        WifiAPlist_t bestNetwork { NULL, NULL };
        int bestNetworkDb = INT_MIN;
        uint8_t bestBSSID[6];
        int32_t bestChannel = 0;

        bk_printf("[WIFI] scan done\r\n");

        if(scanResult == 0) {
            bk_printf("[WIFI] no networks found\r\n");
        } else {
            bk_printf("[WIFI] %d networks found\r\n", scanResult);
            for(int8_t i = 0; i < scanResult; ++i) {

                String ssid_scan;
                int32_t rssi_scan;
                uint8_t sec_scan;
                uint8_t* BSSID_scan;
                int32_t chan_scan;

                WiFi.getNetworkInfo(i, ssid_scan, sec_scan, rssi_scan, BSSID_scan, chan_scan);

                bool known = false;
                for(uint32_t x = APlist.size() ; x > 0; x--) {
                    WifiAPlist_t entry = APlist[x-1];

                    if(ssid_scan == entry.ssid) { // SSID match
                        known = true;
                        if(rssi_scan > bestNetworkDb) { // best network
                            if(sec_scan == WIFI_AUTH_OPEN || entry.passphrase) { // check for passphrase if not open wlan
                                bestNetworkDb = rssi_scan;
                                bestChannel = chan_scan;
                                memcpy((void*) &bestNetwork, (void*) &entry, sizeof(bestNetwork));
                                memcpy((void*) &bestBSSID, (void*) BSSID_scan, sizeof(bestBSSID));
                            }
                        }
                        break;
                    }
                }

                if(known) {
                    bk_printf(" --->   %d: [%d][%02X:%02X:%02X:%02X:%02X:%02X] %s (%d) %c\r\n", i, chan_scan, BSSID_scan[0], BSSID_scan[1], BSSID_scan[2], BSSID_scan[3], BSSID_scan[4], BSSID_scan[5], ssid_scan.c_str(), rssi_scan, (sec_scan == WIFI_AUTH_OPEN) ? ' ' : '*');
                } else {
                    bk_printf("       %d: [%d][%02X:%02X:%02X:%02X:%02X:%02X] %s (%d) %c\r\n", i, chan_scan, BSSID_scan[0], BSSID_scan[1], BSSID_scan[2], BSSID_scan[3], BSSID_scan[4], BSSID_scan[5], ssid_scan.c_str(), rssi_scan, (sec_scan == WIFI_AUTH_OPEN) ? ' ' : '*');
                }
            }
        }

        // clean up ram
        WiFi.scanDelete();

        if(bestNetwork.ssid) {
            bk_printf("[WIFI] Connecting BSSID: %02X:%02X:%02X:%02X:%02X:%02X SSID: %s Channel: %d (%d)\r\n", bestBSSID[0], bestBSSID[1], bestBSSID[2], bestBSSID[3], bestBSSID[4], bestBSSID[5], bestNetwork.ssid, bestChannel, bestNetworkDb);

            WiFi.begin(bestNetwork.ssid, bestNetwork.passphrase, bestChannel, bestBSSID);
            status = WiFi.status();

            auto startTime = millis();
            // wait for connection, fail, or timeout
            while(status != WL_CONNECTED && status != WL_NO_SSID_AVAIL && status != WL_CONNECT_FAILED && (millis() - startTime) <= connectTimeout) {
                rtos_delay_milliseconds(10);
                status = WiFi.status();
            }

            switch(status) {
            case WL_CONNECTED:
                bk_printf("[WIFI] Connecting done.\r\n");
                bk_printf("[WIFI] SSID: %s\r\n", WiFi.SSID().c_str());
                bk_printf("[WIFI] IP: %s\r\n", WiFi.localIP().toString().c_str());
                bk_printf("[WIFI] MAC: %s\r\n", WiFi.BSSIDstr().c_str());
                bk_printf("[WIFI] Channel: %d\r\n", WiFi.channel());
                break;
            case WL_NO_SSID_AVAIL:
                bk_printf("[WIFI] Connecting Failed AP not found.\r\n");
                break;
            case WL_CONNECT_FAILED:
                bk_printf("[WIFI] Connecting Failed.\r\n");
                break;
            default:
                bk_printf("[WIFI] Connecting Failed (%d).\r\n", status);
                break;
            }
        } else {
            bk_printf("[WIFI] no matching wifi found!\r\n");
        }
    } else {
        // start scan
        bk_printf("[WIFI] delete old wifi config...\r\n");
        WiFi.disconnect();

        bk_printf("[WIFI] start scan\r\n");
        // scan wifi async mode
        WiFi.scanNetworks(true);
    }

    return status;
}
