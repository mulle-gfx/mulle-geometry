# mulle-geometry Library Documentation for AI
<!-- Keywords: geometry, graphics, primitives, angles, circles, transforms, 2D -->

## 1. Introduction & Purpose

**mulle-geometry** is a foundational C library providing basic geometric types and operations for 2D graphics programming. It defines familiar macOS/iOS-compatible data types (CGRect, CGPoint, CGSize) alongside custom types (CAAngle, CACircle, CAPolarPoint) designed for animation, layout, and geometric calculations. The library emphasizes normalized angle handling, polar coordinate support, and efficient geometric operations. It is a component of the mulle-gfx ecosystem and depends on mulle-core for foundational C functionality.

The library solves the problem of handling 2D geometry in a consistent, type-safe manner with explicit support for angles, rotations, polar coordinates, and affine transformations. Key features include normalized angle types that prevent common animation bugs, separation between absolute angles and relative rotations, circle-based layout primitives, and comprehensive rectangle manipulation functions.

## 2. Key Concepts & Design Philosophy

### Normalized Angles vs. Rotations
The library distinguishes between `CAAngle` (normalized to [0, 2π), representing absolute orientation) and `CARotation` (unnormalized, representing relative rotation amounts). This prevents animation jitter and makes angle comparisons reliable. Origin for `CAAngle` is East (3 o'clock), sweeping clockwise.

### Polar Coordinates
`CAPolarPoint` represents points in polar form (radius, radians). The library provides seamless conversion between Cartesian (`CGPoint`) and polar representations, which is invaluable for circular motion and animations.

### Prefix Conventions
- **CG** prefix: Legacy CoreGraphics-compatible types (`CGPoint`, `CGRect`, `CGSize`, `CGVector`, `CGFloat`)
- **CA** prefix: Custom types specific to this library (`CAAngle`, `CACircle`, `CAPolarPoint`, `CARotation`)

### Union Types
Some types offer union variants (`CGPointU`, `CGSizeU`, `CGRectU`) that expose both named fields and array access (`.value[index]`), useful for axis-agnostic code.

### Coordinate System
Y-axis points down (typical for graphics), X-axis points right. Top-left is origin for rectangles. Rotations are clockwise when viewed in this coordinate system.

### Memory Management
All types are stack-allocated structures. No dynamic memory allocation occurs within the library itself. The library provides inline functions for performance-critical operations.

## 3. Core API & Data Structures

### 3.1. `CGBase.h` - Fundamental Types

#### `CGFloat`
- **Purpose:** Float precision for all geometric calculations
- **Type Definition:** `typedef float CGFloat;`
- **Constants:** `CGFLOAT_MIN`, `CGFLOAT_MAX`, `CGFLOAT_IS_DOUBLE` (0)

#### `CGFloat2`, `CGFloat3`, `CGFloat4`
- **Purpose:** Vector types for grouped float values, compatible with CGPoint/CGSize/CGRect
- **Lifecycle:**
  - `CGFloat2Make(a, b)` - Create 2-component vector
  - `CGFloat3Make(a, b, c)` - Create 3-component vector
  - `CGFloat4Make(a, b, c, d)` - Create 4-component vector
- **Operations:**
  - `CGFloat2EqualToFloat2(a, b)` - Equality test with NaN support
  - `CGFloat3EqualToFloat3(a, b)` - Memory-based equality
  - `CGFloat4EqualToFloat4(a, b)` - Memory-based equality

#### `CGPoint`
- **Purpose:** Cartesian 2D point with x, y coordinates
- **Key Fields:** `CGFloat x, y`
- **Lifecycle:**
  - `CGPointMake(x, y)` - Create point (asserts non-NaN)
  - `CGPointZero` - Constant (0, 0)
  - `CGPointNull` - Sentinel value (NaN, NaN)
- **Core Operations:**
  - `CGPointAdd(p, q)` - Vector addition
  - `CGPointAddVector(p, q)` - Add vector to point
  - `CGPointSubtract(p, q)` - Vector subtraction
  - `CGPointSubtractVector(p, q)` - Subtract vector from point
  - `CGPointMultiply(p, q)` - Component-wise multiplication
  - `CGPointMultiplyVector(p, q)` - Multiply by vector
  - `CGPointScale(p, scale)` - Scalar multiplication
  - `CGPointOffset(p, dx, dy)` - Translation
  - `CGPointTranslate(point, slope, distance)` - Translate along slope
  - `CGPointNegate(p)` - Negation
- **Distance/Geometry:**
  - `CGPointDistanceToPoint(a, b)` - Euclidean distance
  - `CGPointSquaredDistanceToPoint(a, b)` - Squared distance (faster)
  - `CGPointMidPoint(p1, p2)` - Midpoint between two points
  - `CGPointSlope(p1, p2)` - Slope of line
- **Polar Conversion:**
  - `CGPointGetPolarRadius(p)` - Get radius from origin
  - `CGPointGetPolarRadiansAsAngle(p)` - Get angle in radians
  - `CGPointNormalize(p, radius)` - Normalize to given radius
- **Triangle Operations:**
  - `CAAngleBetweenPoints(p0, p1, p2)` - Angle at p0 formed by p1 and p2
  - `CGPointTriangleArea(p0, p1, p2)` - Area of triangle
  - `CGPointTriangleGetCentroid(p0, p1, p2)` - Center of mass
  - `CGPointTriangleGetCenterOfMass(p0, p1, p2)` - Alias for centroid
  - `CGPointTriangleHasMinimumAngle(p0, p1, p2, minDegrees)` - Quality check
  - `CGPointTriangleHasMinimumArea(p0, p1, p2, minArea)` - Quality check
- **Utility:**
  - `CGPointEqualToPoint(a, b)` - Equality test (handles NaN)
  - `CGPointIsNull(p)` - Check for null/sentinel
  - `CGPointIntegral(p)` - Floor to integers
  - `CGPointClampToSize(p, size)` - Clamp to rectangle

#### `CGSize`
- **Purpose:** 2D dimensions with width and height
- **Key Fields:** `CGFloat width, height`
- **Lifecycle:**
  - `CGSizeMake(w, h)` - Create size (asserts non-NaN)
  - `CGSizeZero` - Constant (0, 0)
- **Core Operations:**
  - `CGSizeScale(size, scale)` - Uniform scaling
  - `CGSizeIntegral(size)` - Ceil to integers
  - `CGSizeGetArea(size)` - Area (absolute value)
- **Inspection:**
  - `CGSizeEqualToSize(a, b)` - Equality test
  - `CGSizeContainsSize(a, b)` - Does a contain b?
  - `CGSizeGetInnerRadius(size)` - Inscribed circle radius
  - `CGSizeGetOuterRadius(size)` - Circumscribed circle radius
  - `CGSizeGetAspectRatio(size)` - width/height (0 if height is 0)
  - `CGSizeGetMinimum(size)` - Smaller dimension
  - `CGSizeGetMaximum(size)` - Larger dimension
  - `CGSizeGetCenter(size)` - Center point (width/2, height/2)

#### `CGVector`
- **Purpose:** Direction vector with dx, dy components (rarely used; prefer `CAPolarPoint`)
- **Key Fields:** `CGFloat dx, dy`
- **Lifecycle:**
  - `CGVectorMake(dx, dy)` - Create vector
  - `CGVectorZero` - Constant (0, 0)
  - `CGVectorNull` - Sentinel (NaN, NaN)
- **Core Operations:**
  - `CGVectorScale(v, scale)` - Scalar multiplication
  - `CGVectorLength(v)` - Euclidean length
  - `CGVectorSquaredLength(v)` - Squared length
  - `CGVectorDotProduct(v1, v2)` - Dot product
  - `CGVectorGetAngle(v)` - Get CAAngle from vector
  - `CGVectorGetRotation(v)` - Get CARotation from vector
  - `CGVectorGetAspectRatio(v)` - dx/dy
  - `CGVectorBySubtractingPoints(p, q)` - Create vector from two points
- **Creation from Angles:**
  - `CGVectorMakeWithRadians(radians)` - Create normal vector from radians
  - `CGVectorMakeNormal(radians)` - Alias for MakeWithRadians
  - `CGVectorMakeWithAngle(angle)` - Create from CAAngle
  - `CGVectorMakeWithRotation(rotation)` - Create from CARotation

#### `CGRect`
- **Purpose:** Rectangle with origin and size
- **Key Fields:** `CGPoint origin; CGSize size;`
- **Lifecycle:**
  - `CGRectMake(x, y, w, h)` - Create rectangle
  - `CGRectMakeWithSize(size)` - Rectangle at origin
  - `CGRectMakeWithOriginAndSize(origin, size)` - Explicit construction
  - `CGRectMakeWithPoints(a, b)` - Standardized rect from two points
  - `CGRectZero` - Constant (0, 0, 0, 0)
  - `CGRectNull` - Sentinel (origin is NaN)
  - `CGRectInfinite` - Infinite rect
- **Geometric Queries:**
  - `CGRectGetMinX(rect)`, `CGRectGetMaxX(rect)`, `CGRectGetMidX(rect)` - X-axis positions
  - `CGRectGetMinY(rect)`, `CGRectGetMaxY(rect)`, `CGRectGetMidY(rect)` - Y-axis positions
  - `CGRectGetWidth(rect)`, `CGRectGetHeight(rect)` - Absolute dimensions
  - `CGRectGetArea(rect)` - Area
  - `CGRectGetCenterPoint(rect)` - Center point (midpoint)
  - `CGRectGetCenter(rect)` - Alias for center point
  - `CGRectGetCenterWithAnchorPoint(rect, anchorPoint)` - Center with anchor (0..1, 0..1)
  - `CGRectGetCornerPoint(rect, direction)` - Corner by CADirection
  - `CGRectGetOrigin(rect)`, `CGRectGetOpposite(rect)` - Diagonal corners
  - `CGRectGetTop/Bottom/Left/Right(rect)` - Edge midpoints
  - `CGRectGetTopLeft/TopRight/BottomLeft/BottomRight(rect)` - Corner points
  - `CGRectGetMaximumSize(rect)`, `CGRectGetMinimumSize(rect)` - Larger/smaller dimension
  - `CGRectGetAspectRatio(rect)` - width/height
  - `CGRectGetInnerRadius(rect)`, `CGRectGetOuterRadius(rect)` - Inscribed/circumscribed circle radii
- **Transformations:**
  - `CGRectOffset(rect, dx, dy)` - Translate
  - `CGRectInset(rect, dx, dy)` - Shrink by dx, dy from all sides
  - `CGRectInsetOrZero(rect, dx, dy)` - Inset (returns CGRectZero if too large)
  - `CGRectExtrude(rect, dx, dy)` - Expand by dx, dy on all sides
  - `CGRectExpand(rect, d, direction)` - Expand in specific direction(s)
  - `CGRectContract(rect, d, direction)` - Contract in specific direction(s)
  - `CGRectStandardize(rect)` - Ensure positive width/height
  - `CGRectIntegral(rect)` - Floor origin, ceil size
- **Setters:**
  - `CGRectBySettingMinX/MinY/MidX/MidY/MaxX/MaxY(rect, value)` - Adjust specific edge
- **Set Operations:**
  - `CGRectIntersection(r1, r2)` - Intersection rectangle (CGRectNull if none)
  - `CGRectUnion(r1, r2)` - Smallest rect containing both
  - `CGRectEnclosePoint(rect, point)` - Expand to include point
- **Tests:**
  - `CGRectEqualToRect(a, b)` - Equality test (memcmp-based, handles NaN)
  - `CGRectIsEmpty(rect)` - Zero width or height?
  - `CGRectIsNull(rect)` - Is sentinel value?
  - `CGRectIsInfinite(rect)` - Contains infinity?
  - `CGRectIsStandardized(rect)` - Positive dimensions?
  - `CGRectContainsPoint(rect, point)` - Point inside?
  - `CGRectContainsRect(a, b)` - Does a fully contain b?
  - `CGRectIntersectsRect(a, b)` - Do they overlap?

#### CGFloat Utility Functions
- **Random:**
  - `CGFloatMakeRandom()` - [0.0, 1.0]
  - `CGFloatMakeRandomMinMax(min, max)` - Uniform distribution
  - `CGFloatMakeNormalDistribution()` - Normal distribution [0, 1]
  - `CGFloatMakeAbnormalDistribution()` - Inverted normal distribution
- **Math:**
  - `CGFloatIsZero(x)`, `CGFloatIsOne(x)` - Exact comparisons
  - `CGFloatIsNegativeZero(x)` - Detect -0.0f
  - `CGFloatMaximum(x, y)`, `CGFloatMinimum(x, y)` - Min/max
  - `CGFloatAbsolute(x)`, `CGFloatSign(x)` - Absolute value and sign
  - `CGFloatClamp(x, min, max)` - Clamp to range
  - `CGFloatClampToUnity(x)` - Clamp to [0, 1]
  - `CGFloatModToUnity(x)` - Modulo to [0, 1)
  - `CGFloatProject(value, start1, stop1, start2, stop2)` - Map from one range to another
  - `CGFloatProjectClamped(value, start1, stop1, start2, stop2)` - Project with clamp to output range
  - `CGFloatSnap(value, multiple, epsilon)` - Snap to nearest multiple within epsilon
- **Utility Types:**
  - `CGFloatRange` - struct with `CGFloat min, max`
  - `CGFloatRangeMake(min, max)` - Create range
  - `CGFloatRangeContains(range, value)` - Is value in range?

#### Type Conversion Functions
- **Between Compatible Types:**
  - `CGPointMakeWithSize(size)`, `CGSizeMakeWithPoint(point)` - Point/Size interchange
  - `CGPointMakeWithVector(vector)`, `CGVectorMakeWithPoint(point)` - Point/Vector interchange
  - `CGRectMakeWithFloat4(float4)`, `CGRectGetFloat4(rect)` - Rect/Float4 interchange
  - Multiple `*MakeWith*` functions for all type combinations

### 3.2. `CGGeometry.h` - Extended Geometry Operations

This header extends `CGBase.h` with additional geometric operations:

#### Edge Operations
- **Edge Line Functions:**
  - `CGEdgeIntersect(p1, p2, p3, p4, *intersection)` - Line segment intersection
  - `CGEdgeContainsPoint(edgeStart, edgeEnd, point)` - Half-plane test (clockwise)
  - `CGEdgeContainsPointCounterClockWise(start, end, point)` - CCW half-plane test
  - `CGEdgeMidpointAtDistance(start, end, distance, direction)` - Perpendicular offset point

#### Polygon Operations
- **Polygon Functions:**
  - `CGPolygonClipAgainstEdge(input, n, edgeStart, edgeEnd, output)` - Sutherland-Hodgman clipping
  - `CGPolygonBoundingBox(points, count)` - Axis-aligned bounding box
  - `CGPolygon4ClipAgainstPolygon4(subject[4], clip[4], result[8])` - Quad clipping
  - `CGPolygon4BoundingBox(points[4])` - Bounding box of 4-point polygon

#### Rect Construction Helpers
- `CGRectMakeWithCenterAndInnerRadius(center, radius)` - Square rect from center and radius
- `CGRectMakeWithCenterAndSize(center, size)` - Rect centered at point
- `CGRectMakeWithOriginAndOpposite(a, b)` - Create from origin and opposite corner

#### Random Generation
- **Random Helpers:**
  - `CGPointMakeRandomInRect(rect)` - Random point in rect
  - `CGSizeMakeRandom(minSize, maxSize)` - Random size in range
  - `CGRectMakeRandomInRect(rect, minSize, maxSize)` - Random rect in rect

#### Snap Functions
- **Snapping to Clean Values:**
  - `CGPointSnap(point, multiple, epsilon)` - Snap point components
  - `CGSizeSnap(size, multiple, epsilon)` - Snap size components
  - `CGRectSnap(rect, multiple, epsilon)` - Snap rect components
  - `CGPointSnapDefault(point)` - Snap to integers with CA_EPSILON
  - `CGSizeSnapDefault(size)`, `CGRectSnapDefault(rect)` - Default snap variants

### 3.3. `CAAngle.h` - Angle and Rotation System

#### `CAAngle`
- **Purpose:** Normalized absolute angle in radians [0, 2π), origin at East (3 o'clock)
- **Type Definition:** `typedef CGFloat CAAngle;`
- **Constants (16-point compass):**
  - Cardinal: `CAAngleE`, `CAAngleS`, `CAAngleW`, `CAAngleN` (0, π/2, π, 3π/2)
  - Ordinal: `CAAngleSE`, `CAAngleSW`, `CAAngleNW`, `CAAngleNE`
  - Intercardinal: `CAAngleESE`, `CAAngleSSE`, `CAAngleSSW`, etc.
  - Aliases: `CAAngleNorth`, `CAAngleEast`, `CAAngleSouth`, `CAAngleWest`
  - Wind names: `CAAngleTramontana`, `CAAngleGreco`, `CAAngleLevante`, etc.
- **Conversions:**
  - `CAAngleToRadians(angle)` - Get radians (identity, with assertions)
  - `CARadiansToAngle(radians)` - Normalize radians to [0, 2π)
  - `CAAngleToDegrees(angle)` - Convert to degrees [0, 360)
  - `CADegreesToAngle(degrees)` - Convert from degrees
  - `CAAngleToHour(angle)` - Convert to clock hour [0, 12]
  - `CAHourToAngle(hour)` - Convert from clock hour
  - `CAAngleToRotation(angle)` - Convert to CARotation [0, 1)
  - `CARotationToAngle(rotation)` - Convert from CARotation
  - `CASlopeToAngle(slope)`, `CAAngleToSlope(angle)` - Slope conversions
- **Operations:**
  - `CAAngleAdd(a, b)` - Add angles (wraps to [0, 2π))
  - `CAAngleSubtract(a, b)` - Subtract angles (normalized)
  - `CAAngleAddRadians(a, radians)` - Add raw radians
  - `CAAngleSubtractRadians(a, radians)` - Subtract raw radians
  - `CAAngleRotate(angle, rotation)` - Apply rotation to angle
  - `CAAngleGetMidAngle(a, b)` - Bisecting angle (clockwise from a to b)
  - `CAAngleGetRotationDirection(a0, a1)` - Which direction is closer?
  - `CAAngleGetRotationTowardsAngle(a, b, dir)` - Rotation needed to reach b from a
  - `CAAngleIsOpposingAngle(a, b)` - Are angles opposite? (±90° to ±270°)

#### `CARotation`
- **Purpose:** Unnormalized rotation amount (can be negative, > 1.0 for multiple rotations)
- **Type Definition:** `typedef CGFloat CARotation;`
- **Orientation:** 0.0 = North (12 o'clock), 0.25 = East, 0.5 = South, 0.75 = West
- **Constants (relative):**
  - `CARotationNone` (0), `CARotation45` (1/8), `CARotation90` (1/4), `CARotation180` (1/2)
  - `CARotation45Right` (+1/8), `CARotation45Left` (-1/8), etc.
- **Constants (absolute - CARotationAsAngle):**
  - `CARotationN` (0), `CARotationE` (1/4), `CARotationS` (1/2), `CARotationW` (3/4)
  - Aliases: `CARotationNorth`, `CARotationEast`, etc.
- **Conversions:**
  - `CARotationToDegrees(rotation)` - To degrees [0, 360)
  - `CADegreesToRotation(degrees)` - From degrees
  - `CARotationToRadiansAsAngle(rotation)` - To radians with π/2 offset
  - `CARotationToRadiansAsRotation(rotation)` - To raw radians (×2π)
  - `CARadiansAsAngleToRotation(radians)` - From radians (with offset)
  - `CARadiansAsRotationToRotation(radians)` - From raw radians (÷2π)
- **Operations:**
  - `CARotationNormalize(rotation)` - Clamp to [-1, 1] keeping sign
  - `CARotationAdd(a, b)`, `CARotationSubtract(a, b)` - Arithmetic (no wrapping)
  - `CARotationTurn45/90/180/270ClockWise(rotation)` - Add fixed amounts
  - `CARotationTurn45/90/270CounterClockWise(rotation)` - Subtract fixed amounts
  - `CARotationGetRotationDirection(rotation)` - Clockwise or counter-clockwise?
  - `CARotationIsOpposingRotation(a, b)` - Are rotations opposite?

#### `CARotationDirection`
- **Purpose:** Enum for rotation direction
- **Values:** `CARotationDirectionCounterClockWise` (1), `CARotationDirectionClockWise` (2)
- **Utility:** `CARotationDirectionUTF8String(direction)` - String representation

#### Degrees
- **Purpose:** Work with degrees instead of radians
- **Functions:**
  - `CADegreesNormalize(degrees)` - Normalize to [0, 360)
  - `CADegreesToRadiansAsAngle(deg)` - Convert to radians
  - `CARadiansAsAngleToDegrees(rad)` - Convert from radians

#### Hour (Clock Face)
- **Purpose:** 12-hour clock representation (no zero, 1-12)
- **Functions:**
  - `CAHourNormalize(hour)` - Normalize to (0, 12]
  - `CAHourToRotation(hour)` - Hour to rotation
  - `CARotationToHour(rotation)` - Rotation to hour

### 3.4. `CACircle.h` - Circle Type

#### `CACircle`
- **Purpose:** Circle representation with center and radius
- **Key Fields:** `CGPoint center; CGFloat radius;`
- **Lifecycle:**
  - `CACircleMake(x, y, radius)` - Create circle (radius becomes absolute)
  - `CACircleMakeWithCenterAndRadius(center, radius)` - Explicit construction
  - `CACircleMakeWithCenterAndPerimeter(center, perimeterPoint)` - From center and point
  - `CACircleMakeNull()` - Sentinel circle (center.x = NAN)
  - `CACircleIsNull(circle)` - Is sentinel?
- **Transformations:**
  - `CACircleOffset(circle, dx, dy)` - Translate center
  - `CACircleExtrude(circle, amount)` - Increase radius
  - `CACircleInset(circle, amount)` - Decrease radius
- **Geometric Queries:**
  - `CACircleGetCenter(circle)` - Get center point
  - `CACircleGetRadius(circle)` - Get radius
  - `CACirclePointAtAngle(circle, angle)` - Point on circumference at angle
  - `CACircleAngleAtPoint(circle, point)` - Angle to point from center
  - `CACircleGetOuterRect(circle)` - Bounding box
  - `CACircleGetInnerRect(circle)` - Inscribed square
  - `CACircleGetFloat3(circle)` - Convert to CGFloat3 (x, y, radius)
- **Distance/Arc Calculations:**
  - `CARadiusDistanceBetweenAngles(radius, a, b, direction)` - Arc length
  - `CACircleDistanceBetweenAngles(circle, a, b, direction)` - Circle arc length
  - `CARadiusAngleByAddingDistance(radius, startAngle, distance, direction)` - Angle after traveling arc
  - `CACircleAngleByAddingDistance(circle, startAngle, distance, direction)` - Angle after arc on circle
  - `CACircleDistanceToRotation(circle, rotation)` - Arc length for rotation amount
  - `CACirclePointAtDistance(circle, startAngle, distance)` - Point after traveling arc
- **Collision/Distance:**
  - `CACircleDistanceToCircle(a, b)` - Center-to-center distance
  - `CACircleIntersectsCircle(a, b)` - Do circles overlap? (distance < r1 + r2)
  - `CACircleOverlapWithCircle(a, b)` - Overlap depth (0 if no overlap)
  - `CACircleCollisionNormal(a, b)` - Unit normal vector from a's center to b's center (returns (1,0) if centers coincide)
- **Containment:**
  - `CACircleContainsPoint(circle, point)` - Is point inside or on circle?
  - `CGPointIsInsideCircle(point, circle)` - Is point strictly inside?
- **Intersections:**
  - `CACircleIntersectionsWithCircle(c0, c1, intersection[2])` - Returns count 0-2
  - `CACircleIntersectionsWithLine(circle, start, end, intersection[2])` - Returns count 0-2
  - `CACircleNearestIntersectionPoint(circle, start, end, near)` - Closest intersection
- **Rect Conversions:**
  - `CGRectGetInnerCircle(rect)` - Inscribed circle
  - `CGRectGetOuterCircle(rect)` - Circumscribed circle
  - `CGRectEncloseCircle(rect, circle)` - Expand rect to contain circle
  - `CACircleOuterRectOfSector(circle, startAngle, endAngle)` - Bounding box of arc

### 3.5. `CAPolarPoint.h` - Polar Coordinates

#### `CAPolarPoint`
- **Purpose:** Point in polar coordinates (radius, radians)
- **Key Fields:** `CGFloat radius, radians;`
- **Note:** Radians can be negative or > 2π (unnormalized), origin at East
- **Lifecycle:**
  - `CAPolarPointMake(radius, radians)` - Create polar point
  - `CAPolarPointMakeWithAngle(radius, angle)` - From CAAngle
  - `CAPolarPointMakeWithRotation(radius, rotation)` - From CARotation (North origin)
  - `CAPolarPointMakeWithPoint(point)` - Convert from Cartesian
  - `CAPolarPointMakeWithVectorAndRadius(vector, radius)` - From vector and radius
- **Conversions:**
  - `CGPointMakeWithPolarPoint(polar)` - To Cartesian
  - `CAPolarPointGetRadius(polar)`, `CAPolarPointGetRadians(polar)` - Get components
  - `CAPolarPointGetAngle(polar)` - Get normalized angle
  - `CAPolarPointGetRotation(polar)` - Get rotation
  - `CAPolarPointGetDegrees(polar)` - Get degrees
- **Operations:**
  - `CAPolarPointAdd(p, q)` - Add two polar points (converts to Cartesian)
  - `CAPolarPointSubtract(p, q)` - Subtract polar points
  - `CAPolarPointAddPoint(polar, point)` - Add Cartesian point
  - `CAPolarPointSubtractPoint(polar, point)` - Subtract Cartesian point
  - `CAPolarPointAddRadians(polar, radians)` - Rotate by radians
  - `CAPolarPointSubtractRadians(polar, radians)` - Rotate by -radians
  - `CAPolarPointAddDegrees(polar, degrees)` - Rotate by degrees
  - `CAPolarPointSubtractDegrees(polar, degrees)` - Rotate by -degrees
  - `CAPolarPointSubtractRotation(polar, rotation)` - Rotate by -CARotation
  - `CAPolarPointAddRadius(polar, delta)` - Change radius
  - `CAPolarPointSubtractRadius(polar, delta)` - Decrease radius
  - `CAPolarPointRotate(polar, rotation)` - Apply rotation
  - `CAPolarPointStandardize(polar)` - Normalize to positive radius, [0, 2π)
- **Utility:**
  - `CAPolarPointEqualToPolarPoint(p, q)` - Exact equality (unreliable due to FP)
  - `CAPolarPointDistanceToPolarPoint(a, b)` - Euclidean distance in Cartesian space
  - `CGPointAddPolarPoint(point, polar)` - Add polar to Cartesian
  - `CAPolarPointGetFloat2(polar)` - Convert to CGFloat2 (radius, radians)

### 3.6. `CADirection.h` - Direction Enum

#### `CADirection`
- **Purpose:** Bitfield for cardinal/ordinal directions
- **Type:** `typedef uintptr_t CADirection;`
- **Values (can be ORed):**
  - Cardinal: `CADirectionTop` (1), `CADirectionBottom` (2), `CADirectionLeft` (4), `CADirectionRight` (8)
  - Aliases: `CADirectionUp`, `CADirectionDown`
- **Operations:**
  - `CADirectionTurn45/90/180/270ClockWise(direction)` - Rotate direction
  - `CADirectionTurn45/90/270CounterClockWise(direction)` - Rotate CCW
  - `CADirectionIsOpposingDirection(dir, other)` - Are directions opposite?
- **Conversions:**
  - `CADirectionToRotation(direction)` - To rotation (North = 0)
  - `CARotationToDirection(rotation)` - From rotation
  - `CADirectionToAngle(direction)` - To angle
  - `CAAngleToDirection(angle)` - From angle
- **String/Character:**
  - `CADirectionUTF8String(direction)` - Text representation
  - `CADirectionBoxCharacter(direction)` - Unicode box drawing char
  - `CADirectionArrowCharacter(direction)` - Unicode arrow char

### 3.7. `CAQuadratic.h` - Quadratic Interpolation

#### `CAQuadratic`
- **Purpose:** Quadratic Bezier curve for 1D interpolation (easing functions)
- **Key Fields:** `CGFloat value[3];` (3 control points)
- **Lifecycle:**
  - `CAQuadraticMake(p0, p1, p2)` - Create curve
  - `CAQuadraticInit(quad*, p0, p1, p2)` - Initialize existing
- **Constants:**
  - `CAQuadraticLinear` - Linear interpolation (0, 0.5, 1)
  - `CAQuadraticEaseIn` - Ease in (0, 0.2, 1)
  - `CAQuadraticEaseOut` - Ease out (0, 0.8, 1)
- **Operations:**
  - `CAQuadraticGetValue(quad*, t)` - Evaluate at t ∈ [0, 1]

#### `CAQuadraticBezier`
- **Purpose:** 2D quadratic Bezier curve
- **Key Fields:** `CAQuadratic x, y;`
- **Lifecycle:**
  - `CAQuadraticBezierMake(p0, p1, p2)` - From three CGPoints
  - `CAQuadraticBezierInit(bez*, p0, p1, p2)` - Initialize existing
- **Operations:**
  - `CAQuadraticBezierGetPoint(bez*, t)` - Get point on curve at t

### 3.8. `CACubic.h` - Cubic Interpolation

#### `CACubic`
- **Purpose:** Cubic Bezier curve for advanced easing
- **Key Fields:** `CGFloat value[4];` (4 control points)
- **Lifecycle:**
  - `CACubicMake(p0, p1, p2, p3)` - Create curve
  - `CACubicInit(cubic*, p0, p1, p2, p3)` - Initialize existing
- **Constants:**
  - `CACubicLinear` - Linear (0, 0.333, 0.666, 1)
  - `CACubicEaseIn` - Ease in (0, 0, 0.1, 1)
  - `CACubicEaseOut` - Ease out (0, 0.9, 0.9, 1)
  - `CACubicEaseInOut` - Both (0, 0.025, 0.975, 1)
- **Operations:**
  - `CACubicGetValue(cubic*, t)` - Evaluate at t ∈ [0, 1]

#### `CACubicBezier`
- **Purpose:** 2D cubic Bezier curve
- **Key Fields:** `CACubic x, y;`
- **Lifecycle:**
  - `CACubicBezierMake(p0, p1, p2, p3)` - From four CGPoints
  - `CACubicBezierInit(bez*, p0, p1, p2, p3)` - Initialize existing
- **Operations:**
  - `CACubicBezierGetPoint(bez*, t)` - Get point on curve at t

#### `CATweenFunction`
- **Purpose:** Function pointer type for custom tween functions
- **Signature:** `CGFloat (*CATweenFunction)(struct CACubic *q, CGFloat t);`

#### Built-in Tween Functions
Pre-built Penner-style easing functions using the `CATweenFunction` signature (the CACubic parameter is unused by these). Available in quadratic, cubic, quartic, quintic, sine, exponential, circular, back, bounce, and elastic variants, each with `EaseIn`, `EaseOut`, and `EaseInOut`:
- `CATweenEaseInQuadratic(q, t)` / `CATweenEaseOutQuadratic(q, t)` / `CATweenEaseInOutQuadratic(q, t)`
- `CATweenEaseInCubic(q, t)` / `CATweenEaseOutCubic(q, t)` / `CATweenEaseInOutCubic(q, t)`
- `CATweenEaseInQuartic(q, t)` / `CATweenEaseOutQuartic(q, t)` / `CATweenEaseInOutQuartic(q, t)`
- `CATweenEaseInQuintic(q, t)` / `CATweenEaseOutQuintic(q, t)` / `CATweenEaseInOutQuintic(q, t)`
- `CATweenEaseInSine(q, t)` / `CATweenEaseOutSine(q, t)` / `CATweenEaseInOutSine(q, t)`
- `CATweenEaseInExponential(q, t)` / `CATweenEaseOutExponential(q, t)` / `CATweenEaseInOutExponential(q, t)`
- `CATweenEaseInCircular(q, t)` / `CATweenEaseOutCircular(q, t)` / `CATweenEaseInOutCircular(q, t)`
- `CATweenEaseInBack(q, t)` / `CATweenEaseOutBack(q, t)` / `CATweenEaseInOutBack(q, t)`
- `CATweenEaseInBounce(q, t)` / `CATweenEaseOutBounce(q, t)` / `CATweenEaseInOutBounce(q, t)`
- `CATweenEaseInElastic(q, t)` / `CATweenEaseOutElastic(q, t)` / `CATweenEaseInOutElastic(q, t)`

### 3.9. `CAPose.h` - Position and Orientation

#### `CAPose`
- **Purpose:** Combined position and orientation (for animation paths)
- **Key Fields:** `CGPoint position; CGVector normal;`
- **Note:** Normal is a unit vector representing orientation
- **Lifecycle:**
  - `CAPoseMake(x, y, dx, dy)` - Create with position and normal components
  - `CAPoseMakeWithPositionAndRotation(pos, rotation)` - From rotation (North = 0)
  - `CAPoseMakeWithPositionAndNormal(pos, normal)` - Explicit normal
  - `CAPoseZero` - Zero position and normal
  - `CAPoseNull` - Sentinel value
- **Accessors:**
  - `CAPoseGetPosition(pose)` - Get position
  - `CAPoseGetNormal(pose)` - Get normal vector
  - `CAPoseGetRotation(pose)` - Get rotation from normal
  - `CAPoseGetAngle(pose)` - Get angle from normal
  - `CAPoseGetFloat4(pose)` - Convert to CGFloat4 (x, y, dx, dy)

### 3.10. `CGAffineTransform.h` - 2D Affine Transformations

#### `CGAffineTransform`
- **Purpose:** 2×3 affine transformation matrix (compatible with nanoVG)
- **Key Fields:** `CGFloat a, b, c, d, tx, ty;`
- **Matrix Layout:** `[a c tx] [b d ty]` (column-major-ish)
- **Constants:**
  - `CGAffineTransformIdentity` - Identity matrix
- **Lifecycle:**
  - `CGAffineTransformMakeInline(a, b, c, d, tx, ty)` - Direct construction
  - `CGAffineTransformMakeIdentityInline()` - Identity
  - `CGAffineTransformMakeTranslationInline(point)` - Translation only
  - `CGAffineTransformMakeScaleInline(vector)` - Scale only
  - `CGAffineTransformMakeSkewInline(sx, sy)` / `MakeSkewXInline(angle)` / `MakeSkewYInline(angle)` - Skew matrices (inline only)
  - `CGAffineTransformMakeRotationRadiansInline(radians)` - Rotation from radians
  - `CGAffineTransformMakeRotationInline(rotation)` - Rotation from CARotation
  - Similar non-inline versions: `CGAffineTransformMake()` family, plus `CGAffineTransformMakeScale(CGVector)`, `CGAffineTransformMakeTranslation(CGPoint)`, `CGAffineTransformMakeRotationRadians(CGFloat)`, `CGAffineTransformMakeRotation(CARotation)`
- **Operations:**
  - `CGAffineTransformConcat(t1, t2)` - Compose transforms (t1 then t2)
  - `CGAffineTransformInvert(t)` - Inverse transform
  - `CGAffineTransformTranslate(t, point)` - Apply translation
  - `CGAffineTransformScale(t, s)` - Apply scale (takes CGVector)
  - `CGAffineTransformRotate(t, radians)` - Apply rotation
- **Applying Transforms:**
  - `CGPointApplyAffineTransform(point, t)` - Transform point
  - `CGSizeApplyAffineTransform(size, t)` - Transform size (no translation)
  - `CGVectorApplyAffineTransform(vector, t)` - Transform vector
  - `CGRectApplyAffineTransform(rect, t)` - Transform rectangle
  - `CGRectApplyAffineTransformAtAnchorPoint(rect, t, anchorPoint)` - Transform around anchor (0..1, 0..1)
- **Projection Functions:**
  - `CGAffineTransformMakeProjection(frame, bounds)` - Matrix for frame-to-bounds projection
  - `CGAffineTransformMakeInverseProjection(frame, bounds)` - Inverse projection matrix
  - `CGPointProjectToFrame(location, frame, bounds)` - Project point from bounds to frame
  - `CGPointProjectToBounds(location, frame, bounds)` - Project point from frame to bounds
  - `CGAffineTransformMakeRotationAroundPoint(center, rotation)` - Rotation matrix around arbitrary point
- **Rectangle Rotation:**
  - `CGRectRotate(rect, rotation)` - Rotate rectangle around its center
- **Tests:**
  - `CGAffineTransformIsIdentity(t)` - Is identity?
  - `CGAffineTransformIsIdentityEpsilon(t, epsilon)` - Near identity?
  - `CGAffineTransformEqualToTransform(t1, t2)` - Equality test

## 4. Performance Characteristics

### Time Complexity
- **CGPoint/CGSize/CGRect basic operations:** O(1) - All inline arithmetic
- **Distance calculations:** O(1) - Single sqrt() call
- **Angle normalization:** O(1) - Single fmodf() call
- **Polar↔Cartesian conversions:** O(1) - Trigonometric functions (typically ~10-30 cycles)
- **Rectangle intersections:** O(1) - Constant number of comparisons
- **Polygon clipping:** O(n) where n is input vertex count (Sutherland-Hodgman is linear)
- **Affine transforms:** O(1) per point - 6 multiplications + 4 additions
- **Bezier evaluation:** O(1) - Fixed polynomial computation

### Space Complexity
- All types are stack-allocated, fixed-size structs
- `CGPoint`, `CGSize`, `CGVector`: 8 bytes (2 floats)
- `CGRect`: 16 bytes (4 floats)
- `CACircle`, `CAPolarPoint`: 8 bytes (2 floats)
- `CAPose`: 16 bytes (4 floats)
- `CGAffineTransform`: 24 bytes (6 floats)
- `CAQuadratic`: 12 bytes (3 floats)
- `CACubic`: 16 bytes (4 floats)

### Thread Safety
- **Thread-safe:** All functions are pure or reentrant (no global mutable state)
- **Not synchronized:** No locking mechanisms (unnecessary for read-only operations)
- **Caveat:** Some functions use `rand()` for random generation, which may not be thread-safe depending on platform

### Optimization Notes
- Most operations are inline for zero call overhead
- Trigonometric functions (`sinf`, `cosf`, `atan2f`) are the main performance bottleneck
- Angle normalization uses `fmodf` which is efficient but not constant-time
- `memcmp` is used for struct equality (faster than field-by-field for CGRect/CGFloat4)
- Squared distance functions avoid sqrt() for performance-critical comparisons
- Union types (`CGPointU`, `CGRectU`) enable array-based access without penalty

## 5. AI Usage Recommendations & Patterns

### Best Practices

1. **Always use type-appropriate constructors**: Use `CGPointMake(x, y)` not manual struct initialization. Constructors include NaN assertions.

2. **Prefer CAAngle over raw radians**: `CAAngle` is normalized and safe for comparisons and animations. Use `CARotation` when you need to accumulate rotation amounts.

3. **Use polar coordinates for circular motion**: Convert to `CAPolarPoint`, modify radius/angle, convert back. This is cleaner than manual trig.

4. **Leverage standardization**: Call `CGRectStandardize()` when negative dimensions might exist (e.g., after user input or transformations).

5. **Use union types for axis-agnostic code**:
   ```c
   CGRectU   rect;
   int       axis;
   
   rect.size.value[ axis] += delta;  // No if/else needed
   ```

6. **Prefer `*MakeWith*` over type casts**: Use `CGPointMakeWithSize(size)` rather than `*(CGPoint*)&size` for clarity.

7. **Check for null/sentinel values**: Use `CGRectIsNull()`, `CGPointIsNull()`, `CACircleIsNull()` before operations.

8. **Use squared distance when possible**: `CGPointSquaredDistanceToPoint()` is faster than `CGPointDistanceToPoint()` for comparisons.

### Common Pitfalls

1. **Don't confuse CAAngle and CARotation**: `CAAngle` is absolute (3 o'clock = 0), `CARotation` is relative (12 o'clock = 0).

2. **Don't compare unnormalized polar points**: `CAPolarPoint` can have multiple representations. Convert to `CGPoint` first, or use `CAPolarPointStandardize()`.

3. **Don't accumulate angles with `CAAngle`**: Use `CARotation` for animations that spin multiple times. `CAAngle` will wrap to [0, 2π).

4. **Don't access `CGRect` fields directly for min/max**: Use `CGRectGetMinX()`, `CGRectGetMaxX()`, etc. which handle negative dimensions correctly.

5. **Don't assume `CGRectIntersectsRect()` is symmetric with `CGRectIntersection()`**: Adjacent rectangles don't "intersect" (by design) but intersection may return non-empty result.

6. **Don't use `rand()` in performance-critical code**: Random functions are for testing/demos, not production rendering loops.

7. **Be aware of epsilon**: `CA_EPSILON` (0.0001) is used in some functions. Results near this value may be clamped to zero.

### Idiomatic Usage

#### Creating and positioning a circle
```c
CACircle   circle;
CGPoint    center;
CGFloat    radius;

center = CGPointMake( 100, 100);
radius = 50;
circle = CACircleMakeWithCenterAndRadius( center, radius);
```

#### Animating rotation with accumulated turns
```c
CARotation   rotation;
CAAngle      currentAngle;

rotation     = CARotation90 * elapsedTime;  // Can be > 1.0
currentAngle = CARotationToAngle( rotation);  // Normalized to [0, 2π)
```

#### Converting polar to Cartesian for circular layout
```c
CAPolarPoint   polar;
CGPoint        position;
CAAngle        angle;
CGFloat        radius;

angle    = CADegreesToAngle( 45.0);
radius   = 100.0;
polar    = CAPolarPointMakeWithAngle( radius, angle);
position = CGPointMakeWithPolarPoint( polar);
```

#### Rectangle intersection with null check
```c
CGRect   intersection;

intersection = CGRectIntersection( rectA, rectB);
if( CGRectIsNull( intersection))
   printf( "No intersection\n");
else
   printf( "Area: %f\n", CGRectGetArea( intersection));
```

#### Clipping polygon to rect
```c
CGPoint      quad[ 4];
CGPoint      result[ 8];
unsigned     count;

quad[ 0] = CGPointMake( 0, 0);
quad[ 1] = CGPointMake( 100, 0);
quad[ 2] = CGPointMake( 100, 100);
quad[ 3] = CGPointMake( 0, 100);

count = CGPolygon4ClipAgainstPolygon4( quad, clipRect, result);
```

#### Using affine transform for rotation around point
```c
CGAffineTransform   t;
CGPoint             center;
CGPoint             rotated;
CAAngle             angle;

center = CGPointMake( 50, 50);
angle  = CADegreesToAngle( 45.0);

t       = CGAffineTransformMakeTranslationInline( CGPointNegate( center));
t       = CGAffineTransformRotate( t, CAAngleToRadians( angle));
t       = CGAffineTransformTranslate( t, center.x, center.y);
rotated = CGPointApplyAffineTransform( pointToRotate, t);
```

## 6. Integration Examples

### Example 1: Creating and Manipulating a Rectangle

```c
#include <mulle-geometry/mulle-geometry.h>
#include <stdio.h>

int   main( void)
{
   CGRect   rect;
   CGRect   inset;
   CGRect   standardized;
   CGPoint  center;
   
   rect         = CGRectMake( 10.0, 20.0, 100.0, 50.0);
   inset        = CGRectInset( rect, 5.0, 5.0);
   standardized = CGRectStandardize( rect);
   center       = CGRectGetCenterPoint( rect);
   
   printf( "Original: %.1f,%.1f %.1fx%.1f\n",
           rect.origin.x, rect.origin.y,
           rect.size.width, rect.size.height);
   printf( "Center: %.1f,%.1f\n", center.x, center.y);
   printf( "Inset: %.1f,%.1f %.1fx%.1f\n",
           inset.origin.x, inset.origin.y,
           inset.size.width, inset.size.height);
   
   return( 0);
}
```

### Example 2: Using Angles for Circular Layout

```c
#include <mulle-geometry/mulle-geometry.h>
#include <stdio.h>

int   main( void)
{
   CACircle       circle;
   CAAngle        angle;
   CGPoint        point;
   int            i;
   int            count;
   
   circle = CACircleMakeWithCenterAndRadius( CGPointMake( 200, 200), 100);
   count  = 8;
   
   for( i = 0; i < count; i++)
   {
      angle = CADegreesToAngle( 360.0 * i / count);
      point = CACirclePointAtAngle( circle, angle);
      
      printf( "Point %d: %.1f,%.1f at %.1f degrees\n",
              i, point.x, point.y, CAAngleToDegrees( angle));
   }
   
   return( 0);
}
```

### Example 3: Polar Coordinate Animation

```c
#include <mulle-geometry/mulle-geometry.h>
#include <stdio.h>

void   spiral_point( CGFloat t)
{
   CAPolarPoint   polar;
   CGPoint        position;
   CGPoint        center;
   CARotation     rotation;
   CGFloat        radius;
   
   center   = CGPointMake( 100, 100);
   rotation = t * 2.0;
   radius   = 20.0 + t * 80.0;
   
   polar    = CAPolarPointMakeWithRotation( radius, rotation);
   position = CGPointMakeWithPolarPoint( polar);
   position = CGPointAdd( center, position);
   
   printf( "t=%.2f: pos=(%.1f,%.1f) rotation=%.2f\n",
           t, position.x, position.y, rotation);
}

int   main( void)
{
   CGFloat   t;
   
   for( t = 0.0; t <= 1.0; t += 0.25)
      spiral_point( t);
   
   return( 0);
}
```

### Example 4: Rectangle Intersection and Union

```c
#include <mulle-geometry/mulle-geometry.h>
#include <stdio.h>

int   main( void)
{
   CGRect   a;
   CGRect   b;
   CGRect   intersection;
   CGRect   unionRect;
   
   a = CGRectMake( 0, 0, 100, 100);
   b = CGRectMake( 50, 50, 100, 100);
   
   intersection = CGRectIntersection( a, b);
   unionRect    = CGRectUnion( a, b);
   
   if( CGRectIntersectsRect( a, b))
      printf( "Rectangles intersect\n");
   
   if( !CGRectIsNull( intersection))
   {
      printf( "Intersection: %.1f,%.1f %.1fx%.1f\n",
              intersection.origin.x, intersection.origin.y,
              intersection.size.width, intersection.size.height);
      printf( "Intersection area: %.1f\n",
              CGRectGetArea( intersection));
   }
   
   printf( "Union: %.1f,%.1f %.1fx%.1f\n",
           unionRect.origin.x, unionRect.origin.y,
           unionRect.size.width, unionRect.size.height);
   
   return( 0);
}
```

### Example 5: Using Quadratic Bezier for Easing

```c
#include <mulle-geometry/mulle-geometry.h>
#include <stdio.h>

int   main( void)
{
   CAQuadratic   easeIn;
   CGFloat       t;
   CGFloat       value;
   
   easeIn = CAQuadraticEaseIn;
   
   printf( "Ease-in curve:\n");
   for( t = 0.0; t <= 1.0; t += 0.1)
   {
      value = CAQuadraticGetValue( &easeIn, t);
      printf( "  t=%.1f -> value=%.3f\n", t, value);
   }
   
   return( 0);
}
```

### Example 6: Affine Transform Composition

```c
#include <mulle-geometry/mulle-geometry.h>
#include <stdio.h>

int   main( void)
{
   CGAffineTransform   t;
   CGPoint             original;
   CGPoint             transformed;
   CAAngle             angle;
   
   original = CGPointMake( 100, 0);
   angle    = CADegreesToAngle( 90.0);
   
   t = CGAffineTransformMakeIdentityInline();
   t = CGAffineTransformTranslate( t, 50.0, 50.0);
   t = CGAffineTransformRotate( t, CAAngleToRadians( angle));
   t = CGAffineTransformScale( t, 2.0, 2.0);
   
   transformed = CGPointApplyAffineTransform( original, t);
   
   printf( "Original: (%.1f, %.1f)\n",
           original.x, original.y);
   printf( "Transformed: (%.1f, %.1f)\n",
           transformed.x, transformed.y);
   
   return( 0);
}
```

### Example 7: Circle Arc Distance Calculation

```c
#include <mulle-geometry/mulle-geometry.h>
#include <stdio.h>

int   main( void)
{
   CACircle              circle;
   CAAngle               start;
   CAAngle               end;
   CGFloat               distance;
   CARotationDirection   direction;
   
   circle    = CACircleMakeWithCenterAndRadius( CGPointZero, 100.0);
   start     = CADegreesToAngle( 0.0);
   end       = CADegreesToAngle( 90.0);
   direction = CARotationDirectionClockWise;
   
   distance = CACircleDistanceBetweenAngles( circle, start, end, direction);
   
   printf( "Arc length (CW): %.2f\n", distance);
   printf( "Expected (quarter circle): %.2f\n", M_PI_f * circle.radius / 2.0);
   
   return( 0);
}
```

### Example 8: Direction Operations

```c
#include <mulle-geometry/mulle-geometry.h>
#include <stdio.h>

int   main( void)
{
   CADirection   dir;
   CADirection   turned;
   CAAngle       angle;
   char          *name;
   
   dir    = CADirectionTop | CADirectionRight;
   name   = CADirectionUTF8String( dir);
   angle  = CADirectionToAngle( dir);
   turned = CADirectionTurn90ClockWise( dir);
   
   printf( "Direction: %s\n", name);
   printf( "Angle: %.1f degrees\n", CAAngleToDegrees( angle));
   printf( "After 90° CW: %s\n", CADirectionUTF8String( turned));
   
   if( CADirectionIsOpposingDirection( dir, CADirectionBottom | CADirectionLeft))
      printf( "Opposite to bottom-left\n");
   
   return( 0);
}
```

## 7. Dependencies

The **mulle-geometry** library has the following direct dependencies:

- **mulle-core** - Amalgamated library providing mulle-c and foundational C utilities
- **math.h** (system) - Standard C math library for trigonometric functions

The library uses the mulle-sde build system and follows the mulle-core project hierarchy. It is typically included as part of the broader mulle-gfx graphics framework.
