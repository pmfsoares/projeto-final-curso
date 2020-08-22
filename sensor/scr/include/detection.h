#ifndef DETECTION_H
#define DETECTION_H

#include <user_config.h>
#include <stdint.h>

double mean(double * arr);

double std(double * arr, double media);

int powerSequence(double* samples, double* power, uint16_t k);

double threshold(double *  power, double mean, double var);

#endif
