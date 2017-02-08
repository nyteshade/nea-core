
#include    <exec/memory.h>
#include    <exec/types.h>
#include    <clib/exec_protos.h>
#include    <pragmas/exec_pragmas.h>

/* These prototypes are just to keep the compiler happy since we don't
** want them in the distributable proto file.
*/

int __saveds __UserLibInit(void);
void __saveds __UserLibCleanup(void);

/* Following global item (UtilityBase) is created so our library can make
** utility.library calls.  Technically putting it in the near section is a waste
** of memory, but for this example, it serves its purpose.  Note also that we
** don't actually MAKE any Utility calls, but we COULD.
*/

struct Library *SysBase = NULL;

/*
*/

int __saveds
__UserLibInit( void )
{
    int retval = 1;

    SysBase = (*((void **)4));

    retval = 0;
        
    return retval;
}


/* About as basic a routine as you can get, this routine cleans up the library
** by providing a matching CloseLibrary of the Utility library base we opened
** in the __UserLibInit() routine.
*/

void __saveds
__UserLibCleanup( void )
{
}









