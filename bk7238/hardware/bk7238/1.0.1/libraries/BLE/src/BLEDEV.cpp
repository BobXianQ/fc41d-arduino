#include "BLEDEV.h"
#include "bk_hal_ble.h"

extern "C" {
#include "app_ble.h"
#include "app_sdp.h"
#include "app_ble_init.h"
#if (BLE_APP_SEC)
#include "app_sec.h"
#endif
}


void ble_arduino_notice_cb(ble_notice_t notice, void *param)
{
    BLEServerCallbacks* callback = BLEServer::getCallbacks();
	switch (notice) {
	case BLE_5_STACK_OK:
		bk_printf("ble stack ok");
		break;
	case BLE_5_WRITE_EVENT:
	{
		write_req_t *w_req = (write_req_t *)param;
        callback->onwrite(w_req->prf_id, w_req->att_idx,w_req->value,w_req->len);
		break;
	}
	case BLE_5_READ_EVENT:
	{
		read_req_t *r_req = (read_req_t *)param;
		callback->onread(r_req->prf_id, r_req->att_idx,r_req->value,&r_req->length);
		break;
	}
	case BLE_5_REPORT_ADV:
	{
		recv_adv_t *r_ind = (recv_adv_t *)param;

		bk_printf("[%s]r_ind:actv_idx:%d,evt_type:%d adv_addr:%02x:%02x:%02x:%02x:%02x:%02x,rssi:%d\r\n",
			((r_ind->evt_type&0x7) == 3)?"scan-rsp":((r_ind->evt_type&0x7) == 1)?"adv":"unknow",
			r_ind->actv_idx,r_ind->evt_type, r_ind->adv_addr[0], r_ind->adv_addr[1], r_ind->adv_addr[2],
			r_ind->adv_addr[3], r_ind->adv_addr[4], r_ind->adv_addr[5],r_ind->rssi);
		break;
	}
	case BLE_5_MTU_CHANGE:
	{
		mtu_change_t *m_ind = (mtu_change_t *)param;
		bk_printf("BLE_5_MTU_CHANGE:conn_idx:%d, mtu_size:%d\r\n", m_ind->conn_idx, m_ind->mtu_size);
		break;
	}
	case BLE_5_CONNECT_EVENT:
	{
		conn_ind_t *c_ind = (conn_ind_t *)param;
		bk_printf("c_ind:conn_idx:%d, addr_type:%d, peer_addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
			c_ind->conn_idx, c_ind->peer_addr_type, c_ind->peer_addr[0], c_ind->peer_addr[1],
			c_ind->peer_addr[2], c_ind->peer_addr[3], c_ind->peer_addr[4], c_ind->peer_addr[5]);
    
        callback->onConnect();
		break;
	}
	case BLE_5_DISCONNECT_EVENT:
	{
		discon_ind_t *d_ind = (discon_ind_t *)param;
		bk_printf("d_ind:conn_idx:%d,reason:%d\r\n", d_ind->conn_idx,d_ind->reason);
        callback->onDisconnect();
		break;
	}
	case BLE_5_ATT_INFO_REQ:
	{
		att_info_req_t *a_ind = (att_info_req_t *)param;
		bk_printf("a_ind:conn_idx:%d\r\n", a_ind->conn_idx);
		a_ind->length = 128;
		a_ind->status = ERR_SUCCESS;
		break;
	}
	case BLE_5_CREATE_DB:
	{
		create_db_t *cd_ind = (create_db_t *)param;
		bk_printf("cd_ind:prf_id:%d, status:%d\r\n", cd_ind->prf_id, cd_ind->status);
		break;
	}
	#if (BLE_CENTRAL)
	case BLE_5_INIT_CONNECT_EVENT:
	{
		conn_ind_t *c_ind = (conn_ind_t *)param;
		#if (CFG_SOC_NAME == SOC_BK7238)
		app_ble_get_peer_feature(c_ind->conn_idx);
		app_ble_set_le_pkt_size(c_ind->conn_idx,LE_MAX_OCTETS);
		app_ble_mtu_exchange(c_ind->conn_idx);
		sdp_discover_all_service(c_ind->conn_idx);
		#endif
		bk_printf("BLE_5_INIT_CONNECT_EVENT:conn_idx:%d, addr_type:%d, peer_addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
			c_ind->conn_idx, c_ind->peer_addr_type, c_ind->peer_addr[0], c_ind->peer_addr[1],
			c_ind->peer_addr[2], c_ind->peer_addr[3], c_ind->peer_addr[4], c_ind->peer_addr[5]);
		break;
	}
	case BLE_5_INIT_DISCONNECT_EVENT:
	{
		discon_ind_t *d_ind = (discon_ind_t *)param;
		bk_printf("BLE_5_INIT_DISCONNECT_EVENT:conn_idx:%d,reason:0x%x\r\n", d_ind->conn_idx,d_ind->reason);
		break;
	}
	#endif
	case BLE_5_INIT_CONN_PARAM_UPDATE_REQ_EVENT:
	{
		conn_param_t *d_ind = (conn_param_t *)param;
		bk_printf("BLE_5_INIT_CONN_PARAM_UPDATE_REQ_EVENT:conn_idx:%d,intv_min:%d,intv_max:%d,time_out:%d\r\n",d_ind->conn_idx,
			d_ind->intv_min,d_ind->intv_max,d_ind->time_out);
		app_ble_send_conn_param_update_cfm(d_ind->conn_idx,true);
	}break;
	case BLE_5_INIT_CONN_PARAM_UPDATE_IND_EVENT:
	{
		conn_update_ind_t *d_ind = (conn_update_ind_t *)param;
		bk_printf("BLE_5_INIT_CONN_PARAM_UPDATE_IND_EVENT:conn_idx:%d,interval:%d,time_out:%d,latency\r\n",d_ind->conn_idx,
			d_ind->interval,d_ind->time_out,d_ind->latency);
	}break;
	case BLE_5_SDP_REGISTER_FAILED:
		bk_printf("BLE_5_SDP_REGISTER_FAILED\r\n");
		break;

    case BLE_5_TX_DONE:
        callback->notify_done();
        break;
	default:
		break;
	}
}


BLEAdvertising* BLEDEV::g_bleAdvertising = nullptr;
BLEServer* BLEDEV::g_pServer = nullptr;

BLEAdvertising* BLEDEV::getAdvertising() {
	if(g_bleAdvertising == nullptr) {
		g_bleAdvertising = new BLEAdvertising();
	}
    
	return g_bleAdvertising; 
}

BLEServer* BLEDEV::createServer() {
	g_pServer = new BLEServer();
	return g_pServer;
}

void BLEDEV::init() 
{
    ble_set_notice_cb(ble_arduino_notice_cb);
	bk_dev_init();
}