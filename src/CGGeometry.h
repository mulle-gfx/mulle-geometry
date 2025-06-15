/* Copyright (c) 2006-2007 Christopher J. W. Lloyd

Copyright (c) Microsoft. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

#ifndef cg_geometry_h__
#define cg_geometry_h__


#include "CGBase.h"
#include "CAAngle.h"
#include "CADirection.h"

#include <stdint.h>
#include <math.h>
#include <assert.h>


// Basically the EPSILON is only used in distance calculations

// https://randomascii.wordpress.com/category/floating-point/
#ifndef CG_VECTOR_EPSILON
# define CG_VECTOR_EPSILON  0.0001f
#endif


static inline CGVector   CGVectorMakeWithAngle( CAAngle angle)
{
   CGFloat   radians;

   assert( ! isnan( angle));

   radians = CAAngleToRadians( angle);
   return( CGVectorMake( sinf( radians), cosf( radians)));
}


static inline CGVector   CGVectorMakeWithRotation( CARotation rotation)
{
   CGFloat   radians;

   assert( ! isnan( rotation));

   radians = -rotation * 2.0f * M_PI_f;
   return( CGVectorMake( sinf( radians), cosf( radians)));
}



static inline CGFloat  CGVectorLength( CGVector v)
{
   CGFloat   length;

   length = sqrtf( v.dx * v.dx + v.dy * v.dy);
   if( length < CG_VECTOR_EPSILON)
      return( 0.0f);
   return( length);
}


static inline CGFloat  CGVectorSquaredLength( CGVector v)
{
   return( v.dx * v.dx + v.dy * v.dy);
}


static inline CGFloat   CGVectorDotProduct( CGVector v1, CGVector v2)
{
   return( v1.dx * v2.dx + v1.dy * v2.dy);
}


static inline CAAngle   CGVectorGetAngle( CGVector vector)
{
   return( CARadiansToAngle( atan2f( vector.dy, vector.dx)));
}


static inline CARotation   CGVectorGetRotation( CGVector vector)
{
   return( CARadiansAsAngleToRotation( atan2f( vector.dy, vector.dx)));
}


// now this is sometimes useful, though it sounds bad
static inline CGFloat   CGVectorGetAspectRatio( CGVector a)
{
   return( CGFloatIsZero( a.dy) ? 0.0f : a.dx / a.dy);
}


static inline CGVector   CGVectorScale( CGVector p, CGFloat scale)
{
   return( CGVectorMake( p.dx * scale, p.dy * scale));
}



/**
*/
static inline int   CGPointEqualToPoint( CGPoint a, CGPoint b)
{
   if( (a.x == b.x) && (a.y == b.y))
      return( 1);
   // this seems fairly cheap
   return( isnan( a.x) && isnan( a.y) && isnan( b.x) && isnan( b.y));
}


static inline CGPoint   CGPointAdd( CGPoint p, CGPoint q)
{
   return( CGPointMake( p.x + q.x, p.y + q.y));
}


static inline CGPoint   CGPointAddVector( CGPoint p, CGVector q)
{
   return( CGPointMake( p.x + q.dx, p.y + q.dy));
}


static inline CGPoint   CGPointSubtract( CGPoint p, CGPoint q)
{
   return( CGPointMake( p.x - q.x, p.y - q.y));
}


static inline CGPoint   CGPointNegate( CGPoint p)
{
   return( CGPointMake( -p.x, -p.y));
}


static inline CGPoint   CGPointSubtractVector( CGPoint p, CGVector q)
{
   return( CGPointMake( p.x - q.dx, p.y - q.dy));
}


static inline CGPoint   CGPointMultiply( CGPoint p, CGPoint q)
{
   return( CGPointMake( p.x * q.x, p.y * q.y));
}


static inline CGPoint   CGPointMultiplyVector( CGPoint p, CGVector q)
{
   return( CGPointMake( p.x * q.dx, p.y * q.dy));
}


static inline CGPoint   CGPointOffset( CGPoint a, CGFloat dx, CGFloat dy)
{
   a.x += dx;
   a.y += dy;
   return a;
}


static inline CGPoint   CGPointTranslate( CGPoint point,
                                          CGFloat slope,
                                          CGFloat distance)
{
   CGVector   v;

   v.dx = sqrtf( distance * distance / (1 + slope * slope));
   v.dy = slope * v.dx;

   return( CGPointMake( point.x + v.dx, point.y + v.dy));
}


static inline CGPoint   CGPointScale( CGPoint p, CGFloat scale)
{
   return( CGPointMake( p.x * scale, p.y * scale));
}


//
// mulle addition (neg. size values possible due to OpenGl code) and flipping
// and the like
//
static inline CGPoint  CGPointClampToSize( CGPoint point, CGSize size)
{
   if( size.width > 0.0)
   {
      if( point.x < 0.0)
         point.x = 0.0;
      if( point.x >= size.width)
         point.x = size.width;  // th
   }
   else
   {
      if( point.x > 0.0)
         point.x = 0.0;
      if( point.x <= size.width)
         point.x = size.width;
   }

   if( size.height > 0.0)
   {
      if( point.y < 0.0)
         point.y = 0.0;
      if( point.y >= size.height)
         point.y = size.height;
   }
   else
   {
      if( point.y > 0.0)
         point.y = 0.0;
      if( point.y <= size.height)
         point.y = size.height;
   }

   return( point);
}


static inline CGFloat  CGPointDistanceToPoint( CGPoint a, CGPoint b)
{
   CGVector  d;

   d = CGVectorMake( b.x - a.x, b.y - a.y);
   return( CGVectorLength( d));
}


static inline CGFloat  CGPointSquaredDistanceToPoint( CGPoint a, CGPoint b)
{
   CGVector  d;

   d = CGVectorMake( b.x - a.x, b.y - a.y);
   return( CGVectorSquaredLength( d));
}


static inline CGPoint   CGPointMidPoint( CGPoint p1, CGPoint p2)
{
   return( CGPointMake( (p1.x + p2.x) / 2, (p1.y + p2.y) / 2));
}


static inline CGFloat   CGPointSlope( CGPoint p1, CGPoint p2)
{
   return( (p2.y - p1.y) / (p2.x - p1.x));
}


// TODO: untested, coded by AI
static inline CAAngle   CAAngleBetweenPoints( CGPoint p0, CGPoint p1, CGPoint p2)
{
   CGFloat   radians;

   radians = atan2f( p2.y - p0.y, p2.x - p0.x) -
             atan2f( p1.y - p0.y, p1.x - p0.x);
   return( CARadiansToAngle( radians));
}


// TODO: untested, coded by AI
static inline CGFloat   CGPointTriangleArea( CGPoint p0, CGPoint p1, CGPoint p2)
{
   CGFloat area;

   area = fabsf( (p0.x * (p1.y - p2.y) + p1.x * (p2.y - p0.y) + p2.x * (p0.y - p1.y)) / 2.0f);
   return( area);
}


// TODO: untested
static inline int   CGPointTriangleHasMinimumAngle( CGPoint p0,
                                                    CGPoint p1,
                                                    CGPoint p2,
                                                    CGFloat minDegrees)
{
   CAAngle   angle;
   CAAngle   minAngle;
   CAAngle   maxAngle;

   assert( minDegrees > 0.0 && minDegrees < 180.0);

   minAngle = CADegreesToAngle( minDegrees);
   maxAngle = CADegreesToAngle( 180.0 - minDegrees);

   angle    = CAAngleBetweenPoints( p0, p1, p2);
   if( isnan( angle) || angle < minAngle || angle > maxAngle)
      return( 0);

   angle = CAAngleBetweenPoints( p1, p2, p0);
   if( isnan( angle) || angle < minAngle || angle > maxAngle)
      return( 0);

   // exploit triangle being 180 degrees angle sum
   return( 1);
}


// TODO: untested
static inline int   CGPointTriangleHasMinimumArea( CGPoint p0,
                                                   CGPoint p1,
                                                   CGPoint p2,
                                                   CGFloat minArea)
{
   CGFloat   area;

   area = CGPointTriangleArea( p0, p1, p2);
   return( area >= minArea);
}


// Or center of mass, but centroid seems to be an "official" name
// like Schwerpunkt, center of mass
//
static inline CGPoint   CGPointTriangleGetCentroid( CGPoint p0,
                                                    CGPoint p1,
                                                    CGPoint p2)
{
   return( CGPointMake( (p0.x + p1.x + p2.x) / 3.0f,
                        (p0.y + p1.y + p2.y) / 3.0f));
}

static inline CGPoint   CGPointTriangleGetCenterOfMass( CGPoint p0,
                                                        CGPoint p1,
                                                        CGPoint p2)
{
   return( CGPointTriangleGetCentroid( p0, p1, p2));
}


// ceilf is compatible with CGRectIntegral
static inline CGPoint   CGPointIntegral( CGPoint p)
{
   return( CGPointMake( floorf( p.x),
                        floorf( p.y)));
}


// ceilf is compatible with CGRectIntegral
static inline CGPoint   CGPointIntegralFuzzy( CGPoint p, CGFloat fuzz)
{
   return( CGPointMake( floorf( p.x + fuzz),
                        floorf( p.y + fuzz)));
}


// now this is sometimes useful, though it sounds bad
static inline CGFloat   CGPointGetAspectRatio( CGPoint a)
{
   return( CGFloatIsZero( a.y) ? 0.0f : a.x / a.y);
}





/**
*/
static inline int   CGSizeEqualToSize( CGSize a, CGSize b)
{
   return( a.width == b.width && a.height == b.height);
}


/**
*/
static inline double   CGSizeGetArea( CGSize a)
{
   return fabs( a.width * a.height);
}


static inline int   CGSizeContainsSize( CGSize a, CGSize b)
{
   return( a.width >= b.width && a.height >= b.height);
}


// ceilf is compatible with CGRectIntegral
static inline CGSize   CGSizeIntegral( CGSize a)
{
   return( CGSizeMake( ceilf( a.width),
                       ceilf( a.height)));
}


// ceilf is compatible with CGRectIntegral
static inline CGSize   CGSizeIntegralFuzzy( CGSize a, CGFloat fuzz)
{
   return( CGSizeMake( ceilf( a.width - fuzz),
                       ceilf( a.height - fuzz)));
}

static inline CGSize   CGSizeScale( CGSize p, CGFloat scale)
{
   return( CGSizeMake( p.width * scale, p.height * scale));
}


static inline CGPoint   CGSizeGetCenter( CGSize size)
{
   return( CGPointMake( size.width / 2.0f, size.height / 2.0f));
}



// get the radius for a circle that touches the inside of a rectangle
static inline CGFloat   CGSizeGetInnerRadius( CGSize a)
{
   a.width  = a.width < 0.0f ? -a.width : a.width;
   a.height = a.height < 0.0f ? -a.height : a.height;
   return( (a.width < a.height ? a.width : a.height) / 2.0);
}


// get the radius that "envelopes" a rectangle of given size
static inline CGFloat   CGSizeGetOuterRadius( CGSize a)
{
   return( sqrtf( a.width * a.width + a.height * a.height) / 2.0);
}



// if a has no size, use 0.0 instead of INFINITY for
// easier code (?)
static inline CGFloat   CGSizeGetAspectRatio( CGSize a)
{
   return( CGFloatIsZero( a.height) ? 0.0f : a.width / a.height);
}


// returns either height or weight, which ever is smaller
static inline CGFloat   CGSizeGetMinimum( CGSize a)
{
   return( a.width < a.height ? a.width : a.height);
}


// returns either height or weight, which ever is bigger
static inline CGFloat   CGSizeGetMaximum( CGSize a)
{
   return( a.width > a.height ? a.width : a.height);
}



/**
*/
// TODO: untested, coded by AI
// TODO: untested, coded by AI


static inline CGFloat   CGRectGetMaximumSize( CGRect a)
{
   return( CGSizeGetMaximum( a.size));
}


static inline CGFloat   CGRectGetMinimumSize( CGRect a)
{
   return( CGSizeGetMinimum( a.size));
}


static inline CGFloat   CGRectGetAspectRatio( CGRect a)
{
   return( CGSizeGetAspectRatio( a.size));
}


// get the radius for a circle that touches the inside of a rectangle
static inline CGFloat   CGRectGetInnerRadius( CGRect r)
{
   return( CGSizeGetInnerRadius( r.size));
}


// get the radius that "envelopes" a rectangle of given size
static inline CGFloat   CGRectGetOuterRadius( CGRect r)
{
   return( CGSizeGetOuterRadius( r.size));
}




/**
*/
static inline int   CGRectIsInfinite( CGRect rect)
{
   return( isinf( rect.origin.x) || isinf(rect.origin.y) ||
           isinf( rect.size.width) || isinf(rect.size.height));
}


static inline int   CGRectIsStandardized( CGRect rect)
{
   return( rect.size.width >= 0.0f && rect.size.height >= 0.0f);
}


/**
//  .-------------.
//  |             |
//  |             |
//  |             |
//  |             |
//  |             |
//  '-------------'
//  ^
//  CGRectGetMinX

*/
static inline CGFloat CGRectGetMinX( CGRect rect)
{
   return( rect.size.width >= 0.0
           ? rect.origin.x
           : rect.origin.x + rect.size.width);
}

/**
//  .-------------.
//  |             |
//  |             |
//  |             |
//  |             |
//  |             |
//  '-------------'
//                 ^
//                 CGRectGetMaxX
//
// GetMaxX is really the first X point not inside the rectangle.
// 
*/
static inline CGFloat CGRectGetMaxX( CGRect rect)
{
   return( rect.size.width >= 0.0
           ? rect.origin.x + rect.size.width
           : rect.origin.x);
}
/**
//  .-------------.
//  |      |      |
//  |      |      |
//  |      |      |
//  |      |      |
//  |      |      |
//  '-------------'
//         ^
//         CGRectGetMidX
*/
static inline CGFloat CGRectGetMidX( CGRect rect)
{
   return( rect.origin.x + rect.size.width * 0.5);
}

/**
//  .-------------. < CGRectGetMinY
//  |             |
//  |             |
//  |             |
//  |             |
//  |             |
//  '-------------'
//
*/
static inline CGFloat CGRectGetMinY( CGRect rect)
{
   return( rect.size.height >= 0.0
           ? rect.origin.y
           : rect.origin.y + rect.size.height);
}

/**
//  .-------------.
//  |             |
//  |             |
//  |             |
//  |             |
//  |             |
//  '-------------'
//                  < CGRectGetMaxY
//
// GetMaxY is really the first Y point not inside the rectangle.
*/
static inline CGFloat CGRectGetMaxY( CGRect rect)
{
   return( rect.size.height >= 0.0
           ? rect.origin.y + rect.size.height
           : rect.origin.y);
}

/**

//  .-------------.
//  |             |
//  |             |
//  |-------------| < CGRectGetMidY
//  |             |
//  |             |
//  '-------------'
*/
static inline CGFloat CGRectGetMidY( CGRect rect)
{
   return( rect.origin.y + rect.size.height * 0.5);
}


/**
*/
static inline CGFloat CGRectGetWidth( CGRect rect)
{
   return( rect.size.width < 0.0 ? -rect.size.width : rect.size.width);
}


/**
*/
static inline CGFloat CGRectGetHeight( CGRect rect)
{
   return( rect.size.height < 0.0 ? -rect.size.height : rect.size.height);
}


static inline double CGRectGetArea( CGRect a)
{
   return CGRectGetWidth( a) * CGRectGetHeight( a);
}


//  .-------------.
//  |             |
//  |             |
//  |      *      | < CGRectGetCenter().y
//  |             |
//  |             |
//  '-------------'
//         ^
//         CGRectGetCenter().x
//

static inline CGPoint   CGRectGetCornerPoint( CGRect rect, CADirection dir)
{
   CGPoint   p;

   switch( dir & (CADirectionLeft|CADirectionRight))
   {
   case CADirectionLeft   : p.x = CGRectGetMinX( rect); break;
   case CADirectionRight  : p.x = CGRectGetMaxX( rect); break;
   default                : p.x = CGRectGetMidX( rect); break;
   }
   switch( dir & (CADirectionTop|CADirectionBottom))
   {
   case CADirectionTop    : p.y = CGRectGetMinY( rect); break;
   case CADirectionBottom : p.y = CGRectGetMaxY( rect); break;
   default                : p.y = CGRectGetMidY( rect); break;
   }
   return( p);
}

//
// When width or height are < 0 (maybe due to rotation), the origin and the
// opposite point are reveresed. The TopLeft is still the one with the least
// x and y though.
//
/**
//  *-------------.
//  |             |
//  |             |
//  |             |
//  |             |
//  |             |
//  '-------------'
//  ^
//  CGRectGetOrigin
*/
static inline CGPoint   CGRectGetOrigin( CGRect rect)
{
   return( rect.origin);
}

/**
//  .-------------.
//  |             |
//  |             |
//  |      * <----:--- center
//  |             |
//  |             |
//  '-------------'
//         ^
//    CGRectGetCenter, like CGRectGetCenterWithAnchorPoint with 0.5, 0.5
*/
static inline CGPoint   CGRectGetCenter( CGRect rect)
{
   return( CGPointMake( CGRectGetMidX( rect), CGRectGetMidY( rect)));
}


/**
//  .-------------.
//  |             |
//  |             |
//  |             |
//  |         * <-:--- anchorPoint
//  |             |
//  '-------------'
//
//    CGRectGetCenterWithAnchorPoint
*/
static inline CGPoint   CGRectGetCenterWithAnchorPoint( CGRect rect, CGPoint anchorPoint)
{
   return( CGPointMake( rect.origin.x + rect.size.width * anchorPoint.x,
                        rect.origin.y + rect.size.height * anchorPoint.y));
}


/**
//  .-------------.
//  |             |
//  |             |
//  |             |
//  |             |
//  |             |
//  '-------------*
//                ^
//          CGRectGetOpposite
*/
static inline CGPoint   CGRectGetOpposite( CGRect rect)
{
   return( CGPointMake( rect.origin.x + rect.size.width,
                        rect.origin.y + rect.size.height));
}

/**
//  .------*------.
//  |             |
//  |             |
//  |             |
//  |             |
//  |             |
//  '-------------'
//         ^
//     CGRectGetTop
*/
static inline CGPoint   CGRectGetTop( CGRect rect)
{
   return( CGPointMake( CGRectGetMidX( rect), CGRectGetMinY( rect)));
}

/**
//  .-------------.
//  |             |
//  |             |
//  |             |
//  |             |
//  |             |
//  '------*------'
//         ^
//    CGRectGetBottom
*/
static inline CGPoint   CGRectGetBottom( CGRect rect)
{
   return( CGPointMake( CGRectGetMidX( rect), CGRectGetMaxY( rect)));
}

/**
//  .-------------.
//  |             |
//  |             |
//  |             *
//  |             |
//  |             |
//  '-------------'
//                ^
//           CGRectGetRight
*/
static inline CGPoint   CGRectGetRight( CGRect rect)
{
   return( CGPointMake( CGRectGetMaxX( rect), CGRectGetMidY( rect)));
}

/**
//  .-------------.
//  |             |
//  |             |
//  *             |
//  |             |
//  |             |
//  '-------------'
//  ^
//  CGRectGetLeft
*/
static inline CGPoint   CGRectGetLeft( CGRect rect)
{
   return( CGPointMake( CGRectGetMinX( rect), CGRectGetMidY( rect)));
}

/**
//  *-------------.
//  |             |
//  |             |
//  |             |
//  |             |
//  |             |
//  '-------------'
//  ^
//  CGRectGetTopLeft
*/
static inline CGPoint   CGRectGetTopLeft( CGRect rect)
{
   return( CGPointMake( CGRectGetMinX( rect), CGRectGetMinY( rect)));
}

/**
//  .-------------*
//  |             |
//  |             |
//  |             |
//  |             |
//  |             |
//  '-------------'
//                ^
//           CGRectGetTopRight
*/
static inline CGPoint   CGRectGetTopRight( CGRect rect)
{
   return( CGPointMake( CGRectGetMaxX( rect), CGRectGetMinY( rect)));
}

/**
//  .-------------.
//  |             |
//  |             |
//  |             |
//  |             |
//  |             |
//  *-------------'
//  ^
//  CGRectGetBottomLeft
*/
static inline CGPoint   CGRectGetBottomLeft( CGRect rect)
{
   return( CGPointMake( CGRectGetMinX( rect), CGRectGetMaxY( rect)));
}

/**
//  .-------------.
//  |             |
//  |             |
//  |             |
//  |             |
//  |             |
//  '-------------*
//                ^
//           CGRectGetBottomRight
*/
static inline CGPoint   CGRectGetBottomRight( CGRect rect)
{
   return( CGPointMake( CGRectGetMaxX( rect), CGRectGetMaxY( rect)));
}




/**
*/
MULLE__GEOMETRY_EXTERN_GLOBAL
int CGRectContainsPoint( CGRect rect, CGPoint point);


/**
*/
static inline int   CGRectIsEmpty( CGRect rect)
{
   return ((rect.size.width == 0.0f) || (rect.size.height == 0.0f));
}

/**
*/
// YES if the two specified rectangles have equal size and origin values,
// or if both rectangles are null rectangles.
static inline int   CGRectEqualToRect( CGRect a, CGRect b)
{
   // With memcmp we don't need special code to check for isnan. The produced
   // code is still small enough for inlining IMI
   return( ! memcmp( &a, &b, sizeof( CGRect)));

//   if( ! CGSizeEqualToSize( a.size, b.size))
//      return( 0);
//   return( CGPointEqualToPoint( a.origin, b.origin));
}


/*
 *   .................................
 *   . input         dy              .
 *   .    .---------------------.    .
 *   .    | output              |    .
 *   .    |                     |    .
 *   .  dx|                     |dx  .
 *   .    |                     |    .
 *   .    |                     |    .
 *   .    '---------------------'    .
 *   .               dy              .
 *   .................................
 */
static inline CGRect   CGRectInset( CGRect rect, CGFloat dx, CGFloat dy)
{
   rect.origin.x    += dx;
   rect.origin.y    += dy;
   rect.size.width  -= dx * 2;
   rect.size.height -= dy * 2;
   return rect;
}

//
// like above, but if insets are too large, make the CGRect zero
//
static inline CGRect   CGRectInsetOrZero( CGRect rect, CGFloat dx, CGFloat dy)
{
   if( rect.size.width <= dx * 2 || rect.size.height <= dy * 2)
      return( CGRectZero);
   return( CGRectInset( rect, dx, dy));
}



/*
 *   .-------------------------------.
 *   | output        dy              |
 *   |    .......................    |
 *   |    : input               :    |
 *   |    :                     :    |
 *   |  dx:                     :dx  |
 *   |    :                     :    |
 *   |    :                     :    |
 *   |    .......................    |
 *   |               dy              |
 *   '-------------------------------'
 */
static inline CGRect   CGRectExtrude( CGRect rect, CGFloat dx, CGFloat dy)
{
   rect.origin.x    -= dx;
   rect.origin.y    -= dy;
   rect.size.width  += dx * 2;
   rect.size.height += dy * 2;
   return rect;
}


static inline CGRectU   CGRectUExtrude( CGRectU rect,
                                        CGFloat dx,
                                        CGFloat dy,
                                        unsigned int index)
{
   assert( index <= 1);
   rect.origin.value[ index]   -= dx;
   rect.origin.value[ ! index] -= dy;
   rect.size.value[ index]     += dx * 2;
   rect.size.value[ ! index]   += dy * 2;
   return rect;
}


// turns negative width/height rect, into positive with different x/y
// mathematically its the same. But you may lose texture orientation.
static inline CGRect   CGRectStandardize( CGRect r)
{
   if( r.size.width < 0.0f)
   {
      r.origin.x   += r.size.width;
      r.size.width = -r.size.width;
   }

   if( r.size.height < 0.0f)
   {
      r.origin.y    += r.size.height;
      r.size.height = -r.size.height;
   }
   return( r);
}

/*
 * CGRectExpand: Expands a rectangle in specified directions
 *
 *   .---------------.
 *   |               |
 *   |               |
 *   |               |   <- Expanded rectangle (CADirectionTop)
 *   |  d            |      (includes rect area)
 *   |  ↑            |
 *   '...............'
 *   :               |
 *   |     rect      :  <- Original rectangle
 *   :               |
 *   '-.-.-.-.-.-.-.-'
 *
 * The 'd' parameter controls expansion amount.
 * The CADirection parameter controls which sides are expanded.
 */
// returns a standardize CGRect
static inline CGRect   CGRectExpand( CGRect rect, CGFloat d, CADirection direction)
{
    rect = CGRectStandardize( rect);
    if( direction & CADirectionTop)
    {
        rect.origin.y    -= d;
        rect.size.height += d;
    }
    if( direction & CADirectionBottom)
    {
        rect.size.height += d;
    }
    if( direction & CADirectionLeft)
    {
        rect.origin.x   -= d;
        rect.size.width += d;
    }
    if( direction & CADirectionRight)
    {
        rect.size.width += d;
    }
   return( rect);
}


static inline CGRect   CGRectContract( CGRect rect, CGFloat d, CADirection direction)
{
   return( CGRectExpand( rect, -d, direction));
}


/*
 * Translate rect origin.
 *
 *     ...............
 *     . input       . dy
 *     .      .-------------.
 *     .      | output      |
 *     .      |             |
 *     .      |             |
 *     .......|             |
 *       dx   |             |
 *            '-------------'
 *
 */
static inline CGRect   CGRectOffset( CGRect rect, CGFloat dx, CGFloat dy)
{
   rect.origin.x += dx;
   rect.origin.y += dy;
   return rect;
}


static inline CGRect   CGRectBySettingMinX( CGRect rect, CGFloat x)
{
   rect.origin.x = x;
   return( rect);
}


static inline CGRect   CGRectBySettingMinY( CGRect rect, CGFloat y)
{
   rect.origin.y = y;
   return( rect);
}


// 0 1 2 3 4
// w=4 midX=2
// 0 1 2 3 4 5 6
// w=6 midX=3
static inline CGRect   CGRectBySettingMidX( CGRect rect, CGFloat x)
{
   CGFloat   diff;

   diff             = x - CGRectGetMidX( rect);
   rect.origin.x   -= diff / 2.0;
   rect.size.width += diff;
   return( rect);
}


static inline CGRect   CGRectBySettingMidY( CGRect rect, CGFloat y)
{
   CGFloat   diff;

   diff              = y - CGRectGetMidY( rect);
   rect.origin.y    -= diff / 2.0;
   rect.size.height += diff;
   return( rect);
}


// more interesting :)
static inline CGRect   CGRectBySettingMaxX( CGRect rect, CGFloat x)
{
   rect.size.width += x - CGRectGetMaxX( rect);
   return( rect);
}


static inline CGRect   CGRectBySettingMaxY( CGRect rect, CGFloat y)
{
   rect.size.width += y - CGRectGetMaxY( rect);
   return( rect);
}


/**
   CGRectMake( 2.0, 0.0, 2.0, 1.0) and CGRectMake( 0.0, 0.0, 2.0, 1.0)
   are not considered intersecting, though mathematically they are.
   This is so that "adjacent" rectangles are not considered intersecting.
   CGRectIntersection will return an intersection of 0 width or height though!
*/
MULLE__GEOMETRY_EXTERN_GLOBAL
int   CGRectIntersectsRect( CGRect a, CGRect b);


/**
*/
MULLE__GEOMETRY_EXTERN_GLOBAL
int   CGRectContainsRect( CGRect a, CGRect b);


// CGRectIntegral rounds the rectangle's origin downward and its size upward
// to the nearest whole integers, such that the result contains the original
// rectangle
MULLE__GEOMETRY_EXTERN_GLOBAL
CGRect   CGRectIntegral( CGRect rect);

MULLE__GEOMETRY_EXTERN_GLOBAL
CGRect   CGRectIntegralFuzzy( CGRect r, CGFloat fuzz);

//
// Returns a rect that represents the intersection of r1 and r2.
// Returns CGRectNull if r1 and r2 do not intersect
//
MULLE__GEOMETRY_EXTERN_GLOBAL
CGRect   CGRectIntersection( CGRect r1, CGRect r2);


// Combine two rectangles to form another
//
// 1.
//      ........      .------.
//      .   a  .      |      |
//      ........      |      |
//                    |      |
//      ,===,     ->  |      |
//      # b #         |      |
//      ´===`         '------'
//
// 2.
//      .....==.      .------.
//      . a .  #      |      |
//      '...'  #  ->  |      |
//      #   b  #      |      |
//      '======'      '------'

CGRect   CGRectUnion( CGRect a, CGRect b);


//
// Extend CGRect to enclose point into area, the returned rectangle
// is standardized
//
CGRect   CGRectEnclosePoint( CGRect r, CGPoint p);


// mulle addition:
// use circle routines for outer radius
static inline CGRect   CGRectMakeWithCenterAndInnerRadius( CGPoint center, CGFloat radius)
{
   return( CGRectMake( center.x - radius, center.y - radius, radius * 2, radius * 2));
}


// mulle addition:
static inline CGRect   CGRectMakeWithCenterAndSize( CGPoint center, CGSize size)
{
   return( CGRectMake( center.x - size.width / 2.0,
                       center.y - size.height / 2.0,
                       size.width,
                       size.height));
}



// mulle addition:
static inline CGPoint   CGPointMakeRandomInRect( CGRect r)
{
   return( CGPointMake( r.size.width * CGFloatMakeRandom() + r.origin.x,
                        r.size.height * CGFloatMakeRandom() + r.origin.y));
}


// mulle addition:
static inline CGSize   CGSizeMakeRandom( CGSize minSize, CGSize maxSize)
{
   assert( minSize.width <= maxSize.width);
   assert( minSize.height <= maxSize.height);

   return( CGSizeMake( minSize.width  + (maxSize.width - minSize.width)  * CGFloatMakeRandom(),
                       minSize.height + (maxSize.height - minSize.width) * CGFloatMakeRandom()));
}


// mulle addition:
static inline CGRect   CGRectMakeRandomInRect( CGRect r, CGSize minSize, CGSize maxSize)
{
   CGRect   rect;

   rect.size   = CGSizeMakeRandom( minSize, maxSize);
   rect.origin = CGPointMakeRandomInRect( CGRectMake( r.origin.x,
                                                      r.origin.y,
                                                      r.size.width - rect.size.width,
                                                      r.size.height - rect.size.height));
   return( rect);
}


//
// CGEdge
//
// CGEdge is just two CGPoints. It is not a proper type, i don't think it would
// be useful...
// Still both points have a common fate...
//

// Line intersection
int   CGEdgeIntersect( CGPoint p1, CGPoint p2, CGPoint p3, CGPoint p4, CGPoint *intersection);

// CGEdgeContainsPoint
//
//              point (outside, < 0)
//                   *
//  edgeStart ------------------> edgeEnd
//            ^  FILL FILL FILL .
//            .  FILL  * point (inside, >= 0)
//            .  FILL FILL FILL .
//            <.................v
//
//  For a clockwise polygon in Y-down coordinates:
//
//  Returns 1 if point is on right side of directed edge (inside)
//  Returns 0 if point is on left side of directed edge (outside)
//
static inline int   CGEdgeContainsPoint( CGPoint edgeStart, CGPoint edgeEnd, CGPoint point)
{
    return( ((edgeEnd.x - edgeStart.x) * (point.y - edgeStart.y) -
             (edgeEnd.y - edgeStart.y) * (point.x - edgeStart.x)) >= 0);
}


static inline int   CGEdgeContainsPointCounterClockWise( CGPoint edgeStart, CGPoint edgeEnd, CGPoint point)
{
    return( ((edgeEnd.x - edgeStart.x) * (point.y - edgeStart.y) -
             (edgeEnd.y - edgeStart.y) * (point.x - edgeStart.x)) <= 0);
}



//
// CGPolygon
//
// CGPolygon is an array of CGPoint with a count.
//


// CGPolygonClipAgainstEdge
//
//   Input:                                  Output:
//
// .....edgeEnd
//        |                                       :
//   +----:-----+ input[ n]                 +-----+ result[ m]
//   |    :     |                           |     :
//   |   rect   |   clipped by edge  -->    | keep:
//   |    :     |                           |     :
//   +----:-----+                           +-----+
//        |                                       :
//  ..... v                                       v
//     edgeStart
//
//   Clips input polygon against single edge using Sutherland-Hodgman algorithm
//   - Keeps vertices inside the edge
//   - Adds intersection points where polygon crosses edge
//   - Discards vertices outside the edge
//   - Edge direction is clockwise!
//   - Top Left is 0,0
//
//   Returns number of vertices in output polygon
//   IMPORTANT: output array must be sized for at least (2 * n) vertices
//              to handle worst case where every edge crosses the clipping edge
//              (see main-cutter demo)
unsigned int   CGPolygonClipAgainstEdge( CGPoint *input,
                                         size_t n,
                                         CGPoint edgeStart,
                                         CGPoint edgeEnd,
                                         CGPoint *output);


// CGPolygonBoundingBox
//
//   Input points:        Output bounding rect:
//
//        P2                      P2
//         *                    +--*---+
//   P1 *      * P3          P1 *      * P3
//                     -->      |      |
//                              |      |
//             * P4             +------* P4
//
//
//
//   Creates minimal axis-aligned rectangle containing all points
//   Returns CGRectZero if count is 0
//
CGRect   CGPolygonBoundingBox( CGPoint *points, size_t count);




//
// CGPolygon4
//
// CGPolygon4 is an array of 4 CGPoints with an implicit count.
//
static inline CGRect   CGPolygon4BoundingBox( CGPoint points[ 4])
{
   return( CGPolygonBoundingBox( points, 4));
}


unsigned int   CGPolygon4ClipAgainstPolygon4( CGPoint subject[ 4],
                                              CGPoint clipRect[ 4],
                                              CGPoint result[ 8]);



// Not enabled, because the interface is somewhat awkward. It usually needs
// a lot of copying...
//
// Find in "rects" the rect with the largest intersection area with r1. If none
// intersect, returns ((uintptr_t) INTPTR_MAX), which is the same as
// NSNotFound or mulle_not_found_e
//
// Example: CGRectIndexOfRectWithLargestIntersection( r1, 3, rects)
//          would return 1
//
//              .-----------.----------.
//              | rects[0]  | rects[1] |
//              |        .--------.    |
//              |        | r1     |    |
//              .--------|        |----'
//              | rects[2'--------'
//              |           |
//              '-----------'
//
// uintptr_t   CGRectIndexOfRectWithLargestIntersection( CGRect r1, uintptr_t n, CGRect *rects);
//



#endif
