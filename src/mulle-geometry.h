//
// this wily header, should work for Objective-C and C as it doubles as
// our envelope header, but is included in C source
//
#ifndef mulle_geometry_envelope_h__
#define mulle_geometry_envelope_h__


#include "include.h"


#define MULLE_GEOMETRY_VERSION   ((0 << 20) | (0 << 8) | 1)

#include "_mulle-geometry-provide.h"

#ifdef __has_include
# if __has_include( "_mulle-geometry-versioncheck.h")
#  include "_mulle-geometry-versioncheck.h"
# endif
#endif

#endif
