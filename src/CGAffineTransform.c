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
   1.0f,
   0.0f,
   0.0f,
   1.0f,
   0.0f,
   0.0f
};




CGAffineTransform   CGAffineTransformMakeRotation( CGFloat angle)
{
   return( CGAffineTransformMakeRotationInline( angle));
}


CGAffineTransform   CGAffineTransformConcat( CGAffineTransform t2, CGAffineTransform t1)
{
   return( CGAffineTransformConcatInline( t2, t1));
}


CGAffineTransform CGAffineTransformRotate( CGAffineTransform t, float angle)
{
   return( CGAffineTransformRotateInline( t, angle));
}



CGAffineTransform   CGAffineTransformTranslate( CGAffineTransform t, float tx, float ty)
{
   return( CGAffineTransformTranslateInline( t, tx, ty));
}


CGAffineTransform CGAffineTransformScale( CGAffineTransform t, float sx, float sy)
{
   return( CGAffineTransformScaleInline( t, sx, sy));
}



CGAffineTransform   CGAffineTransformInvert( CGAffineTransform t)
{
   return( CGAffineTransformInvertInline( t));
}



int   CGAffineTransformIsIdentity( CGAffineTransform t)
{
    return( ! memcmp( &t, &CGAffineTransformIdentity, sizeof( CGAffineTransform)));
}


int   CGAffineTransformEqualToTransform( CGAffineTransform t2, CGAffineTransform t1)
{
    return( ! memcmp( &t1, &t2, sizeof( CGAffineTransform)));
}

//
// MEMO: the ifs probably preclude too much compiler optimization where
//       steps can be computed and combined with other steps
//
CGAffineTransform   CGAffineTransformMakeProjection( CGRect frame, CGRect bounds)
{
   CGAffineTransform   t;
   CGPoint             scale;

   t = CGAffineTransformMakeTranslationInline( -bounds.origin.x, -bounds.origin.y);

   //
   // now translate bounds for context
   //
   //if( ! CGSizeEqualToSize( frame.size, bounds.size))
   {
      scale.x = frame.size.width  / bounds.size.width;
      scale.y = frame.size.height / bounds.size.height;
      t   = CGAffineTransformConcatInline( t, CGAffineTransformMakeScaleInline( scale.x, scale.y));
   }

   //if( ! CGPointEqualToPoint( frame.origin, CGPointMake( 0, 0)))
      t = CGAffineTransformConcatInline( t, CGAffineTransformMakeTranslationInline( frame.origin.x,
                                                                        frame.origin.y));
   return( t);
}


// could also do
CGAffineTransform   CGAffineTransformMakeInverseProjection( CGRect frame, CGRect bounds)
{
   CGPoint             scale;
   CGAffineTransform   t;

   t = CGAffineTransformMakeTranslationInline( bounds.origin.x, bounds.origin.y);
   //if( ! CGSizeEqualToSize( frame.size, bounds.size))
   {
      scale.x = bounds.size.width / frame.size.width;
      scale.y = bounds.size.height / frame.size.height;

      t   = CGAffineTransformConcat( CGAffineTransformMakeScaleInline( scale.x, scale.y),
                                     t);
   }

   //if( ! CGPointEqualToPoint( bounds.origin, CGPointMake( 0, 0)))
      t = CGAffineTransformConcat( CGAffineTransformMakeTranslationInline( -frame.origin.x,
                                                                           -frame.origin.y),
                                   t);
   return( t);
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

// -original-

