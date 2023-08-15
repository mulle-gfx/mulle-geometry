//
// Copyright (c) 2023 Nat!, Mulle kybernetiK
//
#ifndef ca_direction_h__
#define ca_direction_h__

#include "include.h"

#include "CARotation.h"
#include "CAAngle.h"


/* ? move to own header ? */
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

MULLE__GEOMETRY_EXTERN_GLOBAL
CADirection   CARotationToDirection( CARotation angleians);



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

static inline CADirection   CAAngleToDirection( CAAngle angle)
{
   return( CARotationToDirection( CAAngleToRotation( angle)));
}


static inline int   CADirectionIsOpposingDirection( CADirection direction, CADirection other)
{
   switch( direction)
   {
   case CADirectionTop     : return( (other & CADirectionBottom) ? 1 : 0);
   case CADirectionBottom  : return( (other & CADirectionTop) ? 1 : 0);
   case CADirectionLeft    : return( (other & CADirectionRight) ? 1 : 0);
   case CADirectionRight   : return( (other & CADirectionLeft) ? 1 : 0);

   case CADirectionTop|CADirectionLeft      : return( (other & (CADirectionBottom|CADirectionRight)) ? 1 : 0);
   case CADirectionTop|CADirectionRight     : return( (other & (CADirectionBottom|CADirectionLeft)) ? 1 : 0);
   case CADirectionBottom|CADirectionLeft   : return( (other & (CADirectionTop|CADirectionRight)) ? 1 : 0);
   case CADirectionBottom|CADirectionRight  : return( (other & (CADirectionTop|CADirectionLeft)) ? 1 : 0);
   }
   return( 0);
}

#endif
