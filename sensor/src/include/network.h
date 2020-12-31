/**
 * @file network.h
 * @author Pedro Soares (p.soares1997@live.com.pt) @ Projeto EI - ESTG IPP
 * @brief Network header
 * @version 0.1
 * @date 2020-12-03
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef NETWORK_H
#define NETWORK_H

#include "osapi.h"
#include "user_interface.h"
#include "user_config.h"
#include "ets_sys.h"
#include "mqtt.h"
#include "c_types.h"
#include "osapi.h"
#include "espconn.h"

struct espconn* tcp_conn;
esp_tcp tcp_proto;

void ICACHE_FLASH_ATTR udp_tx_data(uint8_t *data, uint16_t len, uint32_t ip_addr);

LOCAL void ICACHE_FLASH_ATTR tcp_connect_cb(void *arg);

void ICACHE_FLASH_ATTR recv_cb(void *arg, char *data, uint16_t len);

void wifi_init();

void ICACHE_FLASH_ATTR wifi_event_cb(System_Event_t *event);

void ICACHE_FLASH_ATTR mqttConnectedCb(uint32_t *args);

void ICACHE_FLASH_ATTR mqttPublishedCb(uint32_t *args);

void ICACHE_FLASH_ATTR mqttDisconnectedCb(uint32_t *args);


#endif