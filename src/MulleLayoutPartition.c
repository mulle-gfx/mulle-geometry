//
// Copyright (c) 2021 Nat!, Mulle kybernetiK
//
#include "MulleLayoutPartition.h"

#include "include-private.h"




//
// void   MulleRectSplitVertically( CGRect frame, CGRect *frame1, CGRect *frame2)
// {
//    frame1->origin      = frame.origin;
//    frame1->size.width  = frame.size.width;
//    frame1->size.height = frame.size.height / 2.0;
//
//    frame2->origin.x    = frame.origin.x;
//    frame2->origin.y    = frame.origin.y + frame1->size.height;
//    frame2->size        = frame1->size;
// }


void   MulleRectPartitionVertically( CGRect frame,
                                     uintptr_t n,
                                     CGRect *rects,
                                     CGFloat space)
{
   CGRect  *p;
   CGRect  *q;
   CGRect  *sentinel;

   assert( n);

   q              = rects;
   q->origin      = frame.origin;
   q->size.width  = frame.size.width;
   q->size.height = (frame.size.height - (n - 1) * space) / n;

   p        = q + 1;
   sentinel = &q[ n];
   while( p < sentinel)
   {
      p->origin.x = frame.origin.x;
      p->origin.y = CGRectGetMaxY( *q) + space;
      p->size     = rects->size;

      q = p;
      ++p;
   }
}

//
// void   MulleRectSplitHorizontally( CGRect frame, CGRect *frame1, CGRect *frame2)
// {
//    frame1->origin      = frame.origin;
//    frame1->size.width  = frame.size.width / 2.0;
//    frame1->size.height = frame.size.height;
//
//    frame2->origin.x    = frame.origin.x + frame1->size.width;
//    frame2->origin.y    = frame.origin.y;
//    frame2->size        = frame1->size;
// }
//

void   MulleRectPartitionHorizontally( CGRect frame,
                                       uintptr_t n,
                                       CGRect *rects,
                                       CGFloat space)
{
   CGRect  *p;
   CGRect  *q;
   CGRect  *sentinel;

   assert( n);

   q              = rects;
   q->origin      = frame.origin;
   q->size.width  = (frame.size.width - (n - 1) * space) / n;
   q->size.height = frame.size.height;

   p        = q + 1;
   sentinel = &q[ n];
   while( p < sentinel)
   {
      p->origin.x =  CGRectGetMaxX( *q) + space;
      p->origin.y = frame.origin.y;
      p->size     = rects->size;

      q = p;
      ++p;
   }
}


