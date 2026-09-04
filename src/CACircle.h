//
//  CACircle.h
//  mulle-geometry
//
//  Copyright (c) 2023 Nat! - Mulle kybernetiK.
//  All rights reserved.
//
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//  Redistributions of source code must retain the above copyright notice, this
//  list of conditions and the following disclaimer.
//
//  Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation
//  and/or other materials provided with the distribution.
//
//  Neither the name of Mulle kybernetiK nor the names of its contributors
//  may be used to endorse or promote products derived from this software
//  without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
//  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
//  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//  POSSIBILITY OF SUCH DAMAGE.
//
#ifndef ca_circle_h__
#define ca_circle_h__

#include "include.h"

#include "CGBase.h"
#include "CGGeometry.h"
#include "CAPolarPoint.h"

#include <assert.h>
#include <math.h>


//
// A CACircle is somewhat like a polar point, with a cartesian center.
// the radius is always positive or zero.
//
typedef struct
{
   CGPoint   center;
   CGFloat   radius;
} CACircle;



static inline CACircle CACircleMake( CGFloat x, CGFloat y, CGFloat radius)
{
   assert( radius >= 0.0f);
   return( (CACircle)
   {
      .center = (CGPoint)
      {
         .x = x,
         .y = y
      },
      .radius = fabsf( radius)
   });
}


static inline CACircle CACircleMakeWithCenterAndRadius( CGPoint p, CGFloat radius)
{
   return( (CACircle) { .center = p, .radius = fabsf( radius) });
}


// specify center and a point on the circumference
static inline CACircle  CACircleMakeWithCenterAndPerimeter( CGPoint center, CGPoint perimeterPoint)
{
   CGFloat  dx;
   CGFloat  dy;
   CGFloat  radius;

   dx     = perimeterPoint.x - center.x;
   dy     = perimeterPoint.y - center.y;
   radius = sqrtf( dx * dx + dy * dy);

   return( CACircleMakeWithCenterAndRadius( center, radius));
}


static inline CACircle   CACircleMakeNull( void)
{
   return( (CACircle) { .center.x = NAN });
}


static inline int   CACircleIsNull( CACircle circle)
{
   return( isnan( circle.center.x));
}


static inline CACircle   CACircleOffset( CACircle circle, CGFloat x, CGFloat y)
{
   return( CACircleMakeWithCenterAndRadius( CGPointOffset( circle.center, x, y), circle.radius));
}


static inline CACircle   CACircleExtrude( CACircle circle, CGFloat amount)
{
   CGFloat   newRadius;

   newRadius = circle.radius + amount;
   if( newRadius < 0.0f)
      newRadius = 0.0f;
   return( CACircleMakeWithCenterAndRadius( circle.center, newRadius));
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
   return( r <= circle.radius);
}


// if its on the circle it doesnt count
static inline int   CGPointIsInsideCircle( CGPoint point, CACircle circle)
{
   CGPoint   p;
   CGFloat   r;

   p = CGPointSubtract( point, circle.center);
   r = CGPointGetPolarRadius( p);
   return( r < circle.radius);
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

// MEMO: same as
//  return CGPointMake( circle.center.x + circle.radius * cosf(angle),
//                      circle.center.y + circle.radius * sinf(angle));
}


static inline CAAngle   CACircleAngleAtPoint( CACircle circle, CGPoint point)
{
   CAPolarPoint polar;
   CGPoint      relativePoint;

   relativePoint = CGPointSubtract( point, circle.center);
   polar         = CAPolarPointMakeWithPoint( relativePoint);

   return( CAPolarPointGetAngle( polar));
}


// Updated function with direction parameter
static inline CGFloat  CARadiusDistanceBetweenAngles( CGFloat radius,
                                                      CAAngle a,
                                                      CAAngle b,
                                                      CARotationDirection direction)
{
   CGFloat   radians;
   CGFloat   distance;

   // NOTE: angle sweeps in clockwise direction (see CAAngle.h), so
   //       going clockwise from a to b means radians increases, while
   //       going counter clockwise means radians decreases. This must
   //       stay in sync with CARadiusAngleByAddingDistance below, which
   //       treats CounterClockWise as decreasing the angle.
   radians = CAAngleToRadians( b) - CAAngleToRadians( a);

   if( direction == CARotationDirectionClockWise)
   {
       if( radians < 0.0)
         radians += CA_2xPI;
   }
   else
   {
       if( radians > 0.0)
           radians -= CA_2xPI;
       radians = -radians; // Make positive for distance calculation
   }

   distance = radius * radians;

   if( distance < CA_EPSILON)
       return( 0.0f);

   return( distance);
}


// Updated function with direction parameter
static inline CGFloat   CACircleDistanceBetweenAngles( CACircle circle,
                                                       CAAngle a,
                                                       CAAngle b,
                                                       CARotationDirection direction)
{
   return( CARadiusDistanceBetweenAngles( circle.radius, a, b, direction));
}



static inline CAAngle   CARadiusAngleByAddingDistance( CGFloat radius,
                                                       CAAngle startAngle,
                                                       CGFloat distance,
                                                       CARotationDirection direction)
{
   CGFloat deltaRadians;
   CGFloat startRadians;
   CGFloat endRadians;
   CGFloat resultingDistance;

   if( radius <= 0.0f)
       return( startAngle);

   deltaRadians = distance / radius;

   if( direction == CARotationDirectionCounterClockWise)
       deltaRadians = -deltaRadians;

   startRadians = CAAngleToRadians( startAngle);
   endRadians   = startRadians + deltaRadians;

   // Normalize to [0, 2π)
   endRadians = fmodf( endRadians, CA_2xPI );
   if( endRadians < 0.0f )
       endRadians += CA_2xPI;

   resultingDistance = radius * fabsf( deltaRadians);
   if( resultingDistance < CA_EPSILON)
       return( startAngle);

   return( CARadiansToAngle( endRadians));
}


static inline CAAngle   CACircleAngleByAddingDistance( CACircle circle,
                                                       CAAngle startAngle,
                                                       CGFloat distance,
                                                       CARotationDirection direction)
{
   return( CARadiusAngleByAddingDistance( circle.radius,
                                          startAngle,
                                          distance,
                                          direction));
}


static inline CGFloat   CACircleDistanceToRotation( CACircle circle,
                                                    CARotation rotation)
{
    CGFloat   distance;

    distance = circle.radius * rotation;
    if( fabsf( distance) < CA_EPSILON)
        return( 0.0f);

    return( distance);
}



MULLE__GEOMETRY_GLOBAL
CGPoint   CACirclePointAtDistance( CACircle circle,
                                   CAAngle startAngle,
                                   CGFloat distance);


static inline CGPoint   CACircleGetCenter( CACircle circle)
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
   point2 = CGPointScale( point1, -1.0f);
   r      = CGRectMakeWithPoints( CGPointAdd( circle.center, point1),
                                  CGPointAdd( circle.center, point2));
   return( CGRectStandardize( r));
}


static inline CACircle   CGRectGetInnerCircle( CGRect rect)
{
   return( CACircleMakeWithCenterAndRadius( CGRectGetCenterPoint( rect),
                         CGRectGetInnerRadius( rect)));
}


static inline CACircle   CGRectGetOuterCircle( CGRect rect)
{
   return( CACircleMakeWithCenterAndRadius( CGRectGetCenterPoint( rect),
                         CGRectGetOuterRadius( rect)));
}


static inline CGRect   CGRectEncloseCircle( CGRect r, CACircle c)
{
   return( CGRectUnion( r, CACircleGetOuterRect( c)));
}


MULLE__GEOMETRY_GLOBAL
CGRect   CACircleOuterRectOfSector( CACircle circle, 
                                    CAAngle startAngle, 
                                    CAAngle endAngle); 

//
// Returns the number of intersections of circle by the infinite line,
// specified by "start" and "end" and the "intersection", the contents
// of intersection that are not used, remain untouched
//
// 0: none
// 1: the tangent point
// 2: two intersection points
//
MULLE__GEOMETRY_GLOBAL
unsigned int  CACircleIntersectionsWithCircle( CACircle c0,
                                               CACircle c1,
                                               CGPoint intersection[ 2]);

MULLE__GEOMETRY_GLOBAL
unsigned int   CACircleIntersectionsWithLine( CACircle circle,
                                              CGPoint start,
                                              CGPoint end,
                                              CGPoint intersection[ 2]);
//
// "start" and "end" points must be inside the circle for this function to work
// The intersection point that is closest to "near" will be returned
//
MULLE__GEOMETRY_GLOBAL
CGPoint  CACircleNearestIntersectionPoint( CACircle circle,
                                           CGPoint start,
                                           CGPoint end,
                                           CGPoint near);

//
// look for a point that is at straight line distance, from the circle point
// defined by angle.
// if distance is negative, get counter clockwise point
MULLE__GEOMETRY_GLOBAL
CGPoint   CACirclePointAtDistance( CACircle circle,
                                   CAAngle startAngle,
                                   CGFloat distance);



static inline CGFloat3   CACircleGetFloat3( CACircle c)
{
   return( CGFloat3Make( c.center.x, c.center.y, c.radius));;
}


static inline CGFloat   CACircleDistanceToCircle( CACircle a, CACircle b)
{
   return( CGPointDistanceToPoint( a.center, b.center));
}


static inline int   CACircleIntersectsCircle( CACircle a, CACircle b)
{
   return( CACircleDistanceToCircle( a, b) < a.radius + b.radius);
}


static inline CGFloat   CACircleOverlapWithCircle( CACircle a, CACircle b)
{
   CGFloat   dist;

   dist = CACircleDistanceToCircle( a, b);
   if( dist >= a.radius + b.radius)
      return( 0.0);
   return( a.radius + b.radius - dist);
}


//
// Returns the normal vector (unit length) pointing from a's center to b's.
// If centers coincide, returns (1,0).
//
static inline CGVector   CACircleCollisionNormal( CACircle a, CACircle b)
{
   CGPoint   diff;
   CGFloat   dist;

   diff = CGPointSubtract( b.center, a.center);
   dist = CGPointGetPolarRadius( diff);
   if( dist < CG_LENGTH_EPSILON)
      return( CGVectorMake( 1.0, 0.0));
   return( CGVectorMake( diff.x / dist, diff.y / dist));
}

#endif

