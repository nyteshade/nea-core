;/* nea_test.c - Execute me to compile me with SAS/C 6.56
delete nea_test nea_test.(o|info|tmp|lnk)
sc LINK TO nea_test LIB LIB:SC.lib,LIB:Amiga.lib IDIR /src data=far nominc strmer streq nostkchk saveds ign=73 /src/nea/types.c /src/nea/file/strings.c nea_test.c
delete nea_test.(o|info|tmp|lnk)
quit
*/

#include <stdio.h>
#include <string.h>

#include <proto/exec.h>
#include <proto/dos.h>
#include <exec/libraries.h>


#include <nea/types.h>
#include <nea/file/strings.h>
#include <nea/pragmas/nea.h>
#include <nea/proto/nea.h>

extern struct ExecBase *SysBase;

int main(int argc, char **argv) {
  BPTR file;
  NEAFString fstring;
  NEAString name;
  NEAPoint *p;
  struct Library *NEABase;
  char *result;

  NEABase = OpenLibrary("nea.library", 0L);
  if (NEABase) {
    Printf("Library opened\n");

    result = sayHi(5, "Great");
    Printf("Result %s\n", result);

    Printf("Creating new integer point at 50,100\n");
    p = NewPointI(50, 100, 0);
    Printf("[x:%ld, y:%ld]\n", p->x.i, p->y.i);

    name = (NEAString)AllocVec(sizeof(NEAByte) * 8, MEMF_ANY|MEMF_CLEAR);
    strcpy(name, "Brielle");
    SetFString(name, &fstring);
    Printf("NEAFString [stack]\n");
    Printf("  value  : %s\n", fstring.value);
    Printf("  length : %ld\n", fstring.length);
    Printf("  alloc'd: %s\n\n", fstring.allocated ? "true" : "false");
    
    Printf("Wrote the above NEAFString to ram:test.bin\n");
    file = Open("ram:test.bin", MODE_NEWFILE);
    WriteFString(file, &fstring);
    Close(file);
    
    Printf("Reading the file ram:test.bin\n");
    file = Open("ram:test.bin", MODE_OLDFILE);
    ReadFString(file, &fstring);
    Close(file);
    
    Printf("NEAFString [disk]\n");
    Printf("  value  : %s\n", fstring.value);
    Printf("  length : %ld\n", fstring.length);
    Printf("  alloc'd: %s\n\n", fstring.allocated ? "true" : "false");
    
    FreeVec(name);
    FreeVec(p);
    FreeVec(result);
    CloseLibrary(NEABase);
    
    Printf("Library closed\n");
  }
  else {
    fprintf(stderr, "Failed to open nea.library\n");
    return 1;
  }

  return 0;
}
