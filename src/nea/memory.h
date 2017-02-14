#ifndef NEA_MEMORY_H
#define NEA_MEMORY_H

#include <exec/memory.h>

/** 
 * The library uses AllocVec/FreeVec for all of its memory allocation
 * routines. These flags are included and utilised throughout the 
 * various modules. If there is a desire to have memory allocated using
 * different flags, then declare NEA_MEMF_FLAGS with the values you wish
 * at the time of compile.
 */
#ifndef NEA_MEMF_FLAGS
#define NEA_MEMF_FLAGS (MEMF_ANY|MEMF_CLEAR)
#endif

#endif