#import "import.h"


static int   CGFloatPointerCompare( CGFloat *a, CGFloat *b, void *unused)
{
   return( *a >= *b ? (*a != *b) : -1);
}


static int   CGFloatPointerInvCompare( CGFloat *a, CGFloat *b, void *unused)
{
   return( *a > *b ? -1 : (*a != *b));
}


static BOOL   drawStuff( CALayer *layer, CGContext *context)
{
   struct NVGcontext   *vg;
   CGRect              canvas;
   CGRect              rect;
   NSUInteger          n;
   NSUInteger          i;

   vg     = CGContextGetNVGContext( context);
   canvas = [layer canvas];
   rect   = CGRectInset( canvas, 50, 50);
   rect   = CGSizeCenterInRect( CGRectGetLargestSquareSize( rect), rect);;
   mvgFillRectWithColor( vg, rect, CGColorMakeBlue());

   n = (NSUInteger) floor( rect.size.width) - 2;
   mulle_flexarray_do( buf, CGFloat, 0, n)
   {
      if( ([context numberOfFrames] / 100) & 0x1)
      {
         for( i = 0; i < n; i++)   
            buf[ i] = CGFloatMakeAbnormalDistributionMinMax( 0.0, rect.size.height);
         mulle_qsort( buf, n, sizeof( CGFloat), (mulle_qsort_cmp_t *) CGFloatPointerCompare);
      }
      else
      {
         for( i = 0; i < n; i++)   
            buf[ i] = CGFloatMakeNormalDistributionMinMax( 0.0, rect.size.height);
         mulle_qsort( buf, n, sizeof( CGFloat), (mulle_qsort_cmp_t *) CGFloatPointerInvCompare);
      }


      nvgBeginPath( vg);
      nvgMoveTo( vg, CGRectGetMinX( rect), CGRectGetMaxY( rect));
      for( i = 0; i < n / 2; i +=2)
      {
         nvgLineTo( vg, CGRectGetMinX( rect) + 1.0 + i,
                        CGRectGetMaxY( rect) - buf[ i]);
      }
      nvgLineTo( vg, CGRectGetMidX( rect), CGRectGetMaxY( rect) - buf[ n / 2]);
      nvgLineTo( vg, CGRectGetMidX( rect), CGRectGetMaxY( rect));
      nvgClosePath( vg);
      nvgFillColor( vg, CGColorMakeWhite());
      nvgFill( vg);

      nvgBeginPath( vg);
      nvgMoveTo( vg, CGRectGetMaxX( rect), CGRectGetMaxY( rect));
      for( i = 1; i < n / 2; i +=2)
      {
         nvgLineTo( vg, CGRectGetMaxX( rect) - 1.0 - i,
                        CGRectGetMaxY( rect) - buf[ i]);
      }
      nvgLineTo( vg, CGRectGetMidX( rect), CGRectGetMaxY( rect) - buf[ n / 2]);
      nvgLineTo( vg, CGRectGetMidX( rect), CGRectGetMaxY( rect));
      nvgClosePath( vg);
      nvgFillColor( vg, CGColorMakeWhiteSmoke());
      nvgFill( vg);
   }

   return( YES);
}


int   main( int argc, char *argv[])
{
   return( UIApplicationRunDrawCallbackHz( (CGGraphicsLayerDrawCallback_t *) drawStuff,
                                           CGSizeMake( 200, 250),
                                           NULL,
                                           10.0f));
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

