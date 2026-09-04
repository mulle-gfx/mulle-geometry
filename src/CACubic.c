//
//  CACubic.c
//  mulle-geometry
//
//  Copyright (c) 2025 Nat! - Mulle kybernetiK.
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
#include "CACubic.h"

#include <math.h>

/*
 * Some easing formulas below are adapted from pytweening by Al Sweigart:
 *   https://github.com/asweigart/pytweening
 *
 * pytweening license (BSD-3-Clause) notice:
 * Copyright (c) 2014, Al Sweigart
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

//
// t is 0 to 1
//
//
// stolen from: https://stackoverflow.com/questions/5634460/quadratic-b%C3%A9zier-curve-calculate-points
//

CGPoint   CACubicBezierGetPoint( CACubicBezier *b, CGFloat t)
{
   CGPoint curvePoint;

   curvePoint.x = CACubicGetValue( &b->x, t);
   curvePoint.y = CACubicGetValue( &b->y, t);

   return( curvePoint);
}


//
// First derivative of a cubic bezier:
//   B'(t) = 3(1-t)²(P1-P0) + 6(1-t)t(P2-P1) + 3t²(P3-P2)
//
CGVector   CACubicBezierGetTangent( CACubicBezier *b, CGFloat t)
{
   CGFloat   u;
   CGFloat   uu;
   CGFloat   tt;
   CGFloat   ut;
   CGFloat   dx, dy;

   assert( b);

   u  = 1.0f - t;
   uu = u * u;
   tt = t * t;
   ut = u * t;

   dx = 3.0f * uu * (b->x.value[1] - b->x.value[0])
      + 6.0f * ut * (b->x.value[2] - b->x.value[1])
      + 3.0f * tt * (b->x.value[3] - b->x.value[2]);
   dy = 3.0f * uu * (b->y.value[1] - b->y.value[0])
      + 6.0f * ut * (b->y.value[2] - b->y.value[1])
      + 3.0f * tt * (b->y.value[3] - b->y.value[2]);

   return( CGVectorMake( dx, dy));
}


//
// De Casteljau split at parameter t.
//
void   CACubicBezierSplit( CACubicBezier *b, CGFloat t,
                           CACubicBezier *left,
                           CACubicBezier *right)
{
   CGFloat   p0x, p1x, p2x, p3x;
   CGFloat   p0y, p1y, p2y, p3y;
   CGFloat   q0x, q1x, q2x;
   CGFloat   q0y, q1y, q2y;
   CGFloat   r0x, r1x;
   CGFloat   r0y, r1y;
   CGFloat   sx, sy;

   assert( b);

   p0x = b->x.value[0]; p0y = b->y.value[0];
   p1x = b->x.value[1]; p1y = b->y.value[1];
   p2x = b->x.value[2]; p2y = b->y.value[2];
   p3x = b->x.value[3]; p3y = b->y.value[3];

   // first level lerps
   q0x = p0x + (p1x - p0x) * t;   q0y = p0y + (p1y - p0y) * t;
   q1x = p1x + (p2x - p1x) * t;   q1y = p1y + (p2y - p1y) * t;
   q2x = p2x + (p3x - p2x) * t;   q2y = p2y + (p3y - p2y) * t;

   // second level lerps
   r0x = q0x + (q1x - q0x) * t;   r0y = q0y + (q1y - q0y) * t;
   r1x = q1x + (q2x - q1x) * t;   r1y = q1y + (q2y - q1y) * t;

   // third level lerp — the split point
   sx  = r0x + (r1x - r0x) * t;   sy  = r0y + (r1y - r0y) * t;

   if( left)
   {
      left->x = CACubicMake( p0x, q0x, r0x, sx);
      left->y = CACubicMake( p0y, q0y, r0y, sy);
   }
   if( right)
   {
      right->x = CACubicMake( sx, r1x, q2x, p3x);
      right->y = CACubicMake( sy, r1y, q2y, p3y);
   }
}


CAPose   CACubicBezierGetPose( CACubicBezier *b, CGFloat t)
{
   CGPoint    position;
   CGVector   tangent;
   CGVector   normal;

   assert( b);

   position = CACubicBezierGetPoint( b, t);
   tangent  = CACubicBezierGetTangent( b, t);
   normal   = CGVectorNormalize( tangent);

   return( CAPoseMakeWithPositionAndNormal( position, normal));
}


#define CA_TWEEN_PI   ((CGFloat) 3.14159265358979323846)

static inline CGFloat   _CATweenEaseOutFromIn( CATweenFunction *f, CGFloat t)
{
   return( 1.0 - (*f)( NULL, 1.0 - t));
}


static inline CGFloat   _CATweenEaseInOutFromIn( CATweenFunction *f, CGFloat t)
{
   if( t < 0.5)
      return( 0.5 * (*f)( NULL, 2.0 * t));
   return( 1.0 - 0.5 * (*f)( NULL, 2.0 - 2.0 * t));
}


CGFloat   CATweenEaseInQuadratic( struct CACubic *q, CGFloat t)
{
   MULLE_C_UNUSED( q);
   return( t * t);
}


CGFloat   CATweenEaseOutQuadratic( struct CACubic *q, CGFloat t)
{
   MULLE_C_UNUSED( q);
   return( _CATweenEaseOutFromIn( CATweenEaseInQuadratic, t));
}


CGFloat   CATweenEaseInOutQuadratic( struct CACubic *q, CGFloat t)
{
   MULLE_C_UNUSED( q);
   return( _CATweenEaseInOutFromIn( CATweenEaseInQuadratic, t));
}


CGFloat   CATweenEaseInCubic( struct CACubic *q, CGFloat t)
{
   MULLE_C_UNUSED( q);
   return( t * t * t);
}


CGFloat   CATweenEaseOutCubic( struct CACubic *q, CGFloat t)
{
   MULLE_C_UNUSED( q);
   return( _CATweenEaseOutFromIn( CATweenEaseInCubic, t));
}


CGFloat   CATweenEaseInOutCubic( struct CACubic *q, CGFloat t)
{
   MULLE_C_UNUSED( q);
   return( _CATweenEaseInOutFromIn( CATweenEaseInCubic, t));
}


CGFloat   CATweenEaseInQuartic( struct CACubic *q, CGFloat t)
{
   MULLE_C_UNUSED( q);
   return( t * t * t * t);
}


CGFloat   CATweenEaseOutQuartic( struct CACubic *q, CGFloat t)
{
   MULLE_C_UNUSED( q);
   return( _CATweenEaseOutFromIn( CATweenEaseInQuartic, t));
}


CGFloat   CATweenEaseInOutQuartic( struct CACubic *q, CGFloat t)
{
   MULLE_C_UNUSED( q);
   return( _CATweenEaseInOutFromIn( CATweenEaseInQuartic, t));
}


CGFloat   CATweenEaseInQuintic( struct CACubic *q, CGFloat t)
{
   MULLE_C_UNUSED( q);
   return( t * t * t * t * t);
}


CGFloat   CATweenEaseOutQuintic( struct CACubic *q, CGFloat t)
{
   MULLE_C_UNUSED( q);
   return( _CATweenEaseOutFromIn( CATweenEaseInQuintic, t));
}


CGFloat   CATweenEaseInOutQuintic( struct CACubic *q, CGFloat t)
{
   MULLE_C_UNUSED( q);
   return( _CATweenEaseInOutFromIn( CATweenEaseInQuintic, t));
}


CGFloat   CATweenEaseInSine( struct CACubic *q, CGFloat t)
{
   MULLE_C_UNUSED( q);
   return( 1.0 - cosf( (t * CA_TWEEN_PI) / 2.0));
}


CGFloat   CATweenEaseOutSine( struct CACubic *q, CGFloat t)
{
   MULLE_C_UNUSED( q);
   return( sinf( (t * CA_TWEEN_PI) / 2.0));
}


CGFloat   CATweenEaseInOutSine( struct CACubic *q, CGFloat t)
{
   MULLE_C_UNUSED( q);
   return( -0.5 * (cosf( CA_TWEEN_PI * t) - 1.0));
}


CGFloat   CATweenEaseInExponential( struct CACubic *q, CGFloat t)
{
   MULLE_C_UNUSED( q);
   if( t == 0.0)
      return( 0.0);
   return( powf( 2.0, 10.0 * t - 10.0));
}


CGFloat   CATweenEaseOutExponential( struct CACubic *q, CGFloat t)
{
   MULLE_C_UNUSED( q);
   if( t == 1.0)
      return( 1.0);
   return( 1.0 - powf( 2.0, -10.0 * t));
}


CGFloat   CATweenEaseInOutExponential( struct CACubic *q, CGFloat t)
{
   MULLE_C_UNUSED( q);
   if( t == 0.0)
      return( 0.0);
   if( t == 1.0)
      return( 1.0);
   if( t < 0.5)
      return( powf( 2.0, 20.0 * t - 10.0) / 2.0);
   return( (2.0 - powf( 2.0, -20.0 * t + 10.0)) / 2.0);
}


CGFloat   CATweenEaseInCircular( struct CACubic *q, CGFloat t)
{
   MULLE_C_UNUSED( q);
   return( 1.0 - sqrtf( 1.0 - t * t));
}


CGFloat   CATweenEaseOutCircular( struct CACubic *q, CGFloat t)
{
   CGFloat   t_1;

   MULLE_C_UNUSED( q);
   t_1 = t - 1.0;
   return( sqrtf( 1.0 - t_1 * t_1));
}


CGFloat   CATweenEaseInOutCircular( struct CACubic *q, CGFloat t)
{
   MULLE_C_UNUSED( q);
   if( t < 0.5)
      return( (1.0 - sqrtf( 1.0 - 4.0 * t * t)) / 2.0);
   return( (sqrtf( 1.0 - powf( -2.0 * t + 2.0, 2.0)) + 1.0) / 2.0);
}


CGFloat   CATweenEaseInBack( struct CACubic *q, CGFloat t)
{
   CGFloat   c1;
   CGFloat   c3;

   MULLE_C_UNUSED( q);
   c1 = 1.70158;
   c3 = c1 + 1.0;
   return( c3 * t * t * t - c1 * t * t);
}


CGFloat   CATweenEaseOutBack( struct CACubic *q, CGFloat t)
{
   CGFloat   c1;
   CGFloat   c3;
   CGFloat   t_1;

   MULLE_C_UNUSED( q);
   c1  = 1.70158;
   c3  = c1 + 1.0;
   t_1 = t - 1.0;
   return( 1.0 + c3 * t_1 * t_1 * t_1 + c1 * t_1 * t_1);
}


CGFloat   CATweenEaseInOutBack( struct CACubic *q, CGFloat t)
{
   CGFloat   c1;
   CGFloat   c2;

   MULLE_C_UNUSED( q);
   c1 = 1.70158;
   c2 = c1 * 1.525;

   if( t < 0.5)
      return( (powf( 2.0 * t, 2.0) * (((c2 + 1.0) * 2.0 * t) - c2)) / 2.0);
   return( (powf( 2.0 * t - 2.0, 2.0) * (((c2 + 1.0) * (2.0 * t - 2.0)) + c2) + 2.0) / 2.0);
}


CGFloat   CATweenEaseOutBounce( struct CACubic *q, CGFloat t)
{
   CGFloat   n1;
   CGFloat   d1;

   MULLE_C_UNUSED( q);
   n1 = 7.5625;
   d1 = 2.75;

   if( t < (1.0 / d1))
      return( n1 * t * t);
   if( t < (2.0 / d1))
      return( n1 * (t - 1.5 / d1) * (t - 1.5 / d1) + 0.75);
   if( t < (2.5 / d1))
      return( n1 * (t - 2.25 / d1) * (t - 2.25 / d1) + 0.9375);
   return( n1 * (t - 2.625 / d1) * (t - 2.625 / d1) + 0.984375);
}


CGFloat   CATweenEaseInBounce( struct CACubic *q, CGFloat t)
{
   MULLE_C_UNUSED( q);
   return( 1.0 - CATweenEaseOutBounce( NULL, 1.0 - t));
}


CGFloat   CATweenEaseInOutBounce( struct CACubic *q, CGFloat t)
{
   MULLE_C_UNUSED( q);
   if( t < 0.5)
      return( (1.0 - CATweenEaseOutBounce( NULL, 1.0 - 2.0 * t)) / 2.0);
   return( (1.0 + CATweenEaseOutBounce( NULL, 2.0 * t - 1.0)) / 2.0);
}


CGFloat   CATweenEaseInElastic( struct CACubic *q, CGFloat t)
{
   CGFloat   c4;

   MULLE_C_UNUSED( q);
   if( t == 0.0 || t == 1.0)
      return( t);

   c4 = (2.0 * CA_TWEEN_PI) / 3.0;
   return( -powf( 2.0, 10.0 * t - 10.0) * sinf( (10.0 * t - 10.75) * c4));
}


CGFloat   CATweenEaseOutElastic( struct CACubic *q, CGFloat t)
{
   CGFloat   c4;

   MULLE_C_UNUSED( q);
   if( t == 0.0 || t == 1.0)
      return( t);

   c4 = (2.0 * CA_TWEEN_PI) / 3.0;
   return( powf( 2.0, -10.0 * t) * sinf( (10.0 * t - 0.75) * c4) + 1.0);
}


CGFloat   CATweenEaseInOutElastic( struct CACubic *q, CGFloat t)
{
   CGFloat   c5;

   MULLE_C_UNUSED( q);
   if( t == 0.0 || t == 1.0)
      return( t);

   c5 = (2.0 * CA_TWEEN_PI) / 4.5;
   if( t < 0.5)
      return( -(powf( 2.0, 20.0 * t - 10.0) * sinf( (20.0 * t - 11.125) * c5)) / 2.0);
   return( (powf( 2.0, -20.0 * t + 10.0) * sinf( (20.0 * t - 11.125) * c5)) / 2.0 + 1.0);
}
