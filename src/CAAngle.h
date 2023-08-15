#ifndef ca_angle_h__
#define ca_angle_h__

// want to get M_PI, but are we included before math?

#include "include.h"

#include "CGBase.h"

#include <math.h>
#include <assert.h>

#include "CARotation.h"


//
// This is CAAngle. It's a typedef for float radians. The origin 0 is east.
// three o clock.
//
//
//          CA_2xPI
//        .---------.
//   M_PI | radians | 0.0
//        '---------'
//          M_PI_2f
//
// Angles normalize to their range. Use CARotation, for non-normalized use.
// In general consider the CAAngle being "radians" an implementation detail,
// and use the various functions to convert radians, degrees, hour or rotation
// from and to CAAngle.
//
typedef CGFloat   CAAngle;


#define CAAngleNNE         ((CAAngle) (13.0f / 16.0f) * CA_2xPI)
#define CAAngleNE          ((CAAngle) (14.0f / 16.0f) * CA_2xPI)
#define CAAngleENE         ((CAAngle) (15.0f / 16.0f) * CA_2xPI)
#define CAAngleE           ((CAAngle)  (0.0f / 16.0f) * CA_2xPI)
#define CAAngleESE         ((CAAngle)  (1.0f / 16.0f) * CA_2xPI)
#define CAAngleSE          ((CAAngle)  (2.0f / 16.0f) * CA_2xPI)
#define CAAngleSSE         ((CAAngle)  (3.0f / 16.0f) * CA_2xPI)
#define CAAngleS           ((CAAngle)  (4.0f / 16.0f) * CA_2xPI)
#define CAAngleSSW         ((CAAngle)  (5.0f / 16.0f) * CA_2xPI)
#define CAAngleSW          ((CAAngle)  (6.0f / 16.0f) * CA_2xPI)
#define CAAngleWSW         ((CAAngle)  (7.0f / 16.0f) * CA_2xPI)
#define CAAngleW           ((CAAngle)  (8.0f / 16.0f) * CA_2xPI)
#define CAAngleWNW         ((CAAngle)  (9.0f / 16.0f) * CA_2xPI)
#define CAAngleNW          ((CAAngle) (10.0f / 16.0f) * CA_2xPI)
#define CAAngleNNW         ((CAAngle) (11.0f / 16.0f) * CA_2xPI)
#define CAAngleN           ((CAAngle) (12.0f / 16.0f) * CA_2xPI)


static inline CGFloat   CAAngleToRadians( CAAngle a)
{
   assert( a >= 0 && a <= CA_2xPI);

   return( a);
}


static inline CAAngle   CARadiansToAngle( CGFloat radians)
{
   CAAngle   a;

   // avoid -0.0f for -CA_2XPI, but 1.0 for 0.0 or -0.0
   if( radians == 0.0f)
      return( 0.0f);
   a = fmodf( radians, CA_2xPI);
   if( a <= 0)
      a += CA_2xPI;
   return( a);
}


static inline CAAngle   CAAngleRotate( CAAngle a, CARotation b)
{
   assert( a >= 0 && a <= CA_2xPI);

   return( CARadiansToAngle( CAAngleToRadians( a) + CARotationToRadians( b)));
}


// make it -pi to +pi
static inline CAAngle   CAAngleAdd( CAAngle a, CAAngle b)
{
   assert( a >= 0 && a <= CA_2xPI);
   assert( b >= 0 && b <= CA_2xPI);

   return( fmodf( a + b, CA_2xPI));  // shortcut because both domains are known
}


static inline CAAngle   CAAngleSubtract( CAAngle a, CAAngle b)
{
   assert( a >= 0 && a <= CA_2xPI);
   assert( b >= 0 && b <= CA_2xPI);

   return( CARadiansToAngle( a - b));
}


static inline CAAngle   CAAngleAddRadians( CAAngle a, CGFloat b)
{
   assert( a >= 0 && a <= CA_2xPI);

   return( CARadiansToAngle( a + b));
}


static inline CAAngle   CAAngleSubtractRadians( CAAngle a, CGFloat b)
{
   assert( a >= 0 && a <= CA_2xPI);

   return( CARadiansToAngle( a - b));
}



static inline CAAngle   CARotationToAngle( CARotation rotation)
{
   CAAngle   angle;

   MULLE_C_ASSERT( M_PI_f >= 3.141 && M_PI_f <= 3.142);
   MULLE_C_ASSERT( CA_2xPI >= 6.282 && CA_2xPI <= 6.284);

   rotation = CARotationTurn90CounterClockWise( rotation);
   angle    = CARadiansToAngle( CARotationToRadians( rotation));
   return( angle);
}


static inline CARotation   CAAngleToRotation( CAAngle angle)
{
   CARotation   rotation;

   assert( angle >= 0 && angle <= CA_2xPI);

   MULLE_C_ASSERT( M_PI_f >= 3.141 && M_PI_f <= 3.142);
   MULLE_C_ASSERT( CA_2xPI >= 6.282 && CA_2xPI <= 6.284);

   rotation = angle + M_PI_2f;
   if( rotation >= CA_2xPI)
      rotation -= CA_2xPI;
   return( rotation / CA_2xPI);
}


static inline CGFloat   CAAngleToSlope( CAAngle angle)
{
   return( tanf( CAAngleToRadians( angle)));
}


static inline CAAngle   CASlopeToAngle( CGFloat slope)
{
   return( atanf( CAAngleToRadians( slope)));
}


#include <math.h>

// not compatible with nanovg
enum
{
   CAAngleDirectionCounterClockWise = 0,
   CAAngleDirectionClockWise        = 1,
};


//
// find rotation direction, where a1 is closer to a0
// returns -1 for counter clock wise
// returns 1 for clockwise
//
static inline int   CAAngleRotationDirection( CAAngle a0, CAAngle a1)
{
   CGFloat   diff;

   diff = CAAngleSubtract( a0, a1);
   return( diff < M_PI_f ? CAAngleDirectionCounterClockWise : CAAngleDirectionClockWise);
}


//
//             0
//        .---------.
//    270 | degrees | 90
//        '---------'
//            180
//
static inline CGFloat   CADegreesNormalize( CGFloat value)
{
   CGFloat   result;

   // so 0.0 stays AS 0.0, but others are 1.0
   // this is nicer as it avoids -0.0 which is ugly
   if( value == 0.0f)
      return( 0.0f);

   result = fmodf( value, 360.0f);
   if( result <= 0.0f)
      result += 360.0f;
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
   return( CARotationToAngle( deg / 360.f));
}


static inline CGFloat   CAAngleToDegrees( CAAngle angle)
{
   assert( angle >= 0 && angle <= CA_2xPI);

   return( CAAngleToRotation( angle) * 360.0f);
}


static inline CGFloat   CADegreesToRadians( CGFloat deg)
{
   return( CARotationToRadians( CADegreesToRotation( deg)));
}


static inline CGFloat   CARadiansToDegrees( CGFloat rad)
{
   return( CARotationToDegrees( CARadiansToRotation( rad)));
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
   assert( angle >= 0 && angle <= CA_2xPI);

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

#endif
