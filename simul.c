#include <stdin.h>
#include <stdlib.h>
#include <mem.h>
#include <string.h>
#include <errno.h>

#include "user_config.h"
#include "state_machine.h"

FILE *fp;
size_t count = 0;
double arr[num_samples];

fp = fopen("sample.csv", "r");

void openFile(String name){
    fp = fopen(name, "r");
     if(fp == NULL){
        fprintf(stderr, "Error reading files.\n");
        return 1;
    }   
}

int main(void *args){
    openFile("sample.csv");
    
}

double[] readSamples(void *args){
    if(count == 0){
       char buffer[100];
       fgets(buffer, 100, fp);
    }
    count = 0;
    while(fscanf(fp, "%d,", arr[count]) != EOF){
        if(count == num_samples){
            break;
        }
        count++;
    }
}