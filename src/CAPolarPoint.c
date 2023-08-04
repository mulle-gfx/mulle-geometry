#include "CAPolarPoint.h"



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

   a        = q.radians - p.radians;
   u        = q.radius * cosf( a);
   v        = q.radius * sinf( a);
   r.radius = sqrtf( (p.radius + u) * (p.radius + u) + (v * v));
   r.radians  = p.radians + atan2( v, p.radius + u);
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
   r.radians  = p.radians - atan2( v, p.radius - u);
   return( r);
}


//
// formula stolen from: https://www.kristakingmath.com/blog/distance-between-polar-points
//
CGFloat   CAPolarPointDistanceToPolarPoint( CAPolarPoint a, CAPolarPoint b)
{
   // cosf returns values from -1 to +1, negative sqrtf is bad though
   // Can we produce negative ?
   // 0.1 * 0.1 = 0.01  0.1 * 0.1 = 0.01  0.01 + 0.01 = 0.02
   // 2 * 0.1 * 0.1 = 0.2 * (-1 .. 1) 
   // doesn't look like it
   return( sqrtf( a.radius * a.radius + b.radius * b.radius -           /* P1 */ 
                  2 * a.radius * b.radius * cosf( a.radians - b.radians))); /* P2 */
}
