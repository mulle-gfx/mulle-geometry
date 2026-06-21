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


// CGAffineTransform is structurally identical to the nanoVG transform
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


MULLE__GEOMETRY_GLOBAL
const CGAffineTransform   CGAffineTransformIdentity;

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



static inline CGAffineTransform   CGAffineTransformMakeTranslationInline( CGPoint t)
{
   return( (CGAffineTransform){ .a = 1.0,
                                .b = 0.0,
                                .c = 0.0,
                                .d = 1.0,
                                .tx = t.x,
                                .ty = t.y });
}


static inline CGAffineTransform   CGAffineTransformMakeScaleInline( CGVector s)
{
   return( (CGAffineTransform){ .a = s.dx,
                                .b = 0.0,
                                .c = 0.0,
                                .d = s.dy,
                                .tx = 0.0,
                                .ty = 0.0 });
}


static inline CGAffineTransform   CGAffineTransformMakeSkewInline( CAAngle sx, CAAngle sy)
{
   return( (CGAffineTransform){ .a = 1.0,
                                .b = tanf( CAAngleToRadians( sy)),
                                .c = tanf( CAAngleToRadians( sx)),
                                .d = 1.0,
                                .tx = 0.0,
                                .ty = 0.0 });
}


static inline CGAffineTransform   CGAffineTransformMakeSkewXInline( CAAngle sx)
{
   return( (CGAffineTransform){ .a  = 1.0,
                                .b  = 0.0,
                                .c  = tanf( CAAngleToRadians( sx)),
                                .d  = 1.0,
                                .tx = 0.0,
                                .ty = 0.0 });
}


static inline CGAffineTransform   CGAffineTransformMakeSkewYInline( CAAngle sy)
{
   return( (CGAffineTransform){ .a  = 1.0,
                                .b  = tanf( CAAngleToRadians( sy)),
                                .c  = 0.0,
                                .d  = 1.0,
                                .tx = 0.0,
                                .ty = 0.0 });
}


#define CG_ANGLE_IS_NEAR( val, comp, amt) (fabs((val) - (comp)) <= (amt))

//
// Many of these functions are actually too big for inlining
// nevertheless especially for constants there might be nice gains
// So its up to you, to choose either the inlining version or the
// non-inlining version
//
static inline CGAffineTransform   _CGAffineTransformMakeRotationRadiansInline( CGFloat radians, CGFloat tooSmall)
{
   CGAffineTransform   ret;

   if( CG_ANGLE_IS_NEAR( radians, ((CGFloat) 3.14159265358979323846f / 2.0f), tooSmall))
   {
      ret.a  = 0;
      ret.b  = 1.0f;
      ret.c  = -1.0f;
      ret.d  = 0.0f;
      ret.tx = 0.0f;
      ret.ty = 0.0f;

      return ret;
   }

   ret.a  = cosf( radians);
   ret.b  = sinf( radians);
   ret.c  = -ret.b;
   ret.d  = ret.a;
   ret.tx = 0.0f;
   ret.ty = 0.0f;

   return ret;
}


static inline CGAffineTransform   CGAffineTransformMakeRotationRadiansInline( CGFloat radians)
{
   return( _CGAffineTransformMakeRotationRadiansInline( radians, 1e-6));
}


static inline CGAffineTransform   CGAffineTransformMakeRotationInline( CARotation rotation)
{
   CGFloat   radians;

   radians = CARotationToRadiansAsRotation( rotation);
   return( CGAffineTransformMakeRotationRadiansInline( radians));
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
   _CGAffineTransformRotateRadiansInline( CGAffineTransform t, CGFloat value, CGFloat tooSmall)
{
    CGAffineTransform   ret;

    ret = _CGAffineTransformMakeRotationRadiansInline( value, tooSmall);
    ret = CGAffineTransformConcatInline( ret, t);

    return ret;
}

static inline CGAffineTransform
   CGAffineTransformRotateRadiansInline( CGAffineTransform t, CGFloat value)
{
    return( _CGAffineTransformRotateRadiansInline( t, value, 1e-6));
}


static inline CGAffineTransform
   CGAffineTransformRotateInline( CGAffineTransform t, CARotation rotation)
{
    CGAffineTransform   ret;

    ret = CGAffineTransformMakeRotationInline( rotation);
    ret = CGAffineTransformConcatInline( ret, t);

    return ret;
}

static inline CGAffineTransform
   CGAffineTransformTranslateInline( CGAffineTransform t, CGPoint point)
{
    CGAffineTransform   ret;

    ret = CGAffineTransformMakeTranslationInline( point);
    ret = CGAffineTransformConcatInline( ret, t);

    return ret;
}


static inline CGAffineTransform
   CGAffineTransformScaleInline( CGAffineTransform t, CGVector s)
{
    CGAffineTransform   ret;

    ret = CGAffineTransformMakeScaleInline( s);
    return( CGAffineTransformConcatInline( ret, t));
}


static inline CGAffineTransform
   _CGAffineTransformInvertInline( CGAffineTransform t, CGFloat tooSmall)
{
   CGAffineTransform   ret;
   float               determinant;

   determinant = (t.a * t.d - t.c * t.b);
   if( determinant > tooSmall && determinant < tooSmall)
   {
      return( CGAffineTransformMakeIdentityInline());
   }

   ret.a  = t.d / determinant;
   ret.b  = -t.b / determinant;
   ret.c  = -t.c / determinant;
   ret.d  = t.a / determinant;
   ret.tx = (-t.d * t.tx + t.c * t.ty) / determinant;
   ret.ty = (t.b * t.tx - t.a * t.ty) / determinant;

   return ret;
}


static inline CGAffineTransform
   CGAffineTransformInvertInline( CGAffineTransform t)
{
   return( _CGAffineTransformInvertInline( t, 1e-6));
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
        .width  = (CGFloat) ((double) t.a * size.width + (double) t.c * size.height),
        .height = (CGFloat) ((double) t.b * size.width + (double) t.d * size.height)
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


// this is likely to be profitable to be inlined
static inline int   CGAffineTransformIsIdentityInline( CGAffineTransform t)
{
   return( t.a == 1.0f &&
           t.b == 0.0f &&
           t.c == 0.0f &&
           t.d == 1.0f &&
           t.tx == 0.0f &&
           t.ty == 0.0f);
}

// this is likely to be profitable to be inlined
static inline int   CGAffineTransformEqualToTransformInline( CGAffineTransform t1, CGAffineTransform t2)
{
   return(  t1.a == t2.a && 
            t1.b == t2.b &&    
            t1.c == t2.c &&    
            t1.d == t2.d &&    
            t1.tx == t2.tx &&    
            t1.ty == t2.ty);   
}


MULLE__GEOMETRY_GLOBAL
int   CGAffineTransformIsIdentity( CGAffineTransform t);

MULLE__GEOMETRY_GLOBAL
int   CGAffineTransformEqualToTransform( CGAffineTransform t1, CGAffineTransform t2);



/**
 */

/*
 * Non-inlining counterparts (except when still inlining :))
 */

//
// Not inlining makes no sense IMO for the next few functions, so they
// actually still are inlining. But in code, we don't want to think 
// about if the function does exist or not, so give them non-inlining
// names...
static inline CGAffineTransform   CGAffineTransformMakeTranslation( CGPoint t)
{
   return( CGAffineTransformMakeTranslationInline( t));
}


static inline CGAffineTransform   CGAffineTransformMakeScale( CGVector s)
{
   return( CGAffineTransformMakeScaleInline( s));
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


// now truely non-inlining functions, where it makes sense (IMO)

MULLE__GEOMETRY_GLOBAL
CGAffineTransform   CGAffineTransformConcat( CGAffineTransform t2, CGAffineTransform t1);

// rotation is usally 0 to +1, but can be anything
MULLE__GEOMETRY_GLOBAL
CGAffineTransform   CGAffineTransformMakeRotationRadians( CGFloat radians);

MULLE__GEOMETRY_GLOBAL
CGAffineTransform   CGAffineTransformRotateRadians( CGAffineTransform t, CGFloat radians);

MULLE__GEOMETRY_GLOBAL
CGAffineTransform   CGAffineTransformInvert( CGAffineTransform t);

MULLE__GEOMETRY_GLOBAL
CGAffineTransform   CGAffineTransformScale( CGAffineTransform t, CGVector s);

MULLE__GEOMETRY_GLOBAL
CGAffineTransform   CGAffineTransformTranslate( CGAffineTransform t, CGPoint point);

MULLE__GEOMETRY_GLOBAL
CGPoint             CGPointApplyAffineTransform( CGPoint point, CGAffineTransform t);

MULLE__GEOMETRY_GLOBAL
CGSize              CGSizeApplyAffineTransform( CGSize size, CGAffineTransform t);

MULLE__GEOMETRY_GLOBAL
CGRect              CGRectApplyAffineTransform( CGRect rect, CGAffineTransform t);


static inline CGAffineTransform   CGAffineTransformMakeRotation( CARotation rotation)
{
   CGFloat   radians;

   radians = CARotationToRadiansAsRotation( rotation);
   return( CGAffineTransformMakeRotationRadians( radians));
}

static inline CGAffineTransform   CGAffineTransformRotate( CGAffineTransform t, CARotation rotation)
{
   CGFloat   radians;

   radians = CARotationToRadiansAsRotation( rotation);
   return( CGAffineTransformRotateRadians( t, radians));
}


// useful for scalings of UIViews maybe
MULLE__GEOMETRY_GLOBAL
CGRect   CGRectApplyAffineTransformAtAnchorPoint( CGRect rect,
                                                  CGAffineTransform t,
                                                  CGPoint anchorPoint);

// mulle additions
MULLE__GEOMETRY_GLOBAL
CGAffineTransform   CGAffineTransformMakeProjection( CGRect frame, CGRect bounds);

MULLE__GEOMETRY_GLOBAL
CGAffineTransform   CGAffineTransformMakeInverseProjection( CGRect frame, CGRect bounds);

// the standard rotation around a center point, translates rotates and then
// translates back
MULLE__GEOMETRY_GLOBAL
CGAffineTransform   CGAffineTransformMakeRotationAroundPoint( CGPoint center, CARotation rotation);

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


static inline CGPoint   CGPointProjectToFrame( CGPoint location,
                                                CGRect frame,
                                                CGRect bounds)
{
   CGPoint             point;
   CGAffineTransform   transform;
//   CGAffineTransform   identity;

//   identity   = CGAffineTransformMakeIdentity();
//   nvgTransformIdentity( transform);
//   transform = CGAffineTransformConcatFrameAndBoundsInverseTransform( identity, [self frame], [self bounds]);
   transform = CGAffineTransformMakeProjection( frame, bounds);
   point     = CGPointApplyAffineTransformInline( location, transform);
//   nvgTransformPoint( &translated.x, &translated.y, transform, point.x, point.y);
   return( point);
}


static inline CGPoint   CGPointProjectToBounds( CGPoint location,
                                                CGRect frame,
                                                CGRect bounds)
{
   CGPoint             point;
   CGAffineTransform   transform;

//   CGAffineTransform   identity;
//   identity   = CGAffineTransformMakeIdentity();
//   nvgTransformIdentity( transform);
   transform = CGAffineTransformMakeInverseProjection( frame, bounds);
//   transform = CGAffineTransformConcatFrameAndBoundsTransform( identity, [self frame], [self bounds]);
   point     = CGPointApplyAffineTransformInline( location, transform);
//   nvgTransformPoint( &translated.x, &translated.y, transform, point.x, point.y);
   return( point);
}


MULLE__GEOMETRY_GLOBAL
CGRect   _CGRectRotate( CGRect rect, CARotation rotation);


static inline CGRect  CGRectRotate( CGRect rect, CARotation rotation)
{
   return( rotation == 0.0f ? rect : _CGRectRotate( rect, rotation));
}



#endif
