//
// Copyright (c) 2023 Nat!, Mulle kybernetiK
//
#ifndef ca_circle_h__
#define ca_circle_h__

#include "include.h"

#include "CGBase.h"
#include "CGGeometry.h"
#include "CAPolarPoint.h"

#include <assert.h>
#include <math.h>



#define CA_EPSILON   0.00001

//
// A CACircle is somewhat like a polar point, with a cartesian center.
//
typedef struct
{
   CGPoint   center;
   CGFloat   radius;
} CACircle;


static inline CACircle   CACircleMake( CGPoint p, CGFloat radius)
{
   assert( radius >= 0.0f);

   return( (CACircle) { p, radius });
}


static inline CACircle   CACircleExtrude( CACircle circle, CGFloat amount)
{
   CGFloat   newRadius;

   newRadius = circle.radius + amount;
   if( newRadius < 0.0f)
      newRadius = 0.0f;
   return( CACircleMake( circle.center, newRadius));
}


static inline CACircle   CACircleInset( CACircle circle, CGFloat amount)
{
   return( CACircleExtrude( circle, -amount));
}



// if its on the circle its ok
static inline int   CACircleContainsPoint( CACircle circle, CGPoint point)
{
   CGPoint   p;
   CGFloat   r;

   p = CGPointSubtract( point, circle.center);
   r = CGPointGetPolarRadius( p);
   return( r <= circle.radius + CA_EPSILON);
}


// if its on the circle it doesnt count
static inline int   CGPointIsInsideCircle( CGPoint point, CACircle circle)
{
   CGPoint   p;
   CGFloat   r;

   p = CGPointSubtract( point, circle.center);
   r = CGPointGetPolarRadius( p);
   return( r  < circle.radius);
}


static inline int   CGPointIsOnCircle( CGPoint point, CACircle circle)
{
   CGPoint   p;
   CGFloat   r;

   p = CGPointSubtract( point, circle.center);
   r = CGPointGetPolarRadius( p);
   return( r - CA_EPSILON >= circle.radius && r + CA_EPSILON < circle.radius);
}



static inline CGPoint   CACirclePointAtAngle( CACircle circle, CAAngle a)
{
   CGFloat         radians;
   CAPolarPoint    polar;
   CGPoint         point;

   radians = CAAngleToRadians( a);
   polar   = CAPolarPointMakeWithAngle( circle.radius, radians);
   point   = CGPointMakeWithPolarPoint( polar);
   point   = CGPointAdd( circle.center, point);
   return( point);
}


static inline CGFloat   CARadiusDistanceBetweenAngles( CGFloat radius,
                                                       CAAngle a,
                                                       CAAngle b)
{
   CGFloat  radians;

   radians = CAAngleToRadians( b) - CAAngleToRadians( a);
   if( radians < 0.0)
      radians += CA_2xPI;
   return( radius * radians);
}


static inline CGFloat   CACircleDistanceBetweenAngles( CACircle circle,
                                                       CAAngle a,
                                                       CAAngle b)
{
   return( CARadiusDistanceBetweenAngles( circle.radius, a, b));
}


CGPoint   CACirclePointAtDistance( CACircle circle, CAAngle startAngle, CGFloat distance);


static inline CGPoint   CACircleGetCenterPoint( CACircle circle)
{
   return( circle.center);
}



static inline CGFloat   CACircleGetRadius( CACircle circle)
{
   return( circle.radius);
}


// This is also the bounding box.
// This is the rect that covers the complete area of the circle and more.
//
// outer radius touches the corner points:
//
// +---+
// -   -
// +---+
//
static inline CGRect   CACircleGetOuterRect( CACircle circle)
{
   CGRect   r;

   r.origin = CGPointMake( circle.center.x - circle.radius, circle.center.y - circle.radius);
   r.size   = CGSizeMake( circle.radius * 2.0, circle.radius * 2.0);
   return( r);
}



// This is the rect that covers the max area of the circle, but none
// that is not covered by it.
//
// inner radius touches the edge points:
//
// --+--
// +   +
// --+--
static inline CGRect   CACircleGetInnerRect( CACircle circle)
{
   CGRect         r;
   CAPolarPoint   corner;
   CGPoint        point1;
   CGPoint        point2;

   corner = CAPolarPointMakeWithAngle( circle.radius, CAAngleSouthEast);
   point1 = CGPointMakeWithPolarPoint( corner);
   point2 = CGPointScale( point1, -1.0);
   r      = CGRectMakeWithPoints( CGPointAdd( circle.center, point1),
                                  CGPointAdd( circle.center, point2));
   return( CGRectStandardize( r));
}


static inline CACircle   CGRectGetInnerCircle( CGRect rect)
{
   return( CACircleMake( CGRectGetCenterPoint( rect),
                         CGRectGetInnerRadius( rect)));
}


static inline CACircle   CGRectGetOuterCircle( CGRect rect)
{
   return( CACircleMake( CGRectGetCenterPoint( rect),
                         CGRectGetOuterRadius( rect)));
}


//
// Returns the number of intersections of circle by the infinite line,
// specified by "start" and "end" and the "intersection", the contents
// of intersection that are not used, remain untouched
//
// 0: none
// 1: the tangent point
// 2: two intersection points
//
unsigned int  CACircleIntersectionsWithCircle( CACircle c0,
                                               CACircle c1,
                                               CGPoint intersection[ 2]);

unsigned int   CACircleIntersectionsWithLine( CACircle circle,
                                              CGPoint start,
                                              CGPoint end,
                                              CGPoint intersection[ 2]);
//
// "start" and "end" points must be inside the circle for this function to work
// The intersection point that is closest to "near" will be returned
//
CGPoint  CACircleNearestIntersectionPoint( CACircle circle,
                                           CGPoint start,
                                           CGPoint end,
                                           CGPoint near);

//
// look for a point that is at straight line distance, from the circle point
// defined by angle.
// if distance is negative, get counter clockwise point
CGPoint   CACirclePointAtDistance( CACircle circle,
                                   CAAngle startAngle,
                                   CGFloat distance);

#endif

