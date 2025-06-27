#ifndef ca_angle_h__
#define ca_angle_h__

// want to get M_PI_f, but are we included before math?

#include "include.h"

#include "CGBase.h"
#include "CADirection.h"

#include <math.h>
#include <assert.h>

#define CA_EPSILON   0.0001f

// also compatible with nanovg,strict enum nor OR
enum
{
   CARotationDirectionCounterClockWise = 1,
   CARotationDirectionClockWise        = 2,
};
typedef uintptr_t    CARotationDirection;


//
// This is CAAngle. It's a typedef for float radians. The origin 0 is east.
// three o clock. There is no CA_2xPI angle, it's 0.0. A CAAngle sweeps in
// clockwise direction. (In mathematical CCW, but math uses a flipped
// coordinate system)
//
//        M_PI_f + M_PI_2f
//        .---------.
//   M_PI_f | radians | 0.0
//        '---------'
//          M_PI_2f
//
// Angles normalize to their range. Use CARotation, for non-normalized use.
// In general consider the CAAngle being "radians" an implementation detail,
// and use the various functions to convert radians, degrees, hour or rotation
// from and to CAAngle.
//
// MEMO: having CA_2xPI as an alternative angle introduces suddenly a duplicate
//       in animations (small jitter).
//
typedef CGFloat   CAAngle;


#define CAAngleNNE   ((CAAngle) (13.0f / 16.0f) * CA_2xPI)
#define CAAngleNE    ((CAAngle) (14.0f / 16.0f) * CA_2xPI)
#define CAAngleENE   ((CAAngle) (15.0f / 16.0f) * CA_2xPI)
#define CAAngleE     ((CAAngle)  (0.0f / 16.0f) * CA_2xPI)
#define CAAngleESE   ((CAAngle)  (1.0f / 16.0f) * CA_2xPI)
#define CAAngleSE    ((CAAngle)  (2.0f / 16.0f) * CA_2xPI)
#define CAAngleSSE   ((CAAngle)  (3.0f / 16.0f) * CA_2xPI)
#define CAAngleS     ((CAAngle)  (4.0f / 16.0f) * CA_2xPI)
#define CAAngleSSW   ((CAAngle)  (5.0f / 16.0f) * CA_2xPI)
#define CAAngleSW    ((CAAngle)  (6.0f / 16.0f) * CA_2xPI)
#define CAAngleWSW   ((CAAngle)  (7.0f / 16.0f) * CA_2xPI)
#define CAAngleW     ((CAAngle)  (8.0f / 16.0f) * CA_2xPI)
#define CAAngleWNW   ((CAAngle)  (9.0f / 16.0f) * CA_2xPI)
#define CAAngleNW    ((CAAngle) (10.0f / 16.0f) * CA_2xPI)
#define CAAngleNNW   ((CAAngle) (11.0f / 16.0f) * CA_2xPI)
#define CAAngleN     ((CAAngle) (12.0f / 16.0f) * CA_2xPI)




static inline CGFloat   CAAngleToRadians( CAAngle a)
{
   MULLE_C_ASSERT( M_PI_f >= 3.141 && M_PI_f <= 3.142);
   MULLE_C_ASSERT( CA_2xPI >= 6.282 && CA_2xPI <= 6.284);

   assert( a >= 0 && a < CA_2xPI);

   return( a);
}


static inline
CGFloat   CARadiansNormalizeInline( CGFloat radians)
{
   CAAngle   a;
   CAAngle   b;

   a = fmodf( radians, CA_2xPI);
   if( a > 0.0f)
      return( a);

   // avoid -0.0f  (and CA_2XPI) for fmodf( -CA_2XPI, CA_2xPI)
   if( a == 0.0f)
      return( 0.0f);

   // in degenerate cases the difference is so small, that we get
   // CA_2xPI back, which is fatal for our asserts
   b = a + CA_2xPI;
   if( b > CA_2xPI - CA_EPSILON)
      b = CA_2xPI - CA_EPSILON;
   return( b);
}


MULLE__GEOMETRY_EXTERN_GLOBAL
CGFloat   CARadiansNormalize( CGFloat radians);


static inline CAAngle   CARadiansToAngle( CGFloat radians)
{
   return( CARadiansNormalize( radians));
}



//
// This is CARotation
//
// You can view it as a relative value, which gives no orientation, but just
// an amount to turn (0.0 keep as is, 0.5 turn 180°, 2.0 turn twice) or as an
// absolute angle value with origin 0.
//
// The origin at twelve o clock is different to radians and angle where '0'
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
// The trigonometric counterpart to CARotation is ("radians" - "radians") or
// "radiansRotation". A "radiansRotation" has no origin, its a relative value
// only.
//
typedef CGFloat      CARotation;
typedef CARotation   CARotationAsAngle;



//
// This is rotation as a relative value. From the current position we rotate
// how much ?
//
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


//
// normalization destroys the "amounts" to turn information, which can be
// useful in animations. It is used when converting to CGFloat mainly.
// Turns: 2.5 -> 0.5,  -10.0 -> -1.0, 1.0 -> 1.0
static inline CARotation   CARotationNormalize( CARotation v)
{
   CARotation a;

   assert( ! isnan(v));

   a = fmodf( v, 1.0);
   if( v - a == 0.0f)
       return( a == 0.0f ? 0.0f : a); // get rid of sign
   return( (a < 0.0f || (a == 0.0f && v < 0.0f)) ? -1.0f : 1.0f);
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



// CGContextType string conversion functions
static inline char   *_CARotationDirectionUTF8String( CARotationDirection value)
{
   switch( value)
   {
   case CARotationDirectionCounterClockWise : return( "CounterClockWise");
   case CARotationDirectionClockWise        : return( "ClockWise");
   }
   return( NULL);
}


static inline char   *CARotationDirectionUTF8String( CARotationDirection value)
{
   char   *s;

   s = _CARotationDirectionUTF8String( value);
   return( s ? s : "???");
}


static inline CAAngle   CARotationToAngle( CARotation rotation)
{
   CAAngle   angle;

   angle = CARadiansToAngle( rotation * CA_2xPI - M_PI_2f);
   return( angle);
}


static inline CARotation   CAAngleToRotation( CAAngle angle)
{
   CARotation   rotation;

   assert( angle >= 0 && angle < CA_2xPI);

   rotation = angle + M_PI_2f;
   if( rotation > CA_2xPI)
      rotation -= CA_2xPI;
   return( rotation / CA_2xPI);
}



static inline CGFloat   CARotationToRadiansAsAngle( CARotation rotation)
{
   CGFloat   radians;
   // Normalize rotation to [0, 1) range
   CGFloat   normalized;

   normalized = fmodf(rotation, 1.0f);
   if (normalized < 0.0f)
      normalized += 1.0f;

   // Convert from [0, 1) to [0, 2π)
   radians = normalized * CA_2xPI;

   // Subtract the π/2 offset
   radians -= M_PI_2f;

   // Normalize to [0, 2π) range
   if (radians < 0.0f)
      radians += CA_2xPI;

   return( radians);
}


static inline CARotation   CARadiansAsAngleToRotation( CGFloat radians)
{
   CARotation   rotation;
   CGFloat      normalized;

   // Normalize radians to [0, 2π) range
   normalized = fmodf(radians, CA_2xPI);
   if( normalized < 0.0f)
      normalized += CA_2xPI;

   // Add π/2 offset
   rotation = normalized + M_PI_2f;
   if( rotation > CA_2xPI)
      rotation -= CA_2xPI;

   // Convert to [0, 1) range
   return( rotation / CA_2xPI);
}


static inline CGFloat   CARotationToRadiansAsRotation( CARotation rotation)
{
   assert( ! isnan( rotation));

   return( rotation * CA_2xPI);
}


//
static inline CARotation   CARadiansAsRotationToRotation( CGFloat radiansRotation)
{
   assert( ! isnan( radiansRotation));

   return( radiansRotation / CA_2xPI);
}


static inline CGFloat   CAAngleRotateToRadiansAsAngle( CAAngle a, CARotation b)
{
   return( CAAngleToRadians( a) + CARotationToRadiansAsRotation( b));
}


static inline CAAngle   CAAngleRotate( CAAngle a, CARotation b)
{
   return( CARadiansToAngle( CAAngleRotateToRadiansAsAngle( a, b)));
}


static inline CARotationDirection   CARotationGetRotationDirection( CARotation a)
{
   return( (a < 0.0)
           ? CARotationDirectionCounterClockWise
           : CARotationDirectionClockWise);
}


//
// considering a circle, rotation a is in a different hemisphere (0.25 / 0.5)
// than b
//
static inline int   CARotationIsOpposingRotation( CARotation a, CARotation b)
{
   CARotation  d;

   d = fmodf( a - b, 1.0f);
   return( (d > 0.25 && d < 0.75) || (d < -0.25 && d > -0.75));
}




//*###############
//* ANGLE
//*###############


// make it -pi to +pi
static inline CAAngle   CAAngleAdd( CAAngle a, CAAngle b)
{
   assert( a >= 0 && a < CA_2xPI);
   assert( b >= 0 && b < CA_2xPI);

   return( fmodf( a + b, CA_2xPI));  // shortcut because both domains are known
}


static inline CAAngle   CAAngleSubtract( CAAngle a, CAAngle b)
{
   assert( a >= 0 && a < CA_2xPI);
   assert( b >= 0 && b < CA_2xPI);

   return( CARadiansToAngle( CAAngleToRadians( a) - CAAngleToRadians( b)));
}


static inline CAAngle   CAAngleAddRadians( CAAngle a, CGFloat b)
{
   assert( a >= 0 && a < CA_2xPI);

   return( CARadiansToAngle( CAAngleToRadians( a) + b));
}


static inline CAAngle   CAAngleSubtractRadians( CAAngle a, CGFloat b)
{
   assert( a >= 0 && a < CA_2xPI);

   return( CARadiansToAngle( CAAngleToRadians( a) - b));
}


// if a < b, then midAngle is clockwise otherwise counterClockwise
// e.g. CADegreesToAngle( 40), CADegreesToAngle( 60) -> CADegreesToAngle( 50)
// e.g. CADegreesToAngle( 60), CADegreesToAngle( 60) -> CADegreesToAngle( 230)
// e.g. CADegreesToAngle( 10), CADegreesToAngle( 10) -> CADegreesToAngle( 190)
//
static inline CAAngle   CAAngleGetMidAngle( CAAngle a, CGFloat b)
{
   assert( a >= 0 && a < CA_2xPI);
   assert( b >= 0 && a < CA_2xPI);

   CGFloat   radians;

   if( a == b)
      radians = M_PI_f;
   else
      radians = CAAngleToRadians( CAAngleSubtract( b, a)) / 2.0;
   return( CAAngleAddRadians( a, radians));
}



static inline CGFloat   CAAngleToSlope( CAAngle angle)
{
   return( tanf( CAAngleToRadians( angle)));
}


static inline CAAngle   CASlopeToAngle( CGFloat slope)
{
   return( CARadiansToAngle( atanf( CAAngleToRadians( slope))));
}



//
// find rotation direction, where a1 is closer to a0
// returns -1 for counter clock wise
// returns 1 for clockwise
//
static inline CARotationDirection   CAAngleGetRotationDirection( CAAngle a0, CAAngle a1)
{
   CGFloat   diff;

   diff = CAAngleSubtract( a0, a1);
   return( diff < M_PI_f ? CARotationDirectionCounterClockWise : CARotationDirectionClockWise);
}


//
// find rotation to rotate angle a to angle b
//
static inline CARotation   CAAngleGetRotationTowardsAngle( CAAngle a, CAAngle b, CARotationDirection dir)
{
   CARotation   rotation;

   assert( dir == CARotationDirectionCounterClockWise || dir == CARotationDirectionClockWise);

   rotation = CARadiansAsRotationToRotation( CAAngleToRadians( b) - CAAngleToRadians( a));
   if( rotation > 0.0)
   {
      if( dir == CARotationDirectionCounterClockWise)
         rotation = -1.0f + rotation;
   }
   else
      if( rotation < 0.0)
         if( dir == CARotationDirectionClockWise)
            rotation = 1.0f + rotation;

   return( rotation);
}



//
//             0
//        .---------.
//    270 | degrees | 90
//        '---------'
//            180
//
static inline CGFloat   CADegreesNormalize( CGFloat degrees)
{
   CGFloat   result;

   result = fmodf( degrees, 360.0f);
   if( result <= 0.0f)
   {
      if( degrees == 0.0f) // CARadiansToAngle
         return( 0.0f);
      result += 360.0f;
   }
   return( result);
}


static inline CARotation   CADegreesToRotation( CGFloat deg)
{
   return( deg / 360.f);
}


static inline CGFloat   CARotationToDegrees( CARotation rotation)
{
   return( CADegreesNormalize( rotation * 360.f));
}



static inline CAAngle   CADegreesToAngle( CGFloat deg)
{
   return( CARotationToAngle( deg / 360.0f));
}


static inline CGFloat   CAAngleToDegrees( CAAngle angle)
{
   assert( angle >= 0 && angle < CA_2xPI);

   return( CAAngleToRotation( angle) * 360.0f);
}


static inline CGFloat   CADegreesToRadiansAsRotation( CGFloat deg)
{
   return( CARotationToRadiansAsRotation( CADegreesToRotation( deg)));
}


static inline CGFloat   CADegreesToRadiansAsAngle( CGFloat deg)
{
   return( CARotationToRadiansAsAngle( CADegreesToRotation( deg)));
}


static inline CGFloat   CARadiansAsAngleToDegrees( CGFloat rad)
{
   return( CARotationToDegrees( CARadiansAsAngleToRotation( rad)));
}


static inline CGFloat   CARadiansAsRotationToDegrees( CGFloat rad)
{
   return( CARotationToDegrees( CARadiansAsRotationToRotation( rad)));
}


static inline int   CAAngleIsOpposingAngle( CAAngle a, CAAngle b)
{
   return( CARotationIsOpposingRotation( CAAngleToRotation( a), CAAngleToRotation( b)));
}



//
// There are no negative values, there is no zero (as on a clock). There being
// no two values is supposed to be a simplification. If it really is a useful
// one, remains to be seen.
//
//           12
//        .------.
//       9| hour |3
//        '------'
//            6

static inline CGFloat   CAHourNormalize( CGFloat value)
{
   CGFloat   result;

   result = fmodf( value, 12.0f);
   if( result <= 0)
      result += 12.0f;
   return( result);
}


static inline CARotation   CAHourToRotation( CGFloat hour)
{
   assert( hour > 0.0f && hour <= 360.0f);

   return( hour == 12.0f ? 0.0f : hour / 12.0f);
}


static inline CGFloat   CARotationToHour( CARotation rotation)
{
   return( CAHourNormalize( rotation * 12.0f));
}


static inline CGFloat   CAAngleToHour( CAAngle angle)
{
   assert( angle >= 0 && angle < CA_2xPI);

   return( angle == CAAngleN
           ? 12.0f
           : CAAngleToRotation( angle) * 12.0f);
}


static inline CAAngle   CAHourToAngle( CGFloat hour)
{
   assert( hour > 0.0f && hour <= 12.0f);

   return( CARotationToAngle( hour / 12.0f));
}



#define CAAngleNorth       CAAngleN
#define CAAngleNorthEast   CAAngleNE
#define CAAngleEast        CAAngleE
#define CAAngleSouthEast   CAAngleSE
#define CAAngleSouth       CAAngleS
#define CAAngleSouthWest   CAAngleSW
#define CAAngleWest        CAAngleW
#define CAAngleNorthWest   CAAngleNW


#define CAAngleTramontana  CAAngleN
#define CAAngleGreco       CAAngleNE
#define CAAngleLevante     CAAngleE
#define CAAngleScirocco    CAAngleSE
#define CAAngleOstro       CAAngleS
#define CAAngleLibeccio    CAAngleSW
#define CAAnglePonente     CAAngleW
#define CAAngleMaestro     CAAngleNW


// This is the absolute orientation, the radiansAngle.
// In a circle, the radius is pointing North. That's also what we would assume
// the orientation of an non-rotated rect is.
// The typedef is gratuitous

#define CARotationN           ((CARotationAsAngle) ( 0.0f / 16.0f))
#define CARotationNNE         ((CARotationAsAngle) ( 1.0f / 16.0f))
#define CARotationNE          ((CARotationAsAngle) ( 2.0f / 16.0f))
#define CARotationENE         ((CARotationAsAngle) ( 3.0f / 16.0f))
#define CARotationE           ((CARotationAsAngle) ( 4.0f / 16.0f))
#define CARotationESE         ((CARotationAsAngle) ( 5.0f / 16.0f))
#define CARotationSE          ((CARotationAsAngle) ( 6.0f / 16.0f))
#define CARotationSSE         ((CARotationAsAngle) ( 7.0f / 16.0f))
#define CARotationS           ((CARotationAsAngle) ( 8.0f / 16.0f))
#define CARotationSSW         ((CARotationAsAngle) ( 9.0f / 16.0f))
#define CARotationSW          ((CARotationAsAngle) (10.0f / 16.0f))
#define CARotationWSW         ((CARotationAsAngle) (11.0f / 16.0f))
#define CARotationW           ((CARotationAsAngle) (12.0f / 16.0f))
#define CARotationWNW         ((CARotationAsAngle) (13.0f / 16.0f))
#define CARotationNW          ((CARotationAsAngle) (14.0f / 16.0f))
#define CARotationNNW         ((CARotationAsAngle) (15.0f / 16.0f))


#define CARotationNorth       CARotationN
#define CARotationNorthEast   CARotationNE
#define CARotationEast        CARotationE
#define CARotationSouthEast   CARotationSE
#define CARotationSouth       CARotationS
#define CARotationSouthWest   CARotationSW
#define CARotationWest        CARotationW
#define CARotationNorthWest   CARotationNW



static inline CARotation   CADirectionToRotation( CADirection direction)
{
   switch( direction)
   {
   case CADirectionTop|CADirectionLeft     : return( CARotationNW);
   case CADirectionTop                     : return( CARotationN);
   case CADirectionTop|CADirectionRight    : return( CARotationNE);
   case CADirectionRight                   : return( CARotationE);
   case CADirectionBottom|CADirectionRight : return( CARotationSE);
   case CADirectionBottom                  : return( CARotationS);
   case CADirectionBottom|CADirectionLeft  : return( CARotationSW);
   case CADirectionLeft                    : return( CARotationW);
   }
   return( 0.0); // or what ?
}



static inline CAAngle   CADirectionToAngle( CADirection direction)
{
   switch( direction)
   {
   case CADirectionTop|CADirectionLeft     : return( CAAngleNW);
   case CADirectionTop                     : return( CAAngleN);
   case CADirectionTop|CADirectionRight    : return( CAAngleNE);
   case CADirectionRight                   : return( CAAngleE);
   case CADirectionBottom|CADirectionRight : return( CAAngleSE);
   case CADirectionBottom                  : return( CAAngleS);
   case CADirectionBottom|CADirectionLeft  : return( CAAngleSW);
   case CADirectionLeft                    : return( CAAngleW);
   }
   return( 0.0); // or what ?
}


MULLE__GEOMETRY_EXTERN_GLOBAL
CADirection   CARotationToDirection( CARotation rotation);

static inline CADirection   CAAngleToDirection( CAAngle angle)
{
   return( CARotationToDirection( CAAngleToRotation( angle)));
}

#endif
