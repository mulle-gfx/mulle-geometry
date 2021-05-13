//
// Copyright (c) 2021 Nat!, Mulle kybernetiK
//
#ifndef mulle_layout_partition_h__
#define mulle_layout_partition_h__

#include "CGGeometry.h"
#include <stdint.h>


void   MulleRectPartitionVertically( CGRect frame, uintptr_t n, CGRect *rects, CGFloat space);
void   MulleRectPartitionHorizontally( CGRect frame, uintptr_t n, CGRect *rects, CGFloat space);

unsigned int   MulleRectSubdivideByRect( CGRect rect, CGRect other, CGRect output[ 4]);



#endif
