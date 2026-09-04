//
//  CADirection.c
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
#include "CADirection.h"

#include "include-private.h"

#include "CAAngle.h"


static unsigned char   turn45clockwise[ 16] =
{
   0,
   /* CADirectionTop    */                   (CADirectionTop|CADirectionRight),
   /* CADirectionBottom */                   (CADirectionBottom|CADirectionLeft),
   0,

   /* CADirectionLeft   */                   (CADirectionTop|CADirectionLeft),
   /* CADirectionLeft|CADirectionTop  */     (CADirectionTop),
   /* CADirectionLeft|CADirectionBottom  */  (CADirectionLeft),

   0,
   /* CADirectionRight   */                  (CADirectionBottom|CADirectionRight),
   /* CADirectionRight|CADirectionTop  */    (CADirectionRight),
   /* CADirectionRight|CADirectionBottom  */ (CADirectionBottom),
   0,

   0,
   0,
   0,
   0
};


CADirection  CADirectionTurn45ClockWise( CADirection direction)
{
   assert( direction < 16);
   if( direction >= 16)
      return( CADirectionNone);
   return( turn45clockwise[ direction]);
}


static unsigned char   turn45counterclockwise[ 16] =
{
   0,
   /* CADirectionTop    */                   (CADirectionTop|CADirectionLeft),
   /* CADirectionBottom */                   (CADirectionBottom|CADirectionRight),
   0,

   /* CADirectionLeft   */                   (CADirectionBottom|CADirectionLeft),
   /* CADirectionLeft|CADirectionTop  */     (CADirectionLeft),
   /* CADirectionLeft|CADirectionBottom  */  (CADirectionBottom),
   0,

   /* CADirectionRight   */                  (CADirectionTop|CADirectionRight),
   /* CADirectionRight|CADirectionTop  */    (CADirectionTop),
   /* CADirectionRight|CADirectionBottom  */ (CADirectionRight),
   0,

   0,
   0,
   0,
   0
};


CADirection  CADirectionTurn45CounterClockWise( CADirection direction)
{
   assert( direction < 16);
   if( direction >= 16)
      return( CADirectionNone);
   return( turn45counterclockwise[ direction]);
}



static unsigned char   turn90clockwise[ 16] =
{
   0,
   /* CADirectionTop    */                   (CADirectionRight),
   /* CADirectionBottom */                   (CADirectionLeft),
   12,
   /* CADirectionLeft   */                   (CADirectionTop),
   /* CADirectionLeft|CADirectionTop  */     (CADirectionTop|CADirectionRight),
   /* CADirectionLeft|CADirectionBottom  */  (CADirectionTop|CADirectionLeft),
   13,
   /* CADirectionRight   */                  (CADirectionBottom),
   /* CADirectionRight|CADirectionTop  */    (CADirectionBottom|CADirectionRight),
   /* CADirectionRight|CADirectionBottom  */ (CADirectionBottom|CADirectionLeft),
   14,
   3,
   11,
   7,
   15
};


CADirection  CADirectionTurn90ClockWise( CADirection direction)
{
   assert( direction < 16);
   if( direction >= 16)
      return( CADirectionNone);
   return( turn90clockwise[ direction]);
}


static unsigned char   turn90counterclockwise[ 16] =
{
   0,
   /* CADirectionTop    */                   (CADirectionLeft),
   /* CADirectionBottom */                   (CADirectionRight),
   12,
   /* CADirectionLeft   */                   (CADirectionBottom),
   /* CADirectionLeft|CADirectionTop  */     (CADirectionBottom|CADirectionLeft),
   /* CADirectionLeft|CADirectionBottom  */  (CADirectionBottom|CADirectionRight),
   14,
   /* CADirectionRight   */                  (CADirectionTop),
   /* CADirectionRight|CADirectionTop  */    (CADirectionTop|CADirectionLeft),
   /* CADirectionRight|CADirectionBottom  */ (CADirectionTop|CADirectionRight),
   13,
   3,
   7,
   11,
   15
};


CADirection   CADirectionTurn90CounterClockWise( CADirection direction)
{
   assert( direction < 16);
   if( direction >= 16)
      return( CADirectionNone);
   return( turn90counterclockwise[ direction]);
}


CADirection   CADirectionTurn180( CADirection direction)
{
   assert( direction < 16);
   // 180 degree turn is direction-agnostic, so just apply a 90 degree
   // turn twice rather than maintaining a third redundant (and error-prone)
   // lookup table.
   return( CADirectionTurn90ClockWise( CADirectionTurn90ClockWise( direction)));
}


static char   *names[ 16] =
{
   "0",
   "CADirectionTop",
   "CADirectionBottom",
   "CADirectionTop|CADirectionBottom",

   "CADirectionLeft",
   "CADirectionTop|CADirectionLeft",
   "CADirectionBottom|CADirectionLeft",
   "CADirectionTop|CADirectionBottom|CADirectionLeft",

   "CADirectionRight",
   "CADirectionTop|CADirectionRight",
   "CADirectionBottom|CADirectionRight",
   "CADirectionTop|CADirectionBottom|CADirectionRight",

   "CADirectionLeft|CADirectionRight",
   "CADirectionTop|CADirectionLeft|CADirectionRight",
   "CADirectionBottom|CADirectionLeft|CADirectionRight",
   "CADirectionTop|CADirectionBottom|CADirectionLeft|CADirectionRight"
};


char   *CADirectionUTF8String( CADirection direction)
{
   if( direction >= 16)
      return( "???");

   return( names[ direction]);
}


// https://en.wikipedia.org/wiki/Box-drawing_character
static uint32_t   chars90[ 16] =
{
   32,
   0x2575,
   0x2577,
   0x2502,

   0x2574, // CADirectionLeft
   0x2518,
   0x2510,
   0x2524,

   0x2576, // CADirectionRight
   0x2514,
   0x250C,
   0x251C,

   0x2500, // CADirectionLeft|CADirectionRight
   0x2534,
   0x252C,
   0x253C
};



uint32_t   CADirectionBoxCharacter( CADirection direction)
{
   if( direction >= 16)
      return( '?');

   return( chars90[ direction]);
}


static uint32_t   arrows[ 16] =
{
   32,
   0x2191, // CADirectionUp
   0x2193, // CADirectionDown
   0x2195,

   0x2190, // CADirectionLeft
   0x2196,
   0x2199,
   32,

   0x2192, // CADirectionRight
   0x2197,
   0x2198,
   32,

   0x2194, // CADirectionLeft|CADirectionRight
   32,
   32,
   0x1F7C4  // 4 pointed star
};

uint32_t   CADirectionArrowCharacter( CADirection direction)
{
   if( direction >= 16)
      return( '?');

   return( arrows[ direction]);
}

