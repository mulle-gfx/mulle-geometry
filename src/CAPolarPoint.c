//
//  CAPolarPoint.c
//  mulle-geometry
//
//  Copyright (c) 2021 Nat! - Mulle kybernetiK.
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
#include "CAPolarPoint.h"


#include "CAAngle.h"


// these functions get fairly huge!
CAPolarPoint   CAPolarPointAdd1( CAPolarPoint p, CAPolarPoint q)
{
   CGPoint   tmp;

   tmp = CGPointAdd( CGPointMakeWithPolarPoint( p), CGPointMakeWithPolarPoint( q));
   return( CAPolarPointMakeWithPoint( tmp));
}

//
// https://math.stackexchange.com/questions/1365622/adding-two-polar-vectors
// implemented from: https://math.stackexchange.com/a/1365667
//
CAPolarPoint   CAPolarPointAdd2( CAPolarPoint p, CAPolarPoint q)
{
   CAPolarPoint   r;
   CGFloat        a;
   CGFloat        u;
   CGFloat        v;

   a          = q.radians - p.radians;
   u          = q.radius * cosf( a);
   v          = q.radius * sinf( a);
   r.radius   = sqrtf( (p.radius + u) * (p.radius + u) + (v * v));
   r.radians  = p.radians + atan2f( v, p.radius + u);
   return( r);
}



// these functions get fairly huge!
CAPolarPoint   CAPolarPointSubtract1( CAPolarPoint p, CAPolarPoint q)
{
   CGPoint   tmp;

   tmp = CGPointSubtract( CGPointMakeWithPolarPoint( p), CGPointMakeWithPolarPoint( q));
   return( CAPolarPointMakeWithPoint( tmp));
}



CAPolarPoint   CAPolarPointSubtract2( CAPolarPoint p, CAPolarPoint q)
{
   CAPolarPoint   r;
   CGFloat        a;
   CGFloat        u;
   CGFloat        v;

   a        = q.radians - p.radians;
   u        = q.radius * cosf( a);
   v        = q.radius * sinf( a);
   r.radius = sqrtf( (p.radius - u) * (p.radius - u) + (v * v));
   r.radians  = p.radians - atan2f( v, p.radius - u);
   return( r);
}


//
// formula stolen from: https://www.kristakingmath.com/blog/distance-between-polar-points
//
CGFloat   CAPolarPointDistanceToPolarPoint( CAPolarPoint a, CAPolarPoint b)
{
   CGFloat   distance;

   // cosf returns values from -1 to +1, negative sqrtf is bad though
   // Can we produce negative ?
   // 0.1 * 0.1 = 0.01  0.1 * 0.1 = 0.01  0.01 + 0.01 = 0.02
   // 2 * 0.1 * 0.1 = 0.2 * (-1 .. 1) 
   // doesn't look like it
   distance = sqrtf( a.radius * a.radius + b.radius * b.radius -              /* P1 */
                     2 * a.radius * b.radius * cosf( a.radians - b.radians)); /* P2 */
   return( distance < CA_EPSILON ? 0.0f : distance);
}
