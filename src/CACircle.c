#include "CACircle.h"


unsigned int   CACircleIntersectionsWithLine( CACircle circle,
                                              CGPoint startPoint,
                                              CGPoint endPoint,
                                              CGPoint intersection[ 2])
{
   CGFloat t1;
   CGFloat t2;
   CGFloat dx;
   CGFloat dy;
   CGFloat a;
   CGFloat b;
   CGFloat c;
   CGFloat discriminant;

   dx = endPoint.x - startPoint.x;
   dy = endPoint.y - startPoint.y;
   a  = dx * dx + dy * dy;
   b  = 2 * (dx * (startPoint.x - circle.center.x) + dy * (startPoint.y - circle.center.y));
   c  = powf( startPoint.x - circle.center.x, 2)
        + powf( startPoint.y - circle.center.y, 2)
        - powf( circle.radius, 2);

   discriminant = b * b - 4 * a * c;

   if( discriminant <= 0.0f + CA_EPSILON)
   {
      if( discriminant < 0.0f)
         return( 0);

      t1 = -b / (2 * a);
      intersection[ 0] = CGPointMake( startPoint.x + t1 * dx, startPoint.y + t1 * dy);
      return( 1);
   }

   t1               = (-b + sqrtf( discriminant)) / (2 * a);
   t2               = (-b - sqrtf( discriminant)) / (2 * a);
   intersection[ 0] = CGPointMake( startPoint.x + t1 * dx, startPoint.y + t1 * dy);
   intersection[ 1] = CGPointMake( startPoint.x + t2 * dx, startPoint.y + t2 * dy);
   return( 2);
}


CGPoint  CACircleNearestIntersectionPoint( CACircle circle,
                                           CGPoint startPoint,
                                           CGPoint endPoint,
                                           CGPoint nearPoint)
{
   CGPoint        intersection[ 2];
   CGFloat        d0;
   CGFloat        d1;
   unsigned int   n;

   assert( CGPointIsInsideCircle( startPoint, circle));
   assert( CGPointIsInsideCircle( endPoint, circle));

   n = CACircleIntersectionsWithLine( circle, startPoint, endPoint, intersection);
   assert( n == 2);
   MULLE_C_UNUSED( n);
   
   d0 = CGPointSquaredDistanceToPoint( nearPoint, intersection[ 0]);
   d1 = CGPointSquaredDistanceToPoint( nearPoint, intersection[ 1]);
   return( d0 < d1 ? intersection[ 0] : intersection[ 1]);
}


unsigned int  CACircleIntersectionsWithCircle( CACircle c0,
                                               CACircle c1,
                                               CGPoint intersection[ 2])
{
   CGFloat dx;
   CGFloat dy;
   CGFloat d;
   CGFloat ex;
   CGFloat ey;
   CGFloat a;
   CGFloat h;

   dx = c1.center.x - c0.center.x;
   dy = c1.center.y - c0.center.y;
   d = sqrtf( dx * dx + dy * dy);

   // Circles too far apart or one circle completely inside the other
   if( d > c0.radius + c1.radius || d < fabsf(c0.radius - c1.radius))
    return( 0);

   ex = dx / d;
   ey = dy / d;
   a  = (c0.radius * c0.radius - c1.radius * c1.radius + d * d) / (2 * d);
   h  = sqrtf( c0.radius * c0.radius - a * a);

   intersection[ 0] =  CGPointMake( c0.center.x + a * ex - h * ey,
                                    c0.center.y + a * ey + h * ex);
   intersection[ 1] =  CGPointMake( c0.center.x + a * ex + h * ey,
                                    c0.center.y + a * ey - h * ex);

   return( 2);
}



CGPoint   CACirclePointAtDistance( CACircle circle, 
                                   CAAngle startAngle, 
                                   CGFloat distance)
{
   CAPolarPoint   q0;
   CGPoint        p0;
   CGPoint        intersection[ 2];

//   if( distance > circle.radius * 2.0)
//      return( CGPointNull);

   q0 = CAPolarPointMakeWithAngle( circle.radius, startAngle);
   p0 = CGPointMakeWithPolarPoint( q0);
   p0 = CGPointAdd( p0, circle.center);
   if( ! CACircleIntersectionsWithCircle( circle, 
                                          CACircleMakeWithCenterAndRadius( p0, fabsf( distance)),
                                          intersection))
      return( CGPointNull);

   //
   // given that the second circle is on the first circle, the following
   // observation was made, that the first intersection is always clockwise
   //
   return( distance > 0 ? intersection[ 0] : intersection[ 1]);
}


// MEMO: AI generated and untested
CGRect   CACircleOuterRectOfSector( CACircle circle,
                                    CAAngle startAngle,
                                    CAAngle endAngle)
{
   CGRect    box;
   CGPoint   ps;
   CGPoint   pe;
   CGFloat   startRadians;
   CGFloat   endRadians;

   if( startAngle == endAngle)
      return( CGRectZero);

   startRadians = CAAngleToRadians( startAngle);
   endRadians   = CAAngleToRadians( endAngle);

   ps = CACirclePointAtAngle( circle, startAngle);
   pe = CACirclePointAtAngle( circle, endAngle);

   box = CGRectMakeWithPoints( ps, pe);

   // If the sector wraps around, the bounding box is the whole circle
   if (endRadians < startRadians)
   {
      CGFloat temp = startRadians;
      startRadians = endRadians;
      endRadians = temp;
   }

   if (endRadians - startRadians >= CA_2xPI)
      return CACircleGetOuterRect(circle);


   // Check if the cardinal directions are within the arc
   if (startRadians < CAAngleE && endRadians > CAAngleE)
      box = CGRectEnclosePoint( box, CGPointMake( circle.center.x + circle.radius, circle.center.y));
   if (startRadians < CAAngleS && endRadians > CAAngleS)
      box = CGRectEnclosePoint( box, CGPointMake( circle.center.x, circle.center.y + circle.radius));
   if (startRadians < CAAngleW && endRadians > CAAngleW)
      box = CGRectEnclosePoint( box, CGPointMake( circle.center.x - circle.radius, circle.center.y));
   if (startRadians < CAAngleN && endRadians > CAAngleN)
      box = CGRectEnclosePoint( box, CGPointMake( circle.center.x, circle.center.y - circle.radius));

   return( box);
}
