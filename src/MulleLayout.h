//
// Copyright (c) 2021 Nat!, Mulle kybernetiK
//
#ifndef mulle_layout_h__
#define mulle_layout_h__

#include "CGGeometry.h"
#include "MulleEdgeInsets.h"


//
// Basically follow the definitions set in here
// https://yogalayout.com/
// except we don't do padding (which is insets used by UIView for the content)
// The margin handling is different, in that we merge margins.
// i.e. ---| |----   will be a distance of 20 and not of 30 like in Yoga
//       10   20
//

// keep this simple.. any additional layouting like centering or justifying
// should be done in a second pass

struct MulleLayout
{
   CGRect            originalRect;
   MulleEdgeInsets   originalMargins;

   CGRect            rect;      // growing rect of the current row
   MulleEdgeInsets   margins;  // growing margins of the current row

   CGFloat           maxMarginY;
};


static inline void  MulleLayoutInit( struct MulleLayout *p,
                                     CGRect rect,
                                     MulleEdgeInsets margins)
{
   // read only (in algorithm)
   p->originalRect    = rect;
   p->originalMargins = margins;

   p->rect.origin     = rect.origin;
   p->rect.size       = CGSizeMake( 0, 0);
   p->margins         = margins;
}


static inline void  _MulleLayoutDone( struct MulleLayout *p)
{
}


static inline void  MulleLayoutDone( struct MulleLayout *p)
{
}


//
// +++.......
// + +   ++ .
// +++   ++ .
// .........+
//
// ###
// # #
// ###

static inline CGSize  MulleLayoutGetRemainingRowSize( struct MulleLayout *p)
{
   CGSize   size;

   size.width  = CGRectGetMaxX( p->originalRect) - (CGRectGetMaxX( p->rect) + p->margins.right);
   size.height = CGRectGetMaxY( p->originalRect) - (CGRectGetMaxY( p->rect) + p->margins.bottom);

   return( size);
}


// Each row starts at 0,0. The origin is added to the returned rect on return.
// When a row is done, the height of the row is added to the origin.
//
// +-------------------+
// |                   |
// +-------------------+
//
static inline void   MulleLayoutNewRow( struct MulleLayout *p)
{
   p->margins.top    = p->originalMargins.top > p->margins.bottom
                         ? p->originalMargins.top
                         : p->margins.bottom;
   p->margins.bottom = p->originalMargins.bottom;
   p->margins.left   = p->originalMargins.left;
   p->margins.right  = p->originalMargins.right;

   p->rect.origin.x  = p->originalRect.origin.x;
   p->rect.origin.y += p->rect.size.height;
   p->rect.size      = CGSizeZero;
}


//
// Straightforward, except when space runs out. The margin handling is also
// a bit complicated. Size doesn't fit, if there isn't room enough for margins,
// which is also the right edge of the space. The larger margin wins.
// Returns zerosized rect, if it doesn't fit. Does not attempt to change 'y'
//
CGRect   MulleLayoutAddToRow( struct MulleLayout *p,
                               CGSize size,
                               MulleEdgeInsets margins);

CGRect   MulleLayoutAddToRowWithOverflow( struct MulleLayout *p,
                                           CGSize size,
                                           MulleEdgeInsets margins);



// TODO: more to come...


#endif
