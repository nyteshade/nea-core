#ifndef NEA_UTIL_LMAP_H
#define NEA_UTIL_LMAP_H

#include <proto/exec.h>
#include <exec/types.h>
#include <exec/lists.h>
#include <exec/nodes.h>
#include <string.h>

#define LMAP_NONE       0
#define LMAP_POINTER	1
#define LMAP_STRING	2
#define LMAP_DECIMAL	3
#define LMAP_INTEGER	4
#define LMAP_BOOLEAN	5
#define LMAP_LMAP	6
#define LMAP_LIST	7

/** Define the LMap as an alias for the struct List type */
typedef struct LMap {
	struct List list;
	ULONG memRequirements;
} LMap;

/** Define a typedef for struct List to make it easier */
typedef struct List List;

/** A short integer designed to represent a LMap data type */
typedef short LMapDataType;

/** The data portion of the LMap */
typedef struct LMapData {
	/*
	 Each LMapData object can accommodate one type of data,
	 a type field and size that can be used for any of the
	 list, string or pointer fields.
	*/
	union {
		APTR		pointer;
		STRPTR  string;
		double	decimal;
		long		integer;
		BOOL		boolean;
		LMap		*map;
		List	  *list;		
	} u;
	LMapDataType type;
} LMapData;

/** 
 * Since the LMap is based on the exec List structures,
 * it needs a compatible struct Node data storage in
 * order to function properly within the system limits.
 */
typedef struct LMapNode {
	struct Node node;
	LMapData data;
} LMapNode;

/** A function typedef for use with iterator methods */
typedef void (*LMapIterFn)(LMapNode *node, STRPTR key, LMapData *value);

/** A function typedef for use with filtering methods */
typedef BOOL (*LMapFilterFn)(LMapData *value, STRPTR key, LMap *map);

/** Initializers */
LMap *NewLMap(ULONG memReqs);
LMapNode *NewLMapNode(ULONG memReqs, LMapDataType type, STRPTR key, APTR data);
LMapNode *CopyLMapNode(ULONG memReqs, LMapNode *source);

/** Destructors */
void FreeLMap(LMap *map);

/** Getters */
APTR		GetLMapPointer(LMap *map, STRPTR key, APTR def);
STRPTR  GetLMapString(LMap *map, STRPTR key, STRPTR def);
double  GetLMapDecimal(LMap *map, STRPTR key, double def);
long		GetLMapInteger(LMap *map, STRPTR key, long def);
BOOL		GetLMapBoolean(LMap *map, STRPTR key, BOOL def);
LMap   *GetLMapMap(LMap *map, STRPTR key, LMap *lmap);
List   *GetLMapList(LMap *map, STRPTR key, List *list);

/** Setters */
void SetLMapPointer(LMap *map, STRPTR key, APTR pointer);
void SetLMapString(LMap *map, STRPTR key, STRPTR string);
void SetLMapDecimal(LMap *map, STRPTR key, double decimal);
void SetLMapInteger(LMap *map, STRPTR key, long integer);
void SetLMapBoolean(LMap *map, STRPTR key, BOOL boolean);
void SetLMapMap(LMap *map, STRPTR key, LMap *lmap);
void SetLMapList(LMap *map, STRPTR key, List *list);

/** Utility methods */
size_t CountLMapNodes(LMap *map);
void ForEachLMapNode(LMap *map, LMapIterFn fn);
LMap *FilterLMap(LMap *map, LMapFilterFn fn);
LMapNode *FindNamedType(LMap *map, STRPTR key, LMapDataType type);

BOOL HasLMapTypeForKey(LMap *map, STRPTR key, LMapDataType type);
List *GetAllLMapValuesForKey(LMap *map, STRPTR key);

#endif
