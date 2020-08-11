#include "user_config.h"

#ifdef SIMULACAO

#include "include/state_machine.h"
#include <stdio.h>
#include <stdlib.h>


#endif

#ifndef SIMULACAO
#include "include/state_machine.h"
#endif

enum states{
  SETUP,
  IDLE,
  PRE_RECORDING,
  RECORDING,
  TRANSMISSION
};

enum states state = SETUP;

void state_engine(){
  while(1){

  printf("\nEntering loop\n");
    switch(state){
      case(SETUP):{
        #ifdef SIMULACAO

          printf("\nIs Defined");
        #endif
        #ifndef SIMULACAO
          printf("\nNot defined");
        #endif

        break;
      }
      case(IDLE):{

        state = PRE_RECORDING;
        break;
      }
      case(PRE_RECORDING):{
        int th;
        th = 1;
        if(th){
          state = RECORDING;
        }else if(!th){
          state = IDLE;
        }
        break;
      }
      case(RECORDING):{
                
        state = TRANSMISSION;
        break;
      }
      case(TRANSMISSION):
        state = IDLE;
        break;
      }
      return;
  }
  printf("\nExited loop\n");
}

