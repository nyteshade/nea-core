#include "types.h"

const NEAPoint POINT_ZERO_INT = {
  (NEAInteger)0, (NEAInteger)0, (NEAInteger)0, NEANUM_INTEGER
};

const NEAPoint POINT_ZERO_DEC = {
  (NEADecimal)0., (NEADecimal)0., (NEADecimal)0., NEANUM_DECIMAL
};

const NEASize SIZE_ZERO_INT = {
  (NEAInteger)0, (NEAInteger)0, NEANUM_INTEGER
};
const NEASize SIZE_ZERO_DEC = {
  (NEADecimal)0., (NEADecimal)0., NEANUM_DECIMAL
};

const NEARect RECT_ZERO_INT = {
  (NEAInteger)0, (NEAInteger)0, (NEAInteger)0, (NEAInteger)0, NEANUM_INTEGER
};
const NEARect RECT_ZERO_DEC = {
  (NEADecimal)0., (NEADecimal)0., (NEADecimal)0., (NEADecimal)0., NEANUM_DECIMAL
};

/** Some default full screen resolutions on the Amiga */
const NEASize NEA_NTSC_HIRES = {
  (NEAInteger)640, (NEAInteger)200, NEANUM_INTEGER
};
const NEASize NEA_NTSC_HIRES_LACED = {
  (NEAInteger)640, (NEAInteger)400, NEANUM_INTEGER
};

/** TODO define these 
NEAPoint *NewPointI(NEAInteger x, NEAInteger y, NEAInteger z);
NEAPoint *NewPointD(NEADecimal x, NEADecimal y, NEADecimal z);

NEAPoint *SetPointI(NEAPoint *p, NEAInteger x, NEAInteger y, NEAInteger z);
NEAPoint *SetPointD(NEAPoint *p, NEADecimal x, NEADecimal y, NEADecimal z);

NEASize  *NewSizeI(NEAInteger width, NEAInteger height);
NEASize  *NewSizeD(NEADecimal width, NEADecimal height);

NEASize  *SetSizeI(NEASize *size, NEAInteger width, NEAInteger height);
NEASize  *SetSizeD(NEASize *size, NEADecimal width, NEADecimal height);

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
*/
