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

NEAPoint *SetPointI(NEAPoint *point, NEAInteger x, NEAInteger y, NEAInteger z) {
    memset(point, 0, sizeof(NEAPoint));
    point->x.i = x;
    point->y.i = y;
    point->z.i = z;
    point->type = NEANUM_INTEGER;
    return point;
}

NEAPoint *SetPointIP(NEAPoint *point, NEAInteger *x, NEAInteger *y, NEAInteger *z) {
    memset(point, 0, sizeof(NEAPoint));
    point->x.I = x;
    point->y.I = y;
    point->z.I = z;
    point->type = NEANUM_INTEGER_PTR;
    return point;
}

NEAPoint *SetPointD(NEAPoint *point, NEADecimal x, NEADecimal y, NEADecimal z) {
    memset(point, 0, sizeof(NEAPoint));
    point->x.d = x;
    point->y.d = y;
    point->z.d = z;
    point->type = NEANUM_DECIMAL;
    return point;
}

NEAPoint *SetPointDP(NEAPoint *point, NEADecimal *x, NEADecimal *y, NEADecimal *z) {
    memset(point, 0, sizeof(NEAPoint));
    point->x.D = x;
    point->y.D = y;
    point->z.D = z;
    point->type = NEANUM_DECIMAL_PTR;
    return point;
}

NEAPoint *SetPointFromPoint(NEAPoint *dest, NEAPoint *source) {
  memset(size, 0, sizeof(NEAPoint));
  switch(source->type) {
  case NEANUM_INTEGER:
    SetPointI(dest, source.x.i, source.y.i, source.z.i);
    break;
  case NEANUM_DECIMAL:
    SetPointD(dest, source.x.d, source.y.d, source.z.d);
    break;
  case NEANUM_INTEGER_PTR:
    SetPointIP(dest, source.x.I, source.y.I, source.z.I);
    break;
  case NEANUM_DECIMAL_PTR:
    SetPointDP(dest, source.x.D, source.y.D, source.z.D);
    break;
  };
  return dest;
}


NEAPoint *NewPointI(NEAInteger x, NEAInteger y, NEAInteger z) {
    NEAPoint *point = (NEAPoint *)AllocMem(sizeof(NEAPoint), NEANUM_MEMFLAGS);
    SetPointI(point, x, y, z);
    return point;
}

NEAPoint *NewPointD(NEADecimal x, NEADecimal y, NEADecimal z) {
    NEAPoint *point = (NEAPoint *)AllocMem(sizeof(NEAPoint), NEANUM_MEMFLAGS);
    SetPointD(point, x, y, z);
    return point;
}

NEAPoint *NewPointIP(NEAInteger *x, NEAInteger *y, NEAInteger *z) {
    NEAPoint *point = (NEAPoint *)AllocMem(sizeof(NEAPoint), NEANUM_MEMFLAGS);
    SetPointIP(point, x, y, z);
    return point;
}

NEAPoint *NewPointDP(NEADecimal *x, NEADecimal *y, NEADecimal *z) {
    NEAPoint *point = (NEAPoint *)AllocMem(sizeof(NEAPoint), NEANUM_MEMFLAGS);
    SetPointDP(point, x, y, z);
    return point;
}

NEASize *SetSizeI(NEASize *size, NEAInteger width, NEAInteger height) {
    memset(size, 0, sizeof(NEASize));
    size->width.i = width;
    size->height.i = height;
    size->type = NEANUM_INTEGER;
    return size;
}

NEASize *SetSizeIP(NEASize *size, NEAInteger *width, NEAInteger *height) {
    memset(size, 0, sizeof(NEASize));
    size->width.I = width;
    size->height.I = height;
    size->type = NEANUM_INTEGER_PTR;
    return size;
}

NEASize *SetSizeD(NEASize *size, NEADecimal width, NEADecimal height) {
    memset(size, 0, sizeof(NEASize));
    size->width.d = width;
    size->height.d = height;
    size->type = NEANUM_DECIMAL;
    return size;
}

NEASize *SetSizeDP(NEASize *size, NEADecimal *width, NEADecimal *height) {
    memset(size, 0, sizeof(NEASize));
    size->width.D = width;
    size->height.D = height;
    size->type = NEANUM_DECIMAL_PTR;
    return size;
}

NEASize *SetSizeFromSize(NEASize *dest, NEASize *source) {
  memset(size, 0, sizeof(NEASize));
  switch(source->type) {
  case NEANUM_INTEGER:
    SetSizeI(dest, source.width.i, source.height.i);
    break;
  case NEANUM_DECIMAL:
    SetSizeD(dest, source.width.d, source.height.d);
    break;
  case NEANUM_INTEGER_PTR:
    SetSizeIP(dest, source.width.I, source.height.I);
    break;
  case NEANUM_DECIMAL_PTR:
    SetSizeDP(dest, source.width.D, source.height.D);
    break;
  };
  return dest;
}

NEASize *NewSizeI(NEAInteger width, NEAInteger height) {
    NEASize *size = (NEASize *)AllocMem(sizeof(NEASize), NEANUM_MEMFLAGS);
    SetSizeI(size, width, height);
    return size;
}

NEASize *NewSizeD(NEADecimal width, NEADecimal height) {
    NEASize *size = (NEASize *)AllocMem(sizeof(NEASize), NEANUM_MEMFLAGS);
    SetSizeD(size, width, height);
    return size;
}

NEASize *NewSizeIP(NEAInteger *width, NEAInteger *height) {
    NEASize *size = (NEASize *)AllocMem(sizeof(NEASize), NEANUM_MEMFLAGS);
    SetSizeIP(size, width, height);
    return size;
}

NEASize *NewSizeDP(NEADecimal *width, NEADecimal *height) {
    NEASize *size = (NEASize *)AllocMem(sizeof(NEASize), NEANUM_MEMFLAGS);
    SetSizeDP(size, width, height);
    return size;
}

NEARect *NewRectI(
  NEAInteger x,
  NEAInteger y,
  NEAInteger width,
  NEAInteger height
) {
    NEARect *rect = AllocVec(sizeof(NEARect), NEANUM_MEMFLAGS);
    SetPointI(&rect->position, x, y, (NEAInteger)0);
    SetSizeI(&rect->size, width, height);
    return rect;
}

NEARect *NewRectD(
  NEADecimal x,
  NEADecimal y,
  NEADecimal width,
  NEADecimal height
) {
    NEARect *rect = AllocVec(sizeof(NEARect), NEANUM_MEMFLAGS);
    SetPointD(&rect->position, x, y, (NEADecimal)0.);
    SetSizeD(&rect->size, width, height);
    return rect;
}

NEARect *NewRectWithPointSize(NEAPoint *point, NEASize *size) {
    NEARect *rect = (NEARect *)AllocVec(sizeof(NEARect), NEANUM_MEMFLAGS);
    SetPointFromPoint(&rect->position, point);
    SetSizeFromSize(&rect->size, size);
    return rect;
}

NEARect *NewRectIWithSize(NEAInteger x, NEAInteger y, NEASize *size) {
  NEARect *rect = (NEARect *)AllocVec(sizeof(NEARect), NEANUM_MEMFLAGS);
  SetPointI(&rect->position, x, y, (NEAInteger)0);
  SetSizeFromSize(&rect->size, size);
  return rect;
}

NEARect *NewRectDWithSize(NEADecimal x, NEADecimal y, NEASize *size) {
  NEARect *rect = (NEARect *)AllocVec(sizeof(NEARect), NEANUM_MEMFLAGS);
  SetPointD(&rect->position, x, y, (NEADecimal)0);
  SetSizeFromSize(&rect->size, size);
  rect->type = NEANUM_DECIMAL;
  return rect;  
}

NEARect *SetRectI(
  NEARect *rect,
  NEAInteger x,
  NEAInteger y,
  NEAInteger width,
  NEAInteger height
) {
  memset(rect, 0, sizeof(NEARect));
  SetPointI(&rect->position, x, y, (NEAInteger)0);
  SetSizeI(&rect->size, width, height);
  return rect;
}

NEARect *SetRectD(
  NEARect *rect,
  NEADecimal x,
  NEADecimal y,
  NEADecimal width,
  NEADecimal height
) {
  memset(rect, 0, sizeof(NEARect));
  SetPointD(&rect->position, x, y, (NEADecimal)0.);
  SetSizeD(&rect->size, width, height);
  return rect;
}

NEARect *SetRectWithPointSize(NEARect *rect, NEAPoint *point, NEASize *size) {
  memset(rect, 0, sizeof(NEARect));
  SetPointFromPoint(&rect->position, point);
  SetSizeFromSize(&rect->size, size);
  return rect;
}

NEARect *SetRectIWithSize(
  NEARect *rect,
  NEAInteger x,
  NEAInteger y,
  NEASize *size
) {
  memset(rect, 0, sizeof(NEARect));
  SetPointI(&rect->position, x, y, (NEAInteger)0);
  SetSizeFromSize(&rect->size, size);
  return rect;
}

NEARect *SetRectDWithSize(
  NEARect *rect,
  NEADecimal x,
  NEADecimal y,
  NEASize *size
) {
  memset(rect, 0, sizeof(NEARect));
  SetPointD(&rect->position, x, y, (NEADecimal)0.);
  SetSizeFromSize(&rect->size, size);
  return rect;
}

