#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <string.h>


#include "user_config.h"
#include "include/simulation.h"

double arr[num_samples];

FILE *  openFile(char *  name){
     FILE *  fp = fopen(name, "r");
     if(fp == NULL){
        fprintf(stderr, "Error reading files.\n");
        return (FILE*) NULL;
    }
     return fp;
}

int readSamples(void *args){
    count++;
    if(count == 1){
       char buffer[100];
       fgets(buffer, 100, csv_file);
       printf("\n%s", buffer);
    }
    count = 0;
    while(fscanf(csv_file, "%lf,", &arr[count]) != EOF){
        printf("\n%lf", arr[count]);
        if(count == num_samples){
            return 1;
        }
        count++;
    }
    return 1;
}
