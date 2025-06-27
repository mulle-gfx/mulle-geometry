#ifndef ca_quadratic_h__
#define ca_quadratic_h__


#include "include.h"

#include <assert.h>

#include "CGGeometry.h"

//// A quadratic Bezier curve.
//
typedef struct CAQuadratic
{
    CGFloat   value[3];  // 3 control points for quadratic
} CAQuadratic;


static inline CAQuadratic   CAQuadraticMake( CGFloat p0, CGFloat p1, CGFloat p2)
{
    return((struct CAQuadratic) {
        .value[0] = p0,
        .value[1] = p1,
        .value[2] = p2
    });
}

static inline void   CAQuadraticInit(CAQuadratic *q,
                                    CGFloat p0,
                                    CGFloat p1,
                                    CGFloat p2)
{
    assert(p0 >= 0.0 && p0 <= 1.0);
    assert(p1 >= 0.0 && p1 <= 1.0);
    assert(p2 >= 0.0 && p2 <= 1.0);

    q->value[0] = p0;
    q->value[1] = p1;
    q->value[2] = p2;
}

#define CAQuadraticLinear    ((struct CAQuadratic) { 0.0f, 0.5f, 1.0f })
#define CAQuadraticEaseIn    ((struct CAQuadratic) { 0.0f, 0.2f, 1.0f })
#define CAQuadraticEaseOut   ((struct CAQuadratic) { 0.0f, 0.8f, 1.0f })

////
// Quadratic Bezier formula: B(t) = (1-t)² * P0 + 2(1-t)t * P1 + t² * P2
//
static inline CGFloat   _CAQuadraticGetValue(CAQuadratic b, CGFloat t)
{
    CGFloat   value;
    CGFloat   oneMinusT;

    assert(t >= 0.0 && t <= 1.0);

    oneMinusT = 1.0 - t;

    value = oneMinusT * oneMinusT * b.value[0] +
            2 * oneMinusT * t * b.value[1] +
            t * t * b.value[2];

    return(value);
}


static inline CGFloat   CAQuadraticGetValue( CAQuadratic *b, CGFloat t)
{
    if( ! b)
        return(0.0);
    return(_CAQuadraticGetValue(*b, t));
}



typedef struct CAQuadraticBezier
{
    CAQuadratic   x;
    CAQuadratic   y;
} CAQuadraticBezier;  // not CGPoint for supposed improved CPU caching


static inline CAQuadraticBezier   CAQuadraticBezierMake(CGPoint p0, CGPoint p1, CGPoint p2)
{
    return((struct CAQuadraticBezier)
    {
        .x = CAQuadraticMake( p0.x, p1.x, p2.x),
        .y = CAQuadraticMake( p0.y, p1.y, p2.y)
    });
}

static inline void   CAQuadraticBezierInit( CAQuadraticBezier *b,
                                            CGPoint p0,
                                            CGPoint p1,
                                            CGPoint p2)
{
    CAQuadraticInit( &b->x, p0.x, p1.x, p2.x);
    CAQuadraticInit( &b->y, p0.y, p1.y, p2.y);
}


CGPoint   CAQuadraticBezierGetPoint( CAQuadraticBezier *b, CGFloat t);

#endif
