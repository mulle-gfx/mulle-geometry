#include "MulleQuadratic.h"

//
// t is 0 to 1
//
//
// stolen from: https://stackoverflow.com/questions/5634460/quadratic-b%C3%A9zier-curve-calculate-points
//

CGPoint   MulleQuadraticBezierGetPointForNormalizedDistance(MulleQuadraticBezier *b,
                                                            CGFloat t)
{
   CGPoint curvePoint;

   curvePoint.x = MulleQuadraticGetValueForNormalizedDistance( &b->x, t);
   curvePoint.y = MulleQuadraticGetValueForNormalizedDistance( &b->y, t);

   return( curvePoint);
}
