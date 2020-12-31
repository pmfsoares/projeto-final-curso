
#include "user_config.h"
#include "simulation.h"
#include "state_machine.h"
#include <stdio.h>
#include <stdlib.h>

states_t state = SETUP;
int main(int argc, char** argv){
    state_engine(state);
    return 0;
}

