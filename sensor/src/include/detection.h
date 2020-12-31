/**
 * @file detection.h
 * @author Pedro Soares (p.soares1997@live.com.pt) @ Projeto EI - ESTG IPP
 * @brief Detection header
 * @version 0.1
 * @date 2020-12-03
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef DETECTION_H
#define DETECTION_H

#include <user_config.h>
#include <stdint.h>

double mean(double * power_arr, int num_samples);

double std(double * power_arr, double mean, int num_samples);

double powerSequence(double* samples, double* power, int power_arr_pos, int num_samples);


double threshold(double mean, double std_var);

int threshold_check(double * power_arr, uint16_t power_arr_pos, uint16_t num_samples);

#endif
