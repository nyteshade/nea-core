;/* nea_test.c - Execute me to compile me with SAS/C 6.56
delete nea_test nea_test.(o|info|tmp)
sc IDIR /src data=far nominc strmer streq nostkchk saveds ign=73 nea_test.c
slink FROM LIB:c.o,nea_test.o TO nea_test LIB LIB:SC.lib,LIB:Amiga.lib
quit
*/

#include <stdio.h>

#include <proto/exec.h>
#include <exec/libraries.h>


#include <nea/types.h>
#include <nea/pragmas/nea.h>
#include <nea/proto/nea.h>

extern struct ExecBase *SysBase;

int main(int argc, char **argv) {
  NEAPoint *p;
  struct Library *NEABase;
  char *result;

  NEABase = OpenLibrary("nea.library", 0L);
  if (NEABase) {
    printf("Library opened\n");

    result = sayHi(5, "Great");
    printf("Result %s\n", result);

    p = NewPointI(50, 100, 0);
    printf("[x:%ld, y:%ld]\n", p->x.i, p->y.i);
    
    FreeVec(p);
    FreeVec(result);
    CloseLibrary(NEABase);
  }
  else {
    fprintf(stderr, "Failed to open nea.library\n");
    return 1;
  }

  return 0;
}
