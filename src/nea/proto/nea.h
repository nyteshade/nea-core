#ifndef NEA_LIBRARY_H
#define NEA_LIBRARY_H

#include <nea/types.h>

char * __saveds __asm 
sayHi(register __d0 int length, register __a0 char *string);

NEAPoint * __saveds __asm 
NewPointI(
  register __d0 NEAInteger x,
  register __d1 NEAInteger y,
  register __d2 NEAInteger z
);

#endif
