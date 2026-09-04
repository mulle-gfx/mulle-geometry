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


#ifndef M_PI_f
# define M_PI_f     ((float) 3.141592653589793238462643383279502884L)
#endif

#ifndef M_PI_2f
# define M_PI_2f    ((float) (3.141592653589793238462643383279502884L / 2.0))
#endif

// M_PI is not part of the ISO C standard (C99, C11, C17, etc.).
// It is a POSIX extension (marked [XSI] in the POSIX spec).
#ifndef M_PI
# define M_PI       ((double) 3.141592653589793238462643383279502884L)
#endif

// this doesn't exist usually (or name it M_2_PI_f ??)
#define CA_2xPI     ((float) (3.141592653589793238462643383279502884L * 2.0))


//
// Epsilons — named by domain so they can be tuned independently.
// All currently 0.0001f but kept separate for future adjustment.
//
#ifndef CA_DISTANCE_EPSILON
# define CA_DISTANCE_EPSILON   0.0001f   // arc/linear distance snap-to-zero
#endif
#ifndef CA_ANGLE_EPSILON
# define CA_ANGLE_EPSILON      0.0001f   // angular dead-zone
#endif
#ifndef CG_LENGTH_EPSILON
# define CG_LENGTH_EPSILON     0.0001f   // vector/point length too-short-to-normalize
#endif


typedef float CGFloat;


static inline int   CGFloatEqualWithTolerance( CGFloat a, CGFloat b, CGFloat tolerance)
{
   return( fabsf( a - b) <= tolerance);
}

#define CGFLOAT_MIN         FLT_MIN
#define CGFLOAT_MAX         FLT_MAX
#define CGFLOAT_IS_DOUBLE   0


//
// for _mulle_objc_typeinfo_compare to CGFloat2 be compatible with CGPoint
// used by MulleCA
//
typedef struct CGFloat2
{
   CGFloat   v0;
   CGFloat   v1;
} CGFloat2;


static inline CGFloat2   CGFloat2Make( CGFloat a, CGFloat b)
{
   return( (CGFloat2){ .v0 = a + 0.0f, .v1 = b + 0.0f });
}

// Equality truth table (under -0.0 sanitization contract):
//
//   Case                 Result   Reason
//   +0.0 == +0.0         YES     identical bits
//   +0.0 == -0.0         n/a     -0.0 cannot exist (Make sanitizes)
//   NaN  == NaN          YES     both all-NaN → isnan fallback
//   NaN  == finite       NO      first check fails, isnan(b) fails
//   +INF == +INF         YES     same bit pattern
//   +INF == -INF         NO      different sign bit
//
static inline int   CGFloat2EqualToFloat2( CGFloat2 a, CGFloat2 b)
{
   // compatible to CGPoint but not neccessarily to CGSize.. is this ba
   if( (a.v0 == b.v0) && (a.v1 == b.v1))
      return( 1);
   // this seems fairly cheap
   return( isnan( a.v0) && isnan( b.v0) && isnan( a.v1) && isnan( b.v1));
}


typedef struct CGFloat3
{
   CGFloat   v0;
   CGFloat   v1;
   CGFloat   v2;
} CGFloat3;


static inline CGFloat3   CGFloat3Make( CGFloat a, CGFloat b, CGFloat c)
{
   return( (CGFloat3){ .v0 = a + 0.0f, .v1 = b + 0.0f, .v2 = c + 0.0f });
}

// Equality truth table (under -0.0 sanitization contract):
//
//   Case                 Result   Reason
//   +0.0 == +0.0         YES     memcmp: identical bits
//   +0.0 == -0.0         n/a     -0.0 cannot exist (Make sanitizes)
//   NaN  == NaN          YES     memcmp: same bit pattern (all NaN are quiet)
//   NaN  == finite       NO      memcmp: different bits
//   +INF == +INF         YES     memcmp: same bit pattern
//   +INF == -INF         NO      memcmp: different sign bit
//
// NOTE: two CGFloat3 created with different NaN payloads would compare
//       unequal — but no Make function produces distinct NaN payloads.
//
static inline int   CGFloat3EqualToFloat3( CGFloat3 a, CGFloat3 b)
{
   return( ! memcmp( &a, &b, sizeof( CGFloat3)));
}



typedef struct CGFloat4
{
   CGFloat   v0;
   CGFloat   v1;
   CGFloat   v2;
   CGFloat   v3;
} CGFloat4;


static inline CGFloat4   CGFloat4Make( CGFloat a, CGFloat b, CGFloat c, CGFloat d)
{
   return( (CGFloat4){ .v0 = a + 0.0f, .v1 = b + 0.0f, .v2 = c + 0.0f, .v3 = d + 0.0f });
}


// Equality: same memcmp semantics as CGFloat3EqualToFloat3 (see above).
static inline int   CGFloat4EqualToFloat4( CGFloat4 a, CGFloat4 b)
{
   return( ! memcmp( &a, &b, sizeof( CGFloat4)));
}



// mulle addition: returns a value between 0.0 and +1.0
static inline CGFloat   CGFloatMakeRandom( void)
{
   return( (CGFloat) (rand() * 1.0 / RAND_MAX));
}


static inline CGFloat   CGFloatMakeRandomMinMax( CGFloat min, CGFloat max)
{
   return( CGFloatMakeRandom() * (max - min) + min);
}


MULLE__GEOMETRY_GLOBAL
CGFloat   CGFloatMakeNormalDistributionMinMax( CGFloat min, CGFloat max);

static inline double   CGFloatMakeNormalDistribution( void)
{
   return( CGFloatMakeNormalDistributionMinMax( 0.0, 1.0));
}


MULLE__GEOMETRY_GLOBAL
CGFloat   CGFloatMakeAbnormalDistributionMinMax( CGFloat min, CGFloat max);

static inline double   CGFloatMakeAbnormalDistribution( void)
{
   return( CGFloatMakeAbnormalDistributionMinMax( 0.0, 1.0));
}


//
// https://bitbashing.io/comparing-floats.html
//
static inline CGFloat   CGFloatIsZero( CGFloat x)
{
   return( x == 0.0);
}

static inline CGFloat   CGFloatIsOne( CGFloat x)
{
   return( x == 1.0f);
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


static inline CGFloat   CGFloatClampToUnity( CGFloat x)
{
   return( x > 1.0f ? 1.0f : (x < 0.0f ? 0.0f : x));
}


static inline CGFloat   CGFloatModToUnity( CGFloat x)
{
   CGFloat    y;

   y = fmodf( x, 1.0f);
   if( y != 0.0f || x == 0.0f)
      return( y);
   return( x < 0 ? -1.0f : 1.0f);
}



static inline CGFloat   CGFloatClamp( CGFloat x, CGFloat min, CGFloat max)
{
   return( x > max ? max : (x < min ? min : x));
}


// this is p5js.map, but "project" sounds more like what it does IMO
// The CGFloatProject is used to project a number having range
// from start1 to stop2 unto a range of start2 to stop2.

static inline CGFloat   CGFloatProject( CGFloat value,
                                        CGFloat start1, CGFloat stop1,
                                        CGFloat start2, CGFloat stop2)
{
   return( (value - start1) / (stop1 - start1) * (stop2 - start2) + start2);
}


// this function ensures that the returned value does not exceed
// start2 stop2
static inline CGFloat   CGFloatProjectClamped( CGFloat value,
                                               CGFloat start1, CGFloat stop1,
                                               CGFloat start2, CGFloat stop2)
{
   CGFloat   projected;

   projected = CGFloatProject( value, start1, stop1, start2, stop2);
   if( start2 < stop2)
      return( CGFloatClamp( projected, start2, stop2));
   return( CGFloatClamp( projected, stop2, start2));
}




#ifdef __GNUC__
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

static inline int  CGFloatIsNegativeZero( CGFloat x)
{
   if( x != 0.0f)
      return( 0);

   if( sizeof( CGFloat) == sizeof( int32_t))
   {
      return( (*(int32_t *) &x) < 0);
   }
   if( sizeof( CGFloat) == sizeof( int64_t))
   {
      return( (*(int64_t *) &x) < 0);
   }
   assert( 0);
   return( 0);
}


typedef struct CGFloatRange
{
   CGFloat  min;
   CGFloat  max;
} CGFloatRange;


static inline CGFloatRange   CGFloatRangeMake( CGFloat min, CGFloat max)
{
   assert( ! isnan( min));
   assert( ! isnan( max));

   return( (CGFloatRange){ .min = min + 0.0f, .max = max + 0.0f });
}

#define CGFloatRangeData( minvalue, maxvalue) ((CGFloatRange){ .min = (minvalue) + 0.0f, .max = (maxvalue) + 0.0f })


static inline int  CGFloatRangeContains( CGFloatRange range, CGFloat value)
{
   return( value >= range.min && value <= range.max);
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


static inline CGPoint   CGPointMake( CGFloat x, CGFloat y)
{
   assert( ! isnan( x));
   assert( ! isnan( y));

   return( (CGPoint){ .x = x + 0.0f, .y = y + 0.0f });
}

#define CGPointData( xvalue, yvalue) ((CGPoint){ .x = (xvalue) + 0.0f, .y = (yvalue) + 0.0f })



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
   assert( ! isnan( x));
   assert( ! isnan( y));

   return( (CGPointU){ .x = x + 0.0f, .y = y + 0.0f });
}


#define CGPointUData( xvalue, yvalue) ((CGPointU){ .x = (xvalue) + 0.0f, .y = (yvalue) + 0.0f })



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
   assert( ! isnan( w));
   assert( ! isnan( h));

   return( (CGSize){ .width = w + 0.0f, .height = h + 0.0f });
}


#define CGSizeData( w, h) ((CGSize){ .width = (w) + 0.0f, .height = (h) + 0.0f })

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
   assert( ! isnan( w));
   assert( ! isnan( h));

   return( (CGSizeU){ .width = w + 0.0f, .height = h + 0.0f });
}


#define CGSizeUData( w, h) ((CGSizeU){ .width = (w) + 0.0f, .height = (h) + 0.0f })

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
   assert( ! isnan( dx));
   assert( ! isnan( dy));

   return( (CGVector){ .dx = dx + 0.0f, .dy = dy + 0.0f });
}


#define CGVectorData( xvalue, yvalue) ((CGVector){ .dx = (xvalue) + 0.0f, .dy = (yvalue) + 0.0f })




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
   assert( ! isnan( dx));
   assert( ! isnan( dy));

   return( (CGVectorU){ .dx = dx + 0.0f, .dy = dy + 0.0f });
}


#define CGVectorUData( xvalue, yvalue) ((CGVectorU){ .dx = (xvalue) + 0.0f, .dy = (yvalue) + 0.0f })


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
   assert( ! isnan( x));
   assert( ! isnan( y));   
   assert( ! isnan( w));
   assert( ! isnan( h));

   return( (CGRect){
                     .origin = (CGPoint) { .x = x + 0.0f, .y = y + 0.0f },
                     .size   = (CGSize)  { .width = w + 0.0f, .height = h + 0.0f }
                   });
}


#define CGRectData( xvalue, yvalue, w, h)                                   \
((CGRect)                                                                   \
{                                                                           \
   .origin = (CGPoint) { .x = (xvalue) + 0.0f, .y = (yvalue) + 0.0f },      \
   .size   = (CGSize)  { .width = (w) + 0.0f, .height = (h) + 0.0f }        \
})


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


static inline CGRect   CGRectMakeWithOriginAndOpposite( CGPoint a, CGPoint b)
{
   return( (CGRect){
                     .origin = a,
                     .size   = (CGSize) { .width = b.x - a.x,
                                          .height = b.y - a.y
                                        }
                   });
}


static inline CGRect   CGRectMakeWithPoints( CGPoint a, CGPoint b)
{
   CGRect   r;

   if( a.x >= b.x)
   {
      r.origin.x   = b.x + 0.0f;
      r.size.width = (a.x - b.x) + 0.0f;
   }
   else
   {
      r.origin.x   = a.x + 0.0f;
      r.size.width = (b.x - a.x) + 0.0f;
   }

   if( a.y >= b.y)
   {
      r.origin.y    = b.y + 0.0f;
      r.size.height = (a.y - b.y) + 0.0f;
   }
   else
   {
      r.origin.y    = a.y + 0.0f;
      r.size.height = (b.y - a.y) + 0.0f;
   }
   return( r);
}


typedef struct CGRectU
{
   CGPointU   origin;
   CGSizeU    size;
} CGRectU;


static inline CGRectU   CGRectUMake( CGFloat x, CGFloat y, CGFloat w, CGFloat h)
{
   assert( ! isnan( x));
   assert( ! isnan( y));
   assert( ! isnan( w));
   assert( ! isnan( h));

   return( (CGRectU){
                     .origin = (CGPointU) { .x = x + 0.0f, .y = y + 0.0f },
                     .size   = (CGSizeU)  { .width = w + 0.0f, .height = h + 0.0f }
                    });
}


#define CGRectUData( xvalue, yvalue, w, h)                                  \
((CGRectU)                                                                  \
{                                                                           \
   .origin = (CGPointU) { .x = (xvalue) + 0.0f, .y = (yvalue) + 0.0f },     \
   .size   = (CGSizeU)  { .width = (w) + 0.0f, .height = (h) + 0.0f }       \
})




//
// MEMO: Using extern const is probably the worst way to initialize to zero
//       as it can't be inlined and forces read/write, though we only need to
//       write.
//       Now, with a macro, you can't take the address of a CGRectZero
//       anymore though.
//
#define CGFloat2Null      CGFloat2Make( NAN, NAN)
#define CGFloat2Zero      CGFloat2Make( 0, 0)
#define CGFloat4Null      CGFloat4Make( NAN, NAN, 0.0f, 0.0f)
#define CGFloat4Infinite  CGFloat4Make( -INFINITY, -INFINITY, INFINITY, INFINITY)

#define CGPointZero       CGPointMake( 0, 0)
#define CGPointUZero      CGPointUMake( 0, 0)
#define CGPointNull       ((CGPoint){ .x = NAN, .y = NAN })
#define CGPointUNull      ((CGPointU){ .x = NAN, .y = NAN })
#define CGSizeZero        CGSizeMake( 0, 0)
#define CGSizeUZero       CGSizeUMake( 0, 0)
// there is no CGSizeNull
#define CGVectorZero      CGVectorMake( 0, 0)
#define CGVectorUZero     CGVectorUMake( 0, 0)
#define CGVectorNull      ((CGVector){ .dx = NAN, .dy = NAN })
#define CGVectorUNull     ((CGVectorU){ .dx = NAN, .dy = NAN })

#define CGRectZero        CGRectMake( 0, 0, 0, 0)
#define CGRectUZero       CGRectUMake( 0, 0, 0, 0)
#define CGRectNull        ((CGRect){ .origin = CGPointNull })
#define CGRectUNull       ((CGRectU){ .origin = CGPointUNull })
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

static inline CGPoint   CGPointMakeWithFloat2( CGFloat2 v2)
{
   return( *(CGPoint *) &v2);
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


static inline CGVector   CGVectorMakeWithFloat2( CGFloat2 v2)
{
   return( *(CGVector *) &v2);
}



static inline CGSize   CGSizeMakeWithPoint( CGPoint v)
{
   return( *(CGSize *) &v);
}


static inline CGSize   CGSizeMakeWithSizeU( CGSizeU v)
{
   return( *(CGSize *) &v);
}


static inline CGSize   CGSizeMakeWithFloat2( CGFloat2 v2)
{
   return( *(CGSize *) &v2);
}



static inline CGSize   CGSizeMakeWithVector( CGVector v)
{
   return( *(CGSize *) &v);
}


static inline CGRect   CGRectMakeWithRectU( CGRectU v)
{
   return( *(CGRect *) &v);
}


static inline CGRect   CGRectMakeWithFloat4( CGFloat4 v4)
{
   return( *(CGRect *) &v4);
}




static inline CGPointU   CGPointUMakeWithPoint( CGPoint v)
{
   return( *(CGPointU *) &v);
}


static inline CGVectorU   CGVectorUMakeWithVector( CGVector v)
{
   return( *(CGVectorU *) &v);
}


static inline CGVectorU   CGVectorUMakeWithFloat2( CGFloat2 v2)
{
   return( *(CGVectorU *) &v2);
}



static inline CGSizeU   CGSizeUMakeWithSize( CGSize v)
{
   return( *(CGSizeU *) &v);
}


static inline CGSizeU   CGSizeUMakeWithFloat2( CGFloat2 v2)
{
   return( *(CGSizeU *) &v2);
}


static inline CGRectU   CGRectUMakeWithRect( CGRect v)
{
   return( *(CGRectU *) &v);
}


static inline CGRectU   CGRectUMakeWithFloat4( CGFloat4 v4)
{
   return( *(CGRectU *) &v4);
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



// CGFloat2 is kinda low level, so we don't expect it to know CGPoint
// therefore use a getter instead of a maker
// in the end this is basically just a type cast
static inline CGFloat2   CGPointGetFloat2( CGPoint p)
{
   return( *(CGFloat2 *) CGPointUMakeWithPoint( p).value);
}


static inline CGFloat2   CGVectorGetFloat2( CGVector p)
{
   return( *(CGFloat2 *) CGVectorUMakeWithVector( p).value);
}


static inline CGFloat2   CGSizeGetFloat2( CGSize p)
{
   return( *(CGFloat2 *) CGSizeUMakeWithSize( p).value);
}

// CGRectGetFloat4
//
// With -O2 this code should compile down to just simple function calls
// without any copying of the parameters:
//
// extern int  foo( CGFloat4 x);
//
//
// int bar( CGRect x, CGRect y)
// {
//     return( foo( CGRectGetFloat4( x)) - foo( CGRectGetFloat4( y)));
// }
//
// gcc13.2: (gcc4.8 seems to be the first who can do fairly well)
// bar:
//         push    rbx
//         sub     rsp, 16
//         movq    QWORD PTR [rsp], xmm2
//         movq    QWORD PTR [rsp+8], xmm3
//         call    foo
//         movq    xmm0, QWORD PTR [rsp]
//         movq    xmm1, QWORD PTR [rsp+8]
//         mov     ebx, eax
//         call    foo
//         add     rsp, 16
//         mov     edx, eax
//         mov     eax, ebx
//         pop     rbx
//         sub     eax, edx
//         ret
//
static inline CGFloat4   CGRectGetFloat4( CGRect p)
{
   return( CGFloat4Make( p.origin.x, p.origin.y, p.size.width, p.size.height));
}

#ifdef __GNUC__
# pragma GCC diagnostic pop
#endif


#endif
