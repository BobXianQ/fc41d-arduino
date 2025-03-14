#include "BLEServer.h"
#include "BLEService.h"
#include "bk_hal_ble.h"

static BLEServerCallbacks* m_pServerCallbacks;

BLEService* BLEServer::createService(const char* uuid)  {

	BLEService* pService = new BLEService(uuid);

	return pService;
} 

void BLEServer::setCallbacks(BLEServerCallbacks* pCallbacks) {
	m_pServerCallbacks = pCallbacks;
} 

BLEServerCallbacks* BLEServer::getCallbacks() {
	return m_pServerCallbacks;
} 

void BLEServer::disconnect() {
	bk_server_disconnect();
}

