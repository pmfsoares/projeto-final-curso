#include "network.h"

#include "c_types.h"
#include "osapi.h"
#include "user_interface.h"
#include "ets_sys.h"
#include "gpio.h"
#include "espconn.h"

LOCAL struct espconn udp_tx;
LOCAL struct espconn udp_conn;
LOCAL esp_udp udp_proto_tx;
LOCAL esp_udp udp_proto;

LOCAL struct espconn tcp_conn;
LOCAL esp_tcp tcp_proto;

LOCAL ip_addr_t esp_ip;

/**
 * @brief Função que inicializa Wifi
 * @return 
 */
LOCAL void wifi_init(){

  char ssid[32] = AP_SSID;
  char password[64] = AP_PASSWORD;

  struct station_config stationConf;


  wifi_set_opmode(STATION_MODE);

  os_memcpy(&stationConf.ssid, ssid, 32);
  os_memcpy(&stationConf.password, password, 64);
  wifi_station_set_config(&stationConf);
  wifi_station_dhcpc_stop();

  struct ip_info info;

  IP4_ADDR(&info.ip,      192, 168,   1, 172);
  IP4_ADDR(&info.gw,      192, 168,   1,   1);
  IP4_ADDR(&info.netmask, 255, 255, 255,   0);

  wifi_set_ip_info(STATION_IF, &info);
  wifi_set_event_handler_cb(wifi_event_cb);

}

/**
 * @brief 
 * @param arg
 * @return 
 */
LOCAL void ICACHE_FLASH_ATTR tcp_connect_cb(void *arg){
  struct espconn *conn = (struct espconn *) arg;
  os_printf("TCP connected\n");
  espconn_regist_recvcb(conn, recv_cb);
}
/**
 * @brief Função de para envio de dados em pacotes com UDP
 * @param data    Dados
 * @param len     Tamanho dos dados
 * @param ip_addr IP de destino
 * @return 
 */
LOCAL void ICACHE_FLASH_ATTR udp_tx_data(uint8_t *data, uint16_t len, uint32_t ip_addr){
  ip_addr_t addr;
  IP4_ADDR(&addr, 192, 168, 1, 80);
  os_memcpy(udp_proto_tx.remote_ip, &addr.addr, 4);
  udp_proto_tx.remote_port = 1234;
  os_printf("\ndest.ip = "IPSTR"\n", IP2STR(&addr.addr));
  udp_tx.type = ESPCONN_UDP;
  udp_tx.state = ESPCONN_NONE;
  udp_tx.proto.udp = &udp_proto_tx;

  if(espconn_create(&udp_tx)){
    os_printf("espconn_create failed\n");
  }
  if(espconn_send(&udp_tx, data, len)){ 
    os_printf("espconn_sendto failed\n");
  }
  espconn_delete(&udp_tx);
}
/**
 * @brief 
 * @param event
 * @return 
 */
LOCAL void ICACHE_FLASH_ATTR wifi_event_cb(System_Event_t *event) {
    struct ip_info info;

    // To determine what actually happened, we need to look at the event.
    switch (event->event) {
        case EVENT_STAMODE_CONNECTED: {
            // We are connected as a station, but we don't have an IP address yet.
            char ssid[33];
            uint8_t len = event->event_info.connected.ssid_len;
            if (len > 32) {
                len = 32;
            }
            strncpy(ssid, event->event_info.connected.ssid, len + 1);
            os_printf("Received EVENT_STAMODE_CONNECTED. "
                      "SSID = %s, BSSID = "MACSTR", channel = %d.\n",
                      ssid, MAC2STR(event->event_info.connected.bssid), event->event_info.connected.channel);
            break;
        }
        case EVENT_STAMODE_DISCONNECTED: {
            // We have been disconnected as a station.
            char ssid[33];
            uint8_t len = event->event_info.connected.ssid_len;
            if (len > 32) {
                len = 32;
            }
            strncpy(ssid, event->event_info.connected.ssid, len + 1);
            os_printf("Received EVENT_STAMODE_DISCONNECTED. "
                      "SSID = %s, BSSID = "MACSTR", channel = %d.\n",
                      ssid, MAC2STR(event->event_info.disconnected.bssid), event->event_info.disconnected.reason);
            break;
        }
        case EVENT_STAMODE_GOT_IP:
            //MQTT_Connect(&mqttClient);
            // We have an IP address, ready to run. Return the IP address, too.
            os_printf("Received EVENT_STAMODE_GOT_IP. IP = "IPSTR", mask = "IPSTR", gateway = "IPSTR"\n", 
                      IP2STR(&event->event_info.got_ip.ip.addr), 
                      IP2STR(&event->event_info.got_ip.mask.addr),
                      IP2STR(&event->event_info.got_ip.gw));
            break;
        case EVENT_STAMODE_DHCP_TIMEOUT:
            // We couldn't get an IP address via DHCP, so we'll have to try re-connecting.
            os_printf("Received EVENT_STAMODE_DHCP_TIMEOUT.\n");
            wifi_station_disconnect();
            wifi_station_connect();
            break;
    }
}

LOCAL void ICACHE_FLASH_ATTR recv_cb(void *arg, char *data, uint16_t len){
  struct espconn *conn = (struct espconn *) arg;
  uint8_t *addr_array = NULL;
  if(conn -> type == ESPCONN_TCP){
    addr_array = conn->proto.tcp->remote_ip;
  }else {
    addr_array = conn->proto.udp->remote_ip;
  }
  if(addr_array != NULL){
    ip_addr_t addr;
    IP4_ADDR(&addr, addr_array[0], addr_array[1], addr_array[2], addr_array[3]);
    char ip_str[15];
    os_printf("%s", data);
  }
}