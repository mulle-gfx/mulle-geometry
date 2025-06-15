#ifndef ca_polar_point_h__
#define ca_polar_point_h__

#include "include.h"

#include "CAAngle.h"
#include "CGGeometry.h"

#include <math.h>

/*
// A CAPolarPoint is a combination of a rotation (radians) and a radius.
// Radians has its origin 0 at 3 o clock, because otherwise its not
// efficient for use with the math.h function. But rotation has an origin
// at 0. Generally speaking, you should be using "CAAngle" preferably, if
// you care about the angle. Use rotation for animation, use radians for
// interfacing with "math.h" and keeping the number of rotations.
//
// If you keep the radius constant and increase the angle you are sweeping
// along a circle in clock wise (!) direction. A standard polar point sweeps
// in counter clockwise direction (!).
//
//
//         . -- ~~~ -- .
//     .-~           ^   ~-.
//    /             /       \
//   /             /         \
//  |             /           |
//  |            /            |  --- 0 radians (internal!)
//  |                         |
//   \                       /
//    \                     /
//     `-.               .-'
//         ~- . ___ . -~
//
// Notes: negative radius is the same as flipping the rotation by 180° deg and
//        having a positive radius of the same value.
//        So a CAPolarPoint can already have two representations of a CGPoint
//        right there already. Furthermore you
//        can have negative and positive multiples of radians, which
//        map to the same point (e.g. add 360° deg)
//        In contrast a CGPoint is unique.
//
//        Ergo: conversion back and forth is a slightly lossy operation.
//              comparing two CAPolarPoints is tricky and error prone.
*/

typedef struct CAPolarPoint
{
   CGFloat   radius;
   CGFloat   radians;    // not a "radiansRotation"
} CAPolarPoint;


// apparently the iso norm for spherical is radius first, then angle
// assume this is the way to go for 2D also
static inline CAPolarPoint   CAPolarPointMake( CGFloat r, CGFloat a)
{
   // infinite radius ? why not
   assert( ! isnan( r));
   assert( ! isinf( a));
   assert( ! isnan( a));

   return( (CAPolarPoint) { .radius = r, .radians = a });
}


static inline CAPolarPoint   CAPolarPointMakeWithRadiansAsAngle( CGFloat r, CGFloat a)
{
   return( CAPolarPointMake( r, a));
}

// MEMO: angle is normalized with a slight "epsilon" towards CA_2xPI
//       so do not use this for multiple conversions over time as errors are
//       bound to accumulate (even more). If you already have an angle
//       this is fine.
//       Otherwise prefer CAPolarPointMakeWithRotation.
//
static inline CAPolarPoint   CAPolarPointMakeWithAngle( CGFloat r, CAAngle a)
{
   return( CAPolarPointMakeWithRadiansAsAngle( r, a));
}

//
// rotation here is "orientation" too as CAPolarPoint sees a rotation of 0
// as "North"
//
static inline CAPolarPoint   CAPolarPointMakeWithRotation( CGFloat radius, CARotation rotation)
{
   CGFloat   radians;

   // we don't want to "angle" normalize here
   radians = rotation * CA_2xPI - M_PI_2f;
   return( CAPolarPointMakeWithRadiansAsAngle( radius, radians));
}


static inline CGFloat   CAPolarPointGetRadius( CAPolarPoint v)
{
   return( v.radius);
}


static inline CARotation   CAPolarPointGetRotation( CAPolarPoint v)
{
   return( CARadiansAsAngleToRotation( v.radians));
}


static inline CGFloat   CAPolarPointGetRadians( CAPolarPoint v)
{
   return( v.radians);
}


static inline CGFloat   CAPolarPointGetDegrees( CAPolarPoint v)
{
   return( CARadiansAsAngleToDegrees( v.radians));
}


static inline CAAngle   CAPolarPointGetAngle( CAPolarPoint v)
{
   return( CARadiansToAngle( v.radians));
}


// ##################
// CAPoint / CGPoint
// ##################
//
// makes angle between 0 and 2 PI, if radius is negative, flip the vector
//
static inline CAPolarPoint   CAPolarPointStandardize( CAPolarPoint v)
{
   if( v.radius < 0.0)
   {
      v.radians = v.radians + M_PI_f;
      v.radius  = -v.radius;
   }
   v.radians = fmodf( v.radians, CA_2xPI);
   return( v);
}


// mulle addition:
static inline CGFloat   CGPointGetPolarRadius( CGPoint p)
{
   return( sqrtf( p.x * p.x + p.y * p.y));
}


// mulle addition:
// it's not an CAAngle, because it can be negative
static inline CGFloat   CGPointGetPolarRadiansAsAngle( CGPoint p)
{
   if( CGPointEqualToPoint( p, CGPointZero))
      return( 0.0);
   return( atan2f( p.y, p.x));
}


static inline CAPolarPoint   CAPolarPointMakeWithPoint( CGPoint p)
{
   return( CAPolarPointMakeWithRadiansAsAngle( CGPointGetPolarRadius( p),
                                               CGPointGetPolarRadiansAsAngle( p)));
}


static inline CGPoint   CGPointMakeWithPolarPoint( CAPolarPoint v)
{
   return( CGPointMake( cosf( v.radians) * v.radius,
                        sinf( v.radians) * v.radius));
}



// http://paperjs.org/reference/point/#normalize
//
// Modifies the point so that it length is 1 without changing its angle
// and returns it as a new point.
//
static inline CGPoint   CGPointNormalize( CGPoint a, CGFloat radius)
{
   CGFloat   radians;

   radians = CGPointGetPolarRadiansAsAngle( a);
   return( CGPointMake( cosf( radians) * radius, sinf( radians) * radius));
}


//
// Attemps to "standardize" and then compare (or to convert to CGPoint and
// compare) are useless, as the FP error makes it hit and miss, mostly miss.
//
static inline int   CAPolarPointEqualToPolarPoint( CAPolarPoint p, CAPolarPoint q)
{
   return( p.radius == q.radius && p.radians == q.radians);
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


//
// MEMO: You can not add two polar points with different center point,
//       by separately adding angle and radius. The radius will just increase
//       and the angle will be changed.
//
static inline CAPolarPoint   CAPolarPointAddRadians( CAPolarPoint v, CGFloat radians)
{
   CAPolarPoint  r;

   r = CAPolarPointMakeWithRadiansAsAngle( v.radius, v.radians + radians);
   return( r);
}

static inline CAPolarPoint   CAPolarPointSubtractRadians( CAPolarPoint v, CGFloat radians)
{
   CAPolarPoint  r;

   r = CAPolarPointMakeWithRadiansAsAngle( v.radius, v.radians - radians);
   return( r);
}


static inline CAPolarPoint   CAPolarPointAddDegrees( CAPolarPoint v, CGFloat degrees)
{
   CAPolarPoint  r;

   r = CAPolarPointMakeWithRadiansAsAngle( v.radius, v.radians + CADegreesToRadiansAsRotation( degrees));
   return( r);
}


static inline CAPolarPoint   CAPolarPointSubtractDegrees( CAPolarPoint v, CGFloat degrees)
{
   CAPolarPoint  r;

   r = CAPolarPointMakeWithRadiansAsAngle( v.radius, v.radians - CADegreesToRadiansAsRotation( degrees));
   return( r);
}


static inline CAPolarPoint   CAPolarPointSubtractRotation( CAPolarPoint v, CARotation a)
{
   CAPolarPoint  r;

   r = CAPolarPointMakeWithRadiansAsAngle( v.radius, v.radians - CARotationToRadiansAsRotation( a));
   return( r);
}


static inline CAPolarPoint   CAPolarPointAddRadius( CAPolarPoint v, CGFloat d)
{
   CAPolarPoint  r;

   r = CAPolarPointMakeWithRadiansAsAngle( v.radius + d, v.radians);
   return( r);
}


static inline CAPolarPoint   CAPolarPointSubtractRadius( CAPolarPoint v,
                                                         CGFloat d)
{
   CAPolarPoint  r;

   r = CAPolarPointMakeWithRadiansAsAngle( v.radius - d, v.radians);
   return( r);
}


static inline CAPolarPoint   CAPolarPointRotate( CAPolarPoint v, CARotation a)
{
   CAPolarPoint  r;

   r = CAPolarPointMakeWithRadiansAsAngle( v.radius, v.radians + a * CA_2xPI);
   return( r);
}


CGFloat   CAPolarPointDistanceToPolarPoint( CAPolarPoint a, CAPolarPoint b);


static inline CGPoint   CGPointAddPolarPoint( CGPoint p, CAPolarPoint v)
{
   return( CGPointAdd( p, CGPointMakeWithPolarPoint( v)));
}


static inline CGFloat2   CAPolarPointGetFloat2( CAPolarPoint a)
{
   return( CGFloat2Make( a.radius, a.radians));
}


static inline CAPolarPoint   CAPolarPointMakeWithVectorAndRadius( CGVector vector, CGFloat radius)
{
   CAPolarPoint   point;

   point.radius  = radius;
   point.radians = atan2f( vector.dy, vector.dx);
   return( point);
}
#endif
