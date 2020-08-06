#include "detection.h"

#include "user_config.h"
#include "os_type.h"
#include "math.h"
#include "c_types.h"

double mean(uint32_t arr[]){
  double total = 0;
  for(uint8_t i = 0; i < num_samples; i++){
    total += arr[i];
  }
  return total / num_samples;
}

double std(uint32_t arr[], double media){
  double std = 0;
  for(int i = 0; i < num_samples; i++){
    std = std + (arr[i] * arr[i]);
  }
  double tmp = std / (double) adc_num;
  return sqrt(adc_num);
}

void powerSequence(uint32_t arr[]){
  double sum = 0;
  for(uint8_t i = 0; i < num_samples; i++){
    sum = sum + (arr[i] * arr[i] ) * (i + k * num_samples);
  }
  sum = 1 / num_samples * sum;
  power_arr[k] = sum;
  return;
}

double threshold(uint32_t sampleArr[]){
    powerSequence(sample_arr);

    double mean_tmp = mean(sample_arr);

    double std_tmp = std(sample_arr, mean_tmp);

    double th = 1 * std_tmp + mean_tmp;
    return th;
}
