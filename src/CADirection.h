//
//  CADirection.h
//  mulle-geometry
//
//  Copyright (c) 2023 Nat! - Mulle kybernetiK.
//  All rights reserved.
//
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//  Redistributions of source code must retain the above copyright notice, this
//  list of conditions and the following disclaimer.
//
//  Redistributions in binary form must reproduce the above copyright notice,
//  this list of conditions and the following disclaimer in the documentation
//  and/or other materials provided with the distribution.
//
//  Neither the name of Mulle kybernetiK nor the names of its contributors
//  may be used to endorse or promote products derived from this software
//  without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
//  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
//  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//  POSSIBILITY OF SUCH DAMAGE.
//
#ifndef ca_direction_h__
#define ca_direction_h__

#include "include.h"


// also compatible with nanovg,strict enum nor OR
enum
{
   CARotationDirectionCounterClockWise = 1,
   CARotationDirectionClockWise        = 2,
};
typedef uintptr_t    CARotationDirection;



/* options to be ORed */
enum
{
   CADirectionNone   = 0,  // AI likes it
   CADirectionUp     = 1,
   CADirectionDown   = 2,
   CADirectionLeft   = 4,
   CADirectionRight  = 8
};

#define CADirectionTop        CADirectionUp
#define CADirectionBottom     CADirectionDown

typedef uintptr_t    CADirection;


MULLE__GEOMETRY_GLOBAL
CADirection   CADirectionTurn45ClockWise( CADirection direction);

MULLE__GEOMETRY_GLOBAL
CADirection   CADirectionTurn45CounterClockWise( CADirection direction);

MULLE__GEOMETRY_GLOBAL
CADirection   CADirectionTurn90ClockWise( CADirection direction);

MULLE__GEOMETRY_GLOBAL
CADirection   CADirectionTurn90CounterClockWise( CADirection direction);

MULLE__GEOMETRY_GLOBAL
CADirection   CADirectionTurn180( CADirection direction);

static inline CADirection   CADirectionTurn270ClockWise( CADirection direction)
{
   return( CADirectionTurn90CounterClockWise( direction));
}


static inline CADirection   CADirectionTurn270CounterClockWise( CADirection direction)
{
   return( CADirectionTurn90ClockWise( direction));
}


MULLE__GEOMETRY_GLOBAL
char       *CADirectionUTF8String( CADirection direction);

MULLE__GEOMETRY_GLOBAL
uint32_t   CADirectionBoxCharacter( CADirection direction);

MULLE__GEOMETRY_GLOBAL
uint32_t   CADirectionArrowCharacter( CADirection direction);


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
