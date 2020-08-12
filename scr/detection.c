#include <detection.h>

#include <user_config.h>

#include <stdint.h>
#include <stddef.h>
#include <math.h>


uint16_t adc_num = num_samples;

double mean(double * arr){
  double total = 0;
  for(uint8_t i = 0; i < num_samples; i++){
    total += arr[i];
  }
  return total / num_samples;
}

double std(double * arr, double media){
  double std = 0;
  for(int i = 0; i < num_samples; i++){
    std = std + (arr[i] * arr[i]);
  }
  double tmp = std / (double) adc_num;
  return sqrt(tmp);
}

int powerSequence(double * samples, double * power, uint16_t k){
  double sum = 0;
  for(uint8_t i = 0; i < num_samples; i++){
    sum = sum + (arr[i] * arr[i] ) * (i + k * num_samples);
  }
  sum = 1 / num_samples * sum;
  power[k] = sum;
  k++;
  return k;
}

double threshold(double * power){

    double mean_tmp = mean(power);

    double std_tmp = std(power, mean_tmp);

    double th = 1 * std_tmp + mean_tmp;
    return th;
}
