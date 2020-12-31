/**
 * @file state_machine.h
 * @author Pedro Soares (p.soares1997@live.com.pt) @ Projeto EI - ESTG IPP
 * @brief State machine header
 * @version 0.1
 * @date 2020-12-03
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "user_config.h"

#ifndef SIMULATION
#include <mqtt.h>
#endif

typedef enum{
  SETUP,
  IDLE,
  PRE_RECORDING,
  RECORDING,
  TRANSMISSION
}states_t;

typedef enum{
  CONFIGURATION,
  EVENT_ACK,
  BINARY_ACK
}responses_t;

#ifndef SIMULATION

MQTT_Client mqttClient;
#endif

void state_engine(states_t state);

void allocMem();

int handleMessage(char* topic, char* data);
/**
 * @brief MQTT publish callback for processing new messages
 * 
 */


void MQTT_setup(void);
void MQTT_init(void);

void printDouble(double num);
#ifdef SIMULATION
/**
 * @brief MQTT client synchronizer which checks for new MQTT messages
 * 
 * @param client mqtt_client to check for new messages
 * @return void* 
 */
void* client_refresher(void* client);


void* retry_daemon_func(void* client);


#endif




#endif
