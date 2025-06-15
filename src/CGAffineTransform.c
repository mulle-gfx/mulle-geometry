//******************************************************************************
//
// Copyright (c) 2016 Microsoft Corporation. All rights reserved.
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

#include "CGAffineTransform.h"

#include <math.h>
#include <string.h>



const CGAffineTransform   CGAffineTransformIdentity =
{
   .a  = 1.0f,
   .b  = 0.0f,
   .c  = 0.0f,
   .d  = 1.0f,
   .tx = 0.0f,
   .ty = 0.0f 
};


int   CGAffineTransformIsIdentity( CGAffineTransform t)
{
   return( CGAffineTransformIsIdentityInline( t));
}


int   CGAffineTransformEqualToTransform( CGAffineTransform t1, CGAffineTransform t2)
{
   return( CGAffineTransformEqualToTransformInline( t1, t2));
}



CGAffineTransform   CGAffineTransformConcat( CGAffineTransform t2, CGAffineTransform t1)
{
   return( CGAffineTransformConcatInline( t2, t1));
}

CGAffineTransform   CGAffineTransformTranslate( CGAffineTransform t, CGPoint point)
{
   return( CGAffineTransformTranslateInline( t, point));
}


CGAffineTransform CGAffineTransformScale( CGAffineTransform t, CGVector s)
{
   return( CGAffineTransformScaleInline( t, s));
}



CGAffineTransform   CGAffineTransformInvert( CGAffineTransform t)
{
   return( CGAffineTransformInvertInline( t));
}


CGPoint   CGPointApplyAffineTransform( CGPoint point, CGAffineTransform t)
{
   return( CGPointApplyAffineTransformInline( point, t));
}


CGSize   CGSizeApplyAffineTransform( CGSize size, CGAffineTransform t)
{
   return( CGSizeApplyAffineTransformInline( size, t));
}

CGRect   CGRectApplyAffineTransform( CGRect rect, CGAffineTransform t)
{
   return( CGRectApplyAffineTransformInline( rect, t));
}

CGAffineTransform   CGAffineTransformMakeRotationRadians( CGFloat radians)
{
   return( CGAffineTransformMakeRotationRadiansInline( radians));
}


CGAffineTransform   CGAffineTransformRotateRadians( CGAffineTransform t, CGFloat radians)
{
   return( CGAffineTransformRotateRadiansInline( t, radians));
}



// center as anchor point of rect is 0.5,0.5
CGRect   CGRectApplyAffineTransformAtAnchorPoint( CGRect rect,
                                                  CGAffineTransform t,
                                                  CGPoint anchorPoint)
{
   CGAffineTransform  s;   // trust the compiler optimizer will reduce this
   CGAffineTransform  u;
   CGAffineTransform  v;
   CGAffineTransform  w;
   CGPoint            transformCenter;

   transformCenter.x = CGRectGetMinX( rect) + CGRectGetWidth( rect) * anchorPoint.x;
   transformCenter.y = CGRectGetMinY( rect) + CGRectGetHeight( rect) * anchorPoint.y;

   s = CGAffineTransformMakeTranslationInline( CGPointNegate( transformCenter));
   u = CGAffineTransformConcatInline( s, t);

   v = CGAffineTransformMakeTranslationInline( transformCenter);
   w = CGAffineTransformConcatInline( u, v);

   return( CGRectApplyAffineTransformInline( rect, w));
}


//
// MEMO: the ifs probably preclude too much compiler optimization where
//       steps can be computed and combined with other steps
//
CGAffineTransform   CGAffineTransformMakeProjection( CGRect frame, CGRect bounds)
{
   CGAffineTransform   t;
   CGVector            scale;

   t = CGAffineTransformMakeTranslationInline( CGPointNegate( bounds.origin));

   //
   // now translate bounds for context
   //
   //if( ! CGSizeEqualToSize( frame.size, bounds.size))
   {
      scale.dx = frame.size.width  / bounds.size.width;
      scale.dy = frame.size.height / bounds.size.height;
      t        = CGAffineTransformConcatInline( t, CGAffineTransformMakeScaleInline( scale));
   }

   //if( ! CGPointEqualToPoint( frame.origin, CGPointMake( 0, 0)))
      t = CGAffineTransformConcatInline( t,
                                         CGAffineTransformMakeTranslationInline( frame.origin));
   return( t);
}


// could also do
CGAffineTransform   CGAffineTransformMakeInverseProjection( CGRect frame, CGRect bounds)
{
   CGVector            scale;
   CGAffineTransform   t;
   CGPoint             point;

   t = CGAffineTransformMakeTranslationInline( bounds.origin);
   //if( ! CGSizeEqualToSize( frame.size, bounds.size))
   {
      scale.dx = bounds.size.width / frame.size.width;
      scale.dy = bounds.size.height / frame.size.height;

      t   = CGAffineTransformConcatInline( CGAffineTransformMakeScaleInline( scale),
                                           t);
   }

   //if( ! CGPointEqualToPoint( bounds.origin, CGPointMake( 0, 0)))
      point = CGPointNegate( frame.origin);
      t     = CGAffineTransformConcatInline( CGAffineTransformMakeTranslationInline( point),
                                             t);
   return( t);
}


CGRect   _CGRectRotate( CGRect rect, CARotation rotation)
{
   CGPoint             center;
   CGAffineTransform   xform;

   center = CGRectGetCenter( rect);
   xform  = CGAffineTransformMakeTranslationInline( CGPointNegate( center));
   xform  = CGAffineTransformConcatInline( CGAffineTransformMakeRotationInline( rotation), xform);
   xform  = CGAffineTransformConcatInline( CGAffineTransformMakeTranslationInline( center), xform);
   return( CGRectApplyAffineTransform( rect, xform));
}



CGAffineTransform   CGAffineTransformMakeRotationAroundPoint( CGPoint center,
                                                              CARotation rotation)
{
   CGAffineTransform   translate;
   CGAffineTransform   translateBack;
   CGAffineTransform   rotate;
   CGAffineTransform   combined;

   translate     = CGAffineTransformMakeTranslationInline( CGPointNegate( center));
   rotate        = CGAffineTransformMakeRotationInline( rotation);
   combined      = CGAffineTransformConcatInline( translate, rotate);
   translateBack = CGAffineTransformMakeTranslationInline( center);
   return( CGAffineTransformConcatInline( combined, translateBack));
}


#if 0
//
// Transform for incoming values of the superview to translate into
// bounds space of the view. For hit tests.
//

CGAffineTransform
   CGAffineTransformConcatFrameAndBoundsTransform( CGAffineTransform transform,
                                                   CGRect frame,
                                                   CGRect bounds)

{
   CGPoint             scale;
   CGAffineTransform   tmp;
   CGAffineTransform   xform;

   tmp   = CGAffineTransformMakeTranslationInline( bounds.origin.x, bounds.origin.y);
//   nvgTransformTranslate( tmp, bounds.origin.x, bounds.origin.y);
   xform = CGAffineTransformConcatInline( tmp, transform);
//   nvgTransformPremultiply( transform, tmp);

   scale.x = bounds.size.width / frame.size.width;
   scale.y = bounds.size.height / frame.size.height;

   tmp  = CGAffineTransformMakeScaleInline( scale.x, scale.y);
//   nvgTransformScale( tmp, scale.x, scale.y);
   xform = CGAffineTransformConcatInline( tmp, xform);
//   nvgTransformPremultiply( transform, tmp);

   tmp = CGAffineTransformMakeTranslationInline( -frame.origin.x, -frame.origin.y);
//   nvgTransformTranslate( tmp, -frame.origin.x, -frame.origin.y);
   tmp = CGAffineTransformConcatInline( tmp, xform);
//   nvgTransformPremultiply( transform, tmp);
   return( tmp);
}



CGAffineTransform
   CGAffineTransformConcatFrameAndBoundsInverseTransform( CGAffineTransform transform,
                                                          CGRect frame,
                                                          CGRect bounds)

{
   CGPoint             scale;
   CGAffineTransform   tmp;
   CGAffineTransform   xform;

   tmp   = CGAffineTransformMakeTranslationInline( frame.origin.x, frame.origin.y);
//   nvgTransformTranslate( tmp, bounds.origin.x, bounds.origin.y);
   xform = CGAffineTransformConcatInline( tmp, transform);
//   nvgTransformPremultiply( transform, tmp);

   scale.x = frame.size.width / bounds.size.width;
   scale.y = frame.size.height / bounds.size.height;

   tmp  = CGAffineTransformMakeScaleInline( scale.x, scale.y);
//   nvgTransformScale( tmp, scale.x, scale.y);
   xform = CGAffineTransformConcatInline( tmp, xform);
//   nvgTransformPremultiply( transform, tmp);

   tmp = CGAffineTransformMakeTranslationInline( -bounds.origin.x, -bounds.origin.y);
//   nvgTransformTranslate( tmp, -frame.origin.x, -frame.origin.y);
   tmp = CGAffineTransformConcatInline( tmp, xform);
//   nvgTransformPremultiply( transform, tmp);
   return( tmp);
}
#endif

// -original-

