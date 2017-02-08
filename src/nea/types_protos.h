/* Prototypes for functions defined in
/src/nea/types.c
 */

extern NEAPoint const POINT_ZERO_INT;

extern NEAPoint const POINT_ZERO_DEC;

extern NEASize const SIZE_ZERO_INT;

extern NEASize const SIZE_ZERO_DEC;

extern NEARect const RECT_ZERO_INT;

extern NEARect const RECT_ZERO_DEC;

extern NEASize const NEA_NTSC_HIRES;

extern NEASize const NEA_NTSC_HIRES_LACED;

NEAPoint * SetPointI(NEAPoint * , NEAInteger , NEAInteger , NEAInteger );

NEAPoint * SetPointIP(NEAPoint * , NEAInteger * , NEAInteger * , NEAInteger * );

NEAPoint * SetPointD(NEAPoint * , NEADecimal , NEADecimal , NEADecimal );

NEAPoint * SetPointDP(NEAPoint * , NEADecimal * , NEADecimal * , NEADecimal * );

NEAPoint * SetPointFromPoint(NEAPoint * , NEAPoint * );

NEAPoint * NewPointI(NEAInteger , NEAInteger , NEAInteger );

NEAPoint * NewPointD(NEADecimal , NEADecimal , NEADecimal );

NEAPoint * NewPointIP(NEAInteger * , NEAInteger * , NEAInteger * );

NEAPoint * NewPointDP(NEADecimal * , NEADecimal * , NEADecimal * );

NEASize * SetSizeI(NEASize * , NEAInteger , NEAInteger );

NEASize * SetSizeIP(NEASize * , NEAInteger * , NEAInteger * );

NEASize * SetSizeD(NEASize * , NEADecimal , NEADecimal );

NEASize * SetSizeDP(NEASize * , NEADecimal * , NEADecimal * );

NEASize * SetSizeFromSize(NEASize * , NEASize * );

NEASize * NewSizeI(NEAInteger , NEAInteger );

NEASize * NewSizeD(NEADecimal , NEADecimal );

NEASize * NewSizeIP(NEAInteger * , NEAInteger * );

NEASize * NewSizeDP(NEADecimal * , NEADecimal * );

NEARect * NewRectI(NEAInteger , NEAInteger , NEAInteger , NEAInteger );

NEARect * NewRectD(NEADecimal , NEADecimal , NEADecimal , NEADecimal );

NEARect * NewRectWithPointSize(NEAPoint * , NEASize * );

NEARect * NewRectIWithSize(NEAInteger , NEAInteger , NEASize * );

NEARect * NewRectDWithSize(NEADecimal , NEADecimal , NEASize * );

NEARect * SetRectI(NEARect * , NEAInteger , NEAInteger , NEAInteger , NEAInteger );

NEARect * SetRectD(NEARect * , NEADecimal , NEADecimal , NEADecimal , NEADecimal );

NEARect * SetRectWithPointSize(NEARect * , NEAPoint * , NEASize * );

NEARect * SetRectIWithSize(NEARect * , NEAInteger , NEAInteger , NEASize * );

NEARect * SetRectDWithSize(NEARect * , NEADecimal , NEADecimal , NEASize * );

