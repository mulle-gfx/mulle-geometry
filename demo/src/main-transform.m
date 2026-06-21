#import "import.h"


#define ROWS      2
#define COLUMNS   3


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

   CGContextTextAlign( context, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
   CGContextText( context, info.bounds.origin, label, (size_t) -1);
}



static void drawDotInCenter( CGContext *context, 
                             CGRect canvas, 
                             CGFloat radius,
                             CGColor color)
{
   CGPoint   center;

   center = CGRectGetCenterPoint( canvas),

   CGContextBeginPath( context);
   CGContextCircle( context, CACircleMakeWithCenterAndRadius( center, radius));
   CGContextFillColor( context, color);
   CGContextFill( context);
}


//
// just a visual test for CGRectGetTopLeft and friends
//
static BOOL   drawStuff( CGGraphicsLayer *layer, CGContext *context)
{
   CGRect              canvas;
   CGRect              rect;
   CGRect              rects[ ROWS][ COLUMNS];
   NSUInteger          x, y;
   CGColor             color;
   CGAffineTransform   xform;
   char                *label;           
   double              factor;

   canvas = [layer canvas];

   CGContextBeginPath( context);
   CGContextRect( context, canvas);
   CGContextFillColor( context, CGColorMakeLightGray());
   CGContextFill( context);

   // subdivide into rows and columns
   CGRectPartitionIntoRowsAndColumns( CGRectInset( canvas, 4, 4), CGSizeMake( 4, 4), ROWS, COLUMNS, (CGRect *) rects);

   for( y = 0; y < ROWS; y++)
      for( x = 0; x < COLUMNS; x++)
      {
         rect = rects[ y][ x];

         color = CGColorMakeWithHSL( (float) (x + y * COLUMNS) / (COLUMNS * ROWS), 1.0, 0.5);

         CGContextBeginPath( context);
         CGContextRect( context, rect);
         CGContextFillColor( context, CGColorBySettingAlpha( color, 0.2));
         CGContextFill( context);

         factor = _CACubicGetValue( CACubicLinear, ([context numberOfFrames] % 120) / 119.0);
         switch( x)
         {
         case 0  : xform = CGAffineTransformMakeTranslation( CGPointMake( CGRectGetWidth( rect) * (1 / 3.0 * factor), 0)); 
                   label = "translation"; 
                   break;
         case 1  : xform = CGAffineTransformMakeRotation( factor); 
                   label = "rotation"; 
                   break;
         case 2  : xform = CGAffineTransformMakeScale( CGVectorMake( factor * 2.0, factor * 2.0)); 
                   label = "scale"; 
                   break;
         default : xform = CGAffineTransformMakeIdentity();
                   label = NULL;
         }

         switch( y)
         {
         case 1  : xform = CGAffineTransformInvert( xform); 
                   label = MulleObjC_asprintf( "%s inverted", label); 
                   break;
         }

         if( label)
            drawRectBehindText( context, CGRectInset( rect, 0.0, 1.0), label, CGContentModeBottom);

         CGContextSave( context);
         {
            CGContextTransform( context, &xform);
            drawDotInCenter( context, rect, 8.0f, color);
         }
         CGContextRestore( context);
      }

   return( YES);
}


int   main( int argc, char *argv[])
{
   return( UIApplicationRunDrawCallbackHz( (CGGraphicsLayerDrawCallback_t *) drawStuff,
                                           CGSizeMake( 400, 300),
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

