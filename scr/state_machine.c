#include "state_machine.h"

enum states{
  SETUP,
  IDLE,
  PRE_RECORDING,
  RECORDING,
  TRANSMISSION
};

enum states state = SETUP;

void state_machine(){
  while(1){
    switch(state){
      case(SETUP):{
        #ifdef SIMULACAO
          printf("SIMULACAO DEFINED");
        #endif
        #ifndef SIMULACAO
          printf("NAO SIMULACAO");
        #endif

        state = IDLE;
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
  }
}

