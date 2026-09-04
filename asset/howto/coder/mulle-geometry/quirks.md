<!-- Keywords: gotchas, pitfalls, invariants, CAAngle, CARotation, CAPolarPoint, CACircle, CGRect, CGAffineTransform, precision -->

## CAAngle vs CARotation — do not confuse

`CAAngle` (East=0, clockwise sweep, normalized to [0, 2π)) is for absolute
orientation. `CARotation` (North=0, unnormalized, unit circle [0,1)) is for
accumulated rotation amounts.

From `src/CAAngle.h:17-36`:
- `CAAngle` does NOT have a `CA_2xPI` sentinel — `0.0` is both 0 and 2π.
  This avoids animation jitter at the wrap boundary.
- `CARotationNormalize` reduces to the canonical orientation in `[0, 1)`:
  `2.5 → 0.5, -10 → 0.0, 1.0 → 0.0`. Use when you want the facing.
- `CARotationGetSweep` answers "how much arc is traced", signed, in `[-1, 1]`:
  `2.5 → 0.5, -10 → -1.0, 1.0 → 1.0`. A full turn stays `1.0` because it
  traces a whole circle; only `0.0` traces nothing. Use for animations and
  arc-sweeping (e.g. `mulle-path`'s arc drawing).
- Assertions guard domain: `CAAngle` functions assert `a >= 0 && a < CA_2xPI`.

## CAAngle origin is East (3 o'clock), not North

- `CAAngleE = 0`, `CAAngleS = π/2`, `CAAngleW = π`, `CAAngleN = 3π/2`
- `CARotationN = 0`, `CARotationE = 0.25`, `CARotationS = 0.5`, `CARotationW = 0.75`
- Conversion: `CAAngleToRotation` subtracts `π/2` then normalizes.

## CAPolarPoint is not unique

From `src/CAPolarPoint.h:36-46`: A CAPolarPoint can represent the same
Cartesian point in multiple ways:
- Negative radius + angle → mirror of positive radius + angle ±π.
- Radians can be negative or > 2π.

**Never compare two `CAPolarPoint` values directly** — the equality function
only checks field equality; convert to `CGPoint` first or use
`CAPolarPointStandardize()`:

```c
CAPolarPoint sp = CAPolarPointStandardize(raw);
// Now radius >= 0, radians in [0, 2π)
```

From `test/25_polar/equal.c`: `CAPolarPointEqualToPolarPoint` is "hit and miss,
mostly miss" (comment in header). Use `CGPoint` comparison instead.

## CACircle radius is always absolute

From `src/CACircle.h:29-41`: `CACircleMake` applies `fabsf(radius)`.
`CACircleExtrude` clamps to `>= 0`. You cannot create a circle with negative
radius — it will be silently flipped.

`CACircleMakeNull` uses `center.x = NAN` as sentinel; check with `CACircleIsNull()`.

## CGVector vs CGPoint — subtle conversion

`CGVectorMakeWithAngle(angle)` creates a **normal** vector
(`sinf(radians), cosf(radians)`), not a point from angle.

From `src/CGGeometry.h:40-45`:
```c
// this creates a normal vector
static inline CGVector CGVectorMakeWithRadians(CGFloat radians) {
   return(CGVectorMake(sinf(radians), cosf(radians)));
}
```

## CGAffineTransform layout matches nanoVG

Matrix is `[a c tx; b d ty]`, which means:
- `a = scaleX`, `b = shearY`, `c = shearX`, `d = scaleY`
- Translation is in `tx`, `ty`.
- `CGAffineTransformMakeRotation(rotation)` takes a `CARotation` (0-1 range),
  while `CGAffineTransformMakeRotationRadiansInline(radians, tooSmall)` takes
  raw radians with an epsilon for near-90° snap.

## `CA_EPSILON` and `CG_VECTOR_EPSILON`

Both are `0.0001f` (from `src/CAAngle.h:14` and `src/CGGeometry.h:34`).
Functions clamp distances, overlaps, and near-zero results to zero when they
fall below this threshold. This means very small circles or near-zero distances
may produce zero results instead of tiny-but-nonzero ones.

## CGRect — negative dimensions handled by accessors

Never read `rect.size.width` directly for layout — use `CGRectGetWidth(rect)`,
`CGRectGetHeight(rect)`, `CGRectGetMinX(rect)`, etc. They return absolute
values. The raw `.size` fields may be negative (e.g. after user input).

`CGRectIntersectsRect(a, b)` returns false for adjacent (touching) rectangles.
But `CGRectIntersection(a, b)` on the same pair may return a zero-area rect
instead of `CGRectNull`. Check both if needed.

`CGRectNull` is `{origin: {NaN, NaN}, size: {0, 0}}`. Check with
`CGRectIsNull()`, not `CGRectEqualToRect(rect, CGRectNull)`.

## `*Make` functions assert non-NaN

From `src/CGBase.h`, constructors like `CGPointMake`, `CGSizeMake` assert that
neither argument is NaN. Use `CGPointNull` / `CGSizeNull` / `CGRectNull`
sentinel values (which deliberately set origin to NaN).

## Random functions use `rand()` (not thread-safe)

From TOC section 4: `CGFloatMakeRandom()` and friends call `rand()` internally.
On platforms where `rand()` is not thread-safe, concurrent calls may produce
unexpected results.

`CGFloatMakeRandom()` returns [0.0, 1.0].
`CGFloatMakeNormalDistribution()` returns normal distribution centered on ~0.5.
`CGFloatMakeAbnormalDistribution()` inverts the normal.

## CAQuadratic/CACubic — unexpected mid-angle edge cases

From `src/CAAngle.h:441-456`:
- `CAAngleGetMidAngle(60°, 60°) → 230°` (not 60°!) — when a == b, it returns
  M_PI radians offset, not the same angle.
- This matters for smooth interpolation between identical starting positions.

## Projection transforms assume same coordinate system

`CGAffineTransformMakeProjection(frame, bounds)` maps between two rect-based
coordinate systems. Both rects are assumed to share the same Cartesian
convention (Y-down). The composition pattern from `test/30_projection/transform-concat.c`
shows how to chain them for nested coordinate spaces.
