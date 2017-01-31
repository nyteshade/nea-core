#ifndef NEA_TYPES_H
#define NEA_TYPES_H

#include <exec/memory.h>

typedef char   * NEAString;
typedef void   * NEAPointer;
typedef short    NEABoolean;

#ifndef NEANUM_MEMFLAGS
#define NEANUM_MEMFLAGS (MEMF_ANY|MEMF_CLEAR)
#endif

#ifdef AMIGA_SMALL_TYPES
typedef float    NEADecimal;
typedef float  * NEADecimalPtr;
typedef int      NEAInteger;
typedef int    * NEAIntegerPtr;
#else
typedef double   NEADecimal;
typedef double * NEADecimalPtr;
typedef long     NEAInteger;
typedef long   * NEAIntegerPtr;
#endif

typedef enum NEANumberType {
  NEANUM_INTEGER = 0,
  NEANUM_DECIMAL,
  NEANUM_INTEGER_PTR,
  NEANUM_DECIMAL_PTR
} NEANumberType;

typedef union NEANumber {
  NEAInteger i;
  NEADecimal d;
  NEAIntegerPtr I;
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
  NEANumber type;
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

/* Point setters that do not allocate memory and work with an existing copy */
NEAPoint *SetPointI(NEAPoint *p, NEAInteger x, NEAInteger y, NEAInteger z);
NEAPoint *SetPointD(NEAPoint *p, NEADecimal x, NEADecimal y, NEADecimal z);

/* Size allocation methods that allocate memory that needs to be freed */
NEASize  *NewSizeI(NEAInteger width, NEAInteger height);
NEASize  *NewSizeD(NEADecimal width, NEADecimal height);

/* Size setters that do not allocate memory and work with an existing copy */
NEASize  *SetSizeI(NEASize *size, NEAInteger width, NEAInteger height);
NEASize  *SetSizeD(NEASize *size, NEADecimal width, NEADecimal height);

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
NEARect *NewRectWithPointSize(NEAPoint point, NEASize size);
NEARect *NewRectWithSizeI(NEAInteger x, NEAInteger y, NEASize size);
NEARect *NewRectWithSizeD(NEADecimal x, NEADecimal y, NEASize size);

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
NEARect *SetRectWithPointSize(NEARect *rect, NEAPoint point, NEASize size);
NEARect *SetRectWithSizeI(
  NEARect *rect,
  NEAInteger x,
  NEAInteger y,
  NEASize size
);
NEARect *SetRectWithSizeD(
  NEARect *rect,
  NEADecimal x,
  NEADecimal y,
  NEASize size
);

#endif
