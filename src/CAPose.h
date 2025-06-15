//
// Copyright (c) <|YEAR|> Nat!, Mulle kybernetiK
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
