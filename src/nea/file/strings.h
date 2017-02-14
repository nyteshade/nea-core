#ifndef NEA_FILE_STRINGS_H
#define NEA_FILE_STRINGS_H

#include <proto/dos.h>
#include <proto/exec.h>
#include <exec/memory.h>
#include <exec/types.h>

#include <nea/types.h>
#include <nea/memory.h>

typedef struct NEAFString {
  NEAString value;
  NEAUInteger length;
  NEABoolean allocated;
} NEAFString;

/**
 * SetFString is the most commonly used function. It allows you to pass in 
 * a char or AmigaOS STRPTR value and receive a populated, and/or allocated 
 * NEAFString structure to use elsewhere. * If NULL is passed for the 
 * NEAFString parameter, * a new instance is allocated via AllocVec and 
 * using NEA_MEMF_FLAGS flags. The length and allocated fields of the 
 * NEAFString will be populated automatically based on parameters.
 *
 * If you wish to allocate memory and duplicate rather than assign the pointer 
 * to the internal string, use DupFString() instead.
 *
 * @param string a NEAString/STRPTR to wrap an instance of NEAFString around. 
 * @param fstring an allocated or stack created NEAFString. If this is NULL, 
 * a new instance will be allocated using AllocVec
 * @return either the internally allocated NEAFString or the one passed in as 
 * the second parameter. NULL will be returned if there was an error 
 * allocating memory
 */
NEAFString * __asm __saveds SetFString(
  register __a0 NEAString string, 
  register __a1 NEAFString *fstring
);

/**
 * This allocates memory for a new instance of NEAFString using AllocVec using 
 * NEA_MEMF_FLAGS. If dupStringToo is TRUE, then memory is also allocated for 
 * the char * inside and the flag allocated is set to TRUE. 
 *
 * @param fstring an instance of NEAFString that is to be duplicated
 * @param dupStringToo a boolean value denoting whether or not to allocate 
 * memory and copy rather than assign the pointer of the underlying character 
 * array in the supplied NEAFString
 * @return NULL if there was an error, or the address of a NEAFString with 
 * duplicated values of the supplied fstring parameter.
 */
NEAFString * __asm __saveds DupFString(
  register __a0 NEAFString *fstring,
  register __d0 NEABoolean dupStringToo
);

/**
 * This will first check to see if the allocated flag is set. If so, it will 
 * free the underlying string with the assumption that it was allocated using 
 * AllocVec. This will cause problems if the string was allocated using 
 * malloc/realloc/calloc or the like. 
 *
 * Then it will call FreeVec on the instance itself
 *
 * @param fstring the allocated NEAFString instance to free the memory for. 
 */
void __asm __saveds FreeFString(
  register __a0 NEAFString *fstring
);

/**
 * Writes a NEAFString structure out to a binary file pointed to by filePtr. 
 * This is assumed to have been opened with the AmigaOS Open() function. The 
 * total number of bytes written will be returned as per the AmigaOS Write() 
 * function. The order of writing is length and then value. Allocated has no 
 * place on the file system and is therefore ignored.
 *
 * @param filePtr a BPTR pointer to a previously opened file
 * @param fstring the NEAFString structure to write to disk
 * @return the number of bytes written to disk
 */
LONG __asm __saveds WriteFString(
  register __d0 BPTR filePtr,
  register __a0 NEAFString *fstring
);

/** 
 * Read a NEAFString structure from disk and populate it with the values 
 * previously read. If the fstring parameter is NULL, a new instance of 
 * NEAFString will be allocated using AllocVec and the NEA_MEMF_FLAGS flags. 
 * The associated allocated property of NEAFString will also be set to TRUE. 
 * This flag will be ignored, however, if the address of an instance is 
 * passed in. This goes doubly for the value address within the supplied 
 * NEAFString structure. If the struct is allocated, so will be the value 
 * property within.
 *
 * @param filePtr a pointer to a file opened with the AmigaOS Open() function. 
 * See dos/dos.h 
 * @param fstring a pointer to a NEAFString struct or NULL if you wish to have 
 * one allocated. 
 * @return either the passed in NEAFString pointer or a pointer to one 
 * allocated. Note that the number of bytes read will be can be calculated by 
 * the length of the value property plus the sizeof a single NEAInteger.
 */
NEAFString * __asm __saveds ReadFString(
  register __d0 BPTR filePtr,
  register __a0 NEAFString *fstring
);

#endif
