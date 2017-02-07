
##
##  nea.library makefile
##
##  Nyteshade Enterprises Amiga library
##
##  Contains a lot of useful code that I got tired of rewriting for each
##  new application I decided to write. Should be backwards compatible and
##  should allow much faster development on the Amiga.

MODNAME=            nea
VERSION=            37
REVISION=           0

LIBFILE=            $(MODNAME).library

FD_CONV=            SC:C/FD2PRAGMA
FD_FILE=            $(MODNAME)_lib.fd
PRAGMA_FILE=        pragmas/$(MODNAME)_pragmas.h
PROTO_FILE=         proto/$(MODNAME).h

C_COMPILER=         SC:C/SC
C_OPTS=             STREQ STRMER NOSTKCHK LIBCODE

LINKER=             SC:C/SLINK

C_SOURCES=          nea_lib.c nea.c

OBJECTS=            nea_lib.o nea.o
LIBS=               LIB:sc.lib LIB:amiga.lib

LIBENT=             LIB:libent.o
LIBINIT=            LIB:libinit.o
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
    @copy $(PROTO_FILE) INCLUDE:proto/
    @copy $(PRAGMA_FILE) INCLUDE:pragmas/
    #@copy $(FD_FILE) FD:
    @flushlibs
    @version $(LIBFILE)








