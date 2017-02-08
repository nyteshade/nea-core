/* Prototypes for functions defined in
/src/nea/util/lmap.c
 */

NEABoolean LMapNodeHasNext(LMapNode * );

LMapNode * LMapNodeNextNode(LMapNode * );

LMapNode * LMapNodePrevNode(LMapNode * );

LMapDataType LMapNodeType(LMapNode * );

LMapNode * LMapHeadNode(LMap * );

LMap * NewLMap(ULONG );

LMapNode * NewLMapNode(ULONG , LMapDataType , NEAString , NEAPointer );

LMapNode * CopyLMapNode(ULONG , LMapNode * );

void FreeLMap(LMap * );

LMapNode * FindNamedType(LMap * , NEAString , LMapDataType );

NEAPointer GetLMapPointer(LMap * , NEAString );

NEAString GetLMapString(LMap * , NEAString );

NEADecimal GetLMapDecimal(LMap * , NEAString );

NEAInteger GetLMapInteger(LMap * , NEAString );

NEABoolean GetLMapBoolean(LMap * , NEAString );

LMap * GetLMapMap(LMap * , NEAString );

List * GetLMapList(LMap * , NEAString );

NEAPointer GetLMapPointerDefault(LMap * , NEAString , NEAPointer );

NEAString GetLMapStringDefault(LMap * , NEAString , NEAString );

NEADecimal GetLMapDecimalDefault(LMap * , NEAString , NEADecimal );

NEAInteger GetLMapIntegerDefault(LMap * , NEAString , NEAInteger );

NEABoolean GetLMapBooleanDefault(LMap * , NEAString , NEABoolean );

LMap * GetLMapMapDefault(LMap * , NEAString , LMap * );

List * GetLMapListDefault(LMap * , NEAString , List * );

void SetLMapPointer(LMap * , NEAString , NEAPointer );

void SetLMapString(LMap * , NEAString , NEAString );

void SetLMapDecimal(LMap * , NEAString , NEADecimal );

void SetLMapInteger(LMap * , NEAString , NEAInteger );

void SetLMapBoolean(LMap * , NEAString , NEABoolean );

void SetLMapMap(LMap * , NEAString , LMap * );

void SetLMapList(LMap * , NEAString , List * );

size_t CountLMapNodes(LMap * );

void ForEachLMapNode(LMap * , LMapIterFn );

LMap * FilterLMap(LMap * , LMapFilterFn );

NEABoolean HasLMapTypeForKey(LMap * , NEAString , LMapDataType );

List * GetAllLMapValuesForKey(LMap * , NEAString );

