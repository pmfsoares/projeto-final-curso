#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <string.h>

#include "user_config.h"
#include "state_machine.h"

FILE *csv_file;
size_t count = 0;
double arr[num_samples];


FILE *  openFile(char *  name){
     FILE *  fp = fopen(name, "r");
     if(fp == NULL){
        fprintf(stderr, "Error reading files.\n");
        return (FILE*) NULL;
    }
     return fp;
}

int main(void *args){
    csv_file = openFile("sample.csv");
    

    int a;
    a = state_machine();
    /*
    for(int i=0; i < 6; i++){
        readSamples();
        printSamples();
    }*/
   return 0; 
}

void printSamples(void *args){
    //for(int i=0; i < num_samples; i++){
    //    printf("\nLinha %d: %.17lf", i, arr[i]);
    //}
    printf("\nLinha: %.17lf", arr[3999]);

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
}
