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
  NEA_BYTE_ARRAY = 32,
  NEA_USER_ARRAY = 64
} ArrayType;

typedef struct Array {
  union {
    NEAPointer *pointers;
    NEAString *strings;
    NEAInteger *integers;
    NEADecimal *decimals;
    NEAByte *bytes;
  };

  ArrayType type;
  NEAUInteger count;
  NEAUInteger countMax;
  NEAUInteger unitSize;

  struct Array *(*addPointer)(struct Array *array, NEAPointer pointer);
  struct Array *(*addString)(struct Array *array, NEAString string);
  struct Array *(*addInteger)(struct Array *array, NEAInteger integer);
  struct Array *(*addDecimal)(struct Array *array, NEADecimal decimal);
  struct Array *(*addByte)(struct Array *array, NEAByte byte);

  FreeArrayItemFn freeItemFn;
} Array;

/* -- Creation and deletion functions ------------------------------------- */

NEAUInteger __saveds __asm ArrayTypeSize(
  register __d0 ArrayType type
);

Array * __saveds __asm NewCustomArray(
  register __d0 NEAUInteger count,
  register __d1 NEAUInteger itemSize,
  register __d2 ArrayType type
);

Array * __saveds __asm NewArray(
  register __d0 NEAUInteger size,
  register __d1 ArrayType type
);

Array * __saveds __asm SetArray(
  register __a0 Array *array,
  register __d0 NEAUInteger size,
  register __a1 NEAPointer *items,
  register __d1 ArrayType type
);

Array * __saveds __asm NewPointerArray(
  register __d0 NEAUInteger size,
  register __a0 NEAPointer *copyThese
);

Array * __saveds __asm NewStringArray(
  register __d0 NEAUInteger size,
  register __a0 NEAString *copyThese
);

Array * __saveds __asm NewIntegerArray(
  register __d0 NEAUInteger size,
  register __a0 NEAInteger *copyThese
);

Array * __saveds __asm NewDecimalArray(
  register __d0 NEAUInteger size,
  register __a0 NEADecimal *copyThese
);

Array * __saveds __asm NewByteArray(
  register __d0 NEAUInteger size,
  register __a0 NEAByte *copyThese
);

void __saveds __asm FreeArray(register __a0 Array *array);

/* -- Manipulation functions ---------------------------------------------- */

Array * __saveds __asm ArrayAppend(
  register __a0 Array *array,
  register __a1 NEAPointer item,
  register __d0 ArrayType type
);

Array * __saveds __asm ArrayAppendPointer(
  register __a0 Array *array,
  register __a1 NEAPointer pointer
);

Array * __saveds __asm ArrayAppendString(
  register __a0 Array *array,
  register __a1 NEAString string
);

Array * __saveds __asm ArrayAppendInteger(
  register __a0 Array *array,
  register __d0 NEAInteger integer
);

Array * __saveds __asm ArrayAppendDecimal(
  register __a0 Array *array,
  register __d0 NEADecimal decimal
);

Array * __saveds __asm ArrayAppendByte(
  register __a0 Array *array,
  register __d0 NEAByte byte
);

void * __saveds __asm ArrayRawData(
  register __a0 Array *array
);

Array * __saveds __asm ArrayResizeData(
  register __a0 Array *array,
  register __d0 NEAUInteger newSize
);

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
