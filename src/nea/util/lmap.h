#ifndef NEA_UTIL_LMAP_H
#define NEA_UTIL_LMAP_H

#include <proto/exec.h>
#include <exec/types.h>
#include <exec/lists.h>
#include <exec/nodes.h>
#include <string.h>

#include <nea/types.h>

/* --------------------------------------------------------------------------*/

typedef enum LMapDataType {
    LMAP_NONE = 0,
    LMAP_POINTER,
    LMAP_STRING,
    LMAP_DECIMAL,
    LMAP_INTEGER,
    LMAP_BOOLEAN,
    LMAP_LMAP,
    LMAP_LIST
} LMapDataType;

/* -- Letting the compiler know these will be defined below -----------------*/

typedef struct List List;
typedef struct LMap LMap;
typedef struct LMapData LMapData;
typedef struct LMapNode LMapNode;
typedef struct LMapDataNode LMapDataNode;

/* -- Function Pointers -----------------------------------------------------*/

/** A function typedef for use with iterator methods */
typedef void (* __asm LMapIterFn)(
  register __a0 LMapNode *node,
  register __a1 NEAString key,
  register __a2 LMapData *value
);

/** A function typedef for use with filtering methods */
typedef NEABoolean (* __asm LMapFilterFn)(
  register __a0 LMapData *value,
  register __a1 NEAString key,
  register __a2 LMap *map
);

/* -- Structures ------------------------------------------------------------*/

/** Define the LMap as an alias for the struct List type */
struct LMap {
  List list;
  ULONG memRequirements;

  LMapNode *(* __asm head)(register __a0 LMap *map);

  NEAPointer (* __asm getPointer)(
    register __a0 LMap *map,
    register __a1 NEAString key
  );
  NEAString (* __asm getString)(
    register __a0 LMap *map,
    register __a1 NEAString key
  );
  NEADecimal (* __asm getDecimal)(
    register __a0 LMap *map,
    register __a1 NEAString key
  );
  NEAInteger (* __asm getInteger)(
    register __a0 LMap *map,
    register __a1 NEAString key
  );
  NEABoolean (* __asm getBoolean)(
    register __a0 LMap *map,
    register __a1 NEAString key
  );
  LMap *(* __asm getMap)(
    register __a0 LMap *map,
    register __a1 NEAString key
  );
  List *(* __asm getList)(
    register __a0 LMap *map,
    register __a1 NEAString key
  );

  LMapNode *(* __asm getNamedType)(
    register __a0 LMap *map,
    register __a1 NEAString key,
    register __d0 LMapDataType type
  );
  List *(* __asm getAllValuesForKey)(
    register __a0 LMap *map,
    register __a1 NEAString key
  );

  NEAPointer (* __asm getPointerDefault)(
    register __a0 LMap *map,
    register __a1 NEAString key,
    register __a2 NEAPointer pointer
  );
  NEAString (* __asm getStringDefault)(
    register __a0 LMap *map,
    register __a1 NEAString key,
    register __a2 NEAString string
  );
  NEADecimal (* __asm getDecimalDefault)(
    register __a0 LMap *map,
    register __a1 NEAString key,
    register __d0 NEADecimal decimal
  );
  NEAInteger (* __asm getIntegerDefault)(
    register __a0 LMap *map,
    register __a1 NEAString key,
    register __d0 NEAInteger integer
  );
  NEABoolean (* __asm getBooleanDefault)(
    register __a0 LMap *map,
    register __a1 NEAString key,
    register __d0 NEABoolean boolean
  );
  LMap *(* __asm getMapDefault)(
    register __a0 LMap *map,
    register __a1 NEAString key,
    register __a2 LMap *defaultMap
  );
  List *(* __asm getListDefault)(
    register __a0 LMap *map,
    register __a1 NEAString key,
    register __a2 List *list
  );

  void (* __asm setPointer)(
    register __a0 LMap *map,
    register __a1 NEAString key,
    register __a2 NEAPointer pointer
  );
  void (* __asm setString)(
    register __a0 LMap *map,
    register __a1 NEAString key,
    register __a2 NEAString string
  );
  void (* __asm setDecimal)(
    register __a0 LMap *map,
    register __a1 NEAString key,
    register __d0 NEADecimal decimal
  );
  void (* __asm setInteger)(
    register __a0 LMap *map,
    register __a1 NEAString key,
    register __d0 NEAInteger integer
  );
  void (* __asm setBoolean)(
    register __a0 LMap *map,
    register __a1 NEAString key,
    register __d0 NEABoolean boolean
  );
  void (* __asm setMap)(
    register __a0 LMap *map,
    register __a1 NEAString key,
    register __a2 LMap *lmap
  );
  void (* __asm setList)(
    register __a0 LMap *map,
    register __a1 NEAString key,
    register __a2 List *list
  );

  void (* __asm forEach)(
    register __a0 LMap *map,
    register __a1 LMapIterFn fn
  );
  LMap *(* __asm filter)(
    register __a0 LMap *map,
    register __a1 LMapFilterFn filterFn
  );

  size_t (* __asm size)(register __a0 LMap *map);

  NEABoolean (* __asm hasTypeForKey)(
    register __a0 LMap *map,
    register __a1 NEAString key,
    register __d0 LMapDataType type
  );
};

/* --------------------------------------------------------------------------*/

/** The data portion of the LMap */
struct LMapData {
  /*
   Each LMapData object can accommodate one type of data,
   a type field and size that can be used for any of the
   list, string or pointer fields.
  */
  union {
    NEAPointer   pointer;
    NEAString    string;
    NEADecimal   decimal;
    NEAInteger   integer;
    NEABoolean   boolean;
    LMap       *map;
    List       *list;
  } u;
  LMapDataType type;
};

/* --------------------------------------------------------------------------*/

/**
 * Since the LMap is based on the exec List structures,
 * it needs a compatible struct Node data storage in
 * order to function properly within the system limits.
 */
struct LMapNode {
  struct Node node;
  LMapData data;

  NEABoolean (* __asm hasNext)(register __a0 LMapNode *node);
  LMapNode *(* __asm prev)(register __a0 LMapNode *node);
  LMapNode *(* __asm next)(register __a0 LMapNode *node);
  LMapDataType (* __asm type)(register __a0 LMapNode *node);
};

/* -- Initializers ----------------------------------------------------------*/

LMap * __asm __saveds NewLMap(register __d0 ULONG memReqs);

/* --------------------------------------------------------------------------*/

LMapNode * __asm __saveds NewLMapNode(
  register __d0 ULONG memReqs,
  register __d1 LMapDataType type,
  register __a0 NEAString key,
  register __a1 NEAPointer data
);

/* --------------------------------------------------------------------------*/

LMapNode * __asm __saveds CopyLMapNode(
  register __d0 ULONG memReqs,
  register __a0 LMapNode *source
);

/* -- Destructors -----------------------------------------------------------*/

void  __asm __saveds FreeLMap(register __a0 LMap *map);

/* -- Getters ---------------------------------------------------------------*/

NEAPointer __asm __saveds GetLMapPointer(
  register __a0 LMap *map,
  register __a1 NEAString key
);

/* --------------------------------------------------------------------------*/

NEAString __asm __saveds GetLMapString(
  register __a0 LMap *map,
  register __a1 NEAString key
);

/* --------------------------------------------------------------------------*/

NEADecimal __asm __saveds GetLMapDecimal(
  register __a0 LMap *map,
  register __a1 NEAString key
);

/* --------------------------------------------------------------------------*/

NEAInteger __asm __saveds GetLMapInteger(
  register __a0 LMap *map,
  register __a1 NEAString key
);

/* --------------------------------------------------------------------------*/

NEABoolean __asm __saveds GetLMapBoolean(
  register __a0 LMap *map,
  register __a1 NEAString key
);

/* --------------------------------------------------------------------------*/

LMap * __asm __saveds GetLMapMap(
  register __a0 LMap *map,
  register __a1 NEAString key
);

/* --------------------------------------------------------------------------*/

List * __asm __saveds GetLMapList(
  register __a0 LMap *map,
  register __a1 NEAString key
);

/* --------------------------------------------------------------------------*/

NEAPointer __asm __saveds GetLMapPointerDefault(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __a2 NEAPointer def
);

/* --------------------------------------------------------------------------*/

NEAString  __asm __saveds GetLMapStringDefault(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __a2 NEAString def
);

/* --------------------------------------------------------------------------*/

NEADecimal  __asm __saveds GetLMapDecimalDefault(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __d0 NEADecimal def
);

/* --------------------------------------------------------------------------*/

NEAInteger  __asm __saveds GetLMapIntegerDefault(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __d0 NEAInteger def
);

/* --------------------------------------------------------------------------*/

NEABoolean  __asm __saveds GetLMapBooleanDefault(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __d0 NEABoolean def
);

/* --------------------------------------------------------------------------*/

LMap * __asm __saveds GetLMapMapDefault(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __a2 LMap *lmap
);

/* --------------------------------------------------------------------------*/

List * __asm __saveds GetLMapListDefault(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __a2 List *list
);

/* -- Setters ---------------------------------------------------------------*/

void __asm __saveds SetLMapPointer(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __a2 NEAPointer pointer
);

/* --------------------------------------------------------------------------*/

void __asm __saveds SetLMapString(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __a2 NEAString string
);

/* --------------------------------------------------------------------------*/

void __asm __saveds SetLMapDecimal(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __d0 NEADecimal decimal
);

/* --------------------------------------------------------------------------*/

void __asm __saveds SetLMapInteger(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __d0 NEAInteger integer
);

/* --------------------------------------------------------------------------*/

void __asm __saveds SetLMapBoolean(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __d0 NEABoolean boolean
);

/* --------------------------------------------------------------------------*/

void __asm __saveds SetLMapMap(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __a2 LMap *lmap
);

/* --------------------------------------------------------------------------*/

void __asm __saveds SetLMapList(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __a2 List *list
);

/* -- Utility methods -------------------------------------------------------*/

size_t __asm __saveds CountLMapNodes(register __a0 LMap *map);

/* --------------------------------------------------------------------------*/

NEABoolean __asm __saveds LMapNodeHasNext(register __a0 LMapNode *node);

/* --------------------------------------------------------------------------*/

LMapNode * __asm __saveds LMapNodeNextNode(register __a0 LMapNode *node);

/* --------------------------------------------------------------------------*/

LMapNode * __asm __saveds LMapNodePrevNode(register __a0 LMapNode *node);

/* --------------------------------------------------------------------------*/

LMapDataType __asm __saveds LMapNodeType(register __a0 LMapNode *node);

/* --------------------------------------------------------------------------*/

LMapNode * __asm __saveds LMapHeadNode(register __a0 LMap *map);

/* --------------------------------------------------------------------------*/

void __asm __saveds ForEachLMapNode(
  register __a0 LMap *map,
  register __a1 LMapIterFn fn
);

/* --------------------------------------------------------------------------*/

LMap * __asm __saveds FilterLMap(
  register __a0 LMap *map,
  register __a1 LMapFilterFn fn
);

/* --------------------------------------------------------------------------*/

LMapNode * __asm __saveds FindNamedType(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __d0 LMapDataType type
);

/* --------------------------------------------------------------------------*/

NEABoolean __asm __saveds HasLMapTypeForKey(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __d0 LMapDataType type
);

/* --------------------------------------------------------------------------*/

List * __asm __saveds GetAllLMapValuesForKey(
  register __a0 LMap *map,
  register __a1 NEAString key
);

/* --------------------------------------------------------------------------*/

NEAString __asm __saveds LMapToString(
  register __a0 NEAString buffer,
  register __a1 LMap *map,
  register __d0 NEABoolean verbose
);

/* --------------------------------------------------------------------------*/

NEAString __asm __saveds LMapNodeToString(
  register __a0 NEAString buffer,
  register __a1 LMapNode *node,
  register __d0 NEABoolean verbose
);

/* --------------------------------------------------------------------------*/

#endif
