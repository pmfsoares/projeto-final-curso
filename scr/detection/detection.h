#ifndef DETECTION_H
#define DETECTION_H

#include "user_config.h"

uint16_t sample_arr[num_samples]
uint16_t power_arr[num_samples];
uint16_t k = 0;

double mean(uint32_t arr[]);

double std(uint32_t arr[], double media);

void powerSequence(uint32_t arr[]);

double threshold(uint32_t pwd_arr[], double std, double mean);

#endif