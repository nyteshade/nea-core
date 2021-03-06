
##
##  armadillo.library makefile
##
##  This is a more-or-less generic makefile, which is currently set
##  to compile armadillo.library but which can easily be changed to
##  use your own files...

MODNAME=            armadillo
VERSION=            37
REVISION=           0

LIBFILE=            $(MODNAME).library

FD_CONV=            SC:C/FD2PRAGMA
FD_FILE=            $(MODNAME)_lib.fd
PRAGMA_FILE=        $(MODNAME)_pragmas.h

C_COMPILER=         SC:C/SC
C_OPTS=             STREQ STRMER NOSTKCHK LIBCODE

LINKER=             SC:C/SLINK

C_SOURCES=          dillo_lib.c dillo.c

OBJECTS=            dillo_lib.o dillo.o
LIBS=               LIB:sc.lib LIB:amiga.lib

LIBENT=             LIB:libent.o
LIBINIT=            LIB:libinitr.o
LIBPREFIX=          _LIB

##############################################################################
# Build the library...

$(LIBFILE): $(OBJECTS) $(LIBS) $(PRAGMA_FILE)

    $(LINKER) WITH <<
TO $(LIBFILE)
FROM $(LIBENT) $(LIBINIT) $(OBJECTS)
LIBFD $(FD_FILE)
LIBPREFIX $(LIBPREFIX)
LIBVERSION $(VERSION)
LIBREVISION $(REVISION)
LIB $(LIBS)
<

$(PRAGMA_FILE): $(FD_FILE)

##############################################################################
# Default rules...
#
.c.o:
    $(C_COMPILER) $(C_OPTS) $*.c

.fd.h:
    $(FD_CONV) $(FD_FILE) $(PRAGMA_FILE)

##############################################################################
# Delete all object files
#
clean:
    @Delete $(OBJECTS)
    @Delete $(LIBFILE)(|.info)
    @Delete $(MODNAME).map(|.info)

##############################################################################
# Load the new library into the system
#
reload:
    @copy $(LIBFILE) LIBS:
    @copy $(FD_FILE) FD:
    @flushlibs
    @version $(LIBFILE)
