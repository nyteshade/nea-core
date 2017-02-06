#include "nea.h"

#include <stdio.h>
#include <string.h>
#include <proto/exec.h>
#include <exec/memory.h>

char * __saveds __asm LIBsayHi(
  register __d0 int length, 
  register __a0 char *string
) {
  char *result;
  result = (char *)AllocVec(100 + strlen(string), MEMF_ANY|MEMF_CLEAR);
  sprintf(result, "The string is \"%s\" and it's length is %d\n", string, length);
  return result;
}
