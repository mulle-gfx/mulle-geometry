//
//  CAQuadratic.h
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


MULLE__GEOMETRY_GLOBAL
CGPoint   CAQuadraticBezierGetPoint( CAQuadraticBezier *b, CGFloat t);

#endif
