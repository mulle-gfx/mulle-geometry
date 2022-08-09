# 🔭 mulle-geometry

#### 🔭 Basic geometric types

This library contains definitions and functions for (macOS/iOS) familiar
datatypes like **CGRect**. CG prefixed datatypes are legacy names. The
advantage of reusing the names is, that they are familiar already and that
documentation already exists outside of the project.

For a nice overview read [NSHipster:Core​Graphics Geometry Primitives](https://nshipster.com/cggeometry/).

CA prefixed datetypes are custom and not documented elsewhere.


Datatype            | Description
--------------------|---------------------------
CGFloat             | legacy name of `float` in CoreGraphics
CGPoint             | cartesian point
CGRect              | rectangle
CGSize              | size
CGVector            | cartesian vector (rarely used)


Datatype            | Description
--------------------|---------------------------
CAAngleDegrees      | the angle in 0-360 degrees (0 is leftwards)
CAAngleDirectional  | angle from -0.999 to +0.999 (0 is upwards)
CAAngleHour         | maps angle to analog hours (0 is upwards)
CAAngleRadians      | the usual angle in radians (0 is leftwards)
CAPolarPoint        | point as an angle and distance from 0,0
CAQuadratic         | interpolation value
CAQuadraticBezier   | bezier curve

### You are here

![Overview](overview.dot.svg)



## mulle-sde

This is a [mulle-sde](//github.com/mulle-sde) project. mulle-sde combines
recursive package management with cross-platform builds via **cmake**:

Action  | Command                               | Description
--------|---------------------------------------|---------------
Build   | `mulle-sde craft [--release|--debug]` | Builds into local `kitchen` folder
Add     | `mulle-sde dependency add --c --github MulleUI mulle-geometry` | Add mulle-geometry to another mulle-sde project as a dependency
Install | `mulle-sde install --prefix /usr/local https://github.com/MulleUI/mulle-geometry.git` | Like `make install`


### Manual Installation


Install the requirements:

Requirements                                      | Description
--------------------------------------------------|-----------------------
[some-requirement](//github.com/some/requirement) | Some requirement

Install into `/usr/local`:

``` sh
cmake -B build \
      -DCMAKE_INSTALL_PREFIX=/usr/local \
      -DCMAKE_PREFIX_PATH=/usr/local \
      -DCMAKE_BUILD_TYPE=Release &&
cmake --build build --config Release &&
cmake --install build --config Release
```


## Attribution

The library may still contain some code from :

```
Copyright (c) 2006-2007 Christopher J. W. Lloyd
Copyright (c) Microsoft. All rights reserved.
```