#include <nea/core/arrays.h>

static NEAString *__strdup(NEAString *source) {
  NEAUInteger size = strlen(source);
  NEAString buffer = (NEAString)AllocVec(sizeof(char) * size, NEA_MEMF_FLAGS);
  return buffer;
}

static void __defaultFreeFn(NEAPointer item) {
  if (item) {
    FreeVec(item);
  }
}

/* ------------------------------------------------------------------------ */

NEAUInteger __saveds __asm ArrayTypeSize(
  register __d0 ArrayType type
) {
  switch (type) {
    case NEA_ARRAY:       return sizeof(NEAPointer);
    case NEA_PTR_ARRAY:   return sizeof(NEAPointer);
    case NEA_STR_ARRAY:   return sizeof(NEAString);
    case NEA_INT_ARRAY:   return sizeof(NEAInteger);
    case NEA_DEC_ARRAY:   return sizeof(NEADecimal);
    case NEA_BYTE_ARRAY:  return sizeof(NEAByte);
    default:              return sizeof(NEAPointer);
  }
}

NEAUInteger __saveds __asm LIBArrayTypeSize(
  register __d0 ArrayType type
) { return ArrayTypeSize(type); }


/* ------------------------------------------------------------------------ */

Array * __saveds __asm NewCustomArray(
  register __d0 NEAUInteger count,
  register __d1 NEAUInteger itemSize
) {
  Array *result = (Array*)AllocMem(itemSize * count, NEA_MEMF_FLAGS);

  if (!result) {
    return NULL;
  }

  return result;
}

Array * __saveds __asm LIBNewCustomArray(
  register __d0 NEAUInteger count,
  register __d1 NEAUInteger itemSize
) { return NewCustomArray(count, itemSize); }

/* ------------------------------------------------------------------------ */

Array * __saveds __asm NewArray(
  register __d0 NEAUInteger size,
  register __d1 ArrayType type
) {
  return NewCustomArray(size, ArrayTypeSize(type));
}

Array * __saveds __asm LIBNewArray(
  register __d0 NEAUInteger size,
  register __d1 ArrayType type
) { return NewArray(size, type); }

/* ------------------------------------------------------------------------ */

Array * __saveds __asm SetArray(
  register __a0 Array *array,
  register __d0 NEAUInteger size,
  register __a1 NEAPointer *items
) {
  memset(array, 0L, size);
  array->items = items;
  array->size = size;

  return array;
}

Array * __saveds __asm LIBSetArray(
  register __a0 Array *array,
  register __d0 NEAUInteger size,
  register __a1 NEAPointer *items
) { return SetArray(array, size, items); }

/* ------------------------------------------------------------------------ */

StringArray * __saveds __asm NewStringArray(
  register __d0 NEAUInteger size,
  register __a0 NEAString *copyThese
) {
  NEAUInteger i;
  StringArray *array = (StringArray *)NewArray(
    size,
    NEA_STR_ARRAY
  );

  if (copyThese) {
    for (i = 0; i < size; i++) array->items[i] = __strdup(copyThese[i]);
    array->freeFn = __defaultFreeFn;
  }

  return array;
}

StringArray * __saveds __asm LIBNewStringArray(
  register __d0 NEAUInteger size,
  register __a0 NEAString *copyThese
) { return NewStringArray(size, copyThese); }

/* ------------------------------------------------------------------------ */

IntegerArray * __saveds __asm NewIntegerArray(
  register __d0 NEAUInteger size,
  register __a0 NEAInteger **copyThese
) {
  IntegerArray *array = (IntegerArray *)NewArray(size, NEA_INT_ARRAY);
  NEAUInteger i;

  if (copyThese) {
    for (i = 0; i < size; i++) array->integers[i] = copyThese[i];
  }

  return array;
}

IntegerArray * __saveds __asm LIBNewIntegerArray(
  register __d0 NEAUInteger size,
  register __a0 NEAInteger **copyThese
) { return NewIntegerArray(size, copyThese); }

/* ------------------------------------------------------------------------ */

DecimalArray * __saveds __asm NewDecimalArray(
  register __d0 NEAUInteger size,
  register __a0 NEADecimal **copyThese
) {
  DecimalArray *array = (DecimalArray *)NewArray(size, NEA_DEC_ARRAY);
  NEAUInteger i;

  if (copyThese) {
    for (i = 0; i < size; i++) array->decimals[i] = copyThese[i];
  }

  return array;
}

DecimalArray * __saveds __asm LIBNewDecimalArray(
  register __d0 NEAUInteger size,
  register __a0 NEADecimal **copyThese
) { return NewDecimalArray(size, copyThese); }

/* ------------------------------------------------------------------------ */

ByteArray * __saveds __asm NewByteArray(
  register __d0 NEAUInteger size,
  register __a0 NEAByte **copyThese
) {
  ByteArray *array = (ByteArray *)NewArray(size, NEA_BYTE_ARRAY);
  NEAUInteger i;

  if (copyThese) {
    for (i = 0; i < size; i++) array->bytes[i] = copyThese[i];
  }

  return array;
}

ByteArray * __saveds __asm LIBNewByteArray(
  register __d0 NEAUInteger size,
  register __a0 NEAByte **copyThese
) { return NewByteArray(size, copyThese); }


/* ------------------------------------------------------------------------ */

void __saveds __asm FreeArray(register __a0 Array *array) {
  NEAUInteger i;

  if (array->freeFn) {
    for (i = 0; i < array->count; i++) array->FreeFn(array->items[i]);
  }

  FreeVec(array);
}

void __saveds __asm LIBFreeArray(register __a0 Array *array) {
  FreeArray(array);
}

/* ------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------ */
