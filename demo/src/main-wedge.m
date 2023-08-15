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
      p0 = CACirclePointAtDistance( CACircleMake( CGPointZero, circle.radius), spaceStartMidAngle, distance);
      q0 = CAPolarPointMakeWithPoint( p0);

      startAngle = CAPolarPointGetAngle( q0);

      p1 = CACirclePointAtDistance( CACircleMake( CGPointZero, circle.radius), spaceEndMidAngle, -distance);
      q1 = CAPolarPointMakeWithPoint( p1);

      endAngle = CAPolarPointGetAngle( q1);
   }

   return( CAWedgeMake( circle, startAngle, endAngle, innerRadius, innerStartAngle, innerEndAngle));
}



static void   mvgWedgePath( struct NVGcontext *vg, CAWedge *wedge)
{
   CGPoint        p0;
   CACircle       innerCircle;
   CGPoint        center;

   center      = CACircleGetCenterPoint( wedge->circle);
   innerCircle = CACircleMake( center, wedge->innerRadius);

   p0 = CACirclePointAtAngle( innerCircle, wedge->innerStartAngle);

   {
      mvgMoveTo( vg, p0);
      mvgArc( vg, center, wedge->innerRadius, wedge->innerStartAngle, wedge->innerEndAngle, NVG_CW);
      mvgArc( vg, center, CACircleGetRadius( wedge->circle), wedge->endAngle, wedge->startAngle, NVG_CCW);
      mvgClosePath( vg);
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
   struct NVGcontext   *vg;
   struct NVGpaint     paint;
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
   factor = _CAQuadraticGetValue( ((struct CAQuadratic) { 0.0f, 0.010f, 0.990f, 1.0f }), factor);

   vg     = CGContextGetNVGContext( context);
   canvas = [layer canvas];
   rect   = CGRectInset( canvas, 100, 100);

   circle      = CGRectGetInnerCircle( rect);

   outerCircle = CACircleInset( circle, 20);
   innerCircle = CACircleInset( outerCircle, 20 + (outerCircle.radius - 30.0) * factor);

//   mvgFillCircleWithColor( vg, innerCircle.center, innerCircle.radius, 1.0, CGColorMakeGray());
//   mvgStrokeCircleWithColor( vg, outerCircle.center, outerCircle.radius, 1.0, CGColorMakeGray());

   [context useDefaultFont];
   mvgTextColor( vg, CGColorMakeBlack(), CGColorMakeWhite());
   mvgTextInRect( vg, "VfL Bochum 1848",
                       NULL,
                       CACircleGetOuterRect( outerCircle),
                       CGContentModeScaleAspectFit,
                       MVGTextAlignVisually);

   gradient = [CGGradient gradientWithStartColor:CGColorMultiplyAlpha( CGColorMakeWhite(), 0.5f)
                                        endColor:CGColorMultiplyAlpha( CGColorMakePersianBlue(), 0.5f)
                                        rotation:0.125];

   paint    = [gradient linearGradientNVGpaintWithBounds:CACircleGetOuterRect( outerCircle)
                                              smoothness:1.0
                                              NVGcontext:vg];
   mvgFillCircleWithNVGpaint( vg, circle, paint);

   gradient = [CGGradient gradientWithStartColor:CGColorMultiplyAlpha( CGColorMakeWhite(), 0.5f)
                                        endColor:CGColorMultiplyAlpha( CGColorMakePersianBlue(), 0.5f)
                                        rotation:0.75];
   paint    = [gradient linearGradientNVGpaintWithBounds:CACircleGetOuterRect( outerCircle)
                                              smoothness:1.0
                                              NVGcontext:vg];

//   paint    = [gradient radialGradientNVGpaintWithCircle:outerCircle
//                                             innerRadius:innerCircle.radius
//                                              NVGcontext:vg];
   mvgFillCircleWithNVGpaint( vg, circle, paint);

   mvgBeginPath( vg);
   mvgCircle( vg, circle);

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
                                              CADegreesToRadians( WEDGE_OFFSET_DEGREES / 2.0f));
#if 0
      spaceStartMidAngle = CADegreesToAngle( rotDeg + WEDGE_OFFSET_DEGREES / 2.0);
      innerStartAngle    = CADegreesToAngle( rotDeg + WEDGE_OFFSET_DEGREES);
      innerEndAngle      = CADegreesToAngle( rotDeg + WEDGE_OFFSET_DEGREES + WEDGE_DEGREES);
      spaceEndMidAngle   = CADegreesToAngle( rotDeg + WEDGE_OFFSET_DEGREES + WEDGE_DEGREES + WEDGE_OFFSET_DEGREES / 2.0);

      // we want to keep this distance on outer, so need to adjust angles.
      // we use equidistancy here, though we should use paralell really

      // the offset is like a small rectangle, separating wedges

      // find point on outercircle

      outerStartAngle = innerStartAngle;
      outerEndAngle   = innerEndAngle;

      distance = CACircleDistanceBetweenAngles( innerCircle, spaceStartMidAngle, innerStartAngle);
      if( distance >= 0.5)
      {
         p0 = CACirclePointAtDistance( CACircleMake( CGPointZero, outerCircle.radius), spaceStartMidAngle, distance);
         q0 = CAPolarPointMakeWithPoint( p0);

         outerStartAngle  = CAPolarPointGetAngle( q0);

//      spaceMidAngle   = CADegreesToAngle( rotDeg + deg + WEDGE_OFFSET_DEGREES + WEDGE_DEGREES + WEDGE_OFFSET_DEGREES);

         p1 = CACirclePointAtDistance( CACircleMake( CGPointZero, outerCircle.radius), spaceEndMidAngle, -distance);
         q1 = CAPolarPointMakeWithPoint( p1);

         outerEndAngle  = CAPolarPointGetAngle( q1);
      }

      wedge = CAWedgeMake( outerCircle, outerStartAngle, outerEndAngle, innerCircle.radius, innerStartAngle, innerEndAngle);
#endif

      mvgWedgePath( vg, &wedge);
      mvgPathWinding( vg, NVG_HOLE);

#if 0
      drawDot( vg, CACirclePointAtAngle( innerCircle, innerStartAngle), CGColorMakeGray(), CGColorMakeRed());
      drawDot( vg, CACirclePointAtAngle( innerCircle, innerEndAngle),   CGColorMakeGray(), CGColorMakeGreen());

//      if( distance >= 0.5)
//      {
//         drawDot( vg, CACirclePointAtAngle( innerCircle, spaceStartMidAngle),   CGColorMakeGreen(), CGColorMakeGray());
//         drawDot( vg, CACirclePointAtAngle( innerCircle, spaceEndMidAngle),   CGColorMakeGreen(), CGColorMakeGray());
//
//         drawDot( vg, CACirclePointAtAngle( outerCircle, spaceStartMidAngle),   CGColorMakeGreen(), CGColorMakeGray());
//         drawDot( vg, CACirclePointAtAngle( outerCircle, spaceEndMidAngle),   CGColorMakeGreen(), CGColorMakeGray());
//      }

      drawDot( vg, CACirclePointAtAngle( outerCircle, outerEndAngle),   CGColorMakeWhite(), CGColorMakeGreen());
      drawDot( vg, CACirclePointAtAngle( outerCircle, outerStartAngle), CGColorMakeWhite(), CGColorMakeRed());
#endif

//      drawLabel( context, CACirclePointAtAngle( outerCircle,
//                                       CADegreesToAngle( rotDeg + WEDGE_OFFSET_DEGREES + WEDGE_DEGREES / 2.0)),
//                                       distance);
   }

   gradient = [CGGradient gradientWithStartColor:CGColorMakeWhite()
                                        endColor:CGColorMakePersianBlue()
                                        rotation:0.0];

   paint    = [gradient radialGradientNVGpaintWithCircle:circle
                                             innerRadius:0.0
                                              NVGcontext:vg];

   mvgFillPaint( vg, paint);
   mvgFill( vg);

   return( YES);
}


int   main( int argc, char *argv[])
{
   return( UIApplicationRunDrawCallbackHz( (CALayerDrawCallback_t *) drawStuff,
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

