#include <detection.h>

#include <user_config.h>

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>


uint16_t adc_num = num_samples;

double mean(double * arr){
  double total = 0.0;
  for(uint16_t i = 0; i < num_samples; i++){
    total += arr[i];
  }
  return total / (double) num_samples;
}

double std(double * arr, double media){
  double std = 0.0;
  for(uint16_t i = 0; i < num_samples; i++){
    std += pow(arr[i] - media, 2);
  }
  double tmp = std / (double) adc_num;
  return sqrt(tmp);
}

int powerSequence(double* samples, double* power, uint16_t k){
  double sum = 0;
  k++;
  for(uint16_t i = 0; i < num_samples; i++){
    sum += pow(samples[i], 2) * (i + k * num_samples);
  }
  double pow_val = (1.0 / (double) num_samples) * sum;
  power[k] = pow_val;
  return k;
}

double threshold(double * power, double mean, double var){
    if(power == NULL){
      printf("\n\n\nERRO ARRAY VAZIO\n\n\n");
      exit(1);
    }
    /*
    double mean_tmp;
    mean_tmp = mean(power);

    double std_tmp;
    std_tmp = std(power, mean_tmp);
    */
    double th;

    th  = (3.0 * var) + mean;
    return th;
}
