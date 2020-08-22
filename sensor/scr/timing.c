#include "timing.h"

#include <c_types.h>
#include <osapi.h>
#include <user_interface.h>
#include <ets_sys.h>
#include <gpio.h>

static inline int32_t asm_ccount(void){
  int32_t r; asm volatile("rsr %0, ccount" : "=r"(r)); return r; }