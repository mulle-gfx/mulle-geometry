### 0.1.1

Various small improvements

## 0.1.0



feature: add cubic bezier navigation, rotation sweep, and affine decomposition helpers

* new CACubicBezierGetTangent, CACubicBezierSplit (De Casteljau), and CACubicBezierGetPose APIs for evaluating bezier direction and splitting curves
* new CARotationGetSweep and CAAngleShortestRotationToAngle helpers for arc tracing and animation
* CGAffineTransformDecompose splits a transform into scale/rotation/skew/translation; add CGVectorApplyAffineTransform and CGVectorNormalize/Perpendicular helpers
* **BREAKING**: CADirectionTurn180CounterClockWise renamed to CADirectionTurn180
* rotation/angle normalization reworked to drop -0.0 edge cases; fixes for CASlopeToAngle and CAHourToRotation asserts
* cross-platform test stdout baselines (darwin/windows/linux) and new angle roundtrip, float, and pose tests
