#include <stdio.h>
#include <stdlib.h>

#include "user_config.h"
#include "include/state_machine.h"
#include "include/simulation.h"

FILE *csv_file;

int main(void *args){
    csv_file = openFile("samples.csv");
    readSamples(csv_file);
    printf("\nBefore\n");
    state_engine();
    printf("\nAfter\n");
    
    
    return 0;
}
