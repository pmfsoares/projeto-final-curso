#ifndef SIMULATION_H
#define SIMULATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <string.h>


FILE * openFile(char * name);

int readSamples(FILE * file_ptr);

#endif