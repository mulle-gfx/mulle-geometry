#import "import.h"


static void   CGContextSectorPath( CGContext *context,
                                   CACircle circle,
                                   CAAngle startAngle,
                                   CAAngle endAngle,
                                   CGFloat innerRadius)
{
   CGPoint        p0;
   CACircle       innerCircle;
   CGPoint        center;

   center      = CACircleGetCenter( circle);
   innerCircle = CACircleMakeWithCenterAndRadius( center, innerRadius);

   p0 = CACirclePointAtAngle( innerCircle, startAngle);

   {
      CGContextBeginPath( context);
      CGContextMoveTo( context, p0);
      CGContextArcRadians( context, innerCircle, startAngle, endAngle, CARotationDirectionClockWise);
      // amusingly mvgArc already draws a line up to p2
      CGContextArcRadians( context, circle, endAngle, startAngle, CARotationDirectionCounterClockWise);
      CGContextClosePath( context);
   }
}



#define WEDGE_OFFSET_DEGREES  2
#define WEDGE_DEGREES         15


static void   drawDot( CGContext *context, CGPoint point, CGColor fillColor, CGColor strokeColor)
{
   CGContextFillCircleWithColor( context, CACircleMakeWithCenterAndRadius( point, 4.0), fillColor);
   CGContextStrokeCircleWithColor( context, CACircleMakeWithCenterAndRadius( point, 6.0), 4.0, strokeColor);
}


static void   drawDotInRect( CGContext *context, CAPolarPoint q, CGRect rect, CGColor fillColor, CGColor strokeColor)
{
   CGPoint   center;

   center = CGPointAdd( CGPointMakeWithPolarPoint( q), CGRectGetCenter( rect));
   drawDot( context, center, fillColor, strokeColor);
}


static void   drawLine( CGContext *context, CGPoint p1, CGPoint p2, CGColor strokeColor)
{
   CGContextStrokeLineWithColor( context, p1, p2, 1.0, strokeColor);
}


static BOOL   drawStuff( CALayer *layer, CGContext *context)
{
   CAAngle             endAngle;
   CAAngle             startAngle;
   CACircle            innerCircle;
   CACircle            outerCircle;
   CGColor             color;
   CGFloat             deg;
   CGFloat             rotDeg;
   CGFloat             factor;
   CGFloat             hue;
   CGRect              canvas;
   CGRect              rect;
   NSUInteger          numberOfFrames;
   struct NVGcontext   *vg;

   numberOfFrames = [context numberOfFrames] % 240;
   if( numberOfFrames >= 120)
      factor = 1.0 - (numberOfFrames - 120) / 119.0;
   else
      factor = numberOfFrames / 119.0;
   factor = _CAQuadraticGetValue( ((struct CAQuadratic) { 0.0f, 0.010f, 0.990f, 1.0f }), factor);


   vg     = CGContextGetNVGContext( context);
   canvas = [layer canvas];
   rect   = CGRectInset( canvas, 100, 100);

   outerCircle = CGRectGetInnerCircle( rect);
   innerCircle = CACircleInset( outerCircle, outerCircle.radius * (1.0 + 15.0 * factor) / 16.0);

   CGContextStrokeCircleWithColor( context, innerCircle, 1.0, CGColorMakeGray());
   CGContextStrokeCircleWithColor( context, outerCircle, 1.0, CGColorMakeGray());

   for( deg = 0; deg < 360.0; deg += (WEDGE_DEGREES + WEDGE_OFFSET_DEGREES * 2))
   {
      hue   = CADegreesToRotation( deg);
      hue   = fmodf( hue, 1.0f);
      color = CGColorMakeWithHSL( hue, 1.0, 0.5);

      rotDeg  = 360.0 * factor;

      startAngle = CADegreesToAngle( rotDeg + deg + WEDGE_OFFSET_DEGREES);
      endAngle   = CADegreesToAngle( rotDeg + deg + WEDGE_OFFSET_DEGREES + WEDGE_DEGREES);

      CGContextSectorPath( context, outerCircle, startAngle, endAngle, innerCircle.radius);

      CGContextFillColor( context, color);
      CGContextFill( context);
   }

   return( YES);
}


int   main( int argc, char *argv[])
{
   return( UIApplicationRunDrawCallbackHz( (CGGraphicsLayerDrawCallback_t *) drawStuff,
                                           CGSizeMake( 800, 800),
                                           NULL,
                                           60.0f));
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

