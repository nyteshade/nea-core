#ifndef NEA_UTIL_LMAP_H
#define NEA_UTIL_LMAP_H

#include <proto/exec.h>
#include <exec/types.h>
#include <exec/lists.h>
#include <exec/nodes.h>
#include <string.h>

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

typedef char * LMString;
typedef void * LMPointer;
typedef short  LMBoolean;

#ifdef AMIGA_SMALL_TYPES
typedef float   LMDecimal;
typedef int     LMInteger;
#else
typedef double  LMDecimal;
typedef long    LMInteger;
#endif

/** Letting the compiler know these will be defined below. */
typedef struct List List;
typedef struct LMap LMap;
typedef struct LMapData LMapData;
typedef struct LMapNode LMapNode;
typedef struct LMapDataNode LMapDataNode;

/** A function typedef for use with iterator methods */
typedef void (*LMapIterFn)(LMapNode *node, LMString key, LMapData *value);

/** A function typedef for use with filtering methods */
typedef LMBoolean (*LMapFilterFn)(LMapData *value, LMString key, LMap *map);

/** Define the LMap as an alias for the struct List type */
struct LMap {
  List list;
  ULONG memRequirements;

  LMPointer (*getPointer)(LMap *map, LMString key);
  LMString (*getString)(LMap *map, LMString key);
  LMDecimal (*getDecimal)(LMap *map, LMString key);
  LMInteger (*getInteger)(LMap *map, LMString key);
  LMBoolean (*getBoolean)(LMap *map, LMString key);
  LMap *(*getMap)(LMap *map, LMString key);
  List *(*getList)(LMap *map, LMString key);

  LMapNode *(*getNamedType)(LMap *map, LMString key, LMapDataType type);
  List *(*getAllValuesForKey)(LMap *map, LMString key);

  LMPointer (*getPointerDefault)(LMap *map, LMString key, LMPointer pointer);
  LMString (*getStringDefault)(LMap *map, LMString key, LMString string);
  LMDecimal (*getDecimalDefault)(LMap *map, LMString key, LMDecimal decimal);
  LMInteger (*getIntegerDefault)(LMap *map, LMString key, LMInteger integer);
  LMBoolean (*getBooleanDefault)(LMap *map, LMString key, LMBoolean boolean);
  LMap *(*getMapDefault)(LMap *map, LMString key, LMap *defaultMap);
  List *(*getListDefault)(LMap *map, LMString key, List *list);

  void (*setPointer)(LMap *map, LMString key, LMPointer pointer);
  void (*setString)(LMap *map, LMString key, LMString string);
  void (*setDecimal)(LMap *map, LMString key, LMDecimal decimal);
  void (*setInteger)(LMap *map, LMString key, LMInteger integer);
  void (*setBoolean)(LMap *map, LMString key, LMBoolean boolean);
  void (*setMap)(LMap *map, LMString key, LMap *lmap);
  void (*setList)(LMap *map, LMString key, List *list);

  void (*forEach)(LMap *map, LMapIterFn fn);
  LMap *(*filter)(LMap *map, LMapFilterFn filterFn);

  size_t (*size)(LMap *map);
  LMBoolean (*hasTypeForKey)(LMap *map, LMString key, LMapDataType type);
};

/** The data portion of the LMap */
struct LMapData {
  /*
   Each LMapData object can accommodate one type of data,
   a type field and size that can be used for any of the
   list, string or pointer fields.
  */
  union {
    LMPointer   pointer;
    LMString    string;
    LMDecimal   decimal;
    LMInteger   integer;
    LMBoolean   boolean;
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

    LMBoolean (*hasNext)(LMapNode *node);
    LMapNode *(*prev)(LMapNode *node);
    LMapNode *(*next)(LMapNode *node);
    LMapDataType (*type)(LMapNode *node);
};

/** Initializers */
LMap *NewLMap(ULONG memReqs);
LMapNode *NewLMapNode(ULONG memReqs, LMapDataType type, LMString key, LMPointer data);
LMapNode *CopyLMapNode(ULONG memReqs, LMapNode *source);

/** Destructors */
void FreeLMap(LMap *map);

/** Getters */
LMPointer   GetLMapPointer(LMap *map, LMString key);
LMString    GetLMapString(LMap *map, LMString key);
LMDecimal   GetLMapDecimal(LMap *map, LMString key);
LMInteger   GetLMapInteger(LMap *map, LMString key);
LMBoolean   GetLMapBoolean(LMap *map, LMString key);
LMap       *GetLMapMap(LMap *map, LMString key);
List       *GetLMapList(LMap *map, LMString key);

LMPointer   GetLMapPointerDefault(LMap *map, LMString key, LMPointer def);
LMString    GetLMapStringDefault(LMap *map, LMString key, LMString def);
LMDecimal   GetLMapDecimalDefault(LMap *map, LMString key, LMDecimal def);
LMInteger   GetLMapIntegerDefault(LMap *map, LMString key, LMInteger def);
LMBoolean   GetLMapBooleanDefault(LMap *map, LMString key, LMBoolean def);
LMap       *GetLMapMapDefault(LMap *map, LMString key, LMap *lmap);
List       *GetLMapListDefault(LMap *map, LMString key, List *list);

/** Setters */
void SetLMapPointer(LMap *map, LMString key, LMPointer pointer);
void SetLMapString(LMap *map, LMString key, LMString string);
void SetLMapDecimal(LMap *map, LMString key, LMDecimal decimal);
void SetLMapInteger(LMap *map, LMString key, LMInteger integer);
void SetLMapBoolean(LMap *map, LMString key, LMBoolean boolean);
void SetLMapMap(LMap *map, LMString key, LMap *lmap);
void SetLMapList(LMap *map, LMString key, List *list);

/** Utility methods */
size_t CountLMapNodes(LMap *map);
void ForEachLMapNode(LMap *map, LMapIterFn fn);
LMap *FilterLMap(LMap *map, LMapFilterFn fn);
LMapNode *FindNamedType(LMap *map, LMString key, LMapDataType type);

LMBoolean HasLMapTypeForKey(LMap *map, LMString key, LMapDataType type);
List *GetAllLMapValuesForKey(LMap *map, LMString key);

#endif
