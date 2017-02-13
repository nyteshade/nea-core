#ifndef NEA_TYPES_H
#define NEA_TYPES_H

#include <exec/memory.h>

typedef char           * NEAString;
typedef void           * NEAPointer;
typedef unsigned char    NEABoolean;
typedef unsigned char    NEAByte;
typedef short            NEAWord;
typedef unsigned short   NEAUWord;

#ifndef NEANUM_MEMFLAGS
#define NEANUM_MEMFLAGS (MEMF_ANY|MEMF_CLEAR)
#endif

#ifdef AMIGA_SMALL_TYPES
typedef float             NEADecimal;
typedef float           * NEADecimalPtr;
typedef int               NEAInteger;
typedef int             * NEAIntegerPtr;
typedef unsigned int      NEAUInteger;
typedef unsigned int    * NEAUIntegerPtr;
#else
typedef double            NEADecimal;
typedef double          * NEADecimalPtr;
typedef long              NEAInteger;
typedef long            * NEAIntegerPtr;
typedef unsigned long     NEAUInteger;
typedef unsigned long   * NEAUIntegerPtr;
#endif

typedef enum NEANumberType {
  NEANUM_INTEGER = 1,
  NEANUM_UINTEGER = 2,
  NEANUM_INTEGER_PTR = 4,
  NEANUM_UINTEGER_PTR = 8,
  NEANUM_DECIMAL = 16,
  NEANUM_DECIMAL_PTR = 32,
} NEANumberType;

typedef union NEANumber {
  NEAInteger i;
  NEAUInteger ui;
  NEADecimal d;
  NEAIntegerPtr I;
  NEAUIntegerPtr UI;
  NEADecimalPtr D;
} NEANumber;

typedef struct NEAPoint {
  NEANumber x;
  NEANumber y;
  NEANumber z;
  NEANumberType type;
} NEAPoint;

typedef struct NEASize {
  NEANumber width;
  NEANumber height;
  NEANumberType type;
} NEASize;

typedef struct NEARect {
  NEAPoint position;
  NEASize size;
  NEANumberType type;
} NEARect;

/** Some sane zero values for the numbers above */
extern const NEAPoint POINT_ZERO_INT; /* { 0, 0, 0, NEANUM_INTEGER } */
extern const NEAPoint POINT_ZERO_DEC; /* { 0, 0, 0, NEANUM_DECIMAL } */

extern const NEASize SIZE_ZERO_INT;   /* { 0, 0, NEANUM_INTEGER } */
extern const NEASize SIZE_ZERO_DEC;   /* { 0, 0, NEANUM_DECIMAL } */

extern const NEARect RECT_ZERO_INT;   /* { 0, 0, 0, 0, NEANUM_INTEGER } */
extern const NEARect RECT_ZERO_DEC;   /* { 0, 0, 0, 0, NEANUM_DECIMAL} */

/** Some default full screen resolutions on the Amiga */
extern const NEASize NEA_NTSC_HIRES;        /* { 640, 200, NEANUM_INTEGER } */
extern const NEASize NEA_NTSC_HIRES_LACED;  /* { 640, 400, NEANUM_INTEGER } */

/* Point allocation methods that allocate memory that needs to be freed */
NEAPoint *NewPointI(NEAInteger x, NEAInteger y, NEAInteger z);
NEAPoint *NewPointD(NEADecimal x, NEADecimal y, NEADecimal z);
NEAPoint *NewPointIP(NEAInteger *x, NEAInteger *y, NEAInteger *z);
NEAPoint *NewPointDP(NEADecimal *x, NEADecimal *y, NEADecimal *z);

/* Point setters that do not allocate memory and work with an existing copy */
NEAPoint *SetPointI(NEAPoint *p, NEAInteger x, NEAInteger y, NEAInteger z);
NEAPoint *SetPointD(NEAPoint *p, NEADecimal x, NEADecimal y, NEADecimal z);
NEAPoint *SetPointIP(NEAPoint *p, NEAInteger *x, NEAInteger *y, NEAInteger *z);
NEAPoint *SetPointDP(NEAPoint *p, NEADecimal *x, NEADecimal *y, NEADecimal *z);
NEAPoint *SetPointFromPoint(NEAPoint *dest, NEAPoint *source);

/* Size allocation methods that allocate memory that needs to be freed */
NEASize  *NewSizeI(NEAInteger width, NEAInteger height);
NEASize  *NewSizeD(NEADecimal width, NEADecimal height);
NEASize  *NewSizeIP(NEAInteger *width, NEAInteger *height);
NEASize  *NewSizeDP(NEADecimal *width, NEADecimal *height);

/* Size setters that do not allocate memory and work with an existing copy */
NEASize  *SetSizeI(NEASize *size, NEAInteger width, NEAInteger height);
NEASize  *SetSizeD(NEASize *size, NEADecimal width, NEADecimal height);
NEASize  *SetSizeIP(NEASize *size, NEAInteger *width, NEAInteger *height);
NEASize  *SetSizeDP(NEASize *size, NEADecimal *width, NEADecimal *height);
NEASize  *SetSizeFromSize(NEASize *dest, NEASize *source);

/* Rect allocation methods that allocate memory that needs to be freed */
NEARect *NewRectI(
  NEAInteger x,
  NEAInteger y,
  NEAInteger width,
  NEAInteger height
);
NEARect *NewRectD(
  NEADecimal x,
  NEADecimal y,
  NEADecimal width,
  NEADecimal height
);
NEARect *NewRectWithPointSize(NEAPoint *point, NEASize *size);
NEARect *NewRectIWithSize(NEAInteger x, NEAInteger y, NEASize *size);
NEARect *NewRectDWithSize(NEADecimal x, NEADecimal y, NEASize *size);

/* Rect setters that do not allocate memory and work with an existing copy */
NEARect *SetRectI(
  NEARect *rect,
  NEAInteger x,
  NEAInteger y,
  NEAInteger width,
  NEAInteger height
);
NEARect *SetRectD(
  NEARect *rect,
  NEADecimal x,
  NEADecimal y,
  NEADecimal width,
  NEADecimal height
);
NEARect *SetRectWithPointSize(NEARect *rect, NEAPoint *point, NEASize *size);
NEARect *SetRectIWithSize(
  NEARect *rect,
  NEAInteger x,
  NEAInteger y,
  NEASize *size
);
NEARect *SetRectDWithSize(
  NEARect *rect,
  NEADecimal x,
  NEADecimal y,
  NEASize *size
);

#endif
