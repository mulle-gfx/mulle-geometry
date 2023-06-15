#ifndef ca_angle_h__
#define ca_angle_h__

// want to get M_PI, but are we included before math?

#include "include.h"

#include "CGBase.h"

#include <math.h>
#include <assert.h>



#ifndef M_PI_f
# define M_PI_f     ((float) 3.141592653589793238462643383279502884L)
#endif

#ifndef M_PI_2f
# define M_PI_2f    ((float) (3.141592653589793238462643383279502884L / 2.0))
#endif

// this doesn't exist usually (or name it M_2_PI_f ??)
#define CA_2xPI  ((float) (3.141592653589793238462643383279502884L * 2.0))



// Angles with orientation.
// With radians and degrees '0' is actually three o clock.
// But when thinking about rotation, you want the orientation like with an
// analog clock, that is CAAngleHour.
//
// Conversion between Radians and Degrees and back is lossless (within FP
// limitations). Conversion from and to CAAngleHour and CAAngleDirectional are
// lossy though.
//
// You will lose the amplitude of the rotation (i.e. 15* M_PI can
// be thought of as 15 turns) and the sign (CAAngleHour).
//
// If you have an indicator needle, that is usually centered and goes left
// (negative) or right (positive), the CAAngleDirectional is nice.
//
//          -M_PI/2
//        .---------.
// +/-M_PI| radians |0.0
//        '---------'
//           M_PI/2

typedef CGFloat   CAAngleRadians;

//           270
//       .---------.
//    180| degrees |0
//       '---------'
//           90

typedef CGFloat   CAAngleDegrees;


//
// There are no negative values (usually)
//
//            0 (12)
//        .------.
//       9| hour |3
//        '------'
//            6

typedef CGFloat   CAAngleHour;

//              0.0
//        .-------------.
//    -0.5| directional |0.5
//        '-------------'
//         -0.999  0.999

typedef CGFloat   CAAngleDirectional;

//            0.0
//        .---------.
//    0.75|  unity  |0.25
//        '---------'
//            0.5

typedef CGFloat   CAAngleUnity;



// MEMO: use CANormalizedFloatToRadians and not CANormalizedFloatAsRadians
//       because there is a calculation involved. Could also use a Get
//       but...
static inline CAAngleRadians   CANormalizedFloatToRadians( CGFloat x)
{
   assert( x >= 0.0 && x <= 1.0);
//   deg = fmodf( deg, 360.0f);
   return( x * M_PI_2f);
}


static inline CAAngleUnity   CAAngleRadiansToUnity( CAAngleRadians rad)
{
   CAAngleUnity   unity;

   unity = (rad + M_PI_2f) / CA_2xPI;
   unity = fmodf( unity, 1.0f);
   unity = unity < 0.0f ? 1.0f + unity : unity;
   return( unity);
}


static inline CAAngleRadians   CAAngleUnityToRadians( CAAngleUnity unity)
{
   CAAngleRadians   rad;

   assert( unity >= 0.0f && unity <= 1.0f);

   MULLE_C_ASSERT( M_PI_f >= 3.141 && M_PI_f <= 3.142);
   MULLE_C_ASSERT( CA_2xPI >= 6.282 && CA_2xPI <= 6.284);

   rad  = unity * CA_2xPI;
   rad  = rad - M_PI_f + M_PI_2f;
   return( rad);
}


static inline CAAngleHour   CAAngleRadiansToHour( CAAngleRadians rad)
{
   CAAngleHour   hour;

   hour = (rad + M_PI_2f) * 12.0f / CA_2xPI;
   hour = fmodf( hour, 12.0f);
   hour = hour < 0.0f ? 12.0f + hour : hour;
   return( hour);
}


static inline CAAngleRadians   CAAngleHourToRadians( CAAngleHour hour)
{
   CAAngleRadians   rad;

   assert( hour >= 0.0f && hour <= 12.0f);

   rad = hour / 12.0f * CA_2xPI;
   rad = rad - M_PI_2f;
   return( rad);
}



static inline CAAngleDegrees   CAAngleRadiansToDegrees( CAAngleRadians rad)
{
//   rad = fmodf( rad, CA_2xPI);
   return( (CGFloat) (rad * 360.0f / CA_2xPI));
}

static inline CAAngleRadians   CAAngleDegreesToRadians( CAAngleDegrees deg)
{
//   deg = fmodf( deg, 360.0f);
   return( deg * M_PI_f / 180.0f);
}

// this normalizes
static inline CAAngleRadians   CAAngleDegreesToUnity( CAAngleDegrees deg)
{
   return( CAAngleRadiansToUnity( CAAngleDegreesToRadians( deg)));
}


// this normalizes
static inline CAAngleRadians   CAAngleDegreesToHour( CAAngleDegrees deg)
{
   return( CAAngleRadiansToHour( CAAngleDegreesToRadians( deg)));
}


//
// an angle from -180 to +180 this is converted to  -1.0 - 1.0 (actually -0.9999 - 0.99999)
//
static inline CAAngleDirectional   CAAngleDegreesToDirectional( CAAngleDegrees deg)
{
   CAAngleDirectional   dir;

   dir = fmodf( (deg + 90.0f) / 360.0, 1.0f);
   return( dir);
}


// make an angle between -pi/2 and +pi/2
// https://stackoverflow.com/questions/24234609/standard-way-to-normalize-an-angle-to-%CF%80-radians-in-java
//
static inline CAAngleRadians   CAAngleRadiansNormalize( CAAngleRadians rad)
{
   return( rad - CA_2xPI * floorf( rad + M_PI_f) / CA_2xPI);
}


//
// make a positive angle value between 0 and 360
//
static inline CAAngleDegrees   CAAngleDegreesNormalize( CAAngleDegrees deg)
{
   CAAngleDegrees   normalized;

   normalized = fmodf( deg, 360.0f);
   if( normalized < 0.0f)
      normalized = 360.0f + normalized;
   return( normalized);
}


#ifdef __GNUC__
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

//
// this can be used to discern full left CAAngleDirectional
//
static inline int  CAIsNegativeZero( CGFloat x)
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


//
// degrees will be always positive, to this is a lossy conversion
//
static inline CAAngleDegrees   CAAngleDirectionalToDegrees( CAAngleDirectional dir)
{
   CAAngleDegrees   deg;

   deg = fmodf( dir * 360.0f, 360.0f);
   deg = deg - 90.0f; //  deg < 0.0 ? deg + 360.0f : deg);
   if( deg < 0.0f)
      return( 360.0f + deg);
   return( deg);
}



enum
{
   CADirectionTop    = 1,
   CADirectionBottom = 2,
   CADirectionLeft   = 4,
   CADirectionRight  = 8
};

#define CADirectionUp    CADirectionTop
#define CADirectionDown  CADirectionBottom

typedef uintptr_t    CADirection;


CADirection   CADirectionTurn45ClockWise( CADirection direction);
CADirection   CADirectionTurn45CounterClockWise( CADirection direction);
CADirection   CADirectionTurn90ClockWise( CADirection direction);
CADirection   CADirectionTurn90CounterClockWise( CADirection direction);
CADirection   CADirectionTurn180( CADirection direction);

static inline CADirection   CADirectionTurn270ClockWise( CADirection direction)
{
   return( CADirectionTurn90CounterClockWise( direction));
}


static inline CADirection   CADirectionTurn270CounterClockWise( CADirection direction)
{
   return( CADirectionTurn90ClockWise( direction));
}


char       *CADirectionUTF8String( CADirection direction);
uint32_t   CADirectionBoxCharacter( CADirection direction);
uint32_t   CADirectionArrowCharacter( CADirection direction);


static inline CAAngleRadians   CADirectionToAngleRadians( CADirection direction)
{
   switch( direction)
   {
   case CADirectionTop|CADirectionLeft     : return( CAAngleHourToRadians( 12.0 - 3.0 / 2.0));
   case CADirectionTop                     : return( CAAngleHourToRadians( 12.0));
   case CADirectionTop|CADirectionRight    : return( CAAngleHourToRadians( 3.0 / 2.0));
   case CADirectionRight                   : return( CAAngleHourToRadians( 3.0));
   case CADirectionBottom|CADirectionRight : return( CAAngleHourToRadians( 6.0 - 3.0 / 2.0));
   case CADirectionBottom                  : return( CAAngleHourToRadians( 6.0));
   case CADirectionBottom|CADirectionLeft  : return( CAAngleHourToRadians( 6.0 + 3.0 / 2.0));
   case CADirectionLeft                    : return( CAAngleHourToRadians( 9.0));
   }
   return( 0.0); // or what ?
}


static inline CADirection   CAAngleRadiansToDirection( CAAngleRadians radians)
{
   CAAngleHour   h;
   CADirection   dir;

   h = CAAngleRadiansToHour( radians);

   // split into top and bottom sector,
   dir = 0;
   if( h > (9.0 + 3.0 / 2.0 / 2.0) || h < (3.0 - 3.0 / 2.0 / 2.0))
      dir = CADirectionTop;
   else
      if( h < (9.0 - 3.0 / 2.0 / 2.0) && h > (3.0 + 3.0 / 2.0 / 2.0))
         dir = CADirectionBottom;

   // then split into right and left sector
   if( h > (0.0 + 3.0 / 2.0 / 2.0) && h < (6.0 - 3.0 / 2.0 / 2.0))
      dir |= CADirectionRight;
   else
      if( h < (12.0 - 3.0 / 2.0 / 2.0) && h > (6.0 + 3.0 / 2.0 / 2.0))
         dir |= CADirectionLeft;

   return( dir);
}


#define CGWindDirectionNNE   CAAngleUnityToRadians(  1.0 / 16.0)
#define CGWindDirectionNE    CAAngleUnityToRadians(  2.0 / 16.0)
#define CGWindDirectionENE   CAAngleUnityToRadians(  3.0 / 16.0)
#define CGWindDirectionE     CAAngleUnityToRadians(  4.0 / 16.0)
#define CGWindDirectionESE   CAAngleUnityToRadians(  5.0 / 16.0)
#define CGWindDirectionSE    CAAngleUnityToRadians(  6.0 / 16.0)
#define CGWindDirectionSSE   CAAngleUnityToRadians(  7.0 / 16.0)
#define CGWindDirectionS     CAAngleUnityToRadians(  8.0 / 16.0)
#define CGWindDirectionSSW   CAAngleUnityToRadians(  9.0 / 16.0)
#define CGWindDirectionSW    CAAngleUnityToRadians( 10.0 / 16.0)
#define CGWindDirectionWSW   CAAngleUnityToRadians( 11.0 / 16.0)
#define CGWindDirectionW     CAAngleUnityToRadians( 12.0 / 16.0)
#define CGWindDirectionWNW   CAAngleUnityToRadians( 13.0 / 16.0)
#define CGWindDirectionNW    CAAngleUnityToRadians( 14.0 / 16.0)
#define CGWindDirectionNNW   CAAngleUnityToRadians( 15.0 / 16.0)
#define CGWindDirectionN     CAAngleUnityToRadians( 16.0 / 16.0)


#define CGWindDirectionGreco       CGWindDirectionNE
#define CGWindDirectionLevante     CGWindDirectionE
#define CGWindDirectionScirocco    CGWindDirectionSE
#define CGWindDirectionOstro       CGWindDirectionS
#define CGWindDirectionLibeccio    CGWindDirectionSW
#define CGWindDirectionPonente     CGWindDirectionW
#define CGWindDirectionMaestro     CGWindDirectionNW
#define CGWindDirectionTramontana  CGWindDirectionN

#endif


