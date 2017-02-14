#ifndef NEA_UTIL_VERSION_H
#define NEA_UTIL_VERSION_H

#include <nea/types.h>
#include <nea/memory.h>

typedef struct SemVer {
  NEAUWord major;
  NEAUWord minor;
  NEAUWord patch;
} SemVer;

/**
 * Sets the values on a SemVer struct. If the supplied SemVer instance is
 * NULL, then a new one will be allocated using AllocVec. 
 *
 * @method SetSemVer
 *
 * @param major the major version number 
 * @param minor the minor version number
 * @param patch the patch version number
 * @param semver a SemVer struct instance to populate or NULL if a new one
 * should be allocated and returned in stead
 *
 * @return either the supplied or allocated SemVer struct with the version 
 * numbers prepopulated.
 */
SemVer * __asm __saveds SetSemVer(
 register __d0 NEAUWord major,
 register __d1 NEAUWord minor,
 register __d2 NEAUWord patch,
 register __a0 SemVer *semver
);

/**
 * Compares two SemVer structures. If the left is greater in version then
 * the result of the comparison will yield -1. If the values are the same
 * then 0 will be returned. Finally if the right version is greater, 1 
 * will be returned.
 *
 * @method CompareVersion
 *
 * @param left the first SemVer structure to compare
 * @param right the second SemVer structure to compare
 *
 * @return -1 if the left is greater, 0 if they are equal and 1 if the right
 * SemVer structure is greater in number.
 */
NEAWord __asm __saveds CompareVersion(
  register __a0 SemVer *left,
  register __a1 SemVer *right
);

/**
 * Returns a string representation of the SemVer structure. SemVer strings
 * are seen as 1.0.0, for example, representing major 1, minor 0 and patch 0.
 * Patch updates are insignificant are usually reserved for bug fixes. Minor 
 * updates are usually reserved for non-breaking changes and minor feature
 * adds or improvements. Major updates are typically allowed to break backwards
 * compatibility and are for refactors and sweeping changes to the ABI. 
 *
 * @method SemVerToString
 *
 * @param semver the SemVer structure to conver to a string
 * @param string the string to copy the results into. If NULL, a new string
 * will be allocated using the AllocVec method. 
 *
 * @return a string in the form of "major.minor.patch" where major, minor and
 * patch are unsigned word integers. 
 */
NEAString __asm __saveds SemVerToString(
  register __a0 SemVer *semver,
  register __a1 NEAString string
);

#endif