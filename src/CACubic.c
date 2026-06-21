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
   return( 1.0 - cos( (t * CA_TWEEN_PI) / 2.0));
}


CGFloat   CATweenEaseOutSine( struct CACubic *q, CGFloat t)
{
   MULLE_C_UNUSED( q);
   return( sin( (t * CA_TWEEN_PI) / 2.0));
}


CGFloat   CATweenEaseInOutSine( struct CACubic *q, CGFloat t)
{
   MULLE_C_UNUSED( q);
   return( -0.5 * (cos( CA_TWEEN_PI * t) - 1.0));
}


CGFloat   CATweenEaseInExponential( struct CACubic *q, CGFloat t)
{
   MULLE_C_UNUSED( q);
   if( t == 0.0)
      return( 0.0);
   return( pow( 2.0, 10.0 * t - 10.0));
}


CGFloat   CATweenEaseOutExponential( struct CACubic *q, CGFloat t)
{
   MULLE_C_UNUSED( q);
   if( t == 1.0)
      return( 1.0);
   return( 1.0 - pow( 2.0, -10.0 * t));
}


CGFloat   CATweenEaseInOutExponential( struct CACubic *q, CGFloat t)
{
   MULLE_C_UNUSED( q);
   if( t == 0.0)
      return( 0.0);
   if( t == 1.0)
      return( 1.0);
   if( t < 0.5)
      return( pow( 2.0, 20.0 * t - 10.0) / 2.0);
   return( (2.0 - pow( 2.0, -20.0 * t + 10.0)) / 2.0);
}


CGFloat   CATweenEaseInCircular( struct CACubic *q, CGFloat t)
{
   MULLE_C_UNUSED( q);
   return( 1.0 - sqrt( 1.0 - t * t));
}


CGFloat   CATweenEaseOutCircular( struct CACubic *q, CGFloat t)
{
   CGFloat   t_1;

   MULLE_C_UNUSED( q);
   t_1 = t - 1.0;
   return( sqrt( 1.0 - t_1 * t_1));
}


CGFloat   CATweenEaseInOutCircular( struct CACubic *q, CGFloat t)
{
   MULLE_C_UNUSED( q);
   if( t < 0.5)
      return( (1.0 - sqrt( 1.0 - 4.0 * t * t)) / 2.0);
   return( (sqrt( 1.0 - pow( -2.0 * t + 2.0, 2.0)) + 1.0) / 2.0);
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
      return( (pow( 2.0 * t, 2.0) * (((c2 + 1.0) * 2.0 * t) - c2)) / 2.0);
   return( (pow( 2.0 * t - 2.0, 2.0) * (((c2 + 1.0) * (2.0 * t - 2.0)) + c2) + 2.0) / 2.0);
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
   return( -pow( 2.0, 10.0 * t - 10.0) * sin( (10.0 * t - 10.75) * c4));
}


CGFloat   CATweenEaseOutElastic( struct CACubic *q, CGFloat t)
{
   CGFloat   c4;

   MULLE_C_UNUSED( q);
   if( t == 0.0 || t == 1.0)
      return( t);

   c4 = (2.0 * CA_TWEEN_PI) / 3.0;
   return( pow( 2.0, -10.0 * t) * sin( (10.0 * t - 0.75) * c4) + 1.0);
}


CGFloat   CATweenEaseInOutElastic( struct CACubic *q, CGFloat t)
{
   CGFloat   c5;

   MULLE_C_UNUSED( q);
   if( t == 0.0 || t == 1.0)
      return( t);

   c5 = (2.0 * CA_TWEEN_PI) / 4.5;
   if( t < 0.5)
      return( -(pow( 2.0, 20.0 * t - 10.0) * sin( (20.0 * t - 11.125) * c5)) / 2.0);
   return( (pow( 2.0, -20.0 * t + 10.0) * sin( (20.0 * t - 11.125) * c5)) / 2.0 + 1.0);
}
