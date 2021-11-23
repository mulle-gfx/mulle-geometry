# 🔭 mulle-geometry

#### 🔭 Basic geometric and time datatypes 


This library contains definitions and functions for familiar datatypes like **CGRect**. As most of these datatypes have legacy names, the prefixes are all
over the place.

Datatype            | Description
--------------------|---------------------------
CGFloat             | legacy name of `float` in CoreGraphics
CGPoint             | cartesian point
CGSize              | size
CGVector            | cartesian vector (rarely used)
CGRect              | rectangle
CAAngleDegrees      | the angle in 0-360 degrees (0 is leftwards)
CAAngleDirectional  | angle from -0.999 to +0.999 (0 is upwards)
CAAngleRadians      | the usual angle in radians (0 is leftwards)
CAAngleHour         | maps angle to analog hours (0 is upwards)
CAPolarPoint        | point as an angle and distance from 0,0			
CAQuadratic 		| interpolation value
CAQuadraticBezier   | bezier curve
CARelativeTime      | a quantity of time like 10s
CAAbsoluteTime      | a timestamp like today 
CARelativeTimeRange | a time range of relative times, like delay, duration
CAAbsoluteTimeRange | an absolute time range, like today until tomorrow



## Attribution

The library may still contain some code from :

```
Copyright (c) 2006-2007 Christopher J. W. Lloyd
Copyright (c) Microsoft. All rights reserved.
```