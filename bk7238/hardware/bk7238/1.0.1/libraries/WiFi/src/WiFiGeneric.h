#ifndef WIFIGENERIC_H_
#define WIFIGENERIC_H_


#include <functional>
#include "WiFiType.h"
#include "IPAddress.h"
#include "bk_err.h"


typedef enum {
	ARDUINO_EVENT_WIFI_READY = 0,
	ARDUINO_EVENT_WIFI_SCAN_DONE,
	ARDUINO_EVENT_WIFI_STA_START,
	ARDUINO_EVENT_WIFI_STA_STOP,
	ARDUINO_EVENT_WIFI_STA_CONNECTED,
	ARDUINO_EVENT_WIFI_STA_DISCONNECTED,
	ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE,
	ARDUINO_EVENT_WIFI_STA_GOT_IP,
	ARDUINO_EVENT_WIFI_STA_GOT_IP6,
	ARDUINO_EVENT_WIFI_STA_LOST_IP,
	ARDUINO_EVENT_WIFI_AP_START,
	ARDUINO_EVENT_WIFI_AP_STOP,
	ARDUINO_EVENT_WIFI_AP_STACONNECTED,
	ARDUINO_EVENT_WIFI_AP_STADISCONNECTED,
	ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED,
	ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED,
	ARDUINO_EVENT_WIFI_AP_GOT_IP6,
	ARDUINO_EVENT_WIFI_FTM_REPORT,
	ARDUINO_EVENT_ETH_START,
	ARDUINO_EVENT_ETH_STOP,
	ARDUINO_EVENT_ETH_CONNECTED,
	ARDUINO_EVENT_ETH_DISCONNECTED,
	ARDUINO_EVENT_ETH_GOT_IP,
	ARDUINO_EVENT_ETH_GOT_IP6,
	ARDUINO_EVENT_WPS_ER_SUCCESS,
	ARDUINO_EVENT_WPS_ER_FAILED,
	ARDUINO_EVENT_WPS_ER_TIMEOUT,
	ARDUINO_EVENT_WPS_ER_PIN,
	ARDUINO_EVENT_WPS_ER_PBC_OVERLAP,
	ARDUINO_EVENT_SC_SCAN_DONE,
	ARDUINO_EVENT_SC_FOUND_CHANNEL,
	ARDUINO_EVENT_SC_GOT_SSID_PSWD,
	ARDUINO_EVENT_SC_SEND_ACK_DONE,
	ARDUINO_EVENT_PROV_INIT,
	ARDUINO_EVENT_PROV_DEINIT,
	ARDUINO_EVENT_PROV_START,
	ARDUINO_EVENT_PROV_END,
	ARDUINO_EVENT_PROV_CRED_RECV,
	ARDUINO_EVENT_PROV_CRED_FAIL,
	ARDUINO_EVENT_PROV_CRED_SUCCESS,
	ARDUINO_EVENT_MAX
} arduino_event_id_t;

typedef union {
	wifi_event_sta_scan_done_t wifi_scan_done;
	wifi_event_sta_connected_t wifi_sta_connected;
	wifi_event_sta_disconnected_t wifi_sta_disconnected;
	wifi_event_ap_staconnected_t wifi_ap_staconnected;
	wifi_event_ap_stadisconnected_t wifi_ap_stadisconnected;
	ip_event_got_ip_t got_ip;
	wifi_sta_config_t prov_cred_recv;
} arduino_event_info_t;

typedef struct{
	arduino_event_id_t event_id;
	arduino_event_info_t event_info;
} arduino_event_t;

typedef void (*WiFiEventCb)(arduino_event_id_t event);
typedef std::function<void(arduino_event_id_t event, arduino_event_info_t info)> WiFiEventFuncCb;
typedef void (*WiFiEventSysCb)(arduino_event_t *event);

typedef size_t wifi_event_id_t;

/// Common constants - bit field definitions
#define BIT0  0x0001
#define BIT1  0x0002
#define BIT2  0x0004
#define BIT3  0x0008
#define BIT4  0x0010
#define BIT5  0x0020
#define BIT6  0x0040
#define BIT7  0x0080
#define BIT8  0x0100
#define BIT9  0x0200
#define BIT10 0x0400
#define BIT11 0x0800
#define BIT12 0x1000
#define BIT13 0x2000
#define BIT14 0x4000
#define BIT15 0x8000

static const int AP_STARTED_BIT    = BIT0;
static const int AP_HAS_IP6_BIT    = BIT1;
static const int AP_HAS_CLIENT_BIT = BIT2;
static const int STA_STARTED_BIT   = BIT3;
static const int STA_CONNECTED_BIT = BIT4;
static const int STA_HAS_IP_BIT    = BIT5;
static const int STA_HAS_IP6_BIT   = BIT6;
static const int ETH_STARTED_BIT   = BIT7;
static const int ETH_CONNECTED_BIT = BIT8;
static const int ETH_HAS_IP_BIT    = BIT9;
static const int ETH_HAS_IP6_BIT   = BIT10;
static const int WIFI_SCANNING_BIT = BIT11;
static const int WIFI_SCAN_DONE_BIT= BIT12;
static const int WIFI_DNS_IDLE_BIT = BIT13;
static const int WIFI_DNS_DONE_BIT = BIT14;

typedef enum {
	WIFI_RX_ANT0 = 0,
	WIFI_RX_ANT1,
	WIFI_RX_ANT_AUTO
} wifi_rx_ant_t;

typedef enum {
	WIFI_TX_ANT0 = 0,
	WIFI_TX_ANT1,
	WIFI_TX_ANT_AUTO
} wifi_tx_ant_t;

class WiFiGenericClass
{
  public:
    WiFiGenericClass(){}

    wifi_event_id_t onEvent(WiFiEventCb cbEvent, arduino_event_id_t event = ARDUINO_EVENT_MAX);
    wifi_event_id_t onEvent(WiFiEventFuncCb cbEvent, arduino_event_id_t event = ARDUINO_EVENT_MAX);
    wifi_event_id_t onEvent(WiFiEventSysCb cbEvent, arduino_event_id_t event = ARDUINO_EVENT_MAX);
    void removeEvent(WiFiEventCb cbEvent, arduino_event_id_t event = ARDUINO_EVENT_MAX);
    void removeEvent(WiFiEventSysCb cbEvent, arduino_event_id_t event = ARDUINO_EVENT_MAX);
    void removeEvent(wifi_event_id_t id);

    static int getStatusBits();
    static int waitStatusBits(int bits, uint32_t timeout_ms);
    int32_t channel(void);
    void enableLongRange(bool enable);
    static bool mode(wifi_mode_t);
    static wifi_mode_t getMode();

    bool enableSTA(bool enable);
    bool enableAP(bool enable);
    bool setSleep(bool enabled);
    bool getSleep();

    static bk_err_t _eventCallback(arduino_event_t *event);
    static int hostByName(const char *aHostname, IPAddress &aResult);
    static IPAddress calculateNetworkID(IPAddress ip, IPAddress subnet);
    static IPAddress calculateBroadcast(IPAddress ip, IPAddress subnet);
    static uint8_t calculateSubnetCIDR(IPAddress subnetMask);
    
  protected:
    static bool _sleepEnabled;
    static int setStatusBits(int bits);
    static int clearStatusBits(int bits);
    friend class WiFiSTAClass;
    friend class WiFiScanClass;
    friend class WiFiAPClass;
};

#endif /* WIFIGENERIC_H_ */