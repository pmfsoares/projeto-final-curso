#ifndef data_packet_h
#define data_packet_h

#include "cJSON.h"

char *create_monitor(char* sensorId, int sample_rate, int sample_size, int timestamp, double *samplesArr);

#endif