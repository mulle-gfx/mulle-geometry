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
#ifndef cg_base_h__
#define cg_base_h__

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


static inline CGPoint   CGPointMake(CGFloat x, CGFloat y)
{
   return( (CGPoint){ .x = x, .y = y });
}


//
// union is not CGPoint, because it's too inconvenient in the debugger
// and its not used that often.
//
typedef struct CGPointU
{
   union
   {
      struct
      {
         CGFloat  x;
         CGFloat  y;
      };
      CGFloat     value[ 2];
   };
} CGPointU;


static inline CGPointU   CGPointUMake(CGFloat x, CGFloat y)
{
   return( (CGPointU){ .x = x, .y = y });
}


static inline CGPoint   CGPointUGetPoint( CGPointU v)
{
   return( *(CGPoint *) &v);
}


static inline CGPointU   CGPointGetPointU( CGPoint v)
{
   return( *(CGPointU *) &v);
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


static inline CGSize   CGSizeMake( CGFloat w, CGFloat h)
{
   return( (CGSize){ .width = w, .height = h });
}


//
// this can be used for variable axis code like:
//    sizeu.value[ axis] += pointu.value[ axis];
// which is otherwise a pain to code with CGSizeGetAxisValue or
// CGSizeBySettingAxisValue
//
typedef struct CGSizeU
{
   union
   {
      struct
      {
         CGFloat  width;
         CGFloat  height;
      };
      CGFloat     value[ 2];
   };
} CGSizeU;


static inline CGSizeU   CGSizeUMake( CGFloat w, CGFloat h)
{
   return( (CGSizeU){ .width = w, .height = h });
}


static inline CGSize   CGSizeUGetSize( CGSizeU v)
{
   return( *(CGSize *) &v);
}


static inline CGSizeU   CGSizeGetSizeU( CGSize v)
{
   return( *(CGSizeU *) &v);
}


//
// hardly ever used. Use CAPolarPoint for real vector stuff
//
typedef struct CGVector
{
   CGFloat  dx;
   CGFloat  dy;
} CGVector;


static inline CGVector   CGVectorMake( CGFloat dx, CGFloat dy)
{
   return( (CGVector){ .dx = dx, .dy = dy });
}


typedef struct CGVectorU
{
   union
   {
      struct
      {
         CGFloat  dx;
         CGFloat  dy;
      };
      CGFloat     value[ 2];
   };
} CGVectorU;


static inline CGVectorU   CGVectorUMake( CGFloat dx, CGFloat dy)
{
   return( (CGVectorU){ .dx = dx, .dy = dy });
}


static inline CGVector   CGVectorUGetVector( CGVectorU v)
{
   return( *(CGVector *) &v);
}


static inline CGVectorU   CGVectorGetVectorU( CGVector v)
{
   return( *(CGVectorU *) &v);
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



static inline CGRect   CGRectMake(CGFloat x, CGFloat y, CGFloat w, CGFloat h)
{
   return( (CGRect){
                     .origin = (CGPoint) { .x = x, .y = y },
                     .size   = (CGSize)  { .width = w, .height = h }
                   });
}


typedef struct CGRectU
{
   CGPointU   origin;
   CGSizeU    size;
} CGRectU;


static inline CGRectU   CGRectUMake(CGFloat x, CGFloat y, CGFloat w, CGFloat h)
{
   return( (CGRectU){
                     .origin = (CGPointU) { .x = x, .y = y },
                     .size   = (CGSizeU)  { .width = w, .height = h }
                   });
}


static inline CGRect   CGRectUGetRect( CGRectU v)
{
   return( *(CGRect *) &v);
}


static inline CGRectU   CGRectGetRectU( CGRect v)
{
   return( *(CGRectU *) &v);
}


//
// MEMO: Using extern const is probably the worst way to initialize to zero
//       as it can't be inlined and forces read/write, though we only need to
//       write.
//       Now, with a macro, you can't take the address of a CGRectZero
//       anymore though.
//
#define CGPointZero       CGPointMake( 0, 0)
#define CGPointUZero      CGPointUMake( 0, 0)
#define CGPointNull       CGPointMake( -INFINITY, -INFINITY)
#define CGSizeZero        CGSizeMake( 0, 0)
#define CGSizeUZero       CGSizeUMake( 0, 0)
// there is no CGSizeNull
#define CGVectorZero      CGVectorMake( 0, 0)
#define CGVectorUZero     CGVectorUMake( 0, 0)

#define CGRectZero        CGRectMake( 0, 0, 0, 0)
#define CGRectUZero       CGRectUMake( 0, 0, 0, 0)
#define CGRectNull        CGRectMake( -INFINITY, -INFINITY, 0.0f, 0.0f)
#define CGRectUNull       CGRectUMake( NSIntegerMin, NSIntegerMin, 0.0f, 0.0f)
#define CGRectInfinite    CGRectMake( -INFINITY, -INFINITY, INFINITY, INFINITY)
#define CGRectUInfinite   CGRectUMake( NSIntegerMin, NSIntegerMin, NSIntegerMax, NSIntegerMax)

#endif
