#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H


typedef enum{
  SETUP,
  IDLE,
  PRE_RECORDING,
  RECORDING,
  TRANSMISSION
}states_t;

void state_engine(states_t state);

#endif
