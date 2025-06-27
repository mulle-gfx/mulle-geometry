#include "CACubic.h"

//
// t is 0 to 1
//
//
// stolen from: https://stackoverflow.com/questions/5634460/quadratic-b%C3%A9zier-curve-calculate-points
//

CGPoint   CACubicBezierGetPoint( CACubicBezier *b, CGFloat t)
{
   CGPoint curvePoint;

   curvePoint.x = CACubicGetValue( &b->x, t);
   curvePoint.y = CACubicGetValue( &b->y, t);

   return( curvePoint);
}
