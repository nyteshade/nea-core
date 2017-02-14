#include <nea/util/version.h>
#include <string.h>

/* -------------------------------------------------------------------------*/

SemVer * __asm __saveds SetSemVer(
 register __d0 NEAUWord major,
 register __d1 NEAUWord minor,
 register __d2 NEAUWord patch,
 register __a0 SemVer *semver
) {
  SemVer *result = semver;
  
  if (!result) {
    result = (SemVer*)AllocVec(sizeof(SemVer), NEA_MEMF_FLAGS);
    if (!result) {
      return NULL;
    }
  }
  
  result->major = major;
  result->minor = minor;
  result->patch = patch;
  
  return result;
}

SemVer * __asm __saveds LIBSetSemVer(
 register __d0 NEAUWord major,
 register __d1 NEAUWord minor,
 register __d2 NEAUWord patch,
 register __a0 SemVer *semver
) {
  return SetSemVer(major, minor, patch, semver);
}

/* -------------------------------------------------------------------------*/

NEAByte __asm __saveds CompareVersion(
  register __a0 SemVer *left,
  register __a1 SemVer *right
) {
  if (left->major < right->major) return 1;
  if (left->major > right->major) return -1;
  if (left->minor < right->minor) return 1;
  if (left->minor > right->minor) return -1;
  if (left->patch < right->patch) return 1;
  if (left->patch > right->patch) return -1;
  if (left->patch == right->patch) return 0;
}

NEAByte __asm __saveds LIBCompareVersion(
  register __a0 SemVer *left,
  register __a1 SemVer *right
) {
  return CompareVersion(left, right);
}

/* -------------------------------------------------------------------------*/

NEAString __asm __saveds SemVerToString(
  register __a0 SemVer *semver,
  register __a1 NEAString string
) {
  NEAString result = string;
  NEAByte size = 3;             /* two dots plus null terminator */
  
  if (!result) {
    if      (semver->major > 99) size += 3;
    else if (semver->major > 9)  size += 2;
    else                         size += 1;
    if      (semver->minor > 99) size += 3;
    else if (semver->minor > 9)  size += 2;
    else                         size += 1;
    if      (semver->patch > 99) size += 3;
    else if (semver->patch > 9)  size += 2;
    else                         size += 1;
    
    /* We have determined the number of digits, now convert to byte sizes */
    size *= sizeof(NEAByte);
    
    result = (NEAString)AllocVec(size, NEA_MEMF_FLAGS);
    if (!result) {
      return NULL;
    }
  }
  
  sprintf(result, "%d.%d.%d", semver->major, semver->minor, semver->patch);
  
  return result;
}

NEAString __asm __saveds LIBSemVerToString(
  register __a0 SemVer *semver,
  register __a1 NEAString string
) {
  return SemVerToString(semver, string);
}