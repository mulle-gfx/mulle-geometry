//******************************************************************************
//
// Copyright (c) Microsoft. All rights reserved.
//
// This code is licensed under the MIT License (MIT).
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
//******************************************************************************

#include "CGGeometry.h"
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>



/*
 * A rectangle with the smallest integer values for its origin and size that
 * contains the source rectangle. That is, given a rectangle with fractional
 * origin or size values, CGRectIntegral rounds the rectangle’s origin
 * downward and its size upward to the nearest whole integers, such that the
 * result contains the original rectangle. Returns a null rectangle if rect
 * is a null rectangle.
 *
 */
CGRect   CGRectIntegral( CGRect r)
{
   CGRect   rect;

   if( CGRectIsNull( r))
      return( r);

   // try to keep texture orientation intact
   rect.origin.x    = floorf( r.origin.x);
   rect.origin.y    = floorf( r.origin.y);
   rect.size.width  = r.size.width < 0.0
                      ? floorf( r.size.width)
                      : ceilf( r.size.width);
   rect.size.height = r.size.height < 0.0
                      ? floorf( r.size.height)
                      : ceilf( r.size.height);

   return( rect);
}


CGRect   CGRectIntegralFuzzy( CGRect r, CGFloat fuzz)
{
   CGRect   rect;

   if( CGRectIsNull( r))
      return( r);

   // subtract fuzz (e..g 0.01) so we have some margin for precision loss.
   rect.origin.x    = floorf( r.origin.x + fuzz);
   rect.origin.y    = floorf( r.origin.y + fuzz);
   rect.size.width  = r.size.width < 0.0
                      ? floorf( r.size.width + fuzz)
                      : ceilf( r.size.width - fuzz);
   rect.size.height = r.size.height < 0.0
                      ? floorf( r.size.height + fuzz)
                      : ceilf( r.size.height - fuzz);
   return( rect);
}



CGRect   CGRectIntersection(CGRect r1, CGRect r2)
{
   CGFloat  x1, y1, x2, y2;

   r1 = CGRectStandardize(r1);
   r2 = CGRectStandardize(r2);

   if( r1.origin.x < r2.origin.x)
   {
      if( r1.origin.x + r1.size.width < r2.origin.x)
         return( CGRectNull);

      x1 = CGFloatMinimum( r1.origin.x + r1.size.width, r2.origin.x);
      x2 = CGFloatMinimum( r1.origin.x + r1.size.width, r2.origin.x + r2.size.width);
   }
   else
   {
      if( r2.origin.x + r2.size.width < r1.origin.x)
         return( CGRectNull);

      x1 = CGFloatMinimum( r2.origin.x + r2.size.width, r1.origin.x);
      x2 = CGFloatMinimum( r2.origin.x + r2.size.width, r1.origin.x + r1.size.width);
   }

   if( r1.origin.y < r2.origin.y)
   {
      if ( r1.origin.y + r1.size.height < r2.origin.y)
         return( CGRectNull);

      y1 = CGFloatMinimum( r1.origin.y + r1.size.height, r2.origin.y);
      y2 = CGFloatMinimum( r1.origin.y + r1.size.height, r2.origin.y + r2.size.height);
   }
   else
   {
      if( r2.origin.y + r2.size.height < r1.origin.y)
         return( CGRectNull);

      y1 = CGFloatMinimum( r2.origin.y + r2.size.height, r1.origin.y);
      y2 = CGFloatMinimum( r2.origin.y + r2.size.height, r1.origin.y + r1.size.height);
   }

   return( CGRectMake( x1, y1, x2 - x1, y2 - y1));
}




CGRect CGRectUnion(CGRect r1, CGRect r2) {
    CGFloat x1, y1, x2, y2;

    if (CGRectIsInfinite(r1)) {
        return r2;
    }
    if (CGRectIsInfinite(r2)) {
        return r1;
    }

    r1 = CGRectStandardize(r1);
    r2 = CGRectStandardize(r2);

    if (r1.origin.x < r2.origin.x) {
        x1 = r1.origin.x;
    } else {
        x1 = r2.origin.x;
    }
    if (r1.origin.y < r2.origin.y) {
        y1 = r1.origin.y;
    } else {
        y1 = r2.origin.y;
    }

    if (r1.origin.x + r1.size.width > r2.origin.x + r2.size.width) {
        x2 = r1.origin.x + r1.size.width;
    } else {
        x2 = r2.origin.x + r2.size.width;
    }
    if (r1.origin.y + r1.size.height > r2.origin.y + r2.size.height) {
        y2 = r1.origin.y + r1.size.height;
    } else {
        y2 = r2.origin.y + r2.size.height;
    }

    return( CGRectMake( x1, y1, x2 - x1, y2 - y1));
}


CGRect   CGRectEnclosePoint( CGRect r, CGPoint p)
{
   CGFloat  diff;

   r = CGRectStandardize( r);
   if( CGRectIsNull( r))
      return( CGRectMake( p.x, p.y, 0, 0));

   diff = CGRectGetMinX( r) - p.x;
   if( diff >= 0.0f)
   {
      r.size.width += diff;
      r.origin.x    = p.x;
   }
   else
   {
      diff = p.x - CGRectGetMaxX( r);
      if( diff >= 0.0f)
         r.size.width += diff;
   }

   diff = CGRectGetMinY( r) - p.y;
   if( diff >= 0.0f)
   {
      r.size.height += diff;
      r.origin.y     = p.y;
   }
   else
   {
      diff = p.y - CGRectGetMaxY( r);
      if( diff >= 0.0f)
          r.size.height += diff;
   }
   return( r);
}


int   CGRectContainsPoint( CGRect rect, CGPoint point)
{
   return( point.x >= CGRectGetMinX(rect) && point.x < CGRectGetMaxX(rect)) &&
         ( point.y >= CGRectGetMinY(rect) && point.y < CGRectGetMaxY(rect));
}

/**
   CGRectMake( 2.0, 0.0, 2.0, 1.0) and CGRectMake( 0.0, 0.0, 2.0, 1.0)
   are not considered intersecting, though mathematically they are.
   This is so that "adjacent" rectangles are not considered intersecting.
   CGRectIntersection will return an intersection of 0 width or height though!
*/
int   CGRectIntersectsRect( CGRect a, CGRect b)
{
   return !((b.origin.x >= CGRectGetMaxX( a)) || (b.origin.y >= CGRectGetMaxY( a)) ||
            (a.origin.x >= CGRectGetMaxX( b)) || (a.origin.y >= CGRectGetMaxY( b)));
}



/**
*/
int   CGRectContainsRect( CGRect a, CGRect b)
{
   return( CGRectGetMinX(b) >= CGRectGetMinX(a) &&
           CGRectGetMaxX(b) <= CGRectGetMaxX(a) &&
           CGRectGetMinY(b) >= CGRectGetMinY(a) &&
           CGRectGetMaxY(b) <= CGRectGetMaxY(a));
}



//
//
// uintptr_t   CGRectIndexOfRectWithLargestIntersection( CGRect r1, uintptr_t n, CGRect *rects)
// {
//    uintptr_t    i;
//    double       largestArea;
//    double       area;
//    uintptr_t    found;
//    CGRect       intersection;
//
//    found       = (uintptr_t) INTPTR_MAX;
//    largestArea = 0.0;
//    r1          = CGRectStandardize( r1);
//
//    for( i = 0; i < n; i++)
//    {
//       intersection = CGRectIntersection( r1, rects[ i]);
//       area         = CGRectGetArea( intersection);
//       if( area > largestArea)
//       {
//          largestArea = area;
//          found       = i;
//       }
//    }
//
//    return( found);
// }
//



static CGFloat   _CGFloatNormalDistribution( void)
{
   CGFloat   u, v, q, p;
   CGFloat   r;

   do
   {
      u = (rand() / ((double)RAND_MAX)) * 2 - 1;
      v = (rand() / ((double)RAND_MAX)) * 2 - 1;
      q = u * u + v * v;
   }
   while( q >= 1 || CGFloatIsZero( q));

   // this produces values from 0 to +infinity (but infinity very rarely)
   p = sqrtf( 2 * -logf(q) / q);
   r = u * p;
   return( r);
}


#ifdef SIGMA
# undef SIGMA
#endif
#define SIGMA     (1 / 3.0)

MULLE__GEOMETRY_EXTERN_GLOBAL
CGFloat   CGFloatMakeNormalDistributionMinMax( CGFloat min, CGFloat max)
{
   CGFloat   r;

   do
      r = 0.5 + SIGMA * _CGFloatNormalDistribution();
   while( r < 0.0 || r > 1.0);
   return( r * (max - min) + min);
}


MULLE__GEOMETRY_EXTERN_GLOBAL
CGFloat   CGFloatMakeAbnormalDistributionMinMax( CGFloat min, CGFloat max)
{
   CGFloat   r;

   do
      r = 0.0 + SIGMA * _CGFloatNormalDistribution();
   while( r < -0.5 || r > 0.5);

   // avg is nearer to 0:
   if( r < 0.0)
      r = -r;        // -0.4, which is unusual is now near the center
   else
      r = 1.0 - r;   // 0.4, which is unuasl is now near the center

   return( r * (max - min) + min);
}

