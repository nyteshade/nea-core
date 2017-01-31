#include "util/lmap.h"
#include <exec/memory.h>

#include <stdio.h>

void PrintNode(LMapNode *node) {
  printf("KEY %s\n", node->node.ln_Name);
  switch(node->data.type) {
    case LMAP_POINTER:
      printf("VAL %p\n", node->data.u.pointer);
      break;
    case LMAP_STRING:
      printf("VAL %s\n", node->data.u.string);
      break;
    case LMAP_DECIMAL:
      printf("VAL %.2f\n", node->data.u.decimal);
      break;
    case LMAP_INTEGER:
      printf("VAL %ld\n", node->data.u.integer);
      break;
    case LMAP_BOOLEAN:
      printf("VAL %s\n", node->data.u.boolean ? "true" : "false");
      break;
    case LMAP_LMAP:
    case LMAP_LIST:
      break;
  }
  printf("\n");

}

void PrintfEachNode(LMapNode *node, NEAString key, LMapData *data) {
  PrintNode(node);
}

int main() {
  LMap *map = NewLMap(MEMF_ANY);
  LMapNode *node;

  map->setString(map, "name", "Brielle Harrison");
  map->setInteger(map, "age", 40);
  map->setDecimal(map, "aging", 40.125);
  map->setString(map, "gender", "Female");

  /* ForEachLMapNode(map, &PrintfEachNode); */

  for (node = map->head(map); node->hasNext(node); node = node->next(node)) {
    PrintNode(node);
  }


  FreeLMap(map);

  return 0;
}
