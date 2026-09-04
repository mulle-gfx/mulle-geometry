#import "import.h"


#define DEGREES   90


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
// The three lines drawn are not parallel, but the points on the circles have the
// same distance relative to each other.
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
   CAPolarPoint        q4;
   CAPolarPoint        q5;
   CGColor             color;
   CGFloat             deg;
   CGFloat             factor;
   CGFloat             hue;
   CGFloat             midDegrees;
   CGPoint             center;
   CGPoint             p0;
   CGPoint             p1;
   CGPoint             p2;
   CGPoint             p3;
   CGPoint             p4;
   CGPoint             p5;
   CGRect              canvas;
   CGRect              rect;
   NSUInteger          numberOfFrames;
   CGFloat             d;
   CGFloat             a;

   numberOfFrames = [context numberOfFrames] % 240;
   if( numberOfFrames >= 120)
      factor = 1.0 - (numberOfFrames - 120) / 119.0;
   else
      factor = numberOfFrames / 119.0;
   factor = _CACubicGetValue( ((struct CACubic) { 0.0f, 0.010f, 0.990f, 1.0f }), factor);


   canvas = [layer canvas];
   rect   = CGRectInset( canvas, 100, 100);

   center      = CGRectGetCenterPoint( rect);
   outerCircle = CGRectGetInnerCircle( rect);
   innerCircle = CACircleInset( outerCircle, outerCircle.radius * (1.0 + 6.0 * factor) / 8.0);

   CGContextStrokeCircleWithColor( context, innerCircle, 1.0, CGColorMakeGray());
   CGContextStrokeCircleWithColor( context, outerCircle, 1.0, CGColorMakeGray());

   for( deg = 0; deg < 360.0; deg += 10 + (80.0 * factor))
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

      q4 = CAPolarPointMakeWithAngle( outerCircle.radius, midAngle);

      d = CADegreesToRadiansAsRotation( DEGREES / 2.0) * innerCircle.radius;
      a = d / outerCircle.radius;

      q3 = CAPolarPointSubtractRadians( q4, a);
      q5 = CAPolarPointAddRadians( q4, a);

      p0 = CGPointAdd( CGPointMakeWithPolarPoint( q0), center);
      p1 = CGPointAdd( CGPointMakeWithPolarPoint( q1), center);
      p2 = CGPointAdd( CGPointMakeWithPolarPoint( q2), center);
      p3 = CGPointAdd( CGPointMakeWithPolarPoint( q3), center);
      p4 = CGPointAdd( CGPointMakeWithPolarPoint( q4), center);
      p5 = CGPointAdd( CGPointMakeWithPolarPoint( q5), center);

      drawLine( context, p0, p3, CGColorMakeMagenta());
      drawLine( context, p1, p4, CGColorMakeIndigo());
      drawLine( context, p2, p5, CGColorMakeHotPink());

      drawDot( context, p0, color, CGColorMakeRed());
      drawDot( context, p1, color, CGColorMakeGreen());
      drawDot( context, p2, color, CGColorMakeBlue());

      drawDot( context, p3, color, CGColorMakeDarkGray());
      drawDot( context, p4, color, CGColorMakeIndigo());
      drawDot( context, p5, color, CGColorMakeBrown());
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

