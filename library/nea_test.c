;/* nea_test.c - Execute me to compile me with SAS/C 6.56
delete nea_test nea_test.(o|info)
sc data=far nominc strmer streq nostkchk saveds ign=73 nea_test.c
slink FROM LIB:c.o,nea_test.o TO nea_test LIB LIB:SC.lib,LIB:Amiga.lib
quit
*/

#include <stdio.h>

#include <proto/exec.h>
#include <exec/libraries.h>

#include <proto/nea.h>
#include <pragmas/nea_pragmas.h>

extern struct ExecBase *SysBase;

int main(int argc, char **argv) {
  struct Library *NEABase;
  char *result;

  NEABase = OpenLibrary("nea.library", 0L);
  if (NEABase) {
    printf("Library opened\n");
    result = sayHi(5, "Great");
    printf("Result %s\n", result);
    FreeVec(result);
    CloseLibrary(NEABase);
  }
  else {
    fprintf(stderr, "Failed to open nea.library\n");
    return 1;
  }

  return 0;
}
