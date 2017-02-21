
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
REVISION=    12

IDIR=        /src
LIBFILE=     $(MODNAME).library

FD_CONV=     SC:C/FD2PRAGMA
FD_FILE=     $(MODNAME)_lib.fd
PRAGMA_FILE= /src/nea/pragmas/$(MODNAME)_pragmas.h
PROTO_FILE=  /src/nea/proto/$(MODNAME).h

C_COMPILER=  SC:C/SC
C_OPTS=      DEBUG=full STREQ STRMER NOSTKCHK LIBCODE IDIR $(IDIR)

LINKER=      SC:C/SLINK addsym

UTIL_SRCS=   $(IDIR)/nea/util/lmap.c $(IDIR)/nea/util/version.c
CORE_SRCS=   $(IDIR)/nea/core/strings.c $(IDIR)/nea/core/arrays.c
FILE_SRCS=   $(IDIR)/nea/file/strings.c
ROOT_SRCS=   $(IDIR)/nea/types.c
C_SOURCES=   nea_lib.c $(ROOT_SRCS) $(CORE_SRCS) $(FILE_SRCS) $(UTIL_SRCS)

UTIL_OBJS=   $(IDIR)/nea/util/lmap.o $(IDIR)/nea/util/version.o
CORE_OBJS=   $(IDIR)/nea/core/strings.o $(IDIR)/nea/core/arrays.o
FILE_OBJS=   $(IDIR)/nea/file/strings.o
ROOT_OBJS=   $(IDIR)/nea/types.o
OBJECTS=     nea_lib.o $(ROOT_OBJS) $(CORE_OBJS) $(FILE_OBJS) $(UTIL_OBJS)

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
