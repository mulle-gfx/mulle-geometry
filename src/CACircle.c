#include "CACircle.h"


unsigned int   CACircleIntersectionsWithLine( CACircle circle,
                                                   CGPoint start,
                                                   CGPoint end,
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

   dx = end.x - start.x;
   dy = end.y - start.y;
   a  = dx * dx + dy * dy;
   b  = 2 * (dx * (start.x - circle.center.x) + dy * (start.y - circle.center.y));
   c  = powf( start.x - circle.center.x, 2)
        + powf( start.y - circle.center.y, 2)
        - powf( circle.radius, 2);

   discriminant = b * b - 4 * a * c;

   if( discriminant < 0)
      return( 0);

   if( discriminant == 0)
   {
      t1 = -b / (2 * a);
      intersection[ 0] = CGPointMake( start.x + t1 * dx, start.y + t1 * dy);
      return( 1);
   }

   t1               = (-b + sqrtf( discriminant)) / (2 * a);
   t2               = (-b - sqrtf( discriminant)) / (2 * a);
   intersection[ 0] = CGPointMake( start.x + t1 * dx, start.y + t1 * dy);
   intersection[ 1] = CGPointMake( start.x + t2 * dx, start.y + t2 * dy);
   return( 2);
}


CGPoint  CACircleNearestIntersectionPoint( CACircle circle,
                                           CGPoint start,
                                           CGPoint end,
                                           CGPoint near)
{
   CGPoint        intersection[ 2];
   CGFloat        d0;
   CGFloat        d1;
   unsigned int   n;

   assert( CGPointIsInsideCircle( start, circle));
   assert( CGPointIsInsideCircle( end, circle));

   n = CACircleIntersectionsWithLine( circle, start, end, intersection);
   assert( n == 2);

   d0 = CGPointSquaredDistanceToPoint( near, intersection[ 0]);
   d1 = CGPointSquaredDistanceToPoint( near, intersection[ 1]);
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



CGPoint   CACirclePointAtDistance( CACircle circle, CAAngle startAngle, CGFloat distance)
{
   CAPolarPoint   q0;
   CAPolarPoint   q1;
   CGPoint        p0;
   CGPoint        intersection[ 2];

//   if( distance > circle.radius * 2.0)
//      return( CGPointNull);

   q0 = CAPolarPointMakeWithAngle( circle.radius, startAngle);
   p0 = CGPointMakeWithPolarPoint( q0);
   p0 = CGPointAdd( p0, circle.center);
   if( ! CACircleIntersectionsWithCircle( circle, CACircleMake( p0, fabsf( distance)), intersection))
      return( CGPointNull);

   //
   // given that the second circle is on the first circle, the following
   // observation was made, that the first intersection is always clockwise
   //
   return( distance > 0 ? intersection[ 0] : intersection[ 1]);
}

