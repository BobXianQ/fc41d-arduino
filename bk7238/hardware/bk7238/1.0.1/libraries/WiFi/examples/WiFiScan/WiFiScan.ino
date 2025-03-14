/*
 *  This sketch demonstrates how to scan WiFi networks.
 *  The API is based on the Arduino WiFi Shield library, but has significant changes as newer WiFi functions are supported.
 *  E.g. the return value of `encryptionType()` different because more modern encryption is supported.
 */
#include "WiFi.h"

void setup()
{
    Serial.begin(115200);

    // Set WiFi to station mode and disconnect from an AP if it was previously connected.
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    Serial.println("Setup done");
}

void loop()
{
    Serial.println("Scan start");

    // WiFi.scanNetworks will return the number of networks found.
    int n = WiFi.scanNetworks();
    Serial.println("Scan done");
    if (n == 0) {
        Serial.println("no networks found");
    } else {
        Serial.print(n);
        Serial.println(" networks found");
        Serial.println("Nr | SSID                             | RSSI | CH | Encryption");
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            Serial.printf("%2d",i + 1);
            Serial.print(" | ");
            Serial.printf("%-32.32s", WiFi.SSID(i).c_str());
            Serial.print(" | ");
            Serial.printf("%4d", WiFi.RSSI(i));
            Serial.print(" | ");
            Serial.printf("%2d", WiFi.channel(i));
            Serial.print(" | ");
            switch (WiFi.encryptionType(i))
            {
            case WIFI_AUTH_OPEN:
                Serial.print("open");
                break;
            case WIFI_AUTH_WEP:
                Serial.print("WEP");
                break;
            case WIFI_AUTH_WPA_TKIP:
                Serial.print("WPA TKIP");
                break;
            case WIFI_AUTH_WPA_AES:
                Serial.print("WPA AES");
                break;
            case WIFI_AUTH_WPA_MIXED:
                Serial.print("WPA AES+TKIP");
                break;
            case WIFI_AUTH_WPA2_TKIP:
                Serial.print("WPA2-TKIP");
                break;
            case WIFI_AUTH_WPA2_AES:
                Serial.print("WPA2 AES");
                break;
            case WIFI_AUTH_WPA2_MIXED:
                Serial.print("WPA2 AES+TKIP");
                break;
            case WIFI_AUTH_WPA3_SAE:
                Serial.print("WAP3");
                break;
            case WIFI_AUTH_WPA3_WPA2_MIXED:
                Serial.print("WAP2+WAP3");
                break;
            case WIFI_AUTH_EAP:
                Serial.print("EAP");
                break;
            case WIFI_AUTH_OWE:
                Serial.print("OWE");
                break;
            case WIFI_AUTH_AUTO:
                Serial.print("AUTO");
                break;
            default:
                Serial.print("unknown");
            }
            Serial.println();
            delay(10);
        }
    }
    Serial.println("");

    // Delete the scan result to free memory for code below.
    WiFi.scanDelete();

    // Wait a bit before scanning again.
    delay(5000);
}
