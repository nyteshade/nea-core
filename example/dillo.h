#ifndef DILLO_H
#define DILLO_H

/*
**  dillo.h - Header file for armadillo.library code modules
**            © Copyright 1993-1999, Amiga, Inc.
**            All Rights Reserved.
**            Written by John Wiederhirn
**
**  This is the header file for the armadillo.library code modules.  It
**  just gives a basic structure definition.
**
*/

struct Armadillo
{
    UBYTE   name[32];
    ULONG   weight;
    BOOL    flat;
};

#endif /* DILLO_H */