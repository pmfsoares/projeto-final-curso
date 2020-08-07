#ifndef DETECTION_H
#define DETECTION_H

#include "user_config.h"
#include <stdint.h>

uint16_t sample_arr[num_samples];
uint64_t power_arr[num_samples];
uint16_t k = 0;

double mean(uint64_t arr[]);

double std(uint64_t arr[], double media);

void powerSequence(uint16_t arr[]);

double threshold(uint16_t sampleArr[]);

#endif
