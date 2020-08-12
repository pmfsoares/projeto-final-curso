#include "user_config.h"


#ifdef SIMULACAO
#include "state_machine.h"
#include "simulation.h"
#include "detection.h"
#include <stdio.h>
#include <stdlib.h>

FILE * csv_file;

#endif

#ifndef SIMULACAO
#include "state_machine.h"
#endif



void state_engine(states_t state){
  double sample_arr[num_samples];
  double power_arr[num_samples/2];
  uint16_t k = 0;
  printf("\nENTROU\n");
  while(1){
    switch(state){
      case(SETUP):{
        printf("\nSETUP\n");
        #ifdef SIMULACAO
          csv_file = openFile("samples.csv");
        #endif
        state = IDLE;
        break;
      }
      case(IDLE):{
        printf("\nON IDLE\n");
        #ifdef SIMULACAO
          readSamples(csv_file, sample_arr);
          for(int a=0; a < num_samples/10; a++){
             printf("\n%lf", sample_arr[a]);
          }
        #endif
        #ifndef SIMULACAO
          printf("Not defined");
        #endif
        state = PRE_RECORDING;
        break;
      }
      case(PRE_RECORDING):{
        printf("\nPre-Rec\n");
        k = powerSequence(sample_arr, power_arr, k);
        double th;
        th = threshold(power_arr);
        printf("\nTh: %d", th);
        if(th > 0){
          state = RECORDING;
        }else if(!th){
          state = IDLE;
        }
        break;
      }
      case(RECORDING):{
        printf("\nRecording\n");
        state = TRANSMISSION;
        break;
      }
      case(TRANSMISSION):{
        printf("\nTransmission\n");
        state = IDLE;
        return;
        break;
      }
    }
  }
  printf("\nExited loop\n");
}

