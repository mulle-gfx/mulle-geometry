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
#ifndef __cg_base_h__
#define __cg_base_h__

#include <float.h>
#include <stdlib.h>
#include <assert.h>


typedef float CGFloat;

#define CGFLOAT_MIN         FLT_MIN
#define CGFLOAT_MAX         FLT_MAX
#define CGFLOAT_IS_DOUBLE   0



// mulle addition:
static inline CGFloat   CGFloatMakeRandom( void)
{
   return( (CGFloat) (rand() * 1.0 / RAND_MAX));
}



static inline CGFloat   CGFloatMaximum( CGFloat x, CGFloat y)
{
   return( x > y ? x : y);
}


static inline CGFloat   CGFloatMinimum( CGFloat x, CGFloat y)
{
   return( x < y ? x : y);
}


// used for CGPoint.value and CGSize.value
typedef enum
{
   CGHorizontalAxis = 0,
   CGVerticalAxis   = 1
} CGAxis;


// MEMO: the union, though convenient sometimes, might vanish, as the
//       debugging is a bit too painful with it. (or update the debugger ?)
//
//    * < point.y             pedantic ASCII art :)
//    ^
//    point.x
//
typedef struct CGPoint
{
	CGFloat  x;
	CGFloat  y;
} CGPoint;


static inline CGFloat  CGPointGetAxisValue( CGPoint p, CGAxis axis)
{
   return( axis == CGVerticalAxis ? p.y : p.x);
}


static inline CGPoint  CGPointBySettingAxisValue( CGPoint p, CGAxis axis, CGFloat value)
{
   if( axis == CGVerticalAxis)
      p.y = value;
   else
      p.x = value;
   return( p);
}

//
//    .-------------.
//    |             |
//    |             |
//    |             | size.height
//    |             |
//    |             |
//    '-------------'
//      size.width
//
typedef struct CGSize
{
	CGFloat  width;
	CGFloat  height;
} CGSize;


static inline CGFloat  CGSizeGetAxisValue( CGSize size, CGAxis axis)
{
   return( axis == CGVerticalAxis ? size.height : size.width);
}


static inline CGSize  CGSizeBySettingAxisValue( CGSize size, CGAxis axis, CGFloat value)
{
   if( axis == CGVerticalAxis)
      size.height = value;
   else
      size.width = value;
   return( size);
}


//
//              frame.origin.x
//                   v
//  frame.origin.y > .-------------.
//                   |             |
//                   |             |
//                   |             | frame.size.height
//                   |             |
//                   |             |
//                   '-------------'
//                   frame.size.width
//
typedef struct CGRect
{
    CGPoint  origin;
    CGSize   size;
} CGRect;


//
// hardly ever used. Use CAPolarPoint for real vector stuff
//
typedef struct CGVector
{
   CGFloat  dx;
   CGFloat  dy;
} CGVector;


static inline CGFloat  CGVectorGetAxisValue( CGVector v, CGAxis axis)
{
   return( axis == CGVerticalAxis ? v.dy : v.dx);
}


static inline CGVector  CGVectorBySettingAxisValue( CGVector v, CGAxis axis, CGFloat value)
{
   if( axis == CGVerticalAxis)
      v.dy = value;
   else
      v.dx = value;
   return( v);
}


//
// MEMO: Using extern const is probably the worst way to initialize to zero
//       as it can't be inlined and forces read/write, though we only need to
//       write.
//       Now, with a macro, you can't take the address of a CGRectZero
//       anymore though.
//
#define CGRectInfinite   CGRectMake( -INFINITY, -INFINITY, INFINITY, INFINITY)
#define CGPointZero      CGPointMake( 0, 0)
#define CGRectZero       CGRectMake( 0, 0, 0, 0)
#define CGSizeZero       CGSizeMake( 0, 0)
#define CGVectorZero     CGVectorMake( 0, 0)
#define CGRectNull       CGRectMake( -INFINITY, -INFINITY, 0.0f, 0.0)



static inline CGRect   CGRectMake(CGFloat x, CGFloat y, CGFloat w, CGFloat h)
{
   return( (CGRect){
                     .origin = (CGPoint) { .x = x, .y = y },
                     .size   = (CGSize)  { .width = w, .height = h }
                   });
}


static inline CGPoint   CGPointMake(CGFloat x, CGFloat y)
{
   return( (CGPoint){ .x = x, .y = y });
}


static inline CGSize   CGSizeMake( CGFloat w, CGFloat h)
{
   return( (CGSize){ .width = w, .height = h });
}

static inline CGVector   CGVectorMake( CGFloat dx, CGFloat dy)
{
   return( (CGVector){ .dx = dx, .dy = dy });
}


#endif
