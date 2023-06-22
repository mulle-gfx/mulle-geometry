#ifndef ca_polar_point_h__
#define ca_polar_point_h__

#include "include.h"

#include "CAAngle.h"
#include "CGGeometry.h"

#include <math.h>

/*
// A polar point is a combination of an angle and a radius. The angle 0 is
// at 3 o clock. If you keep the radius constant and change the angle you
// are sweeping along a circle.
//
//         . -- ~~~ -- .
//     .-~           ^   ~-.
//    /             /       \
//   /             /         \
//  |             /           |
//  |            /            |  --- 0 radians (!)
//  |                         |
//   \                       /
//    \                     /
//     `-.               .-'
//         ~- . ___ . -~
//
// Notes: negative radius is the same as flipping the angle by 180° deg and
//        having a positive radius of the same value.
//        So a CAPolarPoint can already have two representations of a CGPoint
//        right there already. Furthermore you
//        can have negative  and  positive multiples of the angle, which map to
//        the same point (e.g. add 360° deg to the angle)
//        In contrast a CGPoint is unique.
//
//        Ergo: conversion back and forth is a slightly lossy operation.
//              comparing two CAPolarPoints is tricky and error prone.
*/
typedef struct CAPolarPoint
{
   CGFloat          radius;
   CAAngleRadians   angle;
} CAPolarPoint;


// apparently the iso norm for spherical is radius first, then angle
// assume this is the way to go for 2D also
static inline CAPolarPoint   CAPolarPointMake( CGFloat r, CAAngleRadians a)
{
   return( (CAPolarPoint) { .radius = r, .angle = a });
}


static inline CAPolarPoint  CAPolarPointMakeWithPoint( CGPoint p)
{
   return( CAPolarPointMake( CGPointGetPolarRadius( p), 
                             CGPointGetPolarAngle( p)));
}


static inline CGPoint  CGPointMakeWithPolarPoint( CAPolarPoint v)
{
   return( CGPointMake( cosf( v.angle) * v.radius,
                        sinf( v.angle) * v.radius));
}



//
// makes angle between 0 and 2 PI, if radius is negative, flip the vector
//
static inline CAPolarPoint  CAPolarPointStandardize( CAPolarPoint v)
{
   if( v.radius < 0.0)
   {
      v.angle  = v.angle + M_PI_f;
      v.radius = -v.radius;
   }
   v.angle = fmodf( v.angle, CA_2xPI);
   return( v);
}


//
// Attemps to "standardize" and then compare (or to convert to CGPoint and
// compare) are useless, as the FP error makes it hit and miss, mostly miss.
//
static inline int   CAPolarPointEqualToPolarPoint( CAPolarPoint p, CAPolarPoint q)
{
   return( p.radius == q.radius && p.angle == q.angle);
}


CAPolarPoint   CAPolarPointAdd1( CAPolarPoint p, CAPolarPoint q);
CAPolarPoint   CAPolarPointAdd2( CAPolarPoint p, CAPolarPoint q);

static inline CAPolarPoint   CAPolarPointAdd( CAPolarPoint p, CAPolarPoint q)
{
   return( CAPolarPointAdd2( p, q));  // benched faster
}


CAPolarPoint   CAPolarPointSubtract1( CAPolarPoint p, CAPolarPoint q);
CAPolarPoint   CAPolarPointSubtract2( CAPolarPoint p, CAPolarPoint q);

static inline CAPolarPoint   CAPolarPointSubtract( CAPolarPoint p, CAPolarPoint q)
{
   return( CAPolarPointSubtract2( p, q));  // benched faster
}


static inline CAPolarPoint  CAPolarPointAddPoint( CAPolarPoint v, CGPoint p)
{
   CGPoint  r;

   r = CGPointAdd( CGPointMakeWithPolarPoint( v), p);
   return( CAPolarPointMakeWithPoint( r));
}


static inline CAPolarPoint  CAPolarPointSubtractPoint( CAPolarPoint v, CGPoint p)
{
   CGPoint  r;

   r = CGPointSubtract( CGPointMakeWithPolarPoint( v), p);
   return( CAPolarPointMakeWithPoint( r));
}


static inline CAPolarPoint  CAPolarPointAddAngle( CAPolarPoint v, CAAngleRadians a)
{
   CAPolarPoint  r;

   r = CAPolarPointMake( v.radius, v.angle + a);
   return( r);
}


static inline CAPolarPoint  CAPolarPointSubtractAngle( CAPolarPoint v, CAAngleRadians a)
{
   CAPolarPoint  r;

   r = CAPolarPointMake( v.radius, v.angle - a);
   return( r);
}

CGFloat   CAPolarPointDistanceToPolarPoint( CAPolarPoint a, CAPolarPoint b);


static inline CGPoint   CGPointAddPolarPoint( CGPoint p, CAPolarPoint v)
{
   return( CGPointAdd( p, CGPointMakeWithPolarPoint( v)));
}



// inner radius touches the edge points:
//
// --+--
// +   +
// --+--
static inline CGRect   CGRectMakeWithCenterPointAndInnerRadius( CGPoint center, 
                                                                CGFloat radius)
{
   CGRect   r;

   r.origin = CGPointMake( center.x - radius, center.y - radius);
   r.size   = CGSizeMake( radius * 2.0, radius * 2.0);
   return( r);
}


// outer radius touches the corner points:
//
// +---+
// -   -
// +---+
//
static inline CGRect   CGRectMakeWithCenterPointAndOuterRadius( CGPoint center, 
                                                                 CGFloat radius)
{
   CGRect         r;
   CAPolarPoint   corner;
   CGPoint        point1;
   CGPoint        point2;

   corner = CAPolarPointMake( radius, CAAngleDegreesToRadians( 45.0f));
   point1 = CGPointMakeWithPolarPoint( corner);
   point2 = CGPointScale( point1, -1.0);
   r      = CGRectMakeWithPoints( CGPointAdd( center, point1), CGPointAdd( center, point2));
   return( CGRectStandardize( r));
}





#endif
