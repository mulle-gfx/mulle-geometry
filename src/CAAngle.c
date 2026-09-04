//
//  CAAngle.c
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
#include "CAAngle.h"

#include "include-private.h"


// ein bischen gefummelt (0.1), damit die "winde" hübscher passen
// das ganze ist eh nur pi mal daumen
#define DEVIATION  (3.0f / 4.1f)
//#define DEVIATION 0


CADirection   CARotationToDirection( CARotation rotation)
{
   CGFloat       h;
   CADirection   dir;

   h = CARotationToHour( rotation);
   if( h >= 12.0f)
      h = 0.0f;

   // split into top and bottom sector,
   dir = 0;
   if( h > (9.0 + DEVIATION) || h < (3.0 - DEVIATION))
      dir = CADirectionTop;
   else
      if( h < (9.0 - DEVIATION) && h > (3.0 + DEVIATION))
         dir = CADirectionBottom;

   // then split into right and left sector
   if( h > (0.0 + DEVIATION) && h < (6.0 - DEVIATION))
      dir |= CADirectionRight;
   else
      if( h < (12.0 - DEVIATION) && h > (6.0 + DEVIATION))
         dir |= CADirectionLeft;

   return( dir);
}


CGFloat   CARadiansNormalize( CGFloat radians)
{
   return( CARadiansNormalizeInline( radians));
}
