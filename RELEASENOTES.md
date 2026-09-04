### 0.1.0

- Add cubic bezier navigation, rotation sweep, and affine decomposition helpers
- New `CACubicBezierGetTangent`, `CACubicBezierSplit` (De Casteljau), and `CACubicBezierGetPose` APIs
- New `CARotationGetSweep` and `CAAngleShortestRotationToAngle` helpers
- Add `CGAffineTransformDecompose`, `CGVectorApplyAffineTransform`, `CGVectorNormalize`, `CGVectorPerpendicular`
- **BREAKING**: `CADirectionTurn180CounterClockWise` renamed to `CADirectionTurn180`
- Improve rotation/angle normalization and fix `-0.0` edge cases
- Introduce generated public header manifest and reorganize docs

### 0.0.1

Various small improvements
