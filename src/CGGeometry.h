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

#include <stdint.h>
#include <math.h>
#include <assert.h>


static inline CGFloat  CGVectorLength( CGVector v)
{
   return( sqrtf( v.dx * v.dx + v.dy * v.dy));
}


/**
@Status Interoperable
*/
static inline int CGPointEqualToPoint(CGPoint a, CGPoint b) {
    return( (a.x == b.x) && (a.y == b.y));
}


static inline CGPoint   CGPointOffset( CGPoint a, CGFloat dx, CGFloat dy)
{
    a.x += dx;
    a.y += dy;
    return a;
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


static inline CGPoint   CGPointScale( CGPoint p, CGFloat scale)
{
   return( CGPointMake( p.x * scale, p.y * scale));
}


static inline CGVector   CGVectorScale( CGVector p, CGFloat scale)
{
   return( CGVectorMake( p.dx * scale, p.dy * scale));
}


static inline CGSize   CGSizeScale( CGSize p, CGFloat scale)
{
   return( CGSizeMake( p.width * scale, p.height * scale));
}



// mulle addition:
static inline CGFloat   CGPointGetPolarRadius( CGPoint p)
{
   return( sqrtf( p.x * p.x + p.y * p.y));
}



// mulle addition:
static inline CAAngleRadians   CGPointGetPolarAngle( CGPoint p)
{
   if( CGPointEqualToPoint( p, CGPointZero))
      return( 0.0);
   return( atan2f( p.y, p.x));
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

// http://paperjs.org/reference/point/#normalize
static inline CGPoint  CGPointNormalize( CGPoint a, CGFloat radius)
{
   CGFloat   angle;

   angle = CGPointGetPolarAngle( a);
   return( CGPointMake( cosf( angle) * radius,
                        sinf( angle) * radius));
}



/**
@Status Interoperable
*/
static inline int CGSizeEqualToSize(CGSize a, CGSize b) {
    return a.width == b.width && a.height == b.height;
}


/**
@Status Interoperable
*/
static inline double CGSizeGetArea(CGSize a) {
    return a.width* a.height;
}


/**
@Status Interoperable
*/
static inline double CGRectGetArea(CGRect a) {
    return a.size.width * a.size.height;
}


// now this is sometimes useful, though it sounds bad
static inline CGFloat CGVectorGetAspectRatio(CGVector a) {
    return( a.dy == 0.0 ? 0.0 : a.dx / a.dy);
}

// now this is sometimes useful, though it sounds bad
static inline CGFloat CGPointGetAspectRatio(CGPoint a) {
    return( a.y == 0.0 ? 0.0 : a.x / a.y);
}

// if a has no size, use 0.0 instead of INFINITY for 
// easier code (?)
static inline CGFloat CGSizeGetAspectRatio(CGSize a) {
    return( a.height == 0.0 ? 0.0 : a.width / a.height);
}

// returns either height or weight, which ever is smaller
static inline CGFloat CGSizeGetMinimum(CGSize a) {
    return( a.width < a.height ? a.width : a.height);
}

// returns either height or weight, which ever is bigger
static inline CGFloat CGSizeGetMaximum( CGSize a) {
    return( a.width > a.height ? a.width : a.height);
}


static inline CGFloat CGRectGetAspectRatio(CGRect a) {
    return( CGSizeGetAspectRatio( a.size));
}


// get the radius for a circle that touches the inside of a rectangle
static inline CGFloat CGSizeGetInnerRadius( CGSize a)
{
    return( (a.width < a.height ? a.width : a.height) / 2.0);
}

// get the radius that "envelopes" a rectangle of given size
static inline CGFloat CGSizeGetOuterRadius( CGSize a)
{
    return( sqrtf( a.width * a.width + a.height * a.height) / 2.0);
}


// get the radius for a circle that touches the inside of a rectangle
static inline CGFloat CGRectGetInnerRadius( CGRect r)
{
   return( CGSizeGetInnerRadius( r.size));
}

// get the radius that "envelopes" a rectangle of given size
static inline CGFloat CGRectGetOuterRadius( CGRect r)
{
   return( CGSizeGetOuterRadius( r.size));
}




/**
@Status Interoperable
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
static inline CGFloat CGRectGetMinX(CGRect rect) {
    return rect.origin.x;
}

/**
@Status Interoperable
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
static inline CGFloat CGRectGetMaxX(CGRect rect) {
    return rect.origin.x + rect.size.width;
}

/**
@Status Interoperable
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
static inline CGFloat CGRectGetMidX(CGRect rect) {
    return( rect.origin.x + rect.size.width / 2);
}

/**
@Status Interoperable
//  .-------------. < CGRectGetMinY
//  |             |
//  |             |
//  |             |
//  |             |
//  |             |
//  '-------------'
//
*/
static inline CGFloat CGRectGetMinY(CGRect rect) {
    return rect.origin.y;
}

/**
@Status Interoperable
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
static inline CGFloat CGRectGetMaxY(CGRect rect) {
    return(rect.origin.y + rect.size.height);
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
static inline CGFloat CGRectGetMidY(CGRect rect) {
    return( rect.origin.y + rect.size.height / 2);
}

/**
@Status Interoperable
*/
static inline CGFloat CGRectGetWidth(CGRect rect) {
    return rect.size.width;
}

/**
@Status Interoperable
*/
static inline CGFloat CGRectGetHeight(CGRect rect) {
    return rect.size.height;
}




//  .-------------.
//  |             |
//  |             |
//  |      *      | < CGRectGetCenterPoint().y
//  |             |
//  |             |
//  '-------------'
//         ^
//         CGRectGetCenterPoint().x
//
static inline CGPoint   CGRectGetCenterPoint( CGRect rect)
{
   return( CGPointMake( CGRectGetMidX( rect), CGRectGetMidY( rect)));
}


/**
@Status Interoperable
*/
static inline int CGRectContainsPoint(CGRect rect, CGPoint point) {
    return (point.x >= CGRectGetMinX(rect) && point.x < CGRectGetMaxX(rect)) &&
           (point.y >= CGRectGetMinY(rect) && point.y < CGRectGetMaxY(rect));
}

static inline CGRect   CGRectExpand( CGRect rect, CGFloat dx, CADirection direction)
{
    if( direction & CADirectionTop)
    {
        rect.origin.y    -= dx;
        rect.size.height += dx;
    }
    if( direction & CADirectionBottom)
    {
        rect.size.height += dx;
    }
    if( direction & CADirectionLeft)
    {
        rect.origin.x   -= dx;
        rect.size.width += dx;
    }
    if( direction & CADirectionRight)
    {
        rect.size.width += dx;
    }
    return( rect);
}


static inline CGRect   CGRectContract( CGRect rect, CGFloat dx, CADirection direction)
{
    return( CGRectExpand( rect, -dx, direction));
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
static inline CGRect   CGRectInset(CGRect rect, CGFloat dx, CGFloat dy)
{
    rect.origin.x    += dx;
    rect.origin.y    += dy;
    rect.size.width  -= dx * 2;
    rect.size.height -= dy * 2;
    return rect;
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


static inline CGRectU   CGRectUExtrude( CGRectU rect, CGFloat dx, CGFloat dy, unsigned int index)
{
   assert( index <= 1);
   rect.origin.value[ index]   -= dx;
   rect.origin.value[ ! index] -= dy;
   rect.size.value[ index]     += dx * 2;
   rect.size.value[ ! index]   += dy * 2;
   return rect;
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
static inline CGRect   CGRectOffset(CGRect rect, CGFloat dx, CGFloat dy)
{
    rect.origin.x += dx;
    rect.origin.y += dy;
    return rect;
}

/**
@Status Interoperable
*/
static inline int   CGRectIsEmpty(CGRect rect)
{
    return ((rect.size.width == 0) && (rect.size.height == 0));
}

/**
@Status Interoperable
   CGRectMake( 2.0, 0.0, 2.0, 1.0) and CGRectMake( 0.0, 0.0, 2.0, 1.0)
   are not considered intersecting, though mathematically they are.
   This is so that "adjacent" rectangles are not considered intersecting.
   CGRectIntersection will return an intersection of 0 width or height though!
*/
static inline int   CGRectIntersectsRect(CGRect a, CGRect b)
{
    return !((b.origin.x >= CGRectGetMaxX( a)) || (b.origin.y >= CGRectGetMaxY( a)) ||
             (a.origin.x >= CGRectGetMaxX( b)) || (a.origin.y >= CGRectGetMaxY( b)));
}

/**
@Status Interoperable
*/
static inline int   CGRectEqualToRect(CGRect a, CGRect b)
{
    return CGPointEqualToPoint(a.origin, b.origin) && CGSizeEqualToSize(a.size, b.size);
}


/**
@Status Interoperable
*/
static inline int   CGRectIsInfinite(CGRect rect)
{
    return( isinf(rect.origin.x) || isinf(rect.origin.y) || isinf(rect.size.width) || isinf(rect.size.height));
}

/**
@Status Interoperable
*/
static inline int CGRectContainsRect(CGRect a, CGRect b)
{
    return( CGRectGetMinX(b) >= CGRectGetMinX(a) &&
            CGRectGetMaxX(b) <= CGRectGetMaxX(a) &&
            CGRectGetMinY(b) >= CGRectGetMinY(a) &&
            CGRectGetMaxY(b) <= CGRectGetMaxY(a));
}

// CGRectIntegral rounds the rectangle’s origin downward and its size upward
// to the nearest whole integers, such that the result contains the original
// rectangle
CGRect   CGRectIntegral(CGRect rect);

//
// Returns a rect that represents the intersection of r1 and r2.
// Returns CGRectNull if r1 and r2 do not intersect
//
CGRect   CGRectIntersection(CGRect r1, CGRect r2);

// turns negative width/height rect, into positive with different x/y
// mathematically its the same.
CGRect   CGRectStandardize(CGRect rect);



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
