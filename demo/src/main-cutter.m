#import "import.h"


//#define FIXED_ROTATION  0.0

static void   drawRectBehindText( CGContext *context,
                                  CGRect canvas,
                                  char *label,
                                  CGContentMode contentMode)
{
   struct CGTextBoundsInfo   info;

   CGContextFontFace( context, "sans");
   CGContextFontSize( context, 8.0f);

   info = CGContextTextBoundsOptions( context, label, (size_t) -1,
                                               CGRectInset( canvas, 4, 4),
                                               contentMode,
                                               CGTextOptionAlignVisually);
   CGContextFillRoundedRectWithColor( context,
                                      CGRectExtrude( info.bounds, 4, 4),
                                      4.0,
                                      CGColorBySettingAlpha( CGColorMakeBlack(), 0.5));

   CGContextTextColor( context, CGColorMakeWhite());
   CGContextTextBackgroundColor( context, CGColorMakeGray());

   CGContextCombinedTextAlignment( context, CGTextAlignLeft | CGTextAlignTop);
   CGContextText( context, info.bounds.origin, label, (size_t) -1);
}


//
// just a visual test for CGRectGetTopLeft and friends
//
static BOOL   drawStuff( CGGraphicsLayer *layer, CGContext *context)
{
   CGRect              canvas;
   CGRect              rect;
   CGPoint             transformed;
   CGPoint             clipRect[4];
   CGPoint             result[8];
   CGPoint             subject[4];
   float               x;
   float               y;
   int                 i;
   unsigned int        count;
   float               factor;
   CGAffineTransform   rotationTransform;
   char                *label;

   canvas = [layer canvas];

   // Clear background
   CGContextBeginPath( context);
   CGContextRect( context, canvas);
   CGContextFillColor( context, CGColorMakeLightGray());
   CGContextFill( context);

   // Calculate animation factor and setup
   rect = CGSizePlaceInRect( CGSizeMake( 1, 1), canvas, CGContentModeScaleAspectFit|CGContentModeCenterPoint);
   rect = CGRectInset( rect, 75, 75);

   // Draw original rectangle with yellow stroke
   CGContextStrokeRectWithColor( context, rect, 3.0, CGColorMakeYellow());

   // Save graphics state before applying transformation
   CGContextSave( context);
   {
#ifdef FIXED_ROTATION
      factor = FIXED_ROTATION;
#else
      factor = ([context numberOfFrames] % 120) / 119.0;
#endif
      rotationTransform = CGAffineTransformMakeRotationAroundPoint( CGRectGetCenterPoint( rect), factor);
      // Apply rotation transform
      CGContextTransform( context, &rotationTransform);

      // Draw rotated rectangle with green stroke
      CGContextBeginPath( context);
      CGContextRect( context, rect);
      CGContextStrokeColor( context, CGColorMakeGreen());
      CGContextStroke( context);

      // Restore graphics state
   }
   CGContextRestore( context);

   // Subject rectangle (axis-aligned)
   subject[0] = CGPointMake( rect.origin.x, rect.origin.y);                          // top-left
   subject[1] = CGPointMake( rect.origin.x + rect.size.width, rect.origin.y);        // top-right
   subject[2] = CGPointMake( rect.origin.x + rect.size.width, rect.origin.y + rect.size.height); // bottom-right
   subject[3] = CGPointMake( rect.origin.x, rect.origin.y + rect.size.height);      // bottom-left

   for( i = 0; i < 4; i++)
      clipRect[i] = CGPointApplyAffineTransform(subject[i], rotationTransform);

   // Find intersection using the cleaner clipping function
   count = CGPolygon4ClipAgainstPolygon4(subject, clipRect, result);

   // Draw circles at intersection points
   for( i = 0; i < count; i++)
      CGContextFillCircleWithColor(context,
                                 CACircleMakeWithCenterAndRadius( result[i], 5.0),
                                 i >= 4 ? CGColorMakeRed() : CGColorMakeBlue());

   rect = CGPolygonBoundingBox( result, count);
   CGContextStrokeRectWithColor( context, rect, 0.75, CGColorMakeRed());

   label = MulleObjC_asprintf( "Intersection points: %d", count);
   drawRectBehindText( context, rect, label, CGContentModeCenterPoint);

   return( YES);
}


int   main( int argc, char *argv[])
{
   return( UIApplicationRunDrawCallbackHz( (CGGraphicsLayerDrawCallback_t *) drawStuff,
                                           CGSizeMake( 800, 400),
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

