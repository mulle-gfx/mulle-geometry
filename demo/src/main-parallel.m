#import "import.h"


#define DEGREES   45


static void   drawDot( CGContext *context, CGPoint point, CGColor fillColor, CGColor strokeColor)
{
   CGContextFillCircleWithColor( context, CACircleMakeWithCenterAndRadius( point, 4.0), fillColor);
   CGContextStrokeCircleWithColor( context, CACircleMakeWithCenterAndRadius( point, 6.0), 4.0, strokeColor);
}


static void   drawLine( CGContext *context, CGPoint p1, CGPoint p2, CGColor strokeColor)
{
   CGContextStrokeLineWithColor( context, p1, p2, 1.0, strokeColor);
}


//
// Writing this demo, was actually quite painful :)
// The lines drawn are parallel to each other. But the three points on each
// circle do not have the same distance (equidistant).
//
static BOOL   drawStuff( CALayer *layer, CGContext *context)
{
   CAAngle             endAngle;
   CAAngle             midAngle;
   CAAngle             startAngle;
   CACircle            innerCircle;
   CACircle            outerCircle;
   CAPolarPoint        q0;
   CAPolarPoint        q1;
   CAPolarPoint        q2;
   CAPolarPoint        q3;
   CGColor             color;
   CGFloat             deg;
   CGFloat             distance;
   CGFloat             factor;
   CGFloat             hue;
   CGFloat             midDegrees;
   CGFloat             slope;
   CGPoint             center;
   CGPoint             p0;
   CGPoint             p1;
   CGPoint             p2;
   CGPoint             p3;
   CGPoint             p4;
   CGPoint             p5;
   CGPoint             p6;
   CGPoint             p7;
   CGPoint             p8;
   CGPoint             p9;
   CGRect              canvas;
   CGRect              rect;
   NSUInteger          numberOfFrames;
   struct NVGcontext   *vg;

   numberOfFrames = [context numberOfFrames] % 240;
   if( numberOfFrames >= 120)
      factor = 1.0 - (numberOfFrames - 120) / 119.0;
   else
      factor = numberOfFrames / 119.0;
   factor = _CACubicGetValue( ((struct CACubic) { 0.0f, 0.010f, 0.990f, 1.0f }), factor);


   vg     = CGContextGetNVGContext( context);
   canvas = [layer canvas];
   rect   = CGRectInset( canvas, 100, 100);

   center      = CGRectGetCenter( rect);
   outerCircle = CGRectGetInnerCircle( rect);
   innerCircle = CACircleInset( outerCircle, outerCircle.radius * (1.0 + 6.0 * factor) / 8.0);

   CGContextStrokeCircleWithColor( context, innerCircle, 1.0, CGColorMakeGray());
   CGContextStrokeCircleWithColor( context, outerCircle, 1.0, CGColorMakeGray());

   for( deg = 0; deg < 360.0; deg += 5 + (90.0 * factor))
   {
      hue   = CADegreesToRotation( deg);
      hue   = fmodf( hue, 1.0f);
      color = CGColorMakeWithHSL( hue, 1.0, 0.5);

      startAngle = CADegreesToAngle( deg);
      midDegrees = deg + DEGREES / 2.0;
      midAngle   = CADegreesToAngle( midDegrees);
      endAngle   = CADegreesToAngle( deg + DEGREES);

      q0 = CAPolarPointMakeWithAngle( innerCircle.radius, startAngle);
      q1 = CAPolarPointMakeWithAngle( innerCircle.radius, midAngle);
      q2 = CAPolarPointMakeWithAngle( innerCircle.radius, endAngle);
      q3 = CAPolarPointMakeWithAngle( outerCircle.radius, midAngle);

      slope    = CAAngleToSlope( midAngle);
      distance = (outerCircle.radius - innerCircle.radius) / 2.0f;

      p9 = CGPointAdd( CGPointMakeWithPolarPoint( q3), center);

      p0 = CGPointAdd( CGPointMakeWithPolarPoint( q0), center);
      p3 = CGPointTranslate( p0, slope, distance);
      p4 = CACircleNearestIntersectionPoint( outerCircle, p0, p3, p9);

      p1 = CGPointAdd( CGPointMakeWithPolarPoint( q1), center);
      p5 = CGPointTranslate( p1, slope, distance);
      p6 = CACircleNearestIntersectionPoint( outerCircle, p1, p5, p9);

      p2 = CGPointAdd( CGPointMakeWithPolarPoint( q2), center);
      p7 = CGPointTranslate( p2, slope, distance);
      p8 = CACircleNearestIntersectionPoint( outerCircle, p2, p7, p9);

      drawLine( context, p0, p4, CGColorMakeMagenta());
      drawLine( context, p1, p6, CGColorMakeIndigo());
      drawLine( context, p2, p8, CGColorMakeHotPink());

      drawDot( context, p0, color, CGColorMakeRed());
      drawDot( context, p1, color, CGColorMakeGreen());
      drawDot( context, p2, color, CGColorMakeBlue());

      drawDot( context, p4, color, CGColorMakeDarkGray());
      drawDot( context, p6, color, CGColorMakeIndigo());
      drawDot( context, p8, color, CGColorMakeBrown());

      [context useDefaultFont];

      CGContextFontSize( context, 11.0);
      CGContextTextColor( context, CGColorMakeBlack());
      CGContextTextBackgroundColor( context, CGColorMakeWhite());
      CGContextFormatInRect( context, CGRectMake( p0.x + 4.0, p0.y, 200, 16), 0, 0, "d: %g (%g)",
                           CGPointDistanceToPoint( p0, p4),
                           (q1.radians - q0.radians) * innerCircle.radius);
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

