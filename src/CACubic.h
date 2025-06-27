#ifndef ca_cubic_h__
#define ca_cubic_h__


#include "include.h"

#include <assert.h>

#include "CGGeometry.h"

//
// A cubic Bezier curve. You need this for EaseIn/EaseOut
//
typedef struct CACubic
{
   CGFloat   value[ 4];
} CACubic;


static inline CACubic
   CACubicMake( CGFloat p0, CGFloat p1, CGFloat p2, CGFloat p3)
{
   return( (struct CACubic) {
                              .value[ 0] = p0,
                              .value[ 1] = p1,
                              .value[ 2] = p2,
                              .value[ 3] = p3
                            });
}


static inline void   CACubicInit( CACubic *q,
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


#define CACubicLinear    ((struct CACubic) { 0.0f, 0.333f, 0.666f, 1.0f })
#define CACubicEaseInOut ((struct CACubic) { 0.0f, 0.025f, 0.975f, 1.0f })
#define CACubicEaseOut   ((struct CACubic) { 0.0f, 0.9f, 0.9f, 1.0f })
#define CACubicEaseIn    ((struct CACubic) { 0.0f, 0.0f, 0.1f, 1.0f })


//
// possibly too large for inlining ca. 100 bytes!
//
static inline CGFloat   _CACubicGetValue( CACubic b, CGFloat t)
{
   CGFloat   value;

   assert( t >= 0.0 && t <= 1.0);

   value = (1 - t) * (1 - t) * (1 - t) * b.value[0] +
       3 * (1 - t) * (1 - t) * t * b.value[1] +
       3 * (1 - t) * t * t * b.value[2] +
       t * t * t * b.value[3];
   return( value);
}

static inline CGFloat   CACubicGetValue( CACubic *b, CGFloat t)
{
   if( ! b)
      return( 0.0);
   return( _CACubicGetValue( *b, t));
}


typedef struct CACubicBezier
{
   CACubic   x;
   CACubic   y;
} CACubicBezier;  // not CGPoint for supposed improved CPU caching


static inline CACubicBezier
   CACubicBezierMake( CGPoint p0, CGPoint p1, CGPoint p2, CGPoint p3)
{
   return( (struct CACubicBezier)
               {
                .x = CACubicMake( p0.x, p1.x, p2.x, p3.x),
                .y = CACubicMake( p0.y, p1.y, p2.y, p3.y)
               });
}



static inline void   CACubicBezierInit( CACubicBezier *b,
                                        CGPoint p0,
                                        CGPoint p1,
                                        CGPoint p2,
                                        CGPoint p3)
{
   CACubicInit( &b->x, p0.x, p1.x, p2.x, p3.x);
   CACubicInit( &b->y, p0.y, p1.y, p2.y, p3.y);
}

CGPoint   CACubicBezierGetPoint( CACubicBezier *b, CGFloat t);


typedef CGFloat   CATweenFunction( struct CACubic *q, CGFloat t);


#endif
