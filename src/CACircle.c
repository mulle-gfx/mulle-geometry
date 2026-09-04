//
//  CACircle.c
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
#include "CACircle.h"


unsigned int   CACircleIntersectionsWithLine( CACircle circle,
                                              CGPoint startPoint,
                                              CGPoint endPoint,
                                              CGPoint intersection[ 2])
{
   CGFloat dx;
   CGFloat dy;
   CGFloat sx, sy;
   CGFloat a;
   CGFloat b;
   CGFloat c;
   CGFloat discriminant;
   CGFloat t1;
   CGFloat t2;

   dx = endPoint.x - startPoint.x;
   dy = endPoint.y - startPoint.y;
   a  = dx * dx + dy * dy;
   b  = 2 * (dx * (startPoint.x - circle.center.x) + dy * (startPoint.y - circle.center.y));
   sx = startPoint.x - circle.center.x;
   sy = startPoint.y - circle.center.y;
   c  = sx * sx + sy * sy - circle.radius * circle.radius;

   discriminant = b * b - 4 * a * c;

   if( discriminant <= 0.0f + CA_EPSILON)
   {
      if( discriminant < 0.0f)
         return( 0);

      t1 = -b / (2 * a);
      intersection[ 0] = CGPointMake( startPoint.x + t1 * dx, startPoint.y + t1 * dy);
      return( 1);
   }

   t1               = (-b + sqrtf( discriminant)) / (2 * a);
   t2               = (-b - sqrtf( discriminant)) / (2 * a);
   intersection[ 0] = CGPointMake( startPoint.x + t1 * dx, startPoint.y + t1 * dy);
   intersection[ 1] = CGPointMake( startPoint.x + t2 * dx, startPoint.y + t2 * dy);
   return( 2);
}


CGPoint  CACircleNearestIntersectionPoint( CACircle circle,
                                           CGPoint startPoint,
                                           CGPoint endPoint,
                                           CGPoint nearPoint)
{
   CGPoint        intersection[ 2];
   CGFloat        d0;
   CGFloat        d1;
   unsigned int   n;

   assert( CGPointIsInsideCircle( startPoint, circle));
   assert( CGPointIsInsideCircle( endPoint, circle));

   // degenerate: zero-length segment — use center to define a direction
   // but if start is also the center, all directions are equally valid: bail
   if( CGPointEqualToPoint( startPoint, endPoint))
   {
      if( CGPointEqualToPoint( startPoint, circle.center))
         return( CGPointNull);
      endPoint = circle.center;
   }

   n = CACircleIntersectionsWithLine( circle, startPoint, endPoint, intersection);
   if( n != 2)
      return( CGPointNull);

   d0 = CGPointSquaredDistanceToPoint( nearPoint, intersection[ 0]);
   d1 = CGPointSquaredDistanceToPoint( nearPoint, intersection[ 1]);
   return( d0 < d1 ? intersection[ 0] : intersection[ 1]);
}


unsigned int  CACircleIntersectionsWithCircle( CACircle c0,
                                               CACircle c1,
                                               CGPoint intersection[ 2])
{
   CGFloat dx;
   CGFloat dy;
   CGFloat d;
   CGFloat ex;
   CGFloat ey;
   CGFloat a;
   CGFloat h2;
   CGFloat h;

   dx = c1.center.x - c0.center.x;
   dy = c1.center.y - c0.center.y;
   d = sqrtf( dx * dx + dy * dy);

   // Concentric circles — infinite or zero intersections, either way: 0
   if( d < CG_LENGTH_EPSILON)
      return( 0);

   // Circles too far apart or one circle completely inside the other
   if( d > c0.radius + c1.radius || d < fabsf(c0.radius - c1.radius))
    return( 0);

   ex = dx / d;
   ey = dy / d;
   a  = (c0.radius * c0.radius - c1.radius * c1.radius + d * d) / (2 * d);
   h2 = c0.radius * c0.radius - a * a;

   // Tangent (single intersection point) — clamp rounding errors
   if( h2 <= 0.0f)
   {
      intersection[ 0] = CGPointMake( c0.center.x + a * ex,
                                      c0.center.y + a * ey);
      intersection[ 1] = intersection[ 0];
      return( 1);
   }

   h = sqrtf( h2);

   intersection[ 0] =  CGPointMake( c0.center.x + a * ex - h * ey,
                                    c0.center.y + a * ey + h * ex);
   intersection[ 1] =  CGPointMake( c0.center.x + a * ex + h * ey,
                                    c0.center.y + a * ey - h * ex);

   return( 2);
}



CGPoint   CACirclePointAtDistance( CACircle circle, 
                                   CAAngle startAngle, 
                                   CGFloat distance)
{
   CAPolarPoint   q0;
   CGPoint        p0;
   CGPoint        intersection[ 2];

//   if( distance > circle.radius * 2.0)
//      return( CGPointNull);

   q0 = CAPolarPointMakeWithAngle( circle.radius, startAngle);
   p0 = CGPointMakeWithPolarPoint( q0);
   p0 = CGPointAdd( p0, circle.center);
   if( ! CACircleIntersectionsWithCircle( circle, 
                                          CACircleMakeWithCenterAndRadius( p0, fabsf( distance)),
                                          intersection))
      return( CGPointNull);

   //
   // given that the second circle is on the first circle, the following
   // observation was made, that the first intersection is always clockwise
   //
   return( distance > 0 ? intersection[ 0] : intersection[ 1]);
}


// MEMO: AI generated and untested
CGRect   CACircleOuterRectOfSector( CACircle circle,
                                    CAAngle startAngle,
                                    CAAngle endAngle)
{
   CGRect    box;
   CGPoint   ps;
   CGPoint   pe;

   if( startAngle == endAngle)
      return( CGRectZero);

   ps  = CACirclePointAtAngle( circle, startAngle);
   pe  = CACirclePointAtAngle( circle, endAngle);
   box = CGRectMakeWithPoints( ps, pe);

   //
   // Check if cardinal directions (E, S, W, N) lie within the arc.
   // The sector sweeps clockwise from startAngle to endAngle.
   // Angles are in [0, 2π): if end < start, the arc wraps around 0.
   //
   #define ARC_CONTAINS_( cardinal) \
      (startAngle <= endAngle       \
         ? (startAngle < cardinal && cardinal < endAngle)   \
         : (cardinal > startAngle || cardinal < endAngle))

   if( ARC_CONTAINS_( CAAngleE))
      box = CGRectEnclosePoint( box, CGPointMake( circle.center.x + circle.radius, circle.center.y));
   if( ARC_CONTAINS_( CAAngleS))
      box = CGRectEnclosePoint( box, CGPointMake( circle.center.x, circle.center.y + circle.radius));
   if( ARC_CONTAINS_( CAAngleW))
      box = CGRectEnclosePoint( box, CGPointMake( circle.center.x - circle.radius, circle.center.y));
   if( ARC_CONTAINS_( CAAngleN))
      box = CGRectEnclosePoint( box, CGPointMake( circle.center.x, circle.center.y - circle.radius));

   #undef ARC_CONTAINS_

   return( box);
}
