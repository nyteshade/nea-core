#ifndef NEA_CORE_STRINGS_H
#define NEA_CORE_STRINGS_H

#include <nea/types.h>

typedef struct String {
  NEAString string;
  NEAUInteger length;
  NEABoolean allocated;
  
  void (*concat)(struct String *instance, NEAString newString);
} String;

typedef struct String * StringPtr;

StringPtr __saveds __asm StringCreate(register __a0 NEAString fromString);
StringPtr __saveds __asm StringDuplicate(register __a0 NEAString copyThisString);
void      __saveds __asm StringRemove(register __a0 StringPtr instance);
StringPtr __saveds __asm StringConcat(register __a0 StringPtr instance, register __a1 NEAString addMe);

#endif
