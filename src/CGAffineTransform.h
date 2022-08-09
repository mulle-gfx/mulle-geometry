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
#ifndef cg_affine_transform_h__
#define cg_affine_transform_h__

#include "include.h"

#include "CGGeometry.h"
#include <string.h>


// CGAffineTransform is structurally identical to the nanoVG t
typedef struct
{
    CGFloat a;
    CGFloat b;
    CGFloat c;
    CGFloat d;
    CGFloat tx;
    CGFloat ty;
} CGAffineTransform;

// MEMO: not sure this exists in CG
typedef CGAffineTransform   *CGAffineTransformRef;


extern const CGAffineTransform   CGAffineTransformIdentity;

static inline CGAffineTransform   CGAffineTransformMakeInline( CGFloat a,
                                                               CGFloat b,
                                                               CGFloat c,
                                                               CGFloat d,
                                                               CGFloat tx,
                                                               CGFloat ty)
{
   return( (CGAffineTransform){ .a = a,
                                .b = b,
                                .c = c,
                                .d = d,
                                .tx = tx,
                                .ty = ty });
}


static inline CGAffineTransform   CGAffineTransformMakeIdentityInline( void)
{
   return( (CGAffineTransform){ .a = 1.0,
                                .b = 0.0,
                                .c = 0.0,
                                .d = 1.0,
                                .tx = 0.0,
                                .ty = 0.0 });
}



static inline CGAffineTransform   CGAffineTransformMakeTranslationInline( CGFloat tx, CGFloat ty)
{
   return( (CGAffineTransform){ .a = 1.0,
                                .b = 0.0,
                                .c = 0.0,
                                .d = 1.0,
                                .tx = tx,
                                .ty = ty });
}


static inline CGAffineTransform   CGAffineTransformMakeScaleInline(CGFloat sx, CGFloat sy)
{
   return( (CGAffineTransform){ .a = sx,
                                .b = 0.0,
                                .c = 0.0,
                                .d = sy,
                                .tx = 0.0,
                                .ty = 0.0 });
}



#define CG_ANGLE_IS_NEAR( val, comp, amt) (fabs((val) - (comp)) <= (amt))

//
// Many of these functions are actually too big for inlining
// nevertheless especially for constants there might be nice gains
//
static inline CGAffineTransform   CGAffineTransformMakeRotationInline(CGFloat angle)
{
   CGAffineTransform ret;

    // TODO: is this desirable ?
    if (CG_ANGLE_IS_NEAR( angle, ((CGFloat) 3.14159265358979323846f / 2.0f), 0.0001f))
    {
        ret.a = 0;
        ret.b = 1.0f;
        ret.c = -1.0f;
        ret.d = 0.0f;
        ret.tx = 0.0f;
        ret.ty = 0.0f;

        return ret;
    }

    ret.a = cosf(angle);
    ret.b = sinf(angle);
    ret.c = -ret.b;
    ret.d = ret.a;
    ret.tx = 0.0f;
    ret.ty = 0.0f;

    return ret;
}


static inline CGAffineTransform
   CGAffineTransformConcatInline( CGAffineTransform t2, CGAffineTransform t1)
{
    CGAffineTransform   ret;

    ret.a  = t2.a * t1.a + t2.b * t1.c;
    ret.b  = t2.a * t1.b + t2.b * t1.d;
    ret.c  = t2.c * t1.a + t2.d * t1.c;
    ret.d  = t2.c * t1.b + t2.d * t1.d;
    ret.tx = t2.tx * t1.a + t2.ty * t1.c + t1.tx;
    ret.ty = t2.tx * t1.b + t2.ty * t1.d + t1.ty;

    return ret;
}


static inline CGAffineTransform
   CGAffineTransformRotateInline( CGAffineTransform t, float angle)
{
    CGAffineTransform   ret;

    ret = CGAffineTransformMakeRotationInline( angle);
    ret = CGAffineTransformConcatInline( ret, t);

    return ret;
}

static inline CGAffineTransform
   CGAffineTransformTranslateInline( CGAffineTransform t, float x, float y)
{
    CGAffineTransform   ret;

    ret = CGAffineTransformMakeTranslationInline( x, y);
    ret = CGAffineTransformConcatInline( ret, t);

    return ret;
}


static inline CGAffineTransform
   CGAffineTransformScaleInline( CGAffineTransform t, float x, float y)
{
    CGAffineTransform   ret;

    ret = CGAffineTransformMakeScaleInline( x, y);
    return( CGAffineTransformConcatInline( ret, t));
}


static inline CGAffineTransform
   CGAffineTransformInvertInline( CGAffineTransform t)
{
   CGAffineTransform   ret;
   float               determinant;

   determinant = (t.a * t.d - t.c * t.b);
   if( determinant == 0.0)
     return t; // ?

   ret.a  = t.d / determinant;
   ret.b  = -t.b / determinant;
   ret.c  = -t.c / determinant;
   ret.d  = t.a / determinant;
   ret.tx = (-t.d * t.tx + t.c * t.ty) / determinant;
   ret.ty = (t.b * t.tx - t.a * t.ty) / determinant;

   return ret;
}




/* Copyright (c) 2006-2007 Christopher J. W. Lloyd
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


static inline CGPoint
   CGPointApplyAffineTransformInline( CGPoint point, CGAffineTransform t)
{
    CGPoint p;

    p.x = t.a * point.x + t.c * point.y + t.tx;
    p.y = t.b * point.x + t.d * point.y + t.ty;

    return p;
}


static inline CGSize
   CGSizeApplyAffineTransformInline( CGSize size, CGAffineTransform t)
{
   // TODO: not sure why we cast to double here and not elsewhere
    return( (CGSize)
    {
        .width  = (CGFloat)((double) t.a * size.width + (double) t.c * size.height),
        .height = (CGFloat)((double) t.b * size.width + (double) t.d * size.height)
    });
}


//
// This can pretty big...
//
static inline CGRect
   CGRectApplyAffineTransformInline( CGRect rect, CGAffineTransform t)
{
   CGRect   result;

   result.origin = CGPointApplyAffineTransformInline( rect.origin, t);
   result.size   = CGSizeApplyAffineTransformInline( rect.size, t);

   return( result);
}

/*
 * some functions with no inlining available
 */
int   CGAffineTransformIsIdentity( CGAffineTransform t);
int   CGAffineTransformEqualToTransform( CGAffineTransform t1, CGAffineTransform t2);


/**
 @Status Interoperable
*/

/*
 * Non-inlining counterparts (except when still inlining :))
 */

//
// Not inlining makes no sense IMO, but in code we like to write Inline when
// deemed useful and don't want to think about if the function does exist
//
static inline CGAffineTransform   CGAffineTransformMakeTranslation( CGFloat tx, CGFloat ty)
{
   return( CGAffineTransformMakeTranslationInline( tx, ty));
}


static inline CGAffineTransform   CGAffineTransformMakeScale( CGFloat sx, CGFloat sy)
{
   return( CGAffineTransformMakeScaleInline( sx, sy));
}


static inline CGAffineTransform   CGAffineTransformMakeIdentity( void)
{
   return( CGAffineTransformMakeIdentityInline());
}


static inline CGAffineTransform   CGAffineTransformMake( CGFloat a,
                                                         CGFloat b,
                                                         CGFloat c,
                                                         CGFloat d,
                                                         CGFloat tx,
                                                         CGFloat ty)
{
   return( CGAffineTransformMakeInline( a, b, c, d, tx, ty));
}


CGAffineTransform   CGAffineTransformConcat( CGAffineTransform t2, CGAffineTransform t1);
CGAffineTransform   CGAffineTransformMakeRotation( CGFloat angle);
CGAffineTransform   CGAffineTransformInvert( CGAffineTransform t);

CGAffineTransform   CGAffineTransformRotate( CGAffineTransform t, CGFloat radians);
CGAffineTransform   CGAffineTransformScale( CGAffineTransform t, CGFloat sx, CGFloat sy);
CGAffineTransform   CGAffineTransformTranslate( CGAffineTransform t, CGFloat tx, CGFloat ty);

CGPoint  CGPointApplyAffineTransform( CGPoint point, CGAffineTransform t);
CGSize   CGSizeApplyAffineTransform( CGSize size, CGAffineTransform t);
CGRect   CGRectApplyAffineTransform( CGRect rect, CGAffineTransform t);

// useful for scalings of UIViews may be
CGRect   CGRectApplyAffineTransformAtAnchorPoint( CGRect rect,
                                                  CGAffineTransform t,
                                                  CGPoint anchorPoint);

// mulle additions
CGAffineTransform   CGAffineTransformMakeProjection( CGRect frame, CGRect bounds);
CGAffineTransform   CGAffineTransformMakeInverseProjection( CGRect frame, CGRect bounds);

#if 0
CGAffineTransform
   CGAffineTransformConcatFrameAndBoundsTransform( CGAffineTransform transform,
                                                   CGRect frame,
                                                   CGRect bounds);

CGAffineTransform
   CGAffineTransformConcatFrameAndBoundsInverseTransform( CGAffineTransform transform,
                                                          CGRect frame,
                                                          CGRect bounds);
#endif

#endif
