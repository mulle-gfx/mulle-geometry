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

#include "include.h"

#include <float.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <math.h>

typedef float CGFloat;

#define CGFLOAT_MIN         FLT_MIN
#define CGFLOAT_MAX         FLT_MAX
#define CGFLOAT_IS_DOUBLE   0



#ifndef M_PI_f
# define M_PI_f     ((float) 3.141592653589793238462643383279502884L)
#endif

#ifndef M_PI_2f
# define M_PI_2f    ((float) (3.141592653589793238462643383279502884L / 2.0))
#endif

// this doesn't exist usually (or name it M_2_PI_f ??)
#define CA_2xPI  ((float) (3.141592653589793238462643383279502884L * 2.0))



// mulle addition:
static inline CGFloat   CGFloatMakeRandom( void)
{
   return( (CGFloat) (rand() * 1.0 / RAND_MAX));
}


static inline CGFloat   CGFloatMakeRandomMinMax( CGFloat min, CGFloat max)
{
   return( CGFloatMakeRandom() * (max - min) + min);
}


MULLE__GEOMETRY_EXTERN_GLOBAL
CGFloat   CGFloatMakeNormalDistributionMinMax( CGFloat min, CGFloat max);

static inline double   CGFloatMakeNormalDistribution( void)
{
   return( CGFloatMakeNormalDistributionMinMax( 0.0, 1.0));
}


MULLE__GEOMETRY_EXTERN_GLOBAL
CGFloat   CGFloatMakeAbnormalDistributionMinMax( CGFloat min, CGFloat max);

static inline double   CGFloatMakeAbnormalDistribution( void)
{
   return( CGFloatMakeAbnormalDistributionMinMax( 0.0, 1.0));
}


static inline CGFloat   CGFloatMaximum( CGFloat x, CGFloat y)
{
   return( x > y ? x : y);
}


static inline CGFloat   CGFloatMinimum( CGFloat x, CGFloat y)
{
   return( x < y ? x : y);
}


static inline CGFloat   CGFloatAbsolute( CGFloat x)
{
   return( x >= 0.0f ? x : -x);
}


static inline CGFloat   CGFloatSign( CGFloat x)
{
   return( x >= 0.0f ? 1.0f : -1.0f);
}


static inline CGFloat   CGFloatClamp( CGFloat x)
{
   return( x > 1.0f ? 1.0f : (x < 0.0f ? 0.0f : x));
}


static inline CGFloat   CGFloatClampToMinMax( CGFloat x, CGFloat min, CGFloat max)
{
   return( x > max ? max : (x < min ? min : x));
}


// this is p5js.map, but "project" sounds more like what it does IMO
static inline CGFloat   CGFloatProject( CGFloat value,
                                        CGFloat start1, CGFloat stop1,
                                        CGFloat start2, CGFloat stop2)
{
   return( (value - start1) / (stop1 - start1) * (stop2 - start2) + start2);
}


static inline CGFloat   CGFloatProjectClamped( CGFloat value,
                                               CGFloat start1, CGFloat stop1,
                                               CGFloat start2, CGFloat stop2)
{
   CGFloat   projected;

   projected = CGFloatProject( value, start1, stop1, start1, stop2);
   if( start2 < stop2)
      return( CGFloatClampToMinMax( projected, start2, stop2));
   return( CGFloatClampToMinMax( projected, stop2, start2));
}




#ifdef __GNUC__
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

static inline int  CGFloatIsNegativeZero( CGFloat x)
{
   if( x != 0.0f)
      return( 1);

   if( sizeof( CGFloat) == sizeof( int32_t))
   {
      return( (*(int32_t *) &x) < 0);
   }
   if( sizeof( CGFloat) == sizeof( int64_t))
   {
      return( (*(int64_t *) &x) < 0);
   }
   assert( 0);
}

#ifdef __GNUC__
# pragma GCC diagnostic pop
#endif


// used for CGPoint.value and CGSize.value
enum
{
   CGHorizontalAxisIndex = 0,
   CGVerticalAxisIndex   = 1
};



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


static inline CGRect   CGRectMakeWithSize( CGSize size)
{
   return( (CGRect){
                     .origin = { 0.0f, 0.0f },
                     .size   = size
                   });
}



static inline CGRect   CGRectMakeWithOriginAndSize( CGPoint origin, CGSize size)
{
   return( (CGRect){
                     .origin = origin,
                     .size   = size
                   });
}


static inline CGRect   CGRectMakeWithPoints( CGPoint a, CGPoint b)
{
   CGRect   r;

   if( a.x >= b.x)
   {
      r.origin.x   = b.x;
      r.size.width = a.x - b.x;
   }
   else
   {
      r.origin.x   = a.x;
      r.size.width = b.x - a.x;
   }

   if( a.y >= b.y)
   {
      r.origin.y    = b.y;
      r.size.height = a.y - b.y;
   }
   else
   {
      r.origin.y    = a.y;
      r.size.height = b.y - a.y;
   }
   return( r);
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


//
// MEMO: Using extern const is probably the worst way to initialize to zero
//       as it can't be inlined and forces read/write, though we only need to
//       write.
//       Now, with a macro, you can't take the address of a CGRectZero
//       anymore though.
//
#define CGPointZero       CGPointMake( 0, 0)
#define CGPointUZero      CGPointUMake( 0, 0)
#define CGPointNull       CGPointMake( NAN, NAN)
#define CGSizeZero        CGSizeMake( 0, 0)
#define CGSizeUZero       CGSizeUMake( 0, 0)
// there is no CGSizeNull
#define CGVectorZero      CGVectorMake( 0, 0)
#define CGVectorUZero     CGVectorUMake( 0, 0)

#define CGRectZero        CGRectMake( 0, 0, 0, 0)
#define CGRectUZero       CGRectUMake( 0, 0, 0, 0)
#define CGRectNull        CGRectMake( NAN, NAN, 0.0f, 0.0f)
#define CGRectUNull       CGRectUMake( NAN, NAN, 0.0f, 0.0f)
#define CGRectInfinite    CGRectMake( -INFINITY, -INFINITY, INFINITY, INFINITY)
#define CGRectUInfinite   CGRectUMake( -INFINITY, -INFINITY, INFINITY, INFINITY)


#ifdef __GNUC__
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif


//--------------- modern API ------


static inline CGPoint   CGPointMakeWithSize( CGSize v)
{
   return( *(CGPoint *) &v);
}

static inline CGPoint   CGPointMakeWithVector( CGVector v)
{
   return( *(CGPoint *) &v);
}

static inline CGPoint   CGPointMakeWithPointU( CGPointU v)
{
   return( *(CGPoint *) &v);
}



static inline CGVector   CGVectorMakeWithPoint( CGPoint v)
{
   return( *(CGVector *) &v);
}


static inline CGVector   CGVectorMakeWithVectorU( CGVectorU v)
{
   return( *(CGVector *) &v);
}


static inline CGVector   CGVectorMakeWithSize( CGSize v)
{
   return( *(CGVector *) &v);
}



static inline CGSize   CGSizeMakeWithPoint( CGPoint v)
{
   return( *(CGSize *) &v);
}


static inline CGSize   CGSizeMakeWithSizeU( CGSizeU v)
{
   return( *(CGSize *) &v);
}


static inline CGSize   CGSizeMakeWithVector( CGVector v)
{
   return( *(CGSize *) &v);
}


static inline CGRect   CGRectMakeWithRectU( CGRectU v)
{
   return( *(CGRect *) &v);
}




static inline CGPointU   CGPointUMakeWithPoint( CGPoint v)
{
   return( *(CGPointU *) &v);
}


static inline CGVectorU   CGVectorUMakeWithVector( CGVector v)
{
   return( *(CGVectorU *) &v);
}


static inline CGSizeU   CGSizeUMakeWithSize( CGSize v)
{
   return( *(CGSizeU *) &v);
}


static inline CGRectU   CGRectUMakeWithRect( CGRect v)
{
   return( *(CGRectU *) &v);
}


// ----- old API - phased out, because MakeWith is more common
//                 than As

static inline CGPoint   CGSizeAsPoint( CGSize v)
{
   return( *(CGPoint *) &v);
}


static inline CGSize   CGPointAsSize( CGPoint v)
{
   return( *(CGSize *) &v);
}


static inline CGVector   CGPointAsVector( CGPoint v)
{
   return( *(CGVector *) &v);
}


static inline CGPoint   CGVectorAsPoint( CGVector v)
{
   return( *(CGPoint *) &v);
}


static inline CGVector   CGSizeAsVector( CGSize v)
{
   return( *(CGVector *) &v);
}


static inline CGSize   CGVectorAsSize( CGVector v)
{
   return( *(CGSize *) &v);
}



static inline CGPoint   CGPointUAsPoint( CGPointU v)
{
   return( *(CGPoint *) &v);
}


static inline CGPointU   CGPointAsPointU( CGPoint v)
{
   return( *(CGPointU *) &v);
}


static inline CGVector   CGVectorUAsVector( CGVectorU v)
{
   return( *(CGVector *) &v);
}


static inline CGVectorU   CGVectorAsVectorU( CGVector v)
{
   return( *(CGVectorU *) &v);
}


static inline CGSize   CGSizeUAsSize( CGSizeU v)
{
   return( *(CGSize *) &v);
}


static inline CGSizeU   CGSizeAsSizeU( CGSize v)
{
   return( *(CGSizeU *) &v);
}



static inline CGRect   CGRectUAsRect( CGRectU v)
{
   return( *(CGRect *) &v);
}


static inline CGRectU   CGRectAsRectU( CGRect v)
{
   return( *(CGRectU *) &v);
}


static inline int   CGRectIsNull( CGRect v)
{
   return( isnan( v.origin.x));
}


static inline int   CGPointIsNull( CGPoint p)
{
   return( isnan( p.x));
}



#ifdef __GNUC__
# pragma GCC diagnostic pop
#endif


#endif
