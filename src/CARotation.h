//
// Copyright (c) 2023 Nat!, Mulle kybernetiK
//
#ifndef ca_rotation_h__
#define ca_rotation_h__


// want to get M_PI, but are we included before math?

#include "include.h"

#include "CGBase.h"

#include <math.h>
#include <assert.h>


//
// This is CARotation
//
// You can view it as a relative value, which gives no orientation, but just
// an amount to turn (0.0 keep as is, 0.5 turn 180°, 2.0 turn twice) or as an
// absolute value with origin 0, which is then the same as an angle.
//
// The origin at twelve o clock is different to radians where '0'
// is actually three o clock.
//
//            0.0 / 1.0 / -1.0 / 2.0 / -2.0 ...
//        .---------.
//   -0.25|         | 0.25
//    0.75|         | -0.75
//        '---------'
//          0.5/-0.5
//
//
// The trigonometric counterpart to CARotation is "radiansRotation"
// (which is just kept in a CGFloat). "radiansRotation" has no origin, its
// a relative value only.
//
typedef CGFloat   CARotation;


//
// normalization destroys the "amounts" to turn information, which can be
// useful in animations. It is used when converting to CGFloat mainly.
//
static inline CARotation   CARotationNormalize( CARotation v)
{
   CARotation   a;

   assert( ! isnan( v));

   a = fmodf( v, 1.0);
   if( a < 0.0)
      a += 1.0;
   return( a);
}


static inline CGFloat   CARotationToRadians( CARotation rotation)
{
   assert( ! isnan( rotation));

   return( rotation * CA_2xPI);
}


static inline CARotation   CARadiansToRotation( CGFloat radians)
{
   assert( ! isnan( radians));

   return( radians / CA_2xPI);
}



// make it -pi to +pi
static inline CARotation   CARotationAdd( CARotation a, CARotation b)
{
   assert( ! isnan( a));
   assert( ! isnan( b));

   return( a + b);
}


static inline CARotation   CARotationSubtract( CARotation a, CARotation b)
{
   assert( ! isnan( a));
   assert( ! isnan( b));

   return( a - b);
}


static inline CARotation   CARotationTurn45ClockWise( CARotation rotation)
{
   assert( ! isnan( rotation));

   return( rotation + (1.0f / 8.0f));
}


static inline CARotation   CARotationTurn45CounterClockWise( CARotation rotation)
{
   assert( ! isnan( rotation));

   return( rotation - (1.0f / 8.0f));
}


static inline CARotation   CARotationTurn90ClockWise( CARotation rotation)
{
   assert( ! isnan( rotation));

   return( rotation + (2.0f / 8.0f));
}


static inline CARotation   CARotationTurn90CounterClockWise( CARotation rotation)
{
   assert( ! isnan( rotation));

   return( rotation - (2.0f / 8.0f));
}


static inline CARotation   CARotationTurn180( CARotation rotation)
{
   assert( ! isnan( rotation));

   return( rotation + (4.0f / 8.0f));
}


static inline CARotation   CARotationTurn270ClockWise( CARotation rotation)
{
   assert( ! isnan( rotation));

   return( rotation + (6.0f / 8.0f));
}


static inline CARotation   CARotationTurn270CounterClockWise( CARotation rotation)
{
   assert( ! isnan( rotation));

   return( rotation - (6.0f / 8.0f));
}




#define CARotationNone        ((CARotation) (0.0f / 8.0f))
#define CARotation45          ((CARotation) (1.0f / 8.0f))
#define CARotation90          ((CARotation) (2.0f / 8.0f))
#define CARotation135         ((CARotation) (3.0f / 8.0f))
#define CARotation180         ((CARotation) (4.0f / 8.0f))
#define CARotation225         ((CARotation) (5.0f / 8.0f))
#define CARotation270         ((CARotation) (6.0f / 8.0f))
#define CARotation315         ((CARotation) (7.0f / 8.0f))
#define CARotation360         ((CARotation) (8.0f / 8.0f))

#define CARotation45Right     ((CARotation) (+1.0f / 8.0f))
#define CARotation90Right     ((CARotation) (+2.0f / 8.0f))
#define CARotation135Right    ((CARotation) (+3.0f / 8.0f))
#define CARotation180Right    ((CARotation) (+4.0f / 8.0f))
#define CARotation225Right    ((CARotation) (+5.0f / 8.0f))
#define CARotation270Right    ((CARotation) (+6.0f / 8.0f))
#define CARotation315Right    ((CARotation) (+7.0f / 8.0f))
#define CARotation360Right    ((CARotation) (+8.0f / 8.0f))

#define CARotation45Left      ((CARotation) (-1.0f / 8.0f))
#define CARotation90Left      ((CARotation) (-2.0f / 8.0f))
#define CARotation135Left     ((CARotation) (-3.0f / 8.0f))
#define CARotation180Left     ((CARotation) (-4.0f / 8.0f))
#define CARotation225Left     ((CARotation) (-5.0f / 8.0f))
#define CARotation270Left     ((CARotation) (-6.0f / 8.0f))
#define CARotation315Left     ((CARotation) (-7.0f / 8.0f))
#define CARotation360Left     ((CARotation) (-8.0f / 8.0f))


// If the basic orientation is left to right, top down
// then what do we need to rotate to, so the resulting angle seems to be
// in the desired direction ? This is not compatible, interchangeable with
// CAAngle<wind>

#define CARotationNNE         ((CARotation) (-3.0f / 16.0f))
#define CARotationNE          ((CARotation) (-2.0f / 16.0f))
#define CARotationENE         ((CARotation) (-1.0f / 16.0f))
#define CARotationE           ((CARotation) ( 0.0f / 16.0f))
#define CARotationESE         ((CARotation) ( 1.0f / 16.0f))
#define CARotationSE          ((CARotation) ( 2.0f / 16.0f))
#define CARotationSSE         ((CARotation) ( 3.0f / 16.0f))
#define CARotationS           ((CARotation) ( 4.0f / 16.0f))
#define CARotationSSW         ((CARotation) ( 5.0f / 16.0f))
#define CARotationSW          ((CARotation) ( 6.0f / 16.0f))
#define CARotationWSW         ((CARotation) ( 7.0f / 16.0f))
#define CARotationW           ((CARotation) ( 8.0f / 16.0f))
#define CARotationWNW         ((CARotation) (-7.0f / 16.0f))
#define CARotationNW          ((CARotation) (-6.0f / 16.0f))
#define CARotationNNW         ((CARotation) (-5.0f / 16.0f))
#define CARotationN           ((CARotation) (-4.0f / 16.0f))


#define CARotationNorth       CARotationN
#define CARotationNorthEast   CARotationNE
#define CARotationEast        CARotationE
#define CARotationSouthEast   CARotationSE
#define CARotationSouth       CARotationS
#define CARotationSouthWest   CARotationSW
#define CARotationWest        CARotationW
#define CARotationNorthWest   CARotationNW

#endif


