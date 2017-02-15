#include <nea/util/lmap.h>
#include <exec/memory.h>

NEABoolean __asm __saveds LMapNodeHasNext(register __a0 LMapNode *node) {
  return (NEABoolean)(node->node.ln_Succ != NULL);
}

NEABoolean __asm __saveds LIBLMapNodeHasNext(register __a0 LMapNode *node) {
  return LMapNodeHasNext(node);
}

/* --------------------------------------------------------------------------*/

LMapNode * __asm __saveds LMapNodeNextNode(register __a0 LMapNode *node) {
  return (LMapNode*)node->node.ln_Succ;
}

LMapNode * __asm __saveds LIBLMapNodeNextNode(register __a0 LMapNode *node) {
  return LMapNodeNextNode(node);
}

/* --------------------------------------------------------------------------*/

LMapNode * __asm __saveds LMapNodePrevNode(register __a0 LMapNode *node) {
  return (LMapNode*)node->node.ln_Pred;
}

LMapNode * __asm __saveds LIBLMapNodePrevNode(register __a0 LMapNode *node) {
  return LMapNodePrevNode(node);
}

/* --------------------------------------------------------------------------*/

LMapDataType __asm __saveds LMapNodeType(register __a0 LMapNode *node) {
  return node->data.type;
}

LMapDataType __asm __saveds LIBLMapNodeType(register __a0 LMapNode *node) {
  return LMapNodeType(node);
}

/* --------------------------------------------------------------------------*/

LMapNode * __asm __saveds LMapHeadNode(register __a0 LMap *map) {
  return (LMapNode *)map->list.lh_Head;
}

LMapNode * __asm __saveds LIBLMapHeadNode(register __a0 LMap *map) {
  return LMapHeadNode(map);
}

/* -- Initializers ----------------------------------------------------------*/

LMap * __asm __saveds NewLMap(register __d0 ULONG memReqs) {
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

LMap * __asm __saveds LIBNewLMap(register __d0 ULONG memReqs) {
  return NewLMap(memReqs);
}

/* --------------------------------------------------------------------------*/

LMapNode * __asm __saveds NewLMapNode(
  register __d0 ULONG memReqs,
  register __d1 LMapDataType type,
  register __a0 NEAString key,
  register __a1 NEAPointer data
) {
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

LMapNode * __asm __saveds LIBNewLMapNode(
  register __d0 ULONG memReqs,
  register __d1 LMapDataType type,
  register __a0 NEAString key,
  register __a1 NEAPointer data
) {
  return NewLMapNode(memReqs, type, key, data);
}

/* --------------------------------------------------------------------------*/

LMapNode * __asm __saveds CopyLMapNode(
  register __d0 ULONG memReqs,
  register __a0 LMapNode *source
) {
  LMapNode *dest;

  dest = (LMapNode *)AllocVec(sizeof(LMapNode), memReqs | MEMF_CLEAR);
  CopyMemQuick((NEAPointer)&source->data, (NEAPointer)&dest->data, sizeof(LMapData));

  return dest;
}

LMapNode * __asm __saveds LIBCopyLMapNode(
  register __d0 ULONG memReqs,
  register __a0 LMapNode *source
) {
  return CopyLMapNode(memReqs, source);
}

/* --------------------------------------------------------------------------*/

/** Destructors */
void  __asm __saveds FreeLMap(register __a0 LMap *map) {
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

void  __asm __saveds LIBFreeLMap(register __a0 LMap *map) {
  FreeLMap(map);
}

/* --------------------------------------------------------------------------*/

LMapNode * __asm __saveds FindNamedType(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __d0 LMapDataType type
) {
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

LMapNode * __asm __saveds LIBFindNamedType(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __d0 LMapDataType type
) {
  return FindNamedType(map, key, type);
}

/* --------------------------------------------------------------------------*/

/** Getters */
NEAPointer __asm __saveds GetLMapPointer(
  register __a0 LMap *map,
  register __a1 NEAString key
) {
  return GetLMapPointerDefault(map, key, NULL);
}

NEAPointer __asm __saveds LIBGetLMapPointer(
  register __a0 LMap *map,
  register __a1 NEAString key
) {
  return GetLMapPointer(map, key);
}

/* --------------------------------------------------------------------------*/

NEAString __asm __saveds GetLMapString(
  register __a0 LMap *map,
  register __a1 NEAString key
) {
  return GetLMapStringDefault(map, key, NULL);
}

NEAString __asm __saveds LIBGetLMapString(
  register __a0 LMap *map,
  register __a1 NEAString key
) {
  return GetLMapString(map, key);
}

/* --------------------------------------------------------------------------*/

NEADecimal __asm __saveds GetLMapDecimal(
  register __a0 LMap *map,
  register __a1 NEAString key
) {
  return GetLMapDecimalDefault(map, key, -1.);
}

NEADecimal __asm __saveds LIBGetLMapDecimal(
  register __a0 LMap *map,
  register __a1 NEAString key
) {
  return GetLMapDecimal(map, key);
}

/* --------------------------------------------------------------------------*/

NEAInteger __asm __saveds GetLMapInteger(
  register __a0 LMap *map,
  register __a1 NEAString key
) {
  return GetLMapIntegerDefault(map, key, -1);
}

NEAInteger __asm __saveds LIBGetLMapInteger(
  register __a0 LMap *map,
  register __a1 NEAString key
) {
  return GetLMapInteger(map, key);
}

/* --------------------------------------------------------------------------*/

NEABoolean __asm __saveds GetLMapBoolean(
  register __a0 LMap *map,
  register __a1 NEAString key
) {
  return GetLMapBooleanDefault(map, key, FALSE);
}

NEABoolean __asm __saveds LIBGetLMapBoolean(
  register __a0 LMap *map,
  register __a1 NEAString key
) {
  return GetLMapBoolean(map, key);
}

/* --------------------------------------------------------------------------*/

LMap * __asm __saveds GetLMapMap(
  register __a0 LMap *map,
  register __a1 NEAString key
) {
  return GetLMapMapDefault(map, key, NULL);
}

LMap * __asm __saveds LIBGetLMapMap(
  register __a0 LMap *map,
  register __a1 NEAString key
) {
  return GetLMapMap(map, key);
}

/* --------------------------------------------------------------------------*/

List * __asm __saveds GetLMapList(
  register __a0 LMap *map,
  register __a1 NEAString key
) {
  return GetLMapListDefault(map, key, NULL);
}

List * __asm __saveds LIBGetLMapList(
  register __a0 LMap *map,
  register __a1 NEAString key
) {
  return GetLMapList(map, key);
}

/* --------------------------------------------------------------------------*/

NEAPointer __asm __saveds GetLMapPointerDefault(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __a2 NEAPointer def
) {
  LMapNode *node = FindNamedType(map, key, LMAP_POINTER);
  return node ? node : def;
}

NEAPointer __asm __saveds LIBGetLMapPointerDefault(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __a2 NEAPointer def
) {
  return GetLMapPointerDefault(map, key, def);
}

/* --------------------------------------------------------------------------*/

NEAString  __asm __saveds GetLMapStringDefault(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __a2 NEAString def
) {
  LMapNode *node = FindNamedType(map, key, LMAP_STRING);
  return node ? node->data.u.string : def;
}

NEAString  __asm __saveds LIBGetLMapStringDefault(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __a2 NEAString def
) {
  return GetLMapStringDefault(map, key, def);
}

/* --------------------------------------------------------------------------*/

NEADecimal  __asm __saveds GetLMapDecimalDefault(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __d0 NEADecimal def
) {
  LMapNode *node = FindNamedType(map, key, LMAP_DECIMAL);
  return node ? node->data.u.decimal : def;
}

NEADecimal  __asm __saveds LIBGetLMapDecimalDefault(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __d0 NEADecimal def
) {
  return GetLMapDecimalDefault(map, key, def);
}

/* --------------------------------------------------------------------------*/

NEAInteger  __asm __saveds GetLMapIntegerDefault(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __d0 NEAInteger def
) {
  LMapNode *node = FindNamedType(map, key, LMAP_INTEGER);
  return node ? node->data.u.integer : def;
}

NEAInteger  __asm __saveds LIBGetLMapIntegerDefault(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __d0 NEAInteger def
) {
  return GetLMapIntegerDefault(map, key, def);
}

/* --------------------------------------------------------------------------*/

NEABoolean  __asm __saveds GetLMapBooleanDefault(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __d0 NEABoolean def
) {
  LMapNode *node = FindNamedType(map, key, LMAP_BOOLEAN);
  return (NEABoolean)(node ? node->data.u.boolean : def);
}

NEABoolean  __asm __saveds LIBGetLMapBooleanDefault(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __d0 NEABoolean def
) {
  return GetLMapBooleanDefault(map, key, def);
}

/* --------------------------------------------------------------------------*/

LMap * __asm __saveds GetLMapMapDefault(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __a2 LMap *lmap
) {
  LMapNode *node = FindNamedType(map, key, LMAP_LMAP);
  return node ? node->data.u.map : lmap;
}

LMap * __asm __saveds LIBGetLMapMapDefault(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __a2 LMap *lmap
) {
  return GetLMapMapDefault(map, key, lmap);
}

/* --------------------------------------------------------------------------*/

List * __asm __saveds GetLMapListDefault(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __a2 List *list
) {
  LMapNode *node = FindNamedType(map, key, LMAP_LIST);
  return node ? node->data.u.list : list;
}

List * __asm __saveds LIBGetLMapListDefault(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __a2 List *list
) {
  return GetLMapListDefault(map, key, list);
}

/* -- Setters ---------------------------------------------------------------*/

void __asm __saveds SetLMapPointer(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __a2 NEAPointer pointer
) {
  LMapNode *node = FindNamedType(map, key, LMAP_POINTER);
  if (node) {
    node->data.u.pointer = pointer;
  }
  else {
    node = NewLMapNode(MEMF_ANY, LMAP_POINTER, key, pointer);
    AddTail((struct List *)map, (struct Node *)node);
  }
}

void __asm __saveds LIBSetLMapPointer(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __a2 NEAPointer pointer
) {
  SetLMapPointer(map, key, pointer);
}

/* --------------------------------------------------------------------------*/

void __asm __saveds SetLMapString(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __a2 NEAString string
) {
  LMapNode *node = FindNamedType(map, key, LMAP_STRING);
  if (node) {
    node->data.u.string = string;
  }
  else {
    node = NewLMapNode(MEMF_ANY, LMAP_STRING, key, (NEAPointer)string);
    AddTail((struct List *)map, (struct Node *)node);
  }
}

void __asm __saveds LIBSetLMapString(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __a2 NEAString string
) {
  SetLMapString(map, key, string);
}

/* --------------------------------------------------------------------------*/

void __asm __saveds SetLMapDecimal(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __d0 NEADecimal decimal
) {
  LMapNode *node = FindNamedType(map, key, LMAP_DECIMAL);
  NEADecimal ptr = decimal;

  if (node) {
    node->data.u.decimal = decimal;
  }
  else {
    node = NewLMapNode(MEMF_ANY, LMAP_DECIMAL, key, (NEADecimal*)&ptr);
    AddTail((struct List *)map, (struct Node *)node);
  }
}

void __asm __saveds LIBSetLMapDecimal(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __d0 NEADecimal decimal
) {
  SetLMapDecimal(map, key, decimal);
}

/* --------------------------------------------------------------------------*/

void __asm __saveds SetLMapInteger(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __d0 NEAInteger integer
) {
  LMapNode *node = FindNamedType(map, key, LMAP_INTEGER);
  NEAInteger ptr = integer;
  if (node) {
    node->data.u.integer = integer;
  }
  else {
    node = NewLMapNode(MEMF_ANY, LMAP_INTEGER, key, (NEAPointer)&ptr);
    AddTail((struct List *)map, (struct Node *)node);
  }
}

void __asm __saveds LIBSetLMapInteger(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __d0 NEAInteger integer
) {
  SetLMapInteger(map, key, integer);
}

/* --------------------------------------------------------------------------*/

void __asm __saveds SetLMapBoolean(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __d0 NEABoolean boolean
) {
  LMapNode *node = FindNamedType(map, key, LMAP_BOOLEAN);
  NEABoolean ptr = boolean;
  if (node) {
    node->data.u.boolean = boolean;
  }
  else {
    node = NewLMapNode(MEMF_ANY, LMAP_POINTER, key, (NEABoolean *)&ptr);
    AddTail((struct List *)map, (struct Node *)node);
  }
}

void __asm __saveds LIBSetLMapBoolean(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __d0 NEABoolean boolean
) {
  SetLMapBoolean(map, key, boolean);
}

/* --------------------------------------------------------------------------*/

void __asm __saveds SetLMapMap(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __a2 LMap *lmap
) {
  LMapNode *node = FindNamedType(map, key, LMAP_LMAP);
  if (node) {
    node->data.u.map = lmap;
  }
  else {
    node = NewLMapNode(MEMF_ANY, LMAP_LMAP, key, (NEAPointer)lmap);
    AddTail((struct List *)map, (struct Node *)node);
  }
}

void __asm __saveds LIBSetLMapMap(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __a2 LMap *lmap
) {
  SetLMapMap(map, key, lmap);
}

/* --------------------------------------------------------------------------*/

void __asm __saveds SetLMapList(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __a2 List *list
) {
  LMapNode *node = FindNamedType(map, key, LMAP_LIST);
  if (node) {
    node->data.u.list = list;
  }
  else {
    node = NewLMapNode(MEMF_ANY, LMAP_LIST, key, (NEAPointer)list);
    AddTail((struct List *)map, (struct Node *)node);
  }
}

void __asm __saveds LIBSetLMapList(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __a2 List *list
) {
  SetLMapList(map, key, list);
}

/* --------------------------------------------------------------------------*/

/** Utility methods */
size_t __asm __saveds CountLMapNodes(register __a0 LMap *map) {
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

size_t __asm __saveds LIBCountLMapNodes(register __a0 LMap *map) {
  return CountLMapNodes(map);
}

/* --------------------------------------------------------------------------*/

void __asm __saveds ForEachLMapNode(
  register __a0 LMap *map,
  register __a1 LMapIterFn fn
) {
  LMapNode *node;

  for (
    node = (LMapNode *)map->list.lh_Head;
    node && strlen(node->node.ln_Name);
    node = (LMapNode *)node->node.ln_Succ
  ) {
    (*fn)(node, node->node.ln_Name, &node->data);
  }
}

void __asm __saveds LIBForEachLMapNode(
  register __a0 LMap *map,
  register __a1 LMapIterFn fn
) {
  ForEachLMapNode(map, fn);
}

/* --------------------------------------------------------------------------*/

LMap * __asm __saveds FilterLMap(
  register __a0 LMap *map,
  register __a1 LMapFilterFn fn
) {
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

LMap * __asm __saveds LIBFilterLMap(
  register __a0 LMap *map,
  register __a1 LMapFilterFn fn
) {
  return FilterLMap(map, fn);
}

/* --------------------------------------------------------------------------*/

NEABoolean __asm __saveds HasLMapTypeForKey(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __d0 LMapDataType type
) {
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

NEABoolean __asm __saveds LIBHasLMapTypeForKey(
  register __a0 LMap *map,
  register __a1 NEAString key,
  register __d0 LMapDataType type
) {
  return HasLMapTypeForKey(map, key, type);
}

/* --------------------------------------------------------------------------*/

List * __asm __saveds GetAllLMapValuesForKey(
  register __a0 LMap *map,
  register __a1 NEAString key
) {
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

List * __asm __saveds LIBGetAllLMapValuesForKey(
  register __a0 LMap *map,
  register __a1 NEAString key
) {
  return GetAllLMapValuesForKey(map, key);
}

/* --------------------------------------------------------------------------*/

NEAString __asm __saveds LMapToString(
  register __a0 NEAString buffer,
  register __a1 LMap *map,
  register __d0 NEABoolean verbose
) {
  /*
  LMapNode *node;
  NEAString buffer;
  NEAString nodeString;
  NEAInteger bytes = 0;
  NEAInteger bufferSize = 0;
  NEAChar temp[1024];

  while(node = map->head(map); node; node = node->next(node)) {
    nodeString = LMapNodeToString(&temp, node, verbose);
    bytes += strlen(nodeString);
    strcat(buffer,)
  }
  */

  return NULL;
}

NEAString __asm __saveds LIBLMapToString(
  register __a0 NEAString buffer,
  register __a1 LMap *map,
  register __d0 NEABoolean verbose
) {
  return LMapToString(buffer, map, verbose);
}

/* --------------------------------------------------------------------------*/

NEAString __asm __saveds LMapNodeToString(
  register __a0 NEAString buffer,
  register __a1 LMapNode *node,
  register __d0 NEABoolean verbose
) {
  return NULL;
}

NEAString __asm __saveds LIBLMapNodeToString(
  register __a0 NEAString buffer,
  register __a1 LMapNode *node,
  register __d0 NEABoolean verbose
) {
  return LMapNodeToString(buffer, node, verbose);
}
