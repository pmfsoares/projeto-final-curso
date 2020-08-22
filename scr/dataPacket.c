#include "dataPacket.h"
#include "cJSON.h"
#include "user_config.h"

char *create_monitor(char* sensorId, int sample_rate, int sample_size, int timestamp, double * samplesArr){
    
    char *string = NULL;
    cJSON *sensor = NULL;
    cJSON *sample_rt = NULL;
    cJSON *sample_sz = NULL;
    cJSON *time = NULL;
    cJSON *samples = NULL;
    cJSON *sample = NULL;
    
    size_t index = 0;

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
        val = cJSON_CreateNumber(samplesArr[x]);
        if (val == NULL){
            goto end;
        }
        cJSON_AddItemToArray(samples, val);
    }
    string = cJSON_Print(monitor);
    if (string == NULL){
        #ifdef SIMULACAO
            printf("Failed to print monitor.\n");
        #endif
        #ifndef SIMULACAO
            os_printf("Failed to print monitor.\n");
        #endif
    }

end:
    cJSON_Delete(monitor);
    return string;
}