//
//  CAPose.h
//  mulle-geometry
//
//  Copyright (c) 2025 Nat! - Mulle kybernetiK.
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
#ifndef capose_h__
#define capose_h__

#include "CAAngle.h"
#include "CGGeometry.h"

//
// a CAPose is a combination of a point with a rotation. Because we prefer
// rotations over angles. Internally the angle is stored as a normal vector,
// which you can use to get an angle or rotation from. Why done like this ?
// Well we use CAPose only for animation and having a quadruple float struct
// there, simplifies stuff a lot and it works fine with its chief application
// CGPath.
//
// https://en.wikipedia.org/wiki/Pose_(computer_vision)
//
typedef struct CAPose
{
   CGPoint    position;
   CGVector   normal;
} CAPose;


static inline CAPose   CAPoseMake( CGFloat x, CGFloat y, CGFloat dx, CGFloat dy)
{
   CAPose   pose;

   pose.position = CGPointMake( x, y);
   pose.normal   = CGVectorMake( dx, dy);
   return( pose);
}


static inline CAPose   CAPoseMakeWithPositionAndRotation( CGPoint position, CARotation rotation)
{
   CAPose   pose;

   pose.position = position;
   pose.normal   = CGVectorMakeWithRotation( rotation);
   return( pose);
}


static inline CAPose   CAPoseMakeWithPositionAndNormal( CGPoint position, CGVector normal)
{
   CAPose   pose;

   pose.position = position;
   pose.normal   = normal;
   return( pose);
}


static inline CGPoint   CAPoseGetPosition( CAPose pose)
{
   return( pose.position);
}


static inline CGVector   CAPoseGetNormal( CAPose pose)
{
   return( pose.normal);
}


static inline CARotation   CAPoseGetRotation( CAPose pose)
{
   return( CGVectorGetRotation( pose.normal));
}


static inline CAAngle   CAPoseGetAngle( CAPose pose)
{
   return( CGVectorGetAngle( pose.normal));
}


#define CAPoseZero   CAPoseMake( 0, 0, 0, 0)
#define CAPoseNull   ((CAPose){ .position = CGPointNull, .normal = CGVectorNull })




static inline CGFloat4   CAPoseGetFloat4( CAPose p)
{
   return( CGFloat4Make( p.position.x, p.position.y, p.normal.dx, p.normal.dy));
}



#endif
