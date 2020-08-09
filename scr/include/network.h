#ifndef NETWORK_H
#define NETWORK_H

#include "osapi.h"
#include "user_interface.h"
#include "user_config.h"
#include "ets_sys.h"

LOCAL void ICACHE_FLASH_ATTR udp_tx_data(uint8_t *data, uint16_t len, uint32_t ip_addr);

LOCAL void ICACHE_FLASH_ATTR tcp_connect_cb(void *arg);

LOCAL void ICACHE_FLASH_ATTR recv_cb(void *arg, char *data, uint16_t len);

LOCAL void wifi_init();

LOCAL void ICACHE_FLASH_ATTR wifi_event_cb(System_Event_t *event);

#endif