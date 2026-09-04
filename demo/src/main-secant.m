#import "import.h"


static void  drawDotWithLabel( CGContext *context, CGPoint p, CGFloat radians, CGColor color)
{

   [context useDefaultFont];


   CGContextFontSize( context, 10.0);
   CGContextTextColor( context, CGColorMakeBlack());
   CGContextTextBackgroundColor( context, CGColorMakeWhite());
   CGContextFormatInRect( context, CGRectMake( p.x + 4.0, p.y, 100, 16), 0, 0, "%g", radians);

   CGContextBeginPath( context);
   CGContextCircle( context, CACircleMakeWithCenterAndRadius( p, 3.0));
   CGContextFillColor( context, color);
   CGContextFill( context);
}


static BOOL   drawStuff( CALayer *layer, CGContext *context)
{
   CGRect              canvas;
   CGRect              rect;
   CACircle            outerCircle;
   CACircle            innerCircle;
   CGFloat             hue;
   CGFloat             deg;
   CAPolarPoint        q0;
   CAPolarPoint        q1;
   CAPolarPoint        q2;
   CGPoint             p0;
   CGPoint             p1;
   CGPoint             p2;
   CGColor             color;
   NSUInteger          numberOfFrames;

   canvas = [layer canvas];
   rect   = CGRectInset( canvas, 50, 50);

   outerCircle = CGRectGetInnerCircle( rect);
   innerCircle = CACircleInset( outerCircle, outerCircle.radius / 2.0);

   CGContextStrokeCircleWithColor( context, innerCircle, 1.0, CGColorMakeGray());
   CGContextStrokeCircleWithColor( context, outerCircle, 1.0, CGColorMakeGray());

   numberOfFrames = [context numberOfFrames] % 480;

   for( deg = 0; deg < 360.0; deg += (1.0 + (numberOfFrames / 480.0) * 90.0))
   {
      hue   = CADegreesToRotation( deg);
      hue   = fmodf( hue, 1.0f);
      color = CGColorMakeWithHSL( hue, 1.0, 0.5);

      // pick a point on the inner circle
      q0 = CAPolarPointMakeWithAngle( innerCircle.radius, CADegreesToAngle( deg));
      p0 = CGPointMakeWithPolarPoint( q0);
      p0 = CGPointAdd( p0, innerCircle.center);

      // create perpendicular point, offset by 100.0
      q1 = CAPolarPointMakeWithAngle( 50 + 150.0 * (numberOfFrames / 480.0),  CADegreesToAngle( deg + 90.0));
      p1 = CGPointMakeWithPolarPoint( q1);
      p1 = CGPointAdd( p1, innerCircle.center);

      // calculate nearest intersection point to p1
      p2 = CACircleNearestIntersectionPoint( outerCircle, p0, p1, p1);

      CGContextBeginPath( context);
      CGContextMoveTo( context, p0);
      CGContextLineTo( context, p1);
      CGContextStrokeColor( context, CGColorMakeGray());
      CGContextStrokeWidth( context, 1.0);
      CGContextStroke( context);

      CGContextBeginPath( context);
      CGContextMoveTo( context, p0);
      CGContextLineTo( context, p2);
      CGContextStrokeColor( context, color);
      CGContextStrokeWidth( context, 3.0);
      CGContextStroke( context);

      drawDotWithLabel( context, p0, q0.radians, color);
      drawDotWithLabel( context, p1, q1.radians, color);

      q2 = CAPolarPointMakeWithPoint( CGPointSubtract( p2, innerCircle.center));
      drawDotWithLabel( context, p2, q2.radians, color);
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

