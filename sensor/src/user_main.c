/**
 * @file user_main.c
 * @author Pedro Soares (p.soares1997@live.com.pt) @ Projeto EI - ESTG IPP
 * @brief Sensor firmware boot function
 * @version 0.1
 * @date 2020-12-03
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "lwip/ip_addr.h"
#include "user_interface.h"
#include "espconn.h"
#include "uart.h"
#include "mqtt.h"
#include <math.h>
#include <mem.h>
#include <c_types.h>
#include "dataPacket.h"
#include "state_machine.h"

LOCAL uint32_t last_addr = 0;

UartBautRate baud_rate = BIT_RATE_230400;
static os_timer_t val_timer;

void ICACHE_FLASH_ATTR user_init(){
  system_timer_reinit();
  // init gpio subsytem
  uart_init(baud_rate, baud_rate);
  MQTT_setup();
  wifi_init();
  states_t states = SETUP;
  state_engine(states);

}
