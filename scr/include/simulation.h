#ifndef SIMULATION_H
#define SIMULATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <string.h>

FILE *csv_file;
size_t count = 0;

FILE * openFile(char * name);

int readSampels(void * args);

#endif