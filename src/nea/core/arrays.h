#ifndef NEA_CORE_ARRAYS_H
#define NEA_CORE_ARRAYS_H

#include <stdlib.h>
#include <nea/types.h>

typedef void (*FreeArrayItemFn)(NEAPointer item);

typedef enum ArrayType {
  NEA_ARRAY = 1,
  NEA_PTR_ARRAY = 2,
  NEA_STR_ARRAY = 4,
  NEA_INT_ARRAY = 8,
  NEA_DEC_ARRAY = 16,
  NEA_BYTE_ARRAY = 32
} ArrayType;

typedef struct Array {
  NEAPointer *items;
  NEAInteger count;
  FreeArrayItemFn freeFn;
} Array;

typedef struct PointerArray {
  NEAPointer *pointers;
  NEAInteger count;
  FreeArrayItemFn freeFn;
} Array;

typedef struct StringArray {
  NEAString *strings;
  NEAUInteger count;
  FreeArrayItemFn freeFn;
} StringArray;

typedef struct IntegerArray {
  NEAInteger **integers;
  NEAUInteger count;
  FreeArrayItemFn freeFn;
} IntegerArray;

typedef struct DecimalArray {
  NEADecimal **decimals;
  NEAUInteger count;
  FreeArrayItemFn freeFn;
} IntegerArray;

typedef struct ByteArray {
  NEAByte **bytes;
  NEAUInteger count;
  FreeArrayItemFn freeFn;
} ByteArray;

/* -- Creation and deletion functions ------------------------------------- */

NEAUInteger __saveds __asm ArrayTypeSize(
  register __d0 ArrayType type
);

Array * __saveds __asm NewCustomArray(
  register __d0 NEAUInteger count,
  register __d1 NEAUInteger itemSize
);

Array * __saveds __asm NewArray(
  register __d0 NEAUInteger size,
  register __d1 ArrayType type
);

Array * __saveds __asm SetArray(
  register __a0 Array *array,
  register __d0 NEAUInteger size,
  register __a1 NEAPointer *items
);

StringArray * __saveds __asm NewStringArray(
  register __d0 NEAUInteger size,
  register __a0 NEAString *copyThese
);

IntegerArray * __saveds __asm NewIntegerArray(
  register __d0 NEAUInteger size,
  register __a0 NEAInteger **copyThese
);

DecimalArray * __saveds __asm NewDecimalArray(
  register __d0 NEAUInteger size,
  register __a0 NEADecimal **copyThese
);

ByteArray * __saveds __asm NewByteArray(
  register __d0 NEAUInteger size,
  register __a0 NEAByte **copyThese
);

void __saveds __asm FreeArray(register __a0 Array *array);

/* -- Manipulation functions ---------------------------------------------- */

// TODO ArrayAppend(array, item)
// TODO ArrayConcat(array, array)
// TODO ArrayInsert(array, index, item)
// TODO ArrayRemove(array, index)
// TODO ArrayPrepend(array, item)
// TODO consider pop, push and other LIFO,FIFO methods

// TODO ArrayForEach(array, forEachFn)
// TODO ArrayMap(array, mapFn)
// TODO ArrayReduce(array reduceFn)
// TODO consider like methods from JavaScript

#endif
