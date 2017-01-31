#include "lmap.h"

#include <exec/memory.h>

LMBoolean LMapNodeHasNext(LMapNode *node) {
    return node->node.ln_Succ != NULL;
}

LMapNode *LMapNodeNextNode(LMapNode *node) {
    return node->node.ln_Succ;
}

LMapNode *LMapNodePrevNode(LMapNode *node) {
    return node->node.ln_Pred;
}

LMapDataType LMapNodeType(LMapNode *node) {
    return node->data.type;
}

/** Initializers */
LMap *NewLMap(ULONG memReqs) {
	LMap *map;

	map = (LMap *)AllocVec(sizeof(LMap), memReqs);
	NewList(&map->list);
    
    map->getPointer = GetLMapPointer;
    map->getString = GetLMapString;
    map->getDecimal = GetLMapDecimal;
    map->getInteger = GetLMapInteger;
    map->getBoolean = GetLMapBoolean;
    map->getMap = GetLMapMap;
    map->getList = GetLMapList;
    
    map->getNamedType = FindNamedType;
    map->getAllValuesForKey = GetAllLMapValuesForKey;

    map->getPointerDefault = GetLMapPointerDefault;
    map->getStringDefault = GetLMapStringDefault;
    map->getDecimalDefault = GetLMapDecimalDefault;
    map->getIntegerDefault = GetLMapIntegerDefault;
    map->getBooleanDefault = GetLMapBooleanDefault;
    map->getMapDefault = GetLMapMapDefault;
    map->getListDefault = GetLMapListDefault;
    
    map->setPointer = SetLMapPointer;
    map->setString = SetLMapString;
    map->setDecimal = SetLMapDecimal;
    map->setInteger = SetLMapInteger;
    map->setBoolean = SetLMapBoolean;
    map->setMap = SetLMapMap;
    map->setList = SetLMapList;

    map->forEach = ForEachLMapNode;
    map->filter = FilterLMap;
    
    map->size = CountLMapNodes;
    map->hasTypeForKey = HasLMapTypeForKey;
    
	return map;
}

LMapNode *NewLMapNode(ULONG memReqs, LMapDataType type, LMString key, LMPointer data) {
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
			node->data.u.string = (LMString)data; break;
		case LMAP_DECIMAL:
			node->data.u.decimal = *(LMDecimal *)data; break;
		case LMAP_INTEGER:
			node->data.u.integer = *(LMInteger *)data; break;
		case LMAP_LMBooleanEAN:
			node->data.u.boolean = *(LMBoolean *)data ? TRUE : FALSE; break;
		case LMAP_LMAP:
			node->data.u.map = (LMap *)data; break;
		case LMAP_LIST:
			node->data.u.list = (List *)data; break;
		default:
			break;
	}
    
    node->hasNext = LMapNodeHasNext;
    node->next = LMapNodeNextNode;
    node->prev = LMapNodePrevNode;
    node->type = LMapNodeType;

	return node; 
}

LMapNode *CopyLMapNode(ULONG memReqs, LMapNode *source) {
	LMapNode *dest;

	dest = (LMapNode *)AllocVec(sizeof(LMapNode), memReqs | MEMF_CLEAR);
	CopyMemQuick((LMPointer)&source->data, (LMPointer)&dest->data, sizeof(LMapData));

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

LMapNode *FindNamedType(LMap *map, LMString key, LMapDataType type) {
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
LMPointer GetLMapPointer(LMap *map, LMString key) {
	return GetLMapPointerDefault(map, key, NULL);
}

LMString GetLMapString(LMap *map, LMString key) {
	return GetLMapStringDefault(map, key, NULL);
}

LMDecimal GetLMapDecimal(LMap *map, LMString key) {
	return GetLMapDecimalDefault(map, key, -1.);
}

LMInteger GetLMapInteger(LMap *map, LMString key) {
	return GetLMapIntegerDefault(map, key, -1);
}

LMBoolean GetLMapBoolean(LMap *map, LMString key) {
	return GetLMapBooleanDefault(map, key, -1);
}
	
LMap *GetLMapMap(LMap *map, LMString key) {
	return GetLMapMapDefault(map, key, NULL);
}
	
List *GetLMapList(LMap *map, LMString key) {
	return GetLMapListDefault(map, key, NULL);
}

LMPointer GetLMapPointerDefault(LMap *map, LMString key, LMPointer def) {
	LMapNode *node = FindNamedType(map, key, LMAP_POINTER);
	return node ? node : def;
}

LMString GetLMapStringDefault(LMap *map, LMString key, LMString def) {
	LMapNode *node = FindNamedType(map, key, LMAP_STRING);
	return node ? node->data.u.string : def;
}

LMDecimal GetLMapDecimalDefault(LMap *map, LMString key, LMDecimal def) {
	LMapNode *node = FindNamedType(map, key, LMAP_DECIMAL);
	return node ? node->data.u.decimal : def;
}

LMInteger GetLMapIntegerDefault(LMap *map, LMString key, LMInteger def) {
	LMapNode *node = FindNamedType(map, key, LMAP_INTEGER);
	return node ? node->data.u.integer : def;
}

LMBoolean GetLMapBooleanDefault(LMap *map, LMString key, LMBoolean def) {
	LMapNode *node = FindNamedType(map, key, LMAP_LMBooleanEAN);
	return (LMBoolean)(node ? node->data.u.boolean : def);
}
	
LMap *GetLMapMapDefault(LMap *map, LMString key, LMap *def) {
	LMapNode *node = FindNamedType(map, key, LMAP_LMAP);
	return node ? node->data.u.map : def;
}
	
List *GetLMapListDefault(LMap *map, LMString key, List *def) {
	LMapNode *node = FindNamedType(map, key, LMAP_LIST);
	return node ? node->data.u.list : def;
}

/** Setters */
void SetLMapPointer(LMap *map, LMString key, LMPointer pointer) {
	LMapNode *node = FindNamedType(map, key, LMAP_POINTER);
	if (node) {
		node->data.u.pointer = pointer;
	}
	else {
		node = NewLMapNode(MEMF_ANY, LMAP_POINTER, key, pointer);
		AddTail((struct List *)map, (struct Node *)node);
	}
}

void SetLMapString(LMap *map, LMString key, LMString string) {
	LMapNode *node = FindNamedType(map, key, LMAP_STRING);
	if (node) {
		node->data.u.string = string;
	}
	else {
		node = NewLMapNode(MEMF_ANY, LMAP_STRING, key, (LMPointer)string);
		AddTail((struct List *)map, (struct Node *)node);
	}
}

void SetLMapDecimal(LMap *map, LMString key, LMDecimal decimal) {
	LMapNode *node = FindNamedType(map, key, LMAP_DECIMAL);
	if (node) {
		node->data.u.decimal = decimal;
	}
	else {
		node = NewLMapNode(MEMF_ANY, LMAP_DECIMAL, key, (LMPointer)&decimal);
		AddTail((struct List *)map, (struct Node *)node);
	}
}

void SetLMapInteger(LMap *map, LMString key, LMInteger integer) {
	LMapNode *node = FindNamedType(map, key, LMAP_INTEGER);
	if (node) {
		node->data.u.integer = integer;
	}
	else {
		node = NewLMapNode(MEMF_ANY, LMAP_INTEGER, key, (LMPointer)&integer);
		AddTail((struct List *)map, (struct Node *)node);
	}
}

void SetLMapBoolean(LMap *map, LMString key, LMBoolean boolean) {
	LMapNode *node = FindNamedType(map, key, LMAP_LMBooleanEAN);
	if (node) {
		node->data.u.boolean = boolean;
	}
	else {
		node = NewLMapNode(MEMF_ANY, LMAP_POINTER, key, (LMPointer)&boolean);
		AddTail((struct List *)map, (struct Node *)node);
	}
}

void SetLMapMap(LMap *map, LMString key, LMap *lmap) {
	LMapNode *node = FindNamedType(map, key, LMAP_LMAP);
	if (node) {
		node->data.u.map = lmap;
	}
	else {
		node = NewLMapNode(MEMF_ANY, LMAP_LMAP, key, (LMPointer)lmap);
		AddTail((struct List *)map, (struct Node *)node);
	}
}

void SetLMapList(LMap *map, LMString key, List *list) {
	LMapNode *node = FindNamedType(map, key, LMAP_LIST);
	if (node) {
		node->data.u.list = list;
	}
	else {
		node = NewLMapNode(MEMF_ANY, LMAP_LIST, key, (LMPointer)list);
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
	LMBoolean include;

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

LMBoolean HasLMapTypeForKey(LMap *map, LMString key, LMapDataType type) {
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

List *GetAllLMapValuesForKey(LMap *map, LMString key) {
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




