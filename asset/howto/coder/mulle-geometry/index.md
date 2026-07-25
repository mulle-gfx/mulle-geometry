<!-- Keywords: geometry, 2D, CGPoint, CGRect, CAAngle, CACircle, CAPolarPoint, CADirection, CAQuadratic, CACubic, CGAffineTransform -->

# mulle-geometry: geometric types for 2D graphics

Use this bundle when you need to work with points, rects, angles, circles,
polar coordinates, affine transforms, directions, or Bezier easing curves
in a mulle-gfx project.

## Understand first

```bash
mulle-sde api cat CGBase
mulle-sde api cat CAAngle
mulle-sde api cat CACircle
mulle-sde api cat CAPolarPoint
mulle-sde api cat CGAffineTransform
mulle-sde api cat CGGeometry
mulle-sde api cat CAQuadratic
mulle-sde api cat CACubic
mulle-sde api cat CADirection
```

## API families

| Family | Headers | Purpose |
|--------|---------|---------|
| CGBase | `src/CGBase.h` | `CGFloat`, `CGPoint`, `CGSize`, `CGRect`, `CGVector`, `CGFloat2/3/4` — CoreGraphics-compatible primitives |
| CAAngle | `src/CAAngle.h` | `CAAngle` (normalized [0,2π), East=0), `CARotation` (unnormalized, North=0), degrees, hours, wind names |
| CACircle | `src/CACircle.h` | `CACircle` (center+radius), arc distances, intersection, collision |
| CAPolarPoint | `src/CAPolarPoint.h` | `CAPolarPoint` (radius+radians), Cartesian ↔ polar conversion |
| CADirection | `src/CADirection.h` | `CADirection` bitfield (Up/Down/Left/Right), rotation helpers |
| CAQuadratic | `src/CAQuadratic.h` | 1D/2D quadratic Bezier easing (linear, ease-in, ease-out) |
| CACubic | `src/CACubic.h` | 1D/2D cubic Bezier easing (linear, ease-in, ease-out, ease-in-out) |
| CGAffineTransform | `src/CGAffineTransform.h` | 2×3 affine transform matrix (nanoVG-compatible), `CGPointApplyAffineTransform` etc. |
| CAPose | `src/CAPose.h` | Position + normal vector for animation paths |
| CGGeometry | `src/CGGeometry.h` | Polygon clipping (`CGPolygon4ClipAgainstPolygon4`), edge operations, snapping, random generation |

## Primary workflow

1. Create points/rects with `*Make()` constructors (they assert non-NaN).
2. Convert angles with `CADegreesToAngle()` / `CAAngleToDegrees()`.
3. Use `CAPolarPoint` for circular motion, `CACircle` for circle queries.
4. Use `CAQuadratic`/`CACubic` constants for easing values.
5. Use `CGAffineTransform` for composed transforms — note the nanoVG matrix layout.

## Verify

Compile test programs from `test/` or build demos under `demo/`:

```bash
mulle-sde test
```

Test coverage: `test/90-ai-coverage/`, `test/10_float/`, `test/15_angle/`,
`test/25_polar/`, `test/30_projection/`, `test/50_cgrect/`, `test/60_point/`,
`test/70_direction/`, `test/80_circle/`.
