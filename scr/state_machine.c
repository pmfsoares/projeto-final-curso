#include "estados.h"
#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"

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
      case(SETUP):


        state = IDLE;
        break;
      case(IDLE):

        state = PRE_RECORDING;
        break;

      case(PRE_RECORDING):

        if(th){
          state = RECORDING;
        }else if(!th){
          state = IDLE;
        }
        break;
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

