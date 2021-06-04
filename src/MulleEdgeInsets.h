//
// Copyright (c) 2020 Nat!, Mulle kybernetiK
//
#ifndef mulle_edge_insets_h__
#define mulle_edge_insets_h__

#include "include.h"

#include "CGGeometry.h"
#include <string.h>


//
// this field order actually produces slightly better code for
// MulleEdgeInsetsInsetRect (gcc 11. x86_64 -O3)
//
typedef struct MulleEdgeInsets
{
   CGFloat   left;
	CGFloat   top;
   CGFloat   right;
	CGFloat   bottom;
} MulleEdgeInsets;


//
// TLBR (counter clockwise) isn't the worst order imaginable, that would be IMO
// RTBL but it's a second or third. We use it because that's how Apple did it.
// Probably imagining this as a top/left and a bottom/right point, but it makes
// little sense in insets.
//
static inline MulleEdgeInsets   MulleEdgeInsetsMake( CGFloat top,
                                                     CGFloat left,
                                                     CGFloat bottom,
                                                     CGFloat right)
{
	return( (MulleEdgeInsets){ .top = top,
                              .left = left,
                              .bottom = bottom,
                              .right = right });
}


static inline int   MulleEdgeInsetsEqualToEdgeInsets( MulleEdgeInsets insets1,
                                                      MulleEdgeInsets insets2)
{
	return( ! memcmp( &insets1, &insets2, sizeof( MulleEdgeInsets)));
}


static inline CGRect   MulleEdgeInsetsInsetRect( MulleEdgeInsets insets,
                                                 CGRect rect)
{
	rect.origin.x    += insets.left;
	rect.origin.y    += insets.top;
	rect.size.width  -= insets.right + insets.left;
	rect.size.height -= insets.top + insets.bottom;
	return( rect);
}


static inline CGRect   MulleEdgeInsetsExtrudeRect( MulleEdgeInsets insets,
                                                   CGRect rect)
{
	rect.origin.x    -= insets.left;
	rect.origin.y    -= insets.top;
	rect.size.width  += insets.right + insets.left;
	rect.size.height += insets.top + insets.bottom;
	return( rect);
}


#define MulleEdgeInsetsZero      MulleEdgeInsetsMake( 0.0f, 0.0f, 0.0f, 0.0f)

#endif
