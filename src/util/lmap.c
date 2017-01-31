#include "lmap.h"

#include <exec/memory.h>

NEABoolean LMapNodeHasNext(LMapNode *node) {
  return (NEABoolean)(node->node.ln_Succ != NULL);
}

LMapNode *LMapNodeNextNode(LMapNode *node) {
  return (LMapNode*)node->node.ln_Succ;
}

LMapNode *LMapNodePrevNode(LMapNode *node) {
  return (LMapNode*)node->node.ln_Pred;
}

LMapDataType LMapNodeType(LMapNode *node) {
  return node->data.type;
}

LMapNode *LMapHeadNode(LMap *map) {
  return (LMapNode *)map->list.lh_Head;
}

/** Initializers */
LMap *NewLMap(ULONG memReqs) {
  LMap *map;

  map = (LMap *)AllocVec(sizeof(LMap), memReqs);
  NewList(&map->list);

  map->head = LMapHeadNode;

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

LMapNode *NewLMapNode(ULONG memReqs, LMapDataType type, NEAString key, NEAPointer data) {
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
      node->data.u.string = (NEAString)data; break;
    case LMAP_DECIMAL:
      node->data.u.decimal = *(NEADecimal *)data; break;
    case LMAP_INTEGER:
      node->data.u.integer = *(NEAInteger *)data; break;
    case LMAP_BOOLEAN:
      node->data.u.boolean = *(NEABoolean *)data ? TRUE : FALSE; break;
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
  CopyMemQuick((NEAPointer)&source->data, (NEAPointer)&dest->data, sizeof(LMapData));

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

LMapNode *FindNamedType(LMap *map, NEAString key, LMapDataType type) {
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
NEAPointer GetLMapPointer(LMap *map, NEAString key) {
  return GetLMapPointerDefault(map, key, NULL);
}

NEAString GetLMapString(LMap *map, NEAString key) {
  return GetLMapStringDefault(map, key, NULL);
}

NEADecimal GetLMapDecimal(LMap *map, NEAString key) {
  return GetLMapDecimalDefault(map, key, -1.);
}

NEAInteger GetLMapInteger(LMap *map, NEAString key) {
  return GetLMapIntegerDefault(map, key, -1);
}

NEABoolean GetLMapBoolean(LMap *map, NEAString key) {
  return GetLMapBooleanDefault(map, key, -1);
}

LMap *GetLMapMap(LMap *map, NEAString key) {
  return GetLMapMapDefault(map, key, NULL);
}

List *GetLMapList(LMap *map, NEAString key) {
  return GetLMapListDefault(map, key, NULL);
}

NEAPointer GetLMapPointerDefault(LMap *map, NEAString key, NEAPointer def) {
  LMapNode *node = FindNamedType(map, key, LMAP_POINTER);
  return node ? node : def;
}

NEAString GetLMapStringDefault(LMap *map, NEAString key, NEAString def) {
  LMapNode *node = FindNamedType(map, key, LMAP_STRING);
  return node ? node->data.u.string : def;
}

NEADecimal GetLMapDecimalDefault(LMap *map, NEAString key, NEADecimal def) {
  LMapNode *node = FindNamedType(map, key, LMAP_DECIMAL);
  return node ? node->data.u.decimal : def;
}

NEAInteger GetLMapIntegerDefault(LMap *map, NEAString key, NEAInteger def) {
  LMapNode *node = FindNamedType(map, key, LMAP_INTEGER);
  return node ? node->data.u.integer : def;
}

NEABoolean GetLMapBooleanDefault(LMap *map, NEAString key, NEABoolean def) {
  LMapNode *node = FindNamedType(map, key, LMAP_BOOLEAN);
  return (NEABoolean)(node ? node->data.u.boolean : def);
}

LMap *GetLMapMapDefault(LMap *map, NEAString key, LMap *def) {
  LMapNode *node = FindNamedType(map, key, LMAP_LMAP);
  return node ? node->data.u.map : def;
}

List *GetLMapListDefault(LMap *map, NEAString key, List *def) {
  LMapNode *node = FindNamedType(map, key, LMAP_LIST);
  return node ? node->data.u.list : def;
}

/** Setters */
void SetLMapPointer(LMap *map, NEAString key, NEAPointer pointer) {
  LMapNode *node = FindNamedType(map, key, LMAP_POINTER);
  if (node) {
    node->data.u.pointer = pointer;
  }
  else {
    node = NewLMapNode(MEMF_ANY, LMAP_POINTER, key, pointer);
    AddTail((struct List *)map, (struct Node *)node);
  }
}

void SetLMapString(LMap *map, NEAString key, NEAString string) {
  LMapNode *node = FindNamedType(map, key, LMAP_STRING);
  if (node) {
    node->data.u.string = string;
  }
  else {
    node = NewLMapNode(MEMF_ANY, LMAP_STRING, key, (NEAPointer)string);
    AddTail((struct List *)map, (struct Node *)node);
  }
}

void SetLMapDecimal(LMap *map, NEAString key, NEADecimal decimal) {
  LMapNode *node = FindNamedType(map, key, LMAP_DECIMAL);
  if (node) {
    node->data.u.decimal = decimal;
  }
  else {
    node = NewLMapNode(MEMF_ANY, LMAP_DECIMAL, key, (NEADecimal *)&decimal);
    AddTail((struct List *)map, (struct Node *)node);
  }
}

void SetLMapInteger(LMap *map, NEAString key, NEAInteger integer) {
  LMapNode *node = FindNamedType(map, key, LMAP_INTEGER);
  if (node) {
    node->data.u.integer = integer;
  }
  else {
    node = NewLMapNode(MEMF_ANY, LMAP_INTEGER, key, (NEAPointer)&integer);
    AddTail((struct List *)map, (struct Node *)node);
  }
}

void SetLMapBoolean(LMap *map, NEAString key, NEABoolean boolean) {
  LMapNode *node = FindNamedType(map, key, LMAP_BOOLEAN);
  if (node) {
    node->data.u.boolean = boolean;
  }
  else {
    node = NewLMapNode(MEMF_ANY, LMAP_POINTER, key, (NEAPointer)&boolean);
    AddTail((struct List *)map, (struct Node *)node);
  }
}

void SetLMapMap(LMap *map, NEAString key, LMap *lmap) {
  LMapNode *node = FindNamedType(map, key, LMAP_LMAP);
  if (node) {
    node->data.u.map = lmap;
  }
  else {
    node = NewLMapNode(MEMF_ANY, LMAP_LMAP, key, (NEAPointer)lmap);
    AddTail((struct List *)map, (struct Node *)node);
  }
}

void SetLMapList(LMap *map, NEAString key, List *list) {
  LMapNode *node = FindNamedType(map, key, LMAP_LIST);
  if (node) {
    node->data.u.list = list;
  }
  else {
    node = NewLMapNode(MEMF_ANY, LMAP_LIST, key, (NEAPointer)list);
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
  NEABoolean include;

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

NEABoolean HasLMapTypeForKey(LMap *map, NEAString key, LMapDataType type) {
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

List *GetAllLMapValuesForKey(LMap *map, NEAString key) {
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
