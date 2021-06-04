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

#ifndef __cg_geometry_h__
#define __cg_geometry_h__

#include "CGBase.h"
#include <stdint.h>
#include <math.h>
#include <assert.h>




/**
@Status Interoperable
*/
static inline int CGPointEqualToPoint(CGPoint a, CGPoint b) {
    return( (a.x == b.x) && (a.y == b.y));
}


/**
@Status Interoperable
*/
static inline int CGSizeEqualToSize(CGSize a, CGSize b) {
    return a.width == b.width && a.height == b.height;
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
// GetMaxY is a misnomer, its really the first Y point not inside the rectangle.
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
// GetMaxY is a misnomer, its really the first Y point not inside the rectangle.
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
static inline CGRect CGRectInset(CGRect rect, CGFloat dx, CGFloat dy)
{
    rect.origin.x    += dx;
    rect.origin.y    += dy;
    rect.size.width  -= dx * 2;
    rect.size.height -= dy * 2;
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
static inline CGRect CGRectOffset(CGRect rect, CGFloat dx, CGFloat dy)
{
    rect.origin.x += dx;
    rect.origin.y += dy;
    return rect;
}

/**
@Status Interoperable
*/
static inline int CGRectIsEmpty(CGRect rect)
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
static inline int CGRectIntersectsRect(CGRect a, CGRect b)
{
    return !((b.origin.x >= CGRectGetMaxX( a)) || (b.origin.y >= CGRectGetMaxY( a)) ||
             (a.origin.x >= CGRectGetMaxX( b)) || (a.origin.y >= CGRectGetMaxY( b)));
}

/**
@Status Interoperable
*/
static inline int CGRectEqualToRect(CGRect a, CGRect b)
{
    return CGPointEqualToPoint(a.origin, b.origin) && CGSizeEqualToSize(a.size, b.size);
}

/**
@Status Interoperable
*/
static inline int CGRectIsNull(CGRect rect)
{
    return CGRectEqualToRect(rect, CGRectNull);
}

/**
@Status Interoperable
*/
static inline int CGRectIsInfinite(CGRect rect)
{
    return (isinf(rect.origin.x) || isinf(rect.origin.y) || isinf(rect.size.width) || isinf(rect.size.height));
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
CGRect CGRectIntegral(CGRect rect);
CGRect CGRectIntersection(CGRect r1, CGRect r2);

// turns negative width/height rect, into positive with different x/y
// mathematically its the same
CGRect CGRectStandardize(CGRect rect);



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

CGRect CGRectUnion(CGRect a, CGRect b);


#endif
