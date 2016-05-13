#include <stdint.h>

extern void* __stack;
extern void _c_int00();


void excHandler(void)
{
  while(1);
}

/* const because it is meant to be placed in ROM */
#pragma RETAIN(resetVectors)
#pragma location = 0;
const uint32_t resetVectors[] = {
    (uint32_t)(&__stack),     /* Stack Base */
    (uint32_t)(&_c_int00),    /* Reset */
    (uint32_t)(&excHandler),  /* NMI */
    (uint32_t)(&excHandler),  /* Hard Fault */
    (uint32_t)(&excHandler),  /* Mem Fault */
    (uint32_t)(&excHandler),  /* Bus Fault */
    (uint32_t)(&excHandler),  /* Usage Fault */
    (uint32_t)(&excHandler),  /* reserved */
    (uint32_t)(&excHandler),  /* reserved */
    (uint32_t)(&excHandler),  /* reserved */
    (uint32_t)(&excHandler),  /* reserved */
    (uint32_t)(&excHandler),  /* SVCall */
    (uint32_t)(&excHandler),  /* Debug Mon */
    (uint32_t)(&excHandler),  /* reserved */
    (uint32_t)(&excHandler),  /* pendSV */
};
