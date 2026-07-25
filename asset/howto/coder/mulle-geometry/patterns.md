<!-- Keywords: CGPoint, CGRect, CAAngle, CACircle, CAPolarPoint, CGAffineTransform, CADirection, CAQuadratic, CACubic, CAPose -->

## CGBase: points, sizes, rects, vectors

Create with `*Make()` — not manual struct fields:

```c
CGPoint pt = CGPointMake(10, 20);
CGSize  sz = CGSizeMake(100, 50);
CGRect  rc = CGRectMake(10, 20, 100, 50);
CGVector v = CGVectorMake(1, 0);
```

Use accessors instead of direct field reads for rects — they handle negative dimensions:

```c
CGFloat minX = CGRectGetMinX(rect);   // not rect.origin.x
CGFloat midY = CGRectGetMidY(rect);
CGFloat area = CGRectGetArea(rect);
```

From `test/50_cgrect/intersect.c`: always check `CGRectIsNull(intersection)` after `CGRectIntersection`:

```c
CGRect r = CGRectIntersection(a, b);
if (!CGRectIsNull(r)) { /* use r */ }
```

Use `CGRectStandardize()` when dimensions may be negative (e.g. after user input).

Vector from an angle:

```c
CGVector normal = CGVectorMakeWithAngle(CADegreesToAngle(45));  // line normal
```

## CAAngle: angle and rotation system

From `test/15_angle/angle-rotate.c`: convert angles via
rotation to animate towards a target angle in a given direction:

```c
CAAngle    from, to;
CARotation diff;

from = CARotationToAngle(rot1);
to   = CARotationToAngle(rot2);
diff = CAAngleGetRotationTowardsAngle(from, to, CARotationDirectionClockWise);
CAAngle result = CAAngleRotate(from, diff);
```

Use `CAAngle` for absolute orientation (0 = East, normalized to [0, 2π)).
Use `CARotation` for accumulated animation (can be negative, > 1.0, 0 = North).

```c
CAAngle deg = CADegreesToAngle(90);          // 90° clockwise from East
CAAngle hourAngle = CAHourToAngle(3);        // 3 o'clock
CGFloat degrees  = CAAngleToDegrees(angle);  // back to degrees [0,360)
CARotation rot   = CAAngleToRotation(angle); // 0 = North
```

16-point compass constants are available: `CAAngleN`, `CAAngleNNE`, ..., `CAAngleNNW`.
Wind-name aliases: `CAAngleTramontana` (N), `CAAngleGreco` (NE), etc.

From `src/CAAngle.h:440-455` — mid-angle bisects clockwise from `a` to `b`:

```c
// CAAngleGetMidAngle(40°, 60°) → 50°
// CAAngleGetMidAngle(60°, 60°) → 230°
CAAngle mid = CAAngleGetMidAngle(start, end);
```

## CACircle: circle creation, queries, intersection

From `test/80_circle/collision.c`:

```c
CACircle a = CACircleMake(0, 0, 50);
CACircle b = CACircleMake(60, 0, 50);

CGFloat  dist    = CACircleDistanceToCircle(a, b);
int      overlap = CACircleIntersectsCircle(a, b);
CGFloat  amount  = CACircleOverlapWithCircle(a, b);
CGVector normal  = CACircleCollisionNormal(a, b);
```

Circle → rect conversion:

```c
CGRect outer = CACircleGetOuterRect(circle);   // bounding box
CGRect inner = CACircleGetInnerRect(circle);   // inscribed square
CACircle ic  = CGRectGetInnerCircle(rect);     // inscribed circle
CACircle oc  = CGRectGetOuterCircle(rect);     // circumscribed
```

From `demo/src/main-sector.m` — arc path building:

```c
CGPoint p0 = CACirclePointAtAngle(outerCircle, startAngle);
// then use CACirclePointAtAngle with inner/outer for sector path
```

From `src/CACircle.h:155-183` — arc distances with direction:

```c
CGFloat arcLen = CACircleDistanceBetweenAngles(circle, angleA, angleB,
                                                CARotationDirectionClockWise);
CAAngle newAngle = CACircleAngleByAddingDistance(circle, start, distance,
                                                  CARotationDirectionClockWise);
```

From `src/CACircle.h:354-372` — circle-line intersection:

```c
CGPoint intersections[2];
unsigned int n = CACircleIntersectionsWithLine(circle, lineStart, lineEnd, intersections);
CGPoint near = CACircleNearestIntersectionPoint(circle, start, end, nearPt);
```

## CAPolarPoint: polar coordinates

From `demo/src/main-polarpoint.m` — polar + rotation for animated circular layout:

```c
CAPolarPoint pPoint = CAPolarPointMakeWithAngle(radius, CADegreesToAngle(deg));
CGPoint      point  = CGPointMakeWithPolarPoint(pPoint);
```

From `demo/src/main-sector.m` — polar with center offset:

```c
CGPoint center = CGPointAdd(CGPointMakeWithPolarPoint(q),
                            CGRectGetCenterPoint(rect));
```

From `src/CAPolarPoint.h:134-143` — standardize to unique representation:

```c
CAPolarPoint std = CAPolarPointStandardize(p);  // positive radius, [0, 2π)
```

From `src/CAPolarPoint.h:163-174` — bidirectional conversion:

```c
CAPolarPoint pol = CAPolarPointMakeWithPoint(cgPoint);
CGPoint      pt  = CGPointMakeWithPolarPoint(pol);
```

From `src/CAPolarPoint.h:88-95` — create from rotation (0 = North):

```c
CAPolarPoint pol = CAPolarPointMakeWithRotation(radius, rotation);
```

## CADirection: direction bitfield

From `src/CADirection.h:21-28` — values can be ORed:

```c
CADirection dir = CADirectionTop | CADirectionRight;  // upper-right quadrant
```

From `demo/src/main-rect.m` — direction-related rect edge queries:

```c
CGPoint tl = CGRectGetTopLeft(rect);
CGPoint br = CGRectGetBottomRight(rect);
CGPoint cn = CGRectGetCenterPoint(rect);
```

From `src/CAAngle.h:701-714` — direction ↔ rotation:

```c
CARotation rot = CADirectionToRotation(CADirectionBottom);  // → CARotationS
CAAngle    ang = CADirectionToAngle(CADirectionTop | CADirectionLeft); // → CAAngleNW
```

## CAQuadratic / CACubic: Bezier easing

Prefer the provided constants over custom curves:

```c
// Quadratic
CAQuadratic linear  = CAQuadraticLinear;    // (0, 0.5, 1)
CAQuadratic easeIn  = CAQuadraticEaseIn;    // (0, 0.2, 1)
CAQuadratic easeOut = CAQuadraticEaseOut;   // (0, 0.8, 1)

// Cubic
CACubic linear      = CACubicLinear;        // (0, 0.333, 0.666, 1)
CACubic easeIn      = CACubicEaseIn;        // (0, 0, 0.1, 1)
CACubic easeOut     = CACubicEaseOut;       // (0, 0.9, 0.9, 1)
CACubic easeInOut   = CACubicEaseInOut;     // (0, 0.025, 0.975, 1)
```

Evaluate at t ∈ [0, 1]:

```c
CGFloat value = CAQuadraticGetValue(&linear, t);
CGFloat value = _CACubicGetValue(easeIn, t);    // inline, ~100 bytes
```

From `demo/src/main-sector.m` — custom cubic for animation:

```c
factor = _CACubicGetValue(((struct CACubic){0.0f, 0.010f, 0.990f, 1.0f}), factor);
```

From `src/CAQuadratic.h:49-60` — 1D quadratic Bezier `B(t) = (1-t)²·P0 + 2(1-t)t·P1 + t²·P2`.

2D Bezier: `CAQuadraticBezierGetPoint()` / `CACubicBezierGetPoint()`.

## CGAffineTransform: 2D affine transforms

Transform struct is identical to nanoVG (column-major-ish layout: `[a c tx; b d ty]`).

From `demo/src/main-cutter.m` — compose transforms and apply:

```c
CGAffineTransform t;
t = CGAffineTransformMakeIdentityInline();
t = CGAffineTransformRotate(t, radians);
t = CGAffineTransformTranslate(t, dx, dy);

// Apply
CGPoint transformed = CGPointApplyAffineTransform(original, t);
CGRect  transformedRect = CGRectApplyAffineTransform(rect, t);
```

From `demo/src/main-rotate.m` — invert and compose:

```c
CGAffineTransform t = CGAffineTransformMakeRotation(factor);
CGAffineTransform inverted = CGAffineTransformInvert(t);
```

From `test/30_projection/transform-concat.c` — projection transforms:

```c
CGAffineTransform proj = CGAffineTransformMakeProjection(frame, bounds);
CGAffineTransform inv  = CGAffineTransformMakeInverseProjection(frame, bounds);
CGAffineTransform composed = CGAffineTransformConcat(t2, t1);
```

From `src/CGAffineTransform.h:43-56` — direct construction:

```c
CGAffineTransform scaleT = CGAffineTransformMakeScaleInline((CGVector){2.0, 2.0});
CGAffineTransform skewT  = CGAffineTransformMakeSkewInline(angleX, angleY);
```

## CAPose: position + orientation

From `src/CAPose.h:27-78` — created for animation paths, stores normal vector internally:

```c
CAPose pose = CAPoseMake(x, y, dx, dy);              // explicit components
CAPose pose = CAPoseMakeWithPositionAndRotation(pos, rotation); // from CARotation
CGPoint pos = CAPoseGetPosition(pose);
CGVector n  = CAPoseGetNormal(pose);
CARotation r = CAPoseGetRotation(pose);
CAAngle    a = CAPoseGetAngle(pose);
```

Sentinel: `CAPoseNull`, zero: `CAPoseZero`.

## CGGeometry: polygon clipping and edge ops

From `demo/src/main-cutter.m` — Sutherland-Hodgman polygon clipping:

```c
CGPoint subject[4], clipRect[4], result[8];
unsigned int count = CGPolygon4ClipAgainstPolygon4(subject, clipRect, result);

CGRect bbox = CGPolygonBoundingBox(result, count);
```

From `src/CGGeometry.h:61-` — edge intersection and containment:

```c
int inside = CGEdgeContainsPoint(edgeStart, edgeEnd, point);  // clockwise half-plane
int inside = CGEdgeContainsPointCounterClockWise(start, end, point);
```

Snapping:

```c
CGPointSnapDefault(point);       // snap to integer + CA_EPSILON
CGRectSnap(rect, 0.5, 0.01);    // snap to nearest 0.5
```

Random generation:

```c
CGPoint randPt = CGPointMakeRandomInRect(rect);
CGSize  randSz = CGSizeMakeRandom(minSize, maxSize);
CGRect  randRc = CGRectMakeRandomInRect(rect, minSize, maxSize);
```
