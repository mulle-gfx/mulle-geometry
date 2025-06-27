#include "CAQuadratic.h"


CGPoint   CAQuadraticBezierGetPoint( CAQuadraticBezier *b, CGFloat t)
{
   CGPoint   curvePoint;

   curvePoint.x = CAQuadraticGetValue( &b->x, t);
   curvePoint.y = CAQuadraticGetValue( &b->y, t);

   return( curvePoint);
}
