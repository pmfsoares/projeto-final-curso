/**
 * @file network.c
 * @author Pedro Soares (p.soares1997@live.com.pt) @ Projeto EI - ESTG IPP
 * @brief Sensor wifi connection
 * @version 0.1
 * @date 2020-12-03
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "network.h"

#include "c_types.h"
#include "osapi.h"
#include "user_interface.h"
#include "ets_sys.h"
#include "gpio.h"
#include "espconn.h"
#include "mqtt.h"
#include "dataPacket.h"
#include "state_machine.h"



ip_addr_t esp_ip;

/**
 * @brief Callback function for when the device recieves a mqtt message
 * 
 * @param args 
 */
void ICACHE_FLASH_ATTR mqttPublishedCb(uint32_t *args){
  MQTT_Client* client = (MQTT_Client*)args;
  INFO("MQTT: Published\r\n");
}


/**
 * @brief Wifi event callback function, it receives an system event and if receives event that system as an IP connects the MQTT client to the broker 
 * @param event System event
 * @return 
 */
void ICACHE_FLASH_ATTR wifi_event_cb(System_Event_t *event) {
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
            os_printf("GOT IP\n");
            /**
             * @brief NTP Server setup
             * 
             */
            //ip_addr_t *addr_ntp = (ip_addr_t *) os_zalloc(sizeof(ip_addr_t));
            sntp_setservername(0, "time.google.com");
            sntp_set_timezone(0);
            //ipaddr_aton("216.239.35.0", addr);
            //sntp_setserver(1, addr);
            sntp_init();
            //os_free(addr_ntp);
            MQTT_Connect(&mqttClient);
            break;
        case EVENT_STAMODE_DHCP_TIMEOUT:
            // We couldn't get an IP address via DHCP, so we'll have to try re-connecting.
            os_printf("Received EVENT_STAMODE_DHCP_TIMEOUT.\n");
            wifi_station_disconnect();
            wifi_station_connect();
            break;
    }
}
/**
 * @brief Wifi initializer function
 * @return 
 */
void ICACHE_FLASH_ATTR wifi_init(){

  char ssid[32] = AP_SSID;
  char password[64] = AP_PASSWORD;

  struct station_config stationConf;


  wifi_set_opmode(STATION_MODE);

  strncpy(&stationConf.ssid, AP_SSID, 32);
  strncpy(&stationConf.password, AP_PASSWORD, 64);
  wifi_station_set_config(&stationConf);
  wifi_station_dhcpc_stop();

  struct ip_info info;

  IP4_ADDR(&info.ip,      192, 168,   12, 172);
  IP4_ADDR(&info.gw,      192, 168,   12,   1);
  IP4_ADDR(&info.netmask, 255, 255, 255,   0);

  wifi_set_ip_info(STATION_IF, &info);
  wifi_set_event_handler_cb(wifi_event_cb);

}


/**
 * @brief 
 * @param event
 * @return 
 */

void ICACHE_FLASH_ATTR recv_cb(void *arg, char *data, uint16_t len){
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

