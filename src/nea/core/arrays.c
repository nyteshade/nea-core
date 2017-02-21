#include <nea/core/arrays.h>
#include <nea/memory.h>

#include <exec/memory.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <string.h>

static NEAUInteger __strlen(NEAString source) {
  NEAUInteger size = 0;
  NEAString p = source;
  while(*p) {size++; p = p + 1;}
  return size;
}

static NEAString __strdup(NEAString source) {
  NEAUInteger size = __strlen(source);
  NEAString buffer = (NEAString)AllocVec(sizeof(char) * size, NEA_MEMF_FLAGS);
  return buffer;
}

static Array *__appendPointer(Array *array, NEAPointer pointer) {
  return ArrayAppendPointer(array, pointer);
}

static Array *__appendString(Array *array, NEAString string) {
  return ArrayAppendString(array, string);
}

static Array *__appendInteger(Array *array, NEAInteger integer) {
  return ArrayAppendInteger(array, integer);
}

static Array *__appendDecimal(Array *array, NEADecimal decimal) {
  return ArrayAppendDecimal(array, decimal);
}

static Array *__appendByte(Array *array, NEAByte byte) {
  return ArrayAppendByte(array, byte);
}

static void __defaultFreeItemFn(NEAPointer item) {
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
  register __d1 NEAUInteger itemSize,
  register __d2 ArrayType type
) {
  Array *result;

  result = (Array*)AllocMem(sizeof(Array), NEA_MEMF_FLAGS);
  if (!result) {
    return NULL;
  }

  switch (type) {
    case NEA_USER_ARRAY:
    case NEA_PTR_ARRAY:
      result->pointers = (NEAPointer *)AllocVec(
        count * itemSize,
        NEA_MEMF_FLAGS
      );
      if (!result->pointers) {
        FreeVec(result);
        return NULL;
      }
      break;
    case NEA_STR_ARRAY:
      result->strings = (NEAString *)AllocVec(
        count * itemSize,
        NEA_MEMF_FLAGS
      );
      if (!result->strings) {
        FreeVec(result);
        return NULL;
      }
      break;
    case NEA_INT_ARRAY:
      result->integers = (NEAInteger *)AllocVec(
        count * itemSize,
        NEA_MEMF_FLAGS
      );
      if (!result->integers) {
        FreeVec(result);
        return NULL;
      }
      break;
    case NEA_DEC_ARRAY:
      result->decimals = (NEADecimal *)AllocVec(
        count * itemSize,
        NEA_MEMF_FLAGS
      );
      if (!result->decimals) {
        FreeVec(result);
        return NULL;
      }
      break;
    case NEA_BYTE_ARRAY:
      result->bytes = (NEAByte *)AllocVec(
        count * itemSize,
        NEA_MEMF_FLAGS
      );
      if (!result->bytes) {
        FreeVec(result);
        return NULL;
      }
      break;
  }

  result->type = type;
  result->count = 0;
  result->countMax = count;
  result->unitSize = itemSize;

  result->addPointer = __appendPointer;
  result->addString = __appendString;
  result->addInteger = __appendInteger;
  result->addDecimal = __appendDecimal;
  result->addByte = __appendByte;

  result->freeItemFn = NULL;

  return result;
}

Array * __saveds __asm LIBNewCustomArray(
  register __d0 NEAUInteger count,
  register __d1 NEAUInteger itemSize,
  register __d2 ArrayType type
) { return NewCustomArray(count, itemSize, type); }

/* ------------------------------------------------------------------------ */

Array * __saveds __asm NewArray(
  register __d0 NEAUInteger size,
  register __d1 ArrayType type
) {
  return NewCustomArray(size, ArrayTypeSize(type), type);
}

Array * __saveds __asm LIBNewArray(
  register __d0 NEAUInteger size,
  register __d1 ArrayType type
) { return NewArray(size, type); }

/* ------------------------------------------------------------------------ */

Array * __saveds __asm SetArray(
  register __a0 Array *array,
  register __d0 NEAUInteger size,
  register __a1 NEAPointer *items,
  register __d1 ArrayType type
) {
  NEAUInteger unitSize = array->unitSize
    ? array->unitSize
    : ArrayTypeSize(type);

  memset(array, 0L, size * unitSize);

  array->type = type;
  array->count = size;
  array->countMax = size;
  array->unitSize = unitSize;

  switch (type) {
    case NEA_USER_ARRAY:
    case NEA_PTR_ARRAY:   array->pointers = items; break;
    case NEA_STR_ARRAY:   array->strings = (NEAString *)items; break;
    case NEA_INT_ARRAY:   array->integers = (NEAInteger *)items; break;
    case NEA_DEC_ARRAY:   array->decimals = (NEADecimal *)items; break;
    case NEA_BYTE_ARRAY:  array->bytes = (NEAByte *)items; break;
    default: break;
  }

  return array;
}

Array * __saveds __asm LIBSetArray(
  register __a0 Array *array,
  register __d0 NEAUInteger size,
  register __a1 NEAPointer *items,
  register __d1 ArrayType type
) { return SetArray(array, size, items, type); }

/* ------------------------------------------------------------------------ */

Array * __saveds __asm NewPointerArray(
  register __d0 NEAUInteger size,
  register __a0 NEAPointer *copyThese
) {
  NEAUInteger i;
  Array *array = (Array *)NewArray(
    size,
    NEA_PTR_ARRAY
  );

  if (copyThese) {
    for (i = 0; i < size; i++) array->pointers[i] = __strdup(copyThese[i]);
    array->freeItemFn = __defaultFreeItemFn;
  }

  array->type = NEA_PTR_ARRAY;
  array->count = 0;
  array->countMax = size;
  array->unitSize = ArrayTypeSize(NEA_PTR_ARRAY);

  return array;
}

Array * __saveds __asm LIBNewPointerArray(
  register __d0 NEAUInteger size,
  register __a0 NEAPointer *copyThese
) { return NewPointerArray(size, copyThese); }

/* ------------------------------------------------------------------------ */

Array * __saveds __asm NewStringArray(
  register __d0 NEAUInteger size,
  register __a0 NEAString *copyThese
) {
  NEAUInteger i;
  Array *array = (Array *)NewArray(
    size,
    NEA_STR_ARRAY
  );

  if (copyThese) {
    for (i = 0; i < size; i++) array->strings[i] = __strdup(copyThese[i]);
    array->freeItemFn = __defaultFreeItemFn;
  }

  array->type = NEA_STR_ARRAY;
  array->count = 0;
  array->countMax = size;
  array->unitSize = ArrayTypeSize(NEA_STR_ARRAY);

  return array;
}

Array * __saveds __asm LIBNewStringArray(
  register __d0 NEAUInteger size,
  register __a0 NEAString *copyThese
) { return NewStringArray(size, copyThese); }

/* ------------------------------------------------------------------------ */

Array * __saveds __asm NewIntegerArray(
  register __d0 NEAUInteger size,
  register __a0 NEAInteger *copyThese
) {
  Array *array = (Array *)NewArray(size, NEA_INT_ARRAY);
  NEAUInteger i;

  if (copyThese) {
    for (i = 0; i < size; i++) array->integers[i] = copyThese[i];
  }

  array->type = NEA_INT_ARRAY;
  array->count = copyThese ? size : 0;
  array->countMax = size;
  array->unitSize = ArrayTypeSize(NEA_INT_ARRAY);

  return array;
}

Array * __saveds __asm LIBNewIntegerArray(
  register __d0 NEAUInteger size,
  register __a0 NEAInteger *copyThese
) { return NewIntegerArray(size, copyThese); }

/* ------------------------------------------------------------------------ */

Array * __saveds __asm NewDecimalArray(
  register __d0 NEAUInteger size,
  register __a0 NEADecimal *copyThese
) {
  Array *array = (Array *)NewArray(size, NEA_DEC_ARRAY);
  NEAUInteger i;

  if (copyThese) {
    for (i = 0; i < size; i++) array->decimals[i] = copyThese[i];
  }

  array->type = NEA_DEC_ARRAY;
  array->count = 0;
  array->countMax = size;
  array->unitSize = ArrayTypeSize(NEA_DEC_ARRAY);

  return array;
}

Array * __saveds __asm LIBNewDecimalArray(
  register __d0 NEAUInteger size,
  register __a0 NEADecimal *copyThese
) { return NewDecimalArray(size, copyThese); }

/* ------------------------------------------------------------------------ */

Array * __saveds __asm NewByteArray(
  register __d0 NEAUInteger size,
  register __a0 NEAByte *copyThese
) {
  Array *array = (Array *)NewArray(size, NEA_BYTE_ARRAY);
  NEAUInteger i;

  if (copyThese) {
    for (i = 0; i < size; i++) array->bytes[i] = copyThese[i];
  }

  array->type = NEA_BYTE_ARRAY;
  array->count = 0;
  array->countMax = size;
  array->unitSize = ArrayTypeSize(NEA_BYTE_ARRAY);

  return array;
}

Array * __saveds __asm LIBNewByteArray(
  register __d0 NEAUInteger size,
  register __a0 NEAByte *copyThese
) { return NewByteArray(size, copyThese); }


/* ------------------------------------------------------------------------ */

void __saveds __asm FreeArray(register __a0 Array *array) {
  NEAUInteger i;
  void *allocated = NULL;

  if (array->freeItemFn) {
    for (i = 0; i < array->count; i++) {
      switch (array->type) {
        case NEA_PTR_ARRAY:
        case NEA_USER_ARRAY:
          array->freeItemFn((void *)array->pointers[i]);
        case NEA_STR_ARRAY:
          array->freeItemFn((void *)array->strings[i]);
        default:
          break;
      }
    }
  }

  FreeVec(ArrayRawData(array));
  FreeVec(array);
}

void __saveds __asm LIBFreeArray(register __a0 Array *array) {
  FreeArray(array);
}

/* ------------------------------------------------------------------------ */

Array * __saveds __asm ArrayAppend(
  register __a0 Array *array,
  register __a1 NEAPointer item,
  register __d0 ArrayType type
) {
  NEAUInteger count = array->count, i;
  NEAPointer pointer;
  NEAString string;
  NEAInteger integer;
  NEADecimal decimal;
  NEAByte byte;

  if (array->count + 1 > array->countMax) {
    ArrayResizeData(array, array->countMax + 1);
  }

  switch (array->type) {
    case NEA_USER_ARRAY:
    case NEA_PTR_ARRAY:
      pointer = item;
      array->pointers[count] = pointer;
      break;
    case NEA_STR_ARRAY:
      string = item;
      array->strings[count] = (NEAString)item;
      break;
    case NEA_INT_ARRAY:
      integer = *(NEAInteger *)item;
      array->integers[count] = integer;
      break;
    case NEA_DEC_ARRAY:
      decimal = *(NEADecimal *)item;
      array->decimals[count] = decimal;
      break;
    case NEA_BYTE_ARRAY:
      byte = *(NEAByte *)item;
      array->bytes[count] = byte;
      break;
    default:
      break;
  }

  array->count++;

  return array;
}

Array * __saveds __asm LIBArrayAppend(
  register __a0 Array *array,
  register __a1 NEAPointer item,
  register __d0 ArrayType type
) { return ArrayAppend(array, item, type); }

/* ------------------------------------------------------------------------ */

Array * __saveds __asm ArrayAppendPointer(
  register __a0 Array *array,
  register __a1 NEAPointer pointer
) { return ArrayAppend(array, pointer, NEA_PTR_ARRAY); }

Array * __saveds __asm LIBArrayAppendPointer(
  register __a0 Array *array,
  register __a1 NEAPointer pointer
) { return ArrayAppendPointer(array, pointer); }

/* ------------------------------------------------------------------------ */

Array * __saveds __asm ArrayAppendString(
  register __a0 Array *array,
  register __a1 NEAString string
) { return ArrayAppend(array, string, NEA_STR_ARRAY); }

Array * __saveds __asm LIBArrayAppendString(
  register __a0 Array *array,
  register __a1 NEAString string
) { return ArrayAppendString(array, string); }

/* ------------------------------------------------------------------------ */

Array * __saveds __asm ArrayAppendInteger(
  register __a0 Array *array,
  register __d0 NEAInteger integer
) {
  NEAInteger stackValue = integer;
  return ArrayAppend(array, &stackValue, NEA_INT_ARRAY);
}

Array * __saveds __asm LIBArrayAppendInteger(
  register __a0 Array *array,
  register __d0 NEAInteger integer
) { return ArrayAppendInteger(array, integer); }

/* ------------------------------------------------------------------------ */

Array * __saveds __asm ArrayAppendDecimal(
  register __a0 Array *array,
  register __d0 NEADecimal decimal
) {
  NEADecimal stackValue = decimal;
  return ArrayAppend(array, &stackValue, NEA_DEC_ARRAY);
}

Array * __saveds __asm LIBArrayAppendDecimal(
  register __a0 Array *array,
  register __d0 NEADecimal decimal
) { return ArrayAppendDecimal(array, decimal); }

/* ------------------------------------------------------------------------ */

Array * __saveds __asm ArrayAppendByte(
  register __a0 Array *array,
  register __d0 NEAByte byte
) {
  NEAByte stackValue = byte;
  return ArrayAppend(array, &stackValue, NEA_BYTE_ARRAY);
}

Array * __saveds __asm LIBArrayAppendByte(
  register __a0 Array *array,
  register __d0 NEAByte byte
) { return ArrayAppendByte(array, byte); }

/* ------------------------------------------------------------------------ */

void * __saveds __asm ArrayRawData(
  register __a0 Array *array
) {
  /* Fairly certain I could always simply return array->pointers
     but since I am not truly 100% certain, we incur a bit of
     overhead here.
  */
  switch (array->type) {
    case NEA_USER_ARRAY:
    case NEA_PTR_ARRAY:     return (void *)array->pointers;
    case NEA_STR_ARRAY:     return (void *)array->strings;
    case NEA_INT_ARRAY:     return (void *)array->integers;
    case NEA_DEC_ARRAY:     return (void *)array->decimals;
    case NEA_BYTE_ARRAY:    return (void *)array->bytes;
    default:                return NULL;
  }
}

void * __saveds __asm LIBArrayRawData(
  register __a0 Array *array
) { return ArrayRawData(array); }

/* ------------------------------------------------------------------------ */

Array * __saveds __asm ArrayResizeData(
  register __a0 Array *array,
  register __d0 NEAUInteger newSize
) {
  void *oldData = ArrayRawData(array);
  void *newData;
  NEAUInteger srcSize = array->count;

  if (newSize < array->count) {
    srcSize = newSize;
  }

  newData = (void *)AllocVec(
    array->unitSize * newSize,
    NEA_MEMF_FLAGS
  );

  if (oldData) {
    memcpy(newData, oldData, array->unitSize * srcSize);
    FreeVec(oldData);
  }

  array->pointers = newData;
  array->countMax = newSize;
  array->count = array->count > array->countMax
    ? array->countMax
    : array->count;

  return array;
}

Array * __saveds __asm LIBArrayResizeData(
  register __a0 Array *array,
  register __d0 NEAUInteger newSize
) { return ArrayResizeData(array, newSize); }

/* ------------------------------------------------------------------------ */
