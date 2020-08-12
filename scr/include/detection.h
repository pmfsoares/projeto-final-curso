#ifndef DETECTION_H
#define DETECTION_H

#include <user_config.h>
#include <stdint.h>

double sample_arr[num_samples];
uint64_t power_arr[num_samples];
uint16_t k;

double mean(uint64_t arr[]);

double std(uint64_t arr[], double media);

void powerSequence(double arr[]);

double threshold();

#endif
