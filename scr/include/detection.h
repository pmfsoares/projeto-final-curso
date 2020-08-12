#ifndef DETECTION_H
#define DETECTION_H

#include <user_config.h>
#include <stdint.h>

double mean(double * arr);

double std(double * arr, double media);

int powerSequence(double * arr, double * power);

double threshold();

#endif
