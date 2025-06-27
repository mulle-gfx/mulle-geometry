#import "import.h"

#define ANIM_SPEED  480

#define ANIMATION
//#define STATIC_START  100


typedef struct
{
   CACircle   circle;
   CAAngle    startAngle;
   CAAngle    endAngle;
   CGFloat    innerRadius;
   CAAngle    innerStartAngle;
   CAAngle    innerEndAngle;
} CAWedge;


static inline CAWedge   CAWedgeMake( CACircle   circle,
                                     CAAngle    startAngle,
                                     CAAngle    endAngle,
                                     CGFloat    innerRadius,
                                     CAAngle    innerStartAngle,
                                     CAAngle    innerEndAngle)
{
   return( (CAWedge) {
                       .circle          = circle,
                       .startAngle      = startAngle,
                       .endAngle        = endAngle,
                       .innerRadius     = innerRadius,
                       .innerStartAngle = innerStartAngle,
                       .innerEndAngle   = innerEndAngle
                     });
}


static inline CAWedge   CAWedgeMakeWithDistanceRadians( CACircle   circle,
                                                        CAAngle    startAngle,
                                                        CAAngle    endAngle,
                                                        CGFloat    innerRadius,
                                                        CGFloat    radians)
{
   CGFloat        spaceStartMidAngle;
   CGFloat        spaceEndMidAngle;
   CGFloat        innerStartAngle;
   CGFloat        innerEndAngle;
   CGFloat        distance;
   CGPoint        p0;
   CGPoint        p1;
   CAPolarPoint   q0;
   CAPolarPoint   q1;

   spaceStartMidAngle = CAAngleSubtractRadians( startAngle, radians);
   spaceEndMidAngle   = CAAngleAddRadians( endAngle, radians);

      // we want to keep this distance on outer, so need to adjust angles.
      // we use equidistancy here, though we should use paralell really

      // the offset is like a small rectangle, separating wedges

      // find point on outercircle

   innerStartAngle = startAngle;
   innerEndAngle   = endAngle;

   distance = CARadiusDistanceBetweenAngles( innerRadius, spaceStartMidAngle, innerStartAngle);
   if( distance >= 0.5)
   {
      p0 = CACirclePointAtDistance( CACircleMakeWithCenterAndRadius( CGPointZero, circle.radius), spaceStartMidAngle, distance);
      q0 = CAPolarPointMakeWithPoint( p0);

      startAngle = CAPolarPointGetAngle( q0);

      p1 = CACirclePointAtDistance( CACircleMakeWithCenterAndRadius( CGPointZero, circle.radius), spaceEndMidAngle, -distance);
      q1 = CAPolarPointMakeWithPoint( p1);

      endAngle = CAPolarPointGetAngle( q1);
   }

   return( CAWedgeMake( circle, startAngle, endAngle, innerRadius, innerStartAngle, innerEndAngle));
}



static void   CGContextWedgePath( CGContext *context, CAWedge *wedge)
{
   CGPoint        p0;
   CACircle       innerCircle;
   CGPoint        center;

   center      = CACircleGetCenter( wedge->circle);
   innerCircle = CACircleMakeWithCenterAndRadius( center, wedge->innerRadius);

   p0 = CACirclePointAtAngle( innerCircle, wedge->innerStartAngle);

   {
      CGContextMoveTo( context, p0);
      CGContextArcRadians( context, innerCircle, wedge->innerStartAngle, wedge->innerEndAngle, CARotationDirectionClockWise);
      CGContextArcRadians( context, wedge->circle, wedge->endAngle, wedge->startAngle, CARotationDirectionCounterClockWise);
      CGContextClosePath( context);
   }
}



#define WEDGE_OFFSET_DEGREES  10
#define WEDGE_DEGREES         30


static BOOL   drawStuff( CALayer *layer, CGContext *context)
{
   CACircle            innerCircle;
   CACircle            outerCircle;
   CACircle            circle;
   CAWedge             wedge;
   CGColor             color;
   CGFloat             deg;
   CGFloat             factor;
   CGFloat             hue;
   CGFloat             rotDeg;
   CGRect              canvas;
   CGRect              rect;
   NSUInteger          numberOfFrames;
   CGGradient          *gradient;

#ifdef ANIMATION
   numberOfFrames = [context numberOfFrames] % ANIM_SPEED;
#else
   numberOfFrames = STATIC_START;
#endif

   if( numberOfFrames >= (ANIM_SPEED / 2))
      factor = 1.0 - (numberOfFrames - (ANIM_SPEED / 2)) / ((float) (ANIM_SPEED / 2 )- 1);
   else
      factor = numberOfFrames / ((float) (ANIM_SPEED / 2) - 1);
   factor = _CACubicGetValue( ((struct CACubic) { 0.0f, 0.010f, 0.990f, 1.0f }), factor);

   canvas = [layer canvas];
   rect   = CGRectInset( canvas, 100, 100);

   circle      = CGRectGetInnerCircle( rect);

   outerCircle = CACircleInset( circle, 20);
   innerCircle = CACircleInset( outerCircle, 20 + (outerCircle.radius - 30.0) * factor);

//   mvgFillCircleWithColor( vg, innerCircle.center, innerCircle.radius, 1.0, CGColorMakeGray());
//   mvgStrokeCircleWithColor( vg, outerCircle.center, outerCircle.radius, 1.0, CGColorMakeGray());

   [context useDefaultFont];
   CGContextTextColor( context, CGColorMakeBlack());
   CGContextTextBackgroundColor( context, CGColorMakeWhite());
   CGContextTextInRect( context, "VfL Bochum 1848",
                                 (size_t) -1,
                                 CACircleGetOuterRect( outerCircle),
                                 CGContentModeScaleAspectFit,
                                 CGTextOptionAlignVisually);

   gradient = [CGGradient gradientWithStartColor:CGColorMultiplyAlpha( CGColorMakeWhite(), 0.5f)
                                        endColor:CGColorMultiplyAlpha( CGColorMakePersianBlue(), 0.5f)
                                        rotation:0.125];
   CGContextFillCircleWithPaintable( context, circle, gradient);

   gradient = [CGGradient gradientWithStartColor:CGColorMultiplyAlpha( CGColorMakeWhite(), 0.5f)
                                        endColor:CGColorMultiplyAlpha( CGColorMakePersianBlue(), 0.5f)
                                        rotation:0.75];
   CGContextFillCircleWithPaintable( context, circle, gradient);

   CGContextBeginPath( context);
   CGContextCircle( context, circle);

   for( deg = 0; deg < 360.0 - WEDGE_DEGREES; deg += WEDGE_DEGREES + WEDGE_OFFSET_DEGREES)
   {
      hue   = CADegreesToRotation( deg);
      hue   = fmodf( hue, 1.0f);
      color = CGColorMakeWithHSL( hue, 1.0, 0.5);

      rotDeg = 360.0 * factor + deg;

      wedge = CAWedgeMakeWithDistanceRadians( outerCircle,
                                              CADegreesToAngle( rotDeg + WEDGE_OFFSET_DEGREES),
                                              CADegreesToAngle( rotDeg + WEDGE_OFFSET_DEGREES + WEDGE_DEGREES),
                                              innerCircle.radius,
                                              CADegreesToRadiansAsRotation( WEDGE_OFFSET_DEGREES / 2.0f));

      CGContextWedgePath( context, &wedge);
      CGContextPathWinding( context, NVG_HOLE);

//      drawLabel( context, CACirclePointAtAngle( outerCircle,
//                                       CADegreesToAngle( rotDeg + WEDGE_OFFSET_DEGREES + WEDGE_DEGREES / 2.0)),
//                                       distance);
   }

   gradient = [CGGradient radialGradientWithCircle:circle
                                             color:CGColorMakeWhite()
                                       innerRadius:0.0
                                        innerColor:CGColorMakePersianBlue()];
   CGContextFillPaintable( context, gradient);
   CGContextFill( context);

   return( YES);
}


int   main( int argc, char *argv[])
{
   return( UIApplicationRunDrawCallbackHz( (CGGraphicsLayerDrawCallback_t *) drawStuff,
                                           CGSizeMake( 800, 800),
                                           NULL,
#ifdef ANIMATION
                                           60.0f
#else
                                           0.0f
#endif
                                           ));
}




//
//                           ,╓≤æKª╨╙╙▓█D"╙╙╙*╨¥W▄╓,
//                      ╓▄ª╙└╧▓        █           ⁿ▓▓ª╦w
//                  ╓#╜^*█⌐   `╕       █           ╓▌    `╙%w
//               ╓ΦΓ      █    ╙⌐     ,█,,,Φ      ╓▌         ╙¥▄
//            ,▄▀ █▄       █µ,╓╝,╓≤╗≡≡ª╨╨╨¥≡∞╗w╓,ª█,  ,       ]▄▀V,
//          ,Φ└   ║"▀▄     ,▄▀╨"                 "╙¥▓▓       ▄▀  º▄¥,
//        ,#▌Φ▓▄╓  Φ `█Q▄M╙                           ╙¥▄ ,▄▀ ╙▄⌐`  `Φ
//       ▄▀   `V,"▀▀¼╓╩└                                 ╙¥▓╗ ,       ╙▄
//      Φ       `¥═▄▀                            ╒          ▀▄          ▓
//    .▓         ╓▓]V▄H       ▓                 ,Å            ▀,         ▀
//    █      ,-ª▓╛  `██▀½      "▄  ,╓╓╓╓;╓▄▄▄▄╗▄█,             ╙▄         ▀
//   █  `██▄   ▓. - Φ█═.~     ,≤█▄▀-      `   █└"▀▀▀▀▄╓╓╓╓       ▌    ██▄, █
//  ║⌐  " ▀   ▓╨⌐   }▀`      ,█  `           █      .█▀▄▄███▄     ▌  ` ▀    ▌
//  ▌        ▐M A   '┴      ▌                ▀        ╙█▌████▌    ╙▄        ║⌐
// ║⌐        █⌐'║▄ Γr2«    ║▀                 ▄▄▄      ╙▌▄███      █         ▌
// █]     ,▄▐H  ██████═ª  ╓` ▄█▀▀▀▀█▄        ████     ╓███` `╙╙║█  ▐H        █
// █ "% ,▄▀ ║░╠K█═╣███^   ╛╓█▀      ▐█         ,╓╓═ª╙└   █      ╙█▄▐▌▓######▌╫
// █║▄▄▄█▄▄▄║H:  "╙▀███▄╓▄,█▌╓▄,,▄∩  ██p$ΦΦ▀▀▀▀▀ΦΦΦR≡≥▄╓ ║▄       ╙j▌` ▄▀╙╗ `╫
// █"      ╙▐H-═M ▄███████¥█H "^╙    '█, ,«^``     `     ║▌        ▐M▄▀    "Φ█
// ║        !█▄▄████≤═^`╓█ █▌╓▀╦▄▀█   ╙█⌐               ╓█▌        ▓ `       █
//  ▌      ,▄██╫██ ▄█═╜" █▌▐█╣Hj⌐ █    █▌╙¥w,           ██▄       ╓▌        ▐M
//  ╢"▄▄#▀▀╙╟█Ü███▀`▀█  ║██J█▄▌jH █    ██    ╙▀W▄;    ,██  `╙w,   █▄     ,╓ █
//   █╙     ▐███▌▀█w ▀█ ╠██████j▌║█▌   ▀██▄       `╙▓██▀w,     ╙╗█'`"▀█▀╙"`║`
//   ╙▌     Φ████████▄██ ███████████    ═██▀W▄,   ╓█▀└         ╓▀ "    ╙▄ ╓▀
//    ╙▄     ╙▀███╙█▄▐██▄,╙▀█████▄▀▄▀▄   ,█M╗ ▀▄██▀╙""*ⁿ═─≈-,,▄▌,        ▄▀
//     `▌╙µ,▄Φ▀x███▄██████████▀V███▀╙└ ,▄#▌████▀`"*«,      .█▀▄▀▄,      ▄╛
//       ▀Ö█  ;▓████████▄▄█▀▄╙W "V▄██▀▀│▄███▀└ ▀╦     "*═╥▄▀ ║,  ║▀▄  ,▓
//        ╙W▀ '█████╨▀▀█████████   "▀▀████└      ╙▄   ,▄▀'   `▀▀╣H ▐▀▄▀
//          ╙▄  ▀▀Γ╓▌  ▄▀ⁿ`╙▀███      ██▌         ╓█R▀▄A█        ▀▀▄▀
//            ╙W,Φ╙  ║█        ▄╙╙ª%≡╦██≤≤╗╗≡≡╜▀▀   ╣▌   █╕     ,▄▀
//              `▀▄M█┘        ║▀▌   ╫"    *Φ▄  █       Φ` ▀▌, ▄M╙
//                 "▀╦,       M ╙▌  ▀       `█W▐▌       wæQ▄▀╙
//                     ╙▀W╥,╓╣   ╙█║        ╫M  █    ,╓▄╝╙`
//                          ╙╙▀%▄▄▄▌,≤▄_▄≡,▐▓▓▄▄#ª▀"`

