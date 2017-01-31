#ifndef NEA_UTIL_LMAP_H
#define NEA_UTIL_LMAP_H

#include <proto/exec.h>
#include <exec/types.h>
#include <exec/lists.h>
#include <exec/nodes.h>
#include <string.h>

#include "../types.h"

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

/** Letting the compiler know these will be defined below. */
typedef struct List List;
typedef struct LMap LMap;
typedef struct LMapData LMapData;
typedef struct LMapNode LMapNode;
typedef struct LMapDataNode LMapDataNode;

/** A function typedef for use with iterator methods */
typedef void (*LMapIterFn)(LMapNode *node, NEAString key, LMapData *value);

/** A function typedef for use with filtering methods */
typedef NEABoolean (*LMapFilterFn)(LMapData *value, NEAString key, LMap *map);

/** Define the LMap as an alias for the struct List type */
struct LMap {
  List list;
  ULONG memRequirements;

  LMapNode *(*head)(LMap *map);

  NEAPointer (*getPointer)(LMap *map, NEAString key);
  NEAString (*getString)(LMap *map, NEAString key);
  NEADecimal (*getDecimal)(LMap *map, NEAString key);
  NEAInteger (*getInteger)(LMap *map, NEAString key);
  NEABoolean (*getBoolean)(LMap *map, NEAString key);
  LMap *(*getMap)(LMap *map, NEAString key);
  List *(*getList)(LMap *map, NEAString key);

  LMapNode *(*getNamedType)(LMap *map, NEAString key, LMapDataType type);
  List *(*getAllValuesForKey)(LMap *map, NEAString key);

  NEAPointer (*getPointerDefault)(LMap *map, NEAString key, NEAPointer pointer);
  NEAString (*getStringDefault)(LMap *map, NEAString key, NEAString string);
  NEADecimal (*getDecimalDefault)(LMap *map, NEAString key, NEADecimal decimal);
  NEAInteger (*getIntegerDefault)(LMap *map, NEAString key, NEAInteger integer);
  NEABoolean (*getBooleanDefault)(LMap *map, NEAString key, NEABoolean boolean);
  LMap *(*getMapDefault)(LMap *map, NEAString key, LMap *defaultMap);
  List *(*getListDefault)(LMap *map, NEAString key, List *list);

  void (*setPointer)(LMap *map, NEAString key, NEAPointer pointer);
  void (*setString)(LMap *map, NEAString key, NEAString string);
  void (*setDecimal)(LMap *map, NEAString key, NEADecimal decimal);
  void (*setInteger)(LMap *map, NEAString key, NEAInteger integer);
  void (*setBoolean)(LMap *map, NEAString key, NEABoolean boolean);
  void (*setMap)(LMap *map, NEAString key, LMap *lmap);
  void (*setList)(LMap *map, NEAString key, List *list);

  void (*forEach)(LMap *map, LMapIterFn fn);
  LMap *(*filter)(LMap *map, LMapFilterFn filterFn);

  size_t (*size)(LMap *map);
  NEABoolean (*hasTypeForKey)(LMap *map, NEAString key, LMapDataType type);
};

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

/**
 * Since the LMap is based on the exec List structures,
 * it needs a compatible struct Node data storage in
 * order to function properly within the system limits.
 */
struct LMapNode {
  struct Node node;
  LMapData data;

  NEABoolean (*hasNext)(LMapNode *node);
  LMapNode *(*prev)(LMapNode *node);
  LMapNode *(*next)(LMapNode *node);
  LMapDataType (*type)(LMapNode *node);
};

/** Initializers */
LMap *NewLMap(ULONG memReqs);
LMapNode *NewLMapNode(ULONG memReqs, LMapDataType type, NEAString key, NEAPointer data);
LMapNode *CopyLMapNode(ULONG memReqs, LMapNode *source);

/** Destructors */
void FreeLMap(LMap *map);

/** Getters */
NEAPointer   GetLMapPointer(LMap *map, NEAString key);
NEAString    GetLMapString(LMap *map, NEAString key);
NEADecimal   GetLMapDecimal(LMap *map, NEAString key);
NEAInteger   GetLMapInteger(LMap *map, NEAString key);
NEABoolean   GetLMapBoolean(LMap *map, NEAString key);
LMap       *GetLMapMap(LMap *map, NEAString key);
List       *GetLMapList(LMap *map, NEAString key);

NEAPointer   GetLMapPointerDefault(LMap *map, NEAString key, NEAPointer def);
NEAString    GetLMapStringDefault(LMap *map, NEAString key, NEAString def);
NEADecimal   GetLMapDecimalDefault(LMap *map, NEAString key, NEADecimal def);
NEAInteger   GetLMapIntegerDefault(LMap *map, NEAString key, NEAInteger def);
NEABoolean   GetLMapBooleanDefault(LMap *map, NEAString key, NEABoolean def);
LMap       *GetLMapMapDefault(LMap *map, NEAString key, LMap *lmap);
List       *GetLMapListDefault(LMap *map, NEAString key, List *list);

/** Setters */
void SetLMapPointer(LMap *map, NEAString key, NEAPointer pointer);
void SetLMapString(LMap *map, NEAString key, NEAString string);
void SetLMapDecimal(LMap *map, NEAString key, NEADecimal decimal);
void SetLMapInteger(LMap *map, NEAString key, NEAInteger integer);
void SetLMapBoolean(LMap *map, NEAString key, NEABoolean boolean);
void SetLMapMap(LMap *map, NEAString key, LMap *lmap);
void SetLMapList(LMap *map, NEAString key, List *list);

/** Utility methods */
size_t CountLMapNodes(LMap *map);
void ForEachLMapNode(LMap *map, LMapIterFn fn);
LMap *FilterLMap(LMap *map, LMapFilterFn fn);
LMapNode *FindNamedType(LMap *map, NEAString key, LMapDataType type);

NEABoolean HasLMapTypeForKey(LMap *map, NEAString key, LMapDataType type);
List *GetAllLMapValuesForKey(LMap *map, NEAString key);

#endif
