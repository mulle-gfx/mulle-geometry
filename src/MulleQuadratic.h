#ifndef MULLE_QUADRATIC_H__
#define MULLE_QUADRATIC_H__


#include "include.h"

#include "CGBase.h"
#include <assert.h>



typedef struct MulleQuadratic
{
   CGFloat   value[ 4];
} MulleQuadratic;


static inline void   MulleQuadraticInit( MulleQuadratic *q,
                                         CGFloat p0,
                                         CGFloat p1,
                                         CGFloat p2,
                                         CGFloat p3)
{
   assert( p0 >= 0.0 && p0 <= 1.0);
   assert( p1 >= 0.0 && p1 <= 1.0);
   assert( p2 >= 0.0 && p2 <= 1.0);
   assert( p3 >= 0.0 && p3 <= 1.0);

   q->value[ 0] = p0;
   q->value[ 1] = p1;
   q->value[ 2] = p2;
   q->value[ 3] = p3;
}


static inline CGFloat   MulleQuadraticGetValueForNormalizedDistance( MulleQuadratic *b,
                                                                     CGFloat t)
{
   CGFloat   value;

   assert( t >= 0.0 && t <= 1.0);

   value = (1 - t) * (1 - t) * (1 - t) * b->value[0] +
       3 * (1 - t) * (1 - t) * t * b->value[1] +
       3 * (1 - t) * t * t * b->value[2] +
       t * t * t * b->value[3];
   return( value);
}


typedef struct MulleQuadraticBezier
{
   MulleQuadratic   x;
   MulleQuadratic   y;
} MulleQuadraticBezier;  // not CGPoint for supposed improved CPU caching


static inline void   MulleQuadraticBezierInit( MulleQuadraticBezier *b,
                                               CGPoint p0,
                                               CGPoint p1,
                                               CGPoint p2,
                                               CGPoint p3)
{
   MulleQuadraticInit( &b->x, p0.x, p1.x, p2.x, p3.x);
   MulleQuadraticInit( &b->y, p0.y, p1.y, p2.y, p3.y);
}

CGPoint   MulleQuadraticBezierGetPointForNormalizedDistance( MulleQuadraticBezier *b,
                                                             CGFloat t);


#endif
