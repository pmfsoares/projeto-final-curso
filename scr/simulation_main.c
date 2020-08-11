#include <stdio.h>
#include <stdlib.h>

#include "user_config.h"
#include "include/state_machine.h"
#include "include/simulation.h"

int main(void *args){
    csv_file = openFile("sample.csv");
    printf("\nBefore\n");
    state_engine();
    printf("\nAfter\n");
    
    
    return 0;
}
