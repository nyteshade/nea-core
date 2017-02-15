;/* nea_test.c - Execute me to compile me with SAS/C 6.56
delete nea_test nea_test.(o|info|tmp|lnk)
sc LINK TO nea_test LIB LIB:SC.lib,LIB:scm.lib,LIB:Amiga.lib IDIR /src data=far nominc strmer streq nostkchk saveds ign=73 /src/nea/types.c /src/nea/file/strings.c nea_test.c
delete nea_test.(o|info|tmp|lnk)
quit
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <proto/exec.h>
#include <proto/dos.h>
#include <exec/libraries.h>

#include <nea/types.h>
#include <nea/file/strings.h>
#include <nea/util/version.h>
#include <nea/pragmas/nea_pragmas.h>
#include <nea/proto/nea.h>

struct Library *NEABase = 0L;

void testFileStrings(void);
void setup(void);
void teardown(void);

int main(int argc, char **argv) {
  setup();
  testFileStrings();
  teardown();

  return 0;
}

void setup() {
  NEABase = OpenLibrary("nea.library", 0L);
  if (!NEABase) {
    fprintf(stderr, "Failed to open nea.library\n");
    exit(1);
  }
  else {
    printf("nea.library successfully opened.\n");
  }
}

void teardown() {
  if (NEABase) {
    CloseLibrary(NEABase);
    printf("nea.library closed.\n");
  }
}

void testFileStrings() {
    BPTR file;
    NEAChar buffer[1024];
    NEAChar name[8];
    NEAFString fstring;
    
    strcpy(name, "Brielle");
    SetFString(name, &fstring); 
    Printf("%s\n", FStringToString(&fstring, buffer, "stack")); 
    Printf("Wrote the above NEAFString to ram:test.bin\n");
    file = Open("ram:test.bin", MODE_NEWFILE);
    WriteFString(file, &fstring);
    Close(file);

    Printf("Reading the file ram:test.bin\n");
    file = Open("ram:test.bin", MODE_OLDFILE);
    ReadFString(file, &fstring);
    Close(file);
    Printf("%s\n", FStringToString(&fstring, buffer, "disk"));
}