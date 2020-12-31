/**
 * @file dataPacket.h
 * @author Pedro Soares (p.soares1997@live.com.pt) @ Projeto EI - ESTG IPP
 * @brief JSON creator header
 * @version 0.1
 * @date 2020-12-03
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef data_packet_h
#define data_packet_h

#include "cJSON.h"
#include "user_config.h"
#include "c_types.h"

typedef struct {
    int sample_size;
    int sample_rate;
} sensor_config;

typedef enum{
    CONFIG,
    ACK_EVENT,
    UNKNOW
}topics_enum;


topics_enum check_topic(char* topic);

char* create_monitor(char* sensor_id, int sample_rate, int sample_size, int timestamp, double *samples_arr);

char* create_monitor_sensor_config(char* sensorId, char* sensor_mac);

char* create_monitor_without_arr(char* sensor_id, int sample_rate, int sample_size, int timestamp);

sensor_config parse_config(char* message);


#ifndef SIMULATION

char* create_time(char* sensor_mac, int32_t btime, int32_t etime, uint32_t time, int number);
double unpackDouble(const void *buf);
#endif

#endif