#include "include/state_machine.h"
#include "user_config.h"


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
    switch(state){
      case(SETUP):{
        #ifdef SIMULACAO

          printf("\nIs Define");
          return;
        #endif
        #ifndef SIMULACAO
          printf("\nNot defined");
          return;
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
      return;
  }
}

