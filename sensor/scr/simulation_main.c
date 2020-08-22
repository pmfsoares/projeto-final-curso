
#include "user_config.h"
#include "simulation.h"
#include "state_machine.h"
#include "posix_sockets.h"
#include <stdio.h>
#include <stdlib.h>

states_t state = SETUP;
int main(int argc, char** argv){
    printf("\nBefore\n");

    
    state_engine(state);

    printf("\nAfter\n");
    return 0;
}

