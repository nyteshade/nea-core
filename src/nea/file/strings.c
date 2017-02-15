#include <nea/file/strings.h>
#include <stdio.h>
#include <string.h>

/* -------------------------------------------------------------------------*/

NEAFString * __asm __saveds SetFString(
  register __a0 NEAString string,
  register __a1 NEAFString *fstring
) {
  if (!fstring) {
    fstring = (NEAFString *)AllocVec(sizeof(NEAFString), NEA_MEMF_FLAGS);
    if (!fstring) {
      return NULL;
    }
  }
  
  fstring->value = string;
  fstring->length = strlen(string) + 1;
  fstring->allocated = FALSE;
  
  return fstring;
}

NEAFString * __asm __saveds LIBSetFString(
  register __a0 NEAString string,
  register __a1 NEAFString *fstring
) {
  return SetFString(string, fstring);
}

/* -------------------------------------------------------------------------*/

NEAFString * __asm __saveds DupFString(
  register __a0 NEAFString *fstring,
  register __d0 NEABoolean dupStringToo
) {
  NEAFString *result = (NEAFString *)AllocVec(
    sizeof(NEAFString), 
    NEA_MEMF_FLAGS
  );
  if (!result) {
    return NULL;
  }
  
  result->length = fstring->length;
  if (dupStringToo) {
    result->value = (NEAString)AllocVec(
      sizeof(char) * fstring->length, 
      NEA_MEMF_FLAGS
    );
    if (!result->value) {
      FreeVec(result);
      return NULL;
    }
    result->allocated = TRUE;
    CopyMemQuick(fstring->value, result->value, fstring->length);
  }
  else {
    result->value = fstring->value;
    result->allocated = FALSE;
  }
  
  return result;
}

NEAFString * __asm __saveds LIBDupFString(
  register __a0 NEAFString *fstring,
  register __d0 NEABoolean dupStringToo
) {
  return DupFString(fstring, dupStringToo);
}

/* -------------------------------------------------------------------------*/

void __asm __saveds FreeFString(
  register __a0 NEAFString *fstring
) {
  if (fstring) {
    if (fstring->allocated && fstring->value) {
      FreeVec(fstring->value);
      fstring->value = 0L;
    }
    FreeVec(fstring);
  }
}

void __asm __saveds LIBFreeFString(
  register __a0 NEAFString *fstring
) {
  FreeFString(fstring);
}

/* -------------------------------------------------------------------------*/

LONG __asm __saveds WriteFString(
  register __d0 BPTR filePtr,
  register __a0 NEAFString *fstring
) {
  LONG bytes = 0;
  
  /* Write the length and then the string. This is so
   * we can allocate memory when reading before we try
   * to read the string from the file. */
  bytes += Write(filePtr, &fstring->length, sizeof(NEAInteger));
  bytes += Write(filePtr, fstring->value, fstring->length);
  
  return bytes;
}

LONG __asm __saveds LIBWriteFString(
  register __d0 BPTR filePtr,
  register __a0 NEAFString *fstring
) {
  return WriteFString(filePtr, fstring);
}

/* -------------------------------------------------------------------------*/

NEAFString * __asm __saveds ReadFString(
  register __d0 BPTR filePtr,
  register __a0 NEAFString *fstring
) {
  NEAFString *result = fstring;
  
  if (!result) {
    result = (NEAFString*)AllocVec(sizeof(NEAFString), NEA_MEMF_FLAGS);
    if (!result) {
      return NULL;
    }
  }
  
  Read(filePtr, &result->length, sizeof(NEAInteger));
  if (!fstring) {
    result->value = (NEAString)AllocVec(
      sizeof(NEAByte) * result->length, 
      NEA_MEMF_FLAGS
    ); 
    if (!result->value) {
      FreeVec(result);
      return NULL;
    }
  }
  Read(filePtr, result->value, result->length);
  
  return result;
}

NEAFString * __asm __saveds LIBReadFString(
  register __d0 BPTR filePtr,
  register __a0 NEAFString *fstring
) {
  return ReadFString(filePtr, fstring);
}

/* -------------------------------------------------------------------------*/

NEAString __asm __saveds FStringToString(
  register __a0 NEAFString *fstring,
  register __a1 NEAString string,
  register __a2 NEAString note
) {
  NEAString result = string;
  NEAByte size = 48;
  NEAByte template[100];
  NEAByte number[20];
  
  memset(template, 0L, 100);
  memset(number, 0L, 20);
  
  if (!fstring) {
    return NULL;
  }
    
  if (note) {
    sprintf(template, " [%s]", note);
  }
  else {
    sprintf(template, "");
  }

  if (!result) {
    /* Convert the length to a string for sizing purposes */
    sprintf(number, "%lu", fstring->length);
    
    /* Adjust estimated size of String */
    size += strlen(fstring->value);
    size += strlen(number);
    size += fstring->allocated ? 4 : 5;
    size += strlen(template);
    
    result = (NEAString)AllocVec(sizeof(NEAByte) * size, NEA_MEMF_FLAGS);
    if (!result) {
      return NULL;
    }
  }
  
  sprintf(
    result, 
    "NEAFString%s\n  value    : %s\n  length   : %ld\n  allocated: %s\n",
    template, 
    fstring->value, 
    fstring->length, 
    fstring->allocated ? "true" : "false"
  );
    
  return result;
}

NEAString __asm __saveds LIBFStringToString(
  register __a0 NEAFString *fstring,
  register __a1 NEAString string,
  register __a2 NEAString note
) {
  return FStringToString(fstring, string, note);
}
