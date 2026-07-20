### 0.0.1


feature: add arc-distance helpers and normalize circle radius

* new CARadiusAngleByAddingDistance and CACircleAngleByAddingDistance to compute the angle reached after traveling a distance along a circle — simplifies arc/animation math
* CACircle constructors now normalize radius to non-negative values (fabsf), preventing negative-radius bugs and inconsistent circle behavior
* add comprehensive library documentation (asset/dox/TOC.md) providing API reference and examples


* change CAQuadratic to CACubic coz cody tells me its not a quadratic
* added CAQuadratic back for orneriness :P

* pretty much a complete overhaul of CARotation
* new type CAPose added for the sake of CALayer path animation
* new cutter demo for rectangle intersection (could be used to fix nvgIntersectScissor)
* if its not clear, if radians is an angle or radians, the term AsAngle or AsRotation is bolted onto the function name now



* added lots of Data macros for static initialization
* new functions `CGPointProjectToFrame`, `CGPointProjectToBounds`, `CGPointTriangleGetCentroid`, `CGPointTriangleGetCenterOfMass`, `CGRectMakeWithCenterPointAndSize`, `CGRectMakeWithCenterPointAndInnerRadius`



* added CACircle, with that came lot of changes and simplifications

* completely reworked CAAngle, introduced CARotation





* add distance and project functions
* rename functions for more consistency use of Make Get As

* add CGPointOffset function











* have union altenative to base type for easier axis handling



* moved quadratic to MulleCA
* moved affine to MulleCG
* death of the Mulle prefix for enums, structs and functions

* split MulleLayout off from MulleUI


* nach dem Fundamentalumbau mit CALayer im windowspace und der



* new demo render-png to render out a PNG file








* UITextField added

* fix UIView conversion toView
* much improved MullePopUpButton
* layer drawing methods now return a YES if transform or scissor have been affected

* MulleJS is now its own project in MulleWeb
* MulleMenu and MullePopUpButton added (rename to MulleUIMenu ...) ?


* color creation with strings
* javascript interface for nanovg
* new method setSubviews: on UIView
* new UIStackView layout MulleStackViewDistributionFillRowColumn









* CALayer now uses introspection to generate animations for observable properties

































* experimental yoga layouting

* add TODO list to README.md

* improved SVG rendering

* redid CALayer background drawing and border



* UIImage added. UIButton can now change its background image

* UIControl as protocolclass added

* modernized mulle-sde
* UIEvents are distributed to subviews properly
