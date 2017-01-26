#include "util/lmap.h"
#include <exec/memory.h>

#include <stdio.h>

void PrintfEachNode(LMapNode *node, STRPTR key, LMapData *data) {
  printf("KEY %s\n", key);
  switch(data->type) {
    case LMAP_POINTER:
	  printf("VAL %p\n", data->u.pointer);
	  break;
	case LMAP_STRING:
	  printf("VAL %s\n", data->u.string);
      break;
	case LMAP_DECIMAL:
	  printf("VAL %lf\n", data->u.decimal);
      break;
	case LMAP_INTEGER:
	  printf("VAL %ld\n", data->u.integer);
      break;
    case LMAP_BOOLEAN:
	  printf("VAL %s\n", data->u.boolean ? "true" : "false");
      break;
    case LMAP_LMAP:
    case LMAP_LIST:
      break;
  }
  printf("\n");
}

int main() {
	LMap *map = NewLMap(MEMF_ANY);
	
	SetLMapString(map, "name", "Brielle Harrison");
	SetLMapInteger(map, "age", 40);
	SetLMapDecimal(map, "age", 40.125f);
    SetLMapString(map, "gender", "Female");

	ForEachLMapNode(map, &PrintfEachNode);
	FreeLMap(map);
	
	return 0;
}
