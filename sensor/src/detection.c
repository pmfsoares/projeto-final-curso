/**
 * @file detection.c
 * @author Pedro Soares (p.soares1997@live.com.pt) @ Projeto EI - ESTG IPP
 * @brief File with the detection algoritm
 * @version 0.1
 * @date 2020-12-03
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <detection.h>
#include <user_config.h>
#include <state_machine.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>



/**
 * @brief Simple function to calculate the sample mean of the sample array
 * 
 * @param power_arr Sample array
 * @param num_samples Number of samples in the array
 * @return double The calculated mean
 */
#ifdef SIMULATION
double mean(double* power_arr, int num_samples){
#endif
#ifndef SIMULATION
#include <c_types.h>
double ICACHE_FLASH_ATTR mean(double * power_arr, int num_samples){
#endif
  double total = 0.0;
  for(uint16_t i = 0; i < num_samples; i++){
    total += power_arr[i];
  }
  return (total / num_samples);
}

/**
 * @brief Function to calculate the standart deviation of the sample array 
 * 
 * @param power_arr Sample array
 * @param mean Mean of the array
 * @param num_samples Number of samples in the array
 * @return double The calculated standart deviation
 */
#ifdef SIMULATION
double std(double* power_arr, double mean, int num_samples){
#endif
#ifndef SIMULATION
double ICACHE_FLASH_ATTR std(double* power_arr, double mean, int num_samples){
#endif
  double std = 0.0;
  for(uint16_t i = 0; i < num_samples; i++){
    std += pow(power_arr[i] - mean, 2);
  }
  double tmp = std / num_samples;
  return sqrt(tmp);
}
/**
 * @brief Function to calculate the power of the sample block 
 * 
 * @param samples Pointer containing the block of samples
 * @param power Pointer containing the calculated power
 * @param power_arr_pos Current position in the power pointer
 * @param num_samples Number of samples per sample block
 * @return int 
 */
#ifdef SIMULATION
double powerSequence(double* samples, double* power, int power_arr_pos, int num_samples){
#endif
#ifndef SIMULATION
double ICACHE_FLASH_ATTR powerSequence(double* samples, double* power, int power_arr_pos, int num_samples){
#endif
  double sum = 0.0;
  //power_arr_pos++;
  for(uint16_t i = 0; i < num_samples; i++){
    sum += pow(samples[i], 2) * (i + power_arr_pos * num_samples);
  }
  double pow_val = (1.0 / num_samples) * sum;
  //power[power_arr_pos] = pow_val;
  return pow_val;
}

/**
 * @brief Function that calculates the threshold using a given mean and standart deviation
 * 
 * @param mean Mean of the power array
 * @param std_var Standart varition of the power array
 * @return double The calculated standart deviation
 */

#ifdef SIMULATION
double threshold(double mean, double std_var){
#endif
#ifndef SIMULATION
double ICACHE_FLASH_ATTR threshold(double mean, double std_var){
#endif 
    return (1.0 * std_var) + mean;
}

#ifdef SIMULATION
/**
 * @brief Function that calculates the mean, standart deviation, and threshold  and checks if it exceeds the power value for the current sample block 
 * 
 * @param power_arr Pointer to the power
 * @param power_arr_pos Current position of the power pointer
 * @param num_samples Number of samples 
 * @return int Returns 1 if power exceeds threshold and 0 if it doesn't 
 */
int threshold_check(double * power_arr, uint16_t power_arr_pos, uint16_t num_samples){
#endif
#ifndef SIMULATION
int ICACHE_FLASH_ATTR threshold_check(double * power_arr, uint16_t power_arr_pos, uint16_t num_samples){
#endif
  double mean_var, std_var, th_var;
  mean_var = mean(power_arr, num_samples);
  std_var = std(power_arr, mean_var, num_samples);
  th_var = threshold(mean_var, std_var);
  if(power_arr[power_arr_pos] >= th_var){
    return 1;
  }else{
    return 0;
  }
}
