#include <nea/core/strings.h>
#include <nea/memory.h>
#include <proto/exec.h>
#include <string.h>

static void ___StringConcat(StringPtr instance, NEAString newString) {
  NEAString buffer = (NEAString)AllocVec(
    instance->length + strlen(newString) + 1, 
    NEA_MEMF_FLAGS
  );
  
  if (!buffer) return;
  
  strcat(buffer, instance->string);
  strcat(buffer, newString);
  
  FreeVec(instance->string);
  instance->string = buffer;
}

StringPtr __saveds __asm StringCreate(register __a0 NEAString fromString) {
  StringPtr string = (StringPtr)AllocVec(sizeof(String), NEA_MEMF_FLAGS);
  if (!string) return NULL;

  string->string = fromString;
  string->length = strlen(fromString);
  string->concat = ___StringConcat;
  string->allocated = FALSE;
  
  return string;
}

StringPtr __saveds __asm LIBStringCreate(register __a0 NEAString fromString) {
  return StringCreate(fromString);
}

StringPtr __saveds __asm StringDuplicate(register __a0 NEAString copyThisString) {
  StringPtr string = StringCreate(copyThisString);
  NEAString buffer;
  
  if (!string) return NULL;
  
  buffer = (NEAString)AllocVec(string->length * sizeof(char) + 1, NEA_MEMF_FLAGS);
  if (!buffer) {
    FreeVec(string);
    return NULL;
  }
  
  strcpy(buffer, string->string);
  string->string = buffer;
  string->allocated = TRUE;
  
  return string;
}

StringPtr __saveds __asm LIBStringDuplicate(register __a0 NEAString copyThisString) {
  return StringDuplicate(copyThisString);
}

void __saveds __asm StringRemove(register __a0 StringPtr instance) {
  if (instance) {
    if (instance->allocated) {
      FreeVec(instance->string);
    }
    FreeVec(instance);
  }
}

void __saveds __asm LIBStringRemove(register __a0 StringPtr instance) {
  StringRemove(instance);
}

StringPtr __saveds __asm StringConcat(
  register __a0 StringPtr instance,
  register __a1 NEAString addMe
) {
  ___StringConcat(instance, addMe);
  return instance; 
}

StringPtr __saveds __asm LIBStringConcat(
  register __a0 StringPtr instance,
  register __a1 NEAString addMe
) {
  return StringConcat(instance, addMe);
}
