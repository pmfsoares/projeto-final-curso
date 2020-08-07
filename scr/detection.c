#include "detection.h"

#include "user_config.h"

#include <stdint.h>
#include <math.h>
#include <c_types.h>


uint16_t adc_num = num_samples;

double mean(uint64_t arr[]){
  double total = 0;
  for(uint8_t i = 0; i < num_samples; i++){
    total += arr[i];
  }
  return total / num_samples;
}

double std(uint64_t arr[], double media){
  double std = 0;
  for(int i = 0; i < num_samples; i++){
    std = std + (arr[i] * arr[i]);
  }
  double tmp = std / (double) adc_num;
  return sqrt(adc_num);
}

void powerSequence(uint16_t arr[]){
  double sum = 0;
  for(uint8_t i = 0; i < num_samples; i++){
    sum = sum + (arr[i] * arr[i] ) * (i + k * num_samples);
  }
  sum = 1 / num_samples * sum;
  power_arr[k] = sum;
  return;
}

double threshold(uint16_t sampleArr[]){
    powerSequence(sample_arr);

    double mean_tmp = mean(power_arr);

    double std_tmp = std(power_arr, mean_tmp);

    double th = 1 * std_tmp + mean_tmp;
    return th;
}
