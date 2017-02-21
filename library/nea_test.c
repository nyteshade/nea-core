;/* nea_test.c - Execute me to compile me with SAS/C 6.56
delete nea_test nea_test.(o|info|tmp|lnk)
sc LINK TO nea_test LIB LIB:SC.lib,LIB:scm.lib,LIB:Amiga.lib debug=full IDIR /src data=far nominc strmer streq nostkchk saveds ign=73 nea_test.c
delete nea_test.(o|info|tmp|lnk)
quit
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <proto/exec.h>
#include <proto/dos.h>
#include <exec/libraries.h>

#include <nea/pragmas/nea_pragmas.h>
#include <nea/core/strings.h>
#include <nea/core/arrays.h>
#include <nea/file/strings.h>
#include <nea/util/version.h>

struct Library *NEABase = 0L;

void testStrings(void);
void testFileStrings(void);
void testArrays(void);
void setup(void);
void teardown(void);

int main(int argc, char **argv) {
  setup();
  testFileStrings();
  testStrings();
  testArrays();
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

void testArrays() {
  Array *iArray = NewIntegerArray(5, NULL);
  NEAUInteger i;

  printf("Array Tests\nBefore:\n");
  printf("\tCount %lu, Space for %lu\n", iArray->count, iArray->countMax);

  printf("Adding items\n");
  iArray
    ->addInteger(iArray, 1)
    ->addInteger(iArray, 2)
    ->addInteger(iArray, 4)
    ->addInteger(iArray, 8)
    ->addInteger(iArray, 16);

  printf("After adding 5 items:\n");
  printf("\tCount %lu, Space for %lu\n", iArray->count, iArray->countMax);

  printf("Showing all items in array: \n");
  for (i = 0; i < iArray->count; i++) {
    printf("\t[%lu]: %lu\n", i, iArray->integers[i]);
  }

  printf("Freeing array...");
  FreeArray((Array *)iArray);
  printf("done\n");
}

void testStrings() {
  StringPtr string;
  NEAChar sourceA[8] = "Brielle";
  NEAChar sourceB[8] = "Brielle";

  string = StringCreate(sourceA);
  printf("\nStringCreate() test\n\n");
  printf("String: %s\n", *string);
  printf("Length: %lu\n", string->length);

  printf("\nModifying\n");
  strcpy(sourceA, "Lucyann");

  printf("String: %s\n", *string);
  printf("Length: %lu\n", string->length);
  StringRemove(string);

  printf("\nStringDuplicate() test\n\n");
  string = StringDuplicate(sourceB);
  printf("String: %s\n", *string);
  printf("Length: %lu\n", string->length);

  printf("\nModifying\n");
  strcpy(sourceB, "Lucille");

  printf("String: %s\n", *string);
  printf("Length: %lu\n", string->length);
  StringRemove(string);
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
