/**
 * @file simulation.c
 * @author Pedro Soares (p.soares1997@live.com.pt) @ Projeto EI - ESTG IPP
 * @brief Simulation only, for reading the csv file for the samples
 * @version 0.1
 * @date 2020-12-03
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <user_config.h>
#include <include/simulation.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

/**
 * @brief 
 * 
 * @param name 
 * @return FILE* 
 */
FILE *  openFile(char *  name){
     FILE *  fp = fopen(name, "r");
     if(fp == NULL){
        fprintf(stderr, "Error reading files.\n");
        return (FILE*) NULL;
    }
     return fp;
}
/**
 * @brief 
 * 
 * @param file_ptr 
 * @param simulation_arr 
 * @param type 
 * @param num_samples 
 * @return int 
 */
int readSamples(FILE * file_ptr, double* simulation_arr, int type, uint16_t num_samples){
    size_t count = 0;
    if(type == 1){
        char buffer[100];
        fgets(buffer, 100, file_ptr);
        printf("\n%s\n", buffer);
    }
    if(type == 1 || type == 2){
        count = 0;
        while(fscanf(file_ptr, "%lf,", &simulation_arr[count]) != EOF){
            if(count == num_samples){

                return count;
            }
            count++;
        }
    }
    return count;
}
