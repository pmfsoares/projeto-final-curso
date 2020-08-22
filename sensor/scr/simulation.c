
#include <user_config.h>
#include <include/simulation.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <string.h>


FILE *  openFile(char *  name){
     FILE *  fp = fopen(name, "r");
     if(fp == NULL){
        fprintf(stderr, "Error reading files.\n");
        return (FILE*) NULL;
    }
     return fp;
}

int readSamples(FILE * file_ptr, double * simulation_arr){
    size_t count = 0;
    if(count == 0){
       char buffer[100];
       fgets(buffer, 100, file_ptr);
       //printf("\n%s\n", buffer);
    }
    count = 0;
    while(fscanf(file_ptr, "%lf,", &simulation_arr[count]) != EOF){
        if(count == num_samples){
            
            return count;
        }
        count++;
    }
    return count;
}
