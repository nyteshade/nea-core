#include "lmap.h"

#include <exec/memory.h>

/** Initializers */
LMap *NewLMap(ULONG memReqs) {
	LMap *map;

	map = (LMap *)AllocVec(sizeof(LMap), memReqs);
	NewList(&map->list);

	return map;
}

LMapNode *NewLMapNode(ULONG memReqs, LMapDataType type, STRPTR key, APTR data) {
	LMapNode *node;

	node = (LMapNode *)AllocVec(sizeof(LMapNode), memReqs | MEMF_CLEAR);
	node->data.type = type;
	node->node.ln_Type = NT_USER;
	node->node.ln_Pri = 0;
	node->node.ln_Name = key;

	switch(type) {
		case LMAP_POINTER:
			node->data.u.pointer = data; break;
		case LMAP_STRING:
			node->data.u.string = (STRPTR)data; break;
		case LMAP_DECIMAL:
			node->data.u.decimal = *(double *)data; break;
		case LMAP_INTEGER:
			node->data.u.integer = *(long *)data; break;
		case LMAP_BOOLEAN:
			node->data.u.boolean = *(BOOL *)data ? TRUE : FALSE; break;
		case LMAP_LMAP:
			node->data.u.map = (LMap *)data; break;
		case LMAP_LIST:
			node->data.u.list = (List *)data; break;
		default:
			break;
	}

	return node; 
}

LMapNode *CopyLMapNode(ULONG memReqs, LMapNode *source) {
	LMapNode *dest;

	dest = (LMapNode *)AllocVec(sizeof(LMapNode), memReqs | MEMF_CLEAR);
	CopyMemQuick((APTR)&source->data, (APTR)&dest->data, sizeof(LMapData));

	return dest;
}

/** Destructors */
void FreeLMap(LMap *map) {
	LMapNode *rootNode;
  LMapNode *nextNode;

  rootNode = (LMapNode *)(map->list.lh_Head); /* First node */
  while (nextNode = (LMapNode *)(rootNode->node.ln_Succ)) {
    FreeVec(rootNode);
    rootNode = nextNode;
  }
	
	if (map) {
		FreeVec(map);
	}
}

LMapNode *FindNamedType(LMap *map, STRPTR key, LMapDataType type) {
	struct LMapNode *node;

	node = (LMapNode *)FindName((struct List *)map, key);
	if (node) {
		while (node) {
			if (node->data.type == type) {
				return node;
			}

			node = (LMapNode *)FindName((struct List *)node, key);
		}
	}

	return NULL;
}

/** Getters */
APTR GetLMapPointer(LMap *map, STRPTR key, APTR def) {
	LMapNode *node = FindNamedType(map, key, LMAP_POINTER);
	return node ? node : def;
}

STRPTR GetLMapString(LMap *map, STRPTR key, STRPTR def) {
	LMapNode *node = FindNamedType(map, key, LMAP_STRING);
	return node ? node->data.u.string : def;
}

double GetLMapDecimal(LMap *map, STRPTR key, double def) {
	LMapNode *node = FindNamedType(map, key, LMAP_DECIMAL);
	return node ? node->data.u.decimal : def;
}

long GetLMapInteger(LMap *map, STRPTR key, long def) {
	LMapNode *node = FindNamedType(map, key, LMAP_INTEGER);
	return node ? node->data.u.integer : def;
}

BOOL GetLMapBoolean(LMap *map, STRPTR key, BOOL def) {
	LMapNode *node = FindNamedType(map, key, LMAP_BOOLEAN);
	return (BOOL)(node ? node->data.u.boolean : def);
}
	
LMap *GetLMapMap(LMap *map, STRPTR key, LMap *def) {
	LMapNode *node = FindNamedType(map, key, LMAP_LMAP);
	return node ? node->data.u.map : def;
}
	
List *GetLMapList(LMap *map, STRPTR key, List *def) {
	LMapNode *node = FindNamedType(map, key, LMAP_LIST);
	return node ? node->data.u.list : def;
}

/** Setters */
void SetLMapPointer(LMap *map, STRPTR key, APTR pointer) {
	LMapNode *node = FindNamedType(map, key, LMAP_POINTER);
	if (node) {
		node->data.u.pointer = pointer;
	}
	else {
		node = NewLMapNode(MEMF_ANY, LMAP_POINTER, key, pointer);
		AddTail((struct List *)map, (struct Node *)node);
	}
}

void SetLMapString(LMap *map, STRPTR key, STRPTR string) {
	LMapNode *node = FindNamedType(map, key, LMAP_STRING);
	if (node) {
		node->data.u.string = string;
	}
	else {
		node = NewLMapNode(MEMF_ANY, LMAP_STRING, key, (APTR)string);
		AddTail((struct List *)map, (struct Node *)node);
	}
}

void SetLMapDecimal(LMap *map, STRPTR key, double decimal) {
	LMapNode *node = FindNamedType(map, key, LMAP_DECIMAL);
	if (node) {
		node->data.u.decimal = decimal;
	}
	else {
		node = NewLMapNode(MEMF_ANY, LMAP_DECIMAL, key, (APTR)&decimal);
		AddTail((struct List *)map, (struct Node *)node);
	}
}

void SetLMapInteger(LMap *map, STRPTR key, long integer) {
	LMapNode *node = FindNamedType(map, key, LMAP_INTEGER);
	if (node) {
		node->data.u.integer = integer;
	}
	else {
		node = NewLMapNode(MEMF_ANY, LMAP_INTEGER, key, (APTR)&integer);
		AddTail((struct List *)map, (struct Node *)node);
	}
}

void SetLMapBoolean(LMap *map, STRPTR key, BOOL boolean) {
	LMapNode *node = FindNamedType(map, key, LMAP_BOOLEAN);
	if (node) {
		node->data.u.boolean = boolean;
	}
	else {
		node = NewLMapNode(MEMF_ANY, LMAP_POINTER, key, (APTR)&boolean);
		AddTail((struct List *)map, (struct Node *)node);
	}
}

void SetLMapMap(LMap *map, STRPTR key, LMap *lmap) {
	LMapNode *node = FindNamedType(map, key, LMAP_LMAP);
	if (node) {
		node->data.u.map = lmap;
	}
	else {
		node = NewLMapNode(MEMF_ANY, LMAP_LMAP, key, (APTR)lmap);
		AddTail((struct List *)map, (struct Node *)node);
	}
}

void SetLMapList(LMap *map, STRPTR key, List *list) {
	LMapNode *node = FindNamedType(map, key, LMAP_LIST);
	if (node) {
		node->data.u.list = list;
	}
	else {
		node = NewLMapNode(MEMF_ANY, LMAP_LIST, key, (APTR)list);
		AddTail((struct List *)map, (struct Node *)node);
	}
}

/** Utility methods */
size_t CountLMapNodes(LMap *map) {
	size_t size;
	struct List *list;
	struct Node *node;
	
	size = 0;
	list = (struct List *)map;

	for (node = list->lh_Head; node; node = node->ln_Succ) {
		size++;
	}

	return size;
}

void ForEachLMapNode(LMap *map, LMapIterFn fn) {
	LMapNode *node;
	
	for (
		node = (LMapNode *)map->list.lh_Head;
		node && strlen(node->node.ln_Name);
		node = (LMapNode *)node->node.ln_Succ
	) {
		(*fn)(node, node->node.ln_Name, &node->data);
	}
}

LMap *FilterLMap(LMap *map, LMapFilterFn fn) {
	LMap *newMap;
	LMapNode *newNode;
	LMapNode *node;
	BOOL include;

	newMap = NewLMap(map->memRequirements);
	
	for (
 	  node = (LMapNode *)map->list.lh_Head, include = FALSE;
		node && strlen(node->node.ln_Name);
		node = (LMapNode *)node->node.ln_Succ, include = FALSE
	) {
		include = (*fn)(&node->data, node->node.ln_Name, map);
		if (include) {
			newNode = CopyLMapNode(map->memRequirements, node);
			AddTail((struct List *)newMap, (struct Node *)node);
		}
	}

	return newMap;
}

BOOL HasLMapTypeForKey(LMap *map, STRPTR key, LMapDataType type) {
	struct LMapNode *node;

	node = (LMapNode *)FindName((struct List *)map, key);
	if (node) {
		while (node) {
			if (node->data.type == type) {
				return TRUE;
			}

			node = (LMapNode *)FindName((struct List *)node, key);
		}
	}

	return FALSE;
	
}

List *GetAllLMapValuesForKey(LMap *map, STRPTR key) {
	struct List *list;
	LMapNode *newNode;
	LMapNode *node;

	list = (struct List *)AllocVec(sizeof(struct List), MEMF_ANY);
	NewList(list);

	node = (LMapNode *)FindName((struct List *)map, key);
	if (node) {
		while (node) {
			newNode = CopyLMapNode(MEMF_ANY, node);
			AddTail(list, (struct Node *)newNode);
			
			node = (LMapNode *)FindName((struct List *)node, key);
		}
	}

	return list;
}




