//
// Copyright (c) 2021 Nat!, Mulle kybernetiK
//
#include "MulleLayout.h"

#include "include-private.h"




CGRect   MulleLayoutAddToRow( struct MulleLayout *p,
                              CGSize size,
                              MulleEdgeInsets margins)
{
   CGRect    rect;
   CGFloat   margin;

   rect.size   = size;

   margins.top = margins.top > p->margins.top
                     ? margins.top
                     : p->margins.top;

   rect.origin.y = CGRectGetMinY( p->rect) + margins.top;
   if( CGRectGetMaxY( rect) > CGRectGetMaxY( p->originalRect))
      return( CGRectZero);

   margins.left = margins.left > p->margins.right
                     ? margins.left
                     : p->margins.right;
   rect.origin.x = CGRectGetMaxX( p->rect) + margins.left;

   //
   // check that we don't extend over space, we only check against original
   // margins here, so that we can use the full "usable" width
   // The right margin of the rect is ignored.... hmmmm
   //
   if( CGRectGetMaxX( rect) > CGRectGetMaxX( p->originalRect) - p->originalMargins.right)
      return( CGRectZero);

   // left margin is now "fixed" and can be subtracted
   p->margins.right = p->originalMargins.right > margins.right
                         ? p->originalMargins.right
                         : margins.right;

   // we keep track of the maximum Y extents of the margins,
   // NOT the largest margin per se
   if( CGRectGetMaxY( rect) + margins.bottom > CGRectGetMaxY( p->rect) + p->margins.bottom)
      p->margins.bottom = margins.bottom;

   p->rect = CGRectUnion( p->rect, rect);
   return( rect);
}


CGRect   MulleLayoutAddToRowWithOverflow( struct MulleLayout *p,
                                          CGSize size,
                                          MulleEdgeInsets margins)
{
   CGRect   rect;

   rect = MulleLayoutAddToRow( p, size, margins);
   if( rect.size.width != 0.0)
      return( rect);

   MulleLayoutNewRow( p);
   rect = MulleLayoutAddToRow( p, size, margins);
   return( rect);
}

