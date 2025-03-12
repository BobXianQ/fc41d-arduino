#include "WiFi.h"

extern "C" {
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "wlan_ui_pub.h"
}


void WiFiClass::printDiag(Print& p)
{
    const char* modes[] = { "NULL", "STA", "AP", "STA+AP" };

    wifi_mode_t mode;
    bk_wifi_get_mode(&mode);

    uint8_t primaryChan;
    primaryChan = bk_wifi_get_channel();

    p.print("Mode: ");
    p.println(modes[mode]);

    p.print("Channel: ");
    p.println(primaryChan);


    LinkStatusTypeDef link_conf;
    memset(&link_conf, 0, sizeof(link_conf));
	bk_wlan_get_link_status(&link_conf);

    p.print("SSID (");
    p.print(strlen((char *)link_conf.ssid));
    p.print("): ");
    p.println((char *)link_conf.ssid);
}

WiFiClass WiFi;