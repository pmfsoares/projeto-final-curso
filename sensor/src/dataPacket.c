/**
 * @file dataPacket.c
 * @author Pedro Soares (p.soares1997@live.com.pt) @ Projeto EI - ESTG IPP
 * @brief Creation of the JSON objects for communication
 * @version 0.1
 * @date 2020-12-03
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "dataPacket.h"
#include "cJSON.h"
#include "user_config.h"

#ifndef SIMULATION
#include <osapi.h>
#include <mem.h>
#include <ets_sys.h>
#include <user_interface.h>
#endif

#ifdef SIMULATION
topics_enum check_topic(char* topic){
#endif
#ifndef SIMULATION
#include <c_types.h>
topics_enum ICACHE_FLASH_ATTR check_topic(char* topic){
#endif

    if(!strcmp(MQTT_TOPIC_CONFIG, topic)){
        return CONFIG;
    }else if(!strcmp(MQTT_TOPIC_ACK, topic)){
        return ACK_EVENT;
    }else{
        return UNKNOW;
    }
}

/**
 * @brief Creates a cJSON object relative to a recorded event, and stringifies it to be then sent to the server.
 * 
 * @param sensor_id ID do sensor
 * @param sample_rate Sample Rate used on the recorded event
 * @param sample_size Sample Size used on the recorded event
 * @param timestamp Timestamp of the event
 * @param samples_Arr Event recording
 * @return char* The stringified cJSON object
 */

#ifdef SIMULATION
char* create_monitor(char* sensor_id, int sample_rate, int sample_size, int timestamp, double * samples_arr){
#endif
#ifndef SIMULATION
char* ICACHE_FLASH_ATTR create_monitor(char* sensor_id, int sample_rate, int sample_size, int timestamp, double * samples_arr){
#endif
    char *string = NULL;
    cJSON *sensor = NULL;
    cJSON *sample_rt = NULL;
    cJSON *sample_sz = NULL;
    cJSON *time = NULL;
    cJSON *samples = NULL;
    

    cJSON *monitor = cJSON_CreateObject();
    if (monitor == NULL){
        goto end;
    }

    sensor = cJSON_CreateString(sensor_id);
    if (sensor == NULL){
        goto end;
    }
    /* 
     * after creation was successful, immediately add it to the monitor,
     * thereby transferring ownership of the pointer to it */
    cJSON_AddItemToObject(monitor, "sensor_id", sensor);

    sample_rt = cJSON_CreateNumber(sample_rate);
    if(sample_rt == NULL){
        goto end;
    }
    cJSON_AddItemToObject(monitor, "sample_rate", sample_rt);

    sample_sz = cJSON_CreateNumber(sample_size);
    if(sample_sz == NULL){
        goto end;
    }
    cJSON_AddItemToObject(monitor, "sample_size", sample_sz);

    time = cJSON_CreateNumber(timestamp);
    if(time == NULL){
        goto end;
    }
    cJSON_AddItemToObject(monitor, "timestamp", time);

    samples = cJSON_CreateArray();
    if (samples == NULL){
        goto end;
    }
    cJSON *val = NULL;
    cJSON_AddItemToObject(monitor, "data", samples);
    for(int x = 0; x < sample_size; x++){
        val = cJSON_CreateNumber(samples_arr[x]);
        if (val == NULL){
            goto end;
        }
        cJSON_AddItemToArray(samples, val);
    }
    string = cJSON_Print(monitor);
    if (string == NULL){
        #ifdef SIMULATION
            printf("Failed to print monitor.\n");
        #endif
    }

end:
    cJSON_Delete(monitor);
    return string;
}


/**
 * @brief Creates a cJSON object for config request, and returns the stringified version.
 * 
 * @param sensor_id ID do sensor
 * @param sensor_mac MAC of the sensor
 * @return char* The stringified cJSON object
 */
#ifdef SIMULATION
char* create_monitor_sensor_config(char* sensorId, char* sensor_mac){
#endif
#ifndef SIMULATION
char* ICACHE_FLASH_ATTR create_monitor_sensor_config(char* sensorId, char* sensor_mac){
#endif

    char *string = NULL;
    cJSON *sensor = NULL;
    cJSON *mac = NULL;
    

    cJSON *monitor = cJSON_CreateObject();
    if (monitor == NULL)
    {
        goto end;
    }

    sensor = cJSON_CreateString(sensorId);
    if (sensor == NULL){
        goto end;
    }
    /* after creation was successful, immediately add it to the monitor,
     * thereby transferring ownership of the pointer to it */
    cJSON_AddItemToObject(monitor, "sensor_id", sensor);
    
    mac = cJSON_CreateString(sensor_mac);
    if( mac == NULL){
        goto end;
    }

    cJSON_AddItemToObject(monitor, "sensor_mac", mac);
    string = cJSON_Print(monitor);
    if (string == NULL){
        #ifdef SIMULATION
            printf("Failed to print monitor.\n");
        #endif
    }
end:
    cJSON_Delete(monitor);
    return string;
}
/**
 * @brief Config message parser, 
 * 
 * @param message char* message recieved from broker
 * @return sensor_config struct containing the json values to be used
 */
#ifdef SIMULATION
sensor_config parse_config(char* message){
#endif
#ifndef SIMULATION
sensor_config ICACHE_FLASH_ATTR parse_config(char* message){
#endif
    cJSON* root = cJSON_Parse(message);
    cJSON* rate = cJSON_GetObjectItem(root, "sample_rate");
    cJSON* size = cJSON_GetObjectItem(root, "sample_size");

    sensor_config ret_conf;
    ret_conf.sample_size = size->valueint;
    ret_conf.sample_rate = rate->valueint; 

    #ifdef SIMULATION
        free(root);
        free(rate);
        free(size);
    #endif
    #ifndef SIMULATION
        os_free(root);
        os_free(rate);
        os_free(size);
    #endif

    return ret_conf;

}
/**
 * @brief Creates a cJSON object relative to a recorded event, and stringifies it to be then sent to the server.
 * 
 * @param sensor_id ID do sensor
 * @param sample_rate Sample Rate used on the recorded event
 * @param sample_size Sample Size used on the recorded event
 * @param timestamp Timestamp of the event
 * @param samples_Arr Event recording
 * @return char* The stringified cJSON object
 */
#ifndef SIMULATION
char* ICACHE_FLASH_ATTR create_monitor_without_arr(char* sensor_id, int sample_rate, int sample_size, int timestamp){
    char *string = NULL;
    cJSON *sensor = NULL;
    cJSON *sample_rt = NULL;
    cJSON *sample_sz = NULL;
    cJSON *time = NULL;
    

    cJSON *monitor = cJSON_CreateObject();
    if (monitor == NULL){
        goto end;
    }

    sensor = cJSON_CreateString(sensor_id);
    if (sensor == NULL){
        goto end;
    }
    cJSON_AddItemToObject(monitor, "sensor_mac", sensor);

    sample_rt = cJSON_CreateNumber(sample_rate);
    if(sample_rt == NULL){
        goto end;
    }
    cJSON_AddItemToObject(monitor, "sample_rate", sample_rt);

    sample_sz = cJSON_CreateNumber(sample_size);
    if(sample_sz == NULL){
        goto end;
    }
    cJSON_AddItemToObject(monitor, "sample_size", sample_sz);
    
    time = cJSON_CreateNumber(timestamp);
    if(time == NULL){
        goto end;
    }
    cJSON_AddItemToObject(monitor, "timestamp", time);
    
    string = cJSON_Print(monitor);
    if (string == NULL){
        #ifdef SIMULATION
            printf("Failed to print monitor.\n");
        #endif
    }

end:
    cJSON_Delete(monitor);
    return string;
}
/**
 * @brief Function that receives an array of unsigned chars(bytes) and converts into a double
 * 
 * @param buf Unsigned char array
 * @return double 
 */
double unpackDouble(const void *buf) {
    unsigned char* b = (unsigned char *)buf;
    uint64_t temp = 0;
    temp = ((b[0] << 56) |
            (b[1] << 48) |
            (b[2] << 40) |
            (b[3] << 32) |
            (b[4] << 24) |
            (b[5] << 16) |
            (b[6] <<  8) |
            (b[7] <<  0) );
    return *((double *) &temp);
}
char* ICACHE_FLASH_ATTR create_time(char* sensor_mac, int32_t btime, int32_t etime, uint32_t time, int number){
    char *string = NULL;
    cJSON *sensor = NULL;
    cJSON *btime_obj = NULL;
    cJSON *etime_obj = NULL;
    cJSON *time_obj = NULL;
    cJSON *numb = NULL;
    

    cJSON *monitor = cJSON_CreateObject();
    if (monitor == NULL){
        goto end;
    }

    sensor = cJSON_CreateString(sensor_mac);
    if (sensor == NULL){
        goto end;
    }
    cJSON_AddItemToObject(monitor, "sensor_mac", sensor);

    btime_obj = cJSON_CreateNumber(btime);
    if(btime_obj == NULL){
        goto end;
    }
    cJSON_AddItemToObject(monitor, "btime", btime_obj);

    etime_obj = cJSON_CreateNumber(etime);
    if(etime_obj == NULL){
        goto end;
    }
    cJSON_AddItemToObject(monitor, "etime", etime_obj);
    
    time_obj = cJSON_CreateNumber(time);
    if(time_obj == NULL){
        goto end;
    }
    cJSON_AddItemToObject(monitor, "time", time_obj);

    numb = cJSON_CreateNumber(number);
    if(numb == NULL){
        goto end;
    }
    cJSON_AddItemToObject(monitor, "numb_samples", numb);
    string = cJSON_Print(monitor);
    if (string == NULL){
        #ifdef SIMULATION
            printf("Failed to print monitor.\n");
        #endif
    }

end:
    cJSON_Delete(monitor);
    return string;
}

#endif
