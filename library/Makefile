
##
##  nea.library makefile
##
##  Nyteshade Enterprises Amiga library
##
##  Contains a lot of useful code that I got tired of rewriting for each
##  new application I decided to write. Should be backwards compatible and
##  should allow much faster development on the Amiga.

MODNAME=     nea
VERSION=     37
REVISION=    5

IDIR=        /src
LIBFILE=     $(MODNAME).library

FD_CONV=     SC:C/FD2PRAGMA
FD_FILE=     $(MODNAME)_lib.fd
PRAGMA_FILE= /src/nea/pragmas/$(MODNAME)_pragmas.h
PROTO_FILE=  /src/nea/proto/$(MODNAME).h

C_COMPILER=  SC:C/SC
C_OPTS=      STREQ STRMER NOSTKCHK LIBCODE IDIR $(IDIR)

LINKER=      SC:C/SLINK

C_SOURCES=   nea_lib.c $(IDIR)/nea/util/lmap.c $(IDIR)/nea/types.c $(IDIR)/nea/file/strings.c $(IDIR)/nea/util/version.c

OBJECTS=     nea_lib.o $(IDIR)/nea/types.o $(IDIR)/nea/util/lmap.o $(IDIR)/nea/file/strings.o $(IDIR)/nea/util/version.o
LIBS=        LIB:sc.lib LIB:amiga.lib LIB:scm.lib

LIBENT=      LIB:libent.o
LIBINIT=     LIB:libinitr.o
LIBPREFIX=   _LIB

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
    @flushlibs
    @version $(LIBFILE)

pragmas:
    $(FD_CONV) $(FD_FILE) $(PRAGMA_FILE)





