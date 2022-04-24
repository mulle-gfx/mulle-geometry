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



/**
 @Status Interoperable
*/
CGRect CGRectStandardize(CGRect r) {
    CGRect out;

    if (r.size.width < 0.0f) {
        out.origin.x = r.origin.x + r.size.width;
        out.size.width = -r.size.width;
    } else {
        out.origin.x = r.origin.x;
        out.size.width = r.size.width;
    }

    if (r.size.height < 0.0f) {
        out.origin.y = r.origin.y + r.size.height;
        out.size.height = -r.size.height;
    } else {
        out.origin.y = r.origin.y;
        out.size.height = r.size.height;
    }
    return out;
}


/**
 @Status Interoperable
*/
CGRect CGRectIntegral(CGRect r)
{
   CGRect   rect;

   if( CGRectIsNull( r))
      return( r);

    rect              = CGRectStandardize(r);  // TODO: compatible ?
    rect.size.width   = ceilf(rect.origin.x + rect.size.width);
    rect.size.height  = ceilf(rect.origin.y + rect.size.height);
    rect.origin.x     = floorf(rect.origin.x);
    rect.origin.y     = floorf(rect.origin.y);
    rect.size.width  -= rect.origin.x;
    rect.size.height -= rect.origin.y;

    return r;
}


/**
 @Status Interoperable
*/
static CGRect   _CGRectIntersection(CGRect r1, CGRect r2)
{
    r2 = CGRectStandardize(r2);

    CGFloat  x1, y1, x2, y2;
    int isNull = 0;
    CGRect out;

    if (r1.origin.x < r2.origin.x)
    {
        if( r1.origin.x + r1.size.width < r2.origin.x)
            return( CGRectNull);

        x1 = CGFloatMinimum(r1.origin.x + r1.size.width, r2.origin.x);
        x2 = CGFloatMinimum(r1.origin.x + r1.size.width, r2.origin.x + r2.size.width);
    }
    else
    {
        if (r2.origin.x + r2.size.width < r1.origin.x)
           return (CGRectNull);

        x1 = CGFloatMinimum(r2.origin.x + r2.size.width, r1.origin.x);
        x2 = CGFloatMinimum(r2.origin.x + r2.size.width, r1.origin.x + r1.size.width);
    }

    if (r1.origin.y < r2.origin.y)
    {
        if (r1.origin.y + r1.size.height < r2.origin.y)
           return (CGRectNull);

        y1 = CGFloatMinimum(r1.origin.y + r1.size.height, r2.origin.y);
        y2 = CGFloatMinimum(r1.origin.y + r1.size.height, r2.origin.y + r2.size.height);
    }
    else
    {
        if (r2.origin.y + r2.size.height < r1.origin.y)
           return (CGRectNull);

        y1 = CGFloatMinimum(r2.origin.y + r2.size.height, r1.origin.y);
        y2 = CGFloatMinimum(r2.origin.y + r2.size.height, r1.origin.y + r1.size.height);
    }


   out.origin.x = x1;
   out.origin.y = y1;
   out.size.width = x2 - x1;
   out.size.height = y2 - y1;

   return out;
}

CGRect   CGRectIntersection(CGRect r1, CGRect r2)
{
   return( _CGRectIntersection( CGRectStandardize(r1), r2));
}



/**
 @Status Interoperable
*/
CGRect CGRectUnion(CGRect r1, CGRect r2) {
    if (CGRectIsInfinite(r1)) {
        return r2;
    }
    if (CGRectIsInfinite(r2)) {
        return r1;
    }

    r1 = CGRectStandardize(r1);
    r2 = CGRectStandardize(r2);

    float x1, y1, x2, y2;

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

    CGRect ret;
    ret.origin.x = x1;
    ret.origin.y = y1;
    ret.size.width = x2 - x1;
    ret.size.height = y2 - y1;

    return ret;
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
//       intersection =  CGRectIntersection( r1, rects[ i]);
//       area         = intersection.size.width * intersection.size.height;
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