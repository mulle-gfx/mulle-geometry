#import "import.h"



static void   gradientFillRGBABytes( unsigned char * data, NSUInteger w, NSUInteger h)
{
    int     x, y;
    float   r;
    float   g;
    float   b;
    float   step_x;
    float   step_y;

    step_x = 1.0 / (w - 1);
    step_y = 1.0 / (h - 1);

//    1.0 -> 1.0  r
//    |
//    0.0 -> 0.0  r
//
//    0.0 -> 1.0  g
//    |
//    0.0 -> 1.0  g
//
//    0.0 -> 0.0  b
//    |
//    1.0 -> 0.0  b

    for (y = 0; y < h; y++)
    {
        for( x = 0; x < w; x++)
        {
            r  = 1.0 - step_y * y;
            g  = step_x * x;
            b  = step_y * y - step_x * x;
            if( b < 0)
               b = 0.0;

            assert( r >= 0.0 && r <= 1.0);
            assert( g >= 0.0 && g <= 1.0);
            assert( b >= 0.0 && b <= 1.0);

            data[((x + w * y) * 4) + 0] = 255 * r;
            data[((x + w * y) * 4) + 1] = 255 * g;
            data[((x + w * y) * 4) + 2] = 255 * b;
            data[((x + w * y) * 4) + 3] = 255; // Alpha channel
        }
    }
}




void   drawGradientRectangle( CGContext* context, CGRect rect, CGPaint *paint)
{
    float   x = rect.origin.x;
    float   y = rect.origin.y;
    float   width = rect.size.width;
    float   height = rect.size.height;

    CGContextBeginPath( context);
    CGContextRect( context, rect);
    CGContextFillPaintableOptions( context, paint, rect, NULL);
    CGContextFill( context);

    // stroke an outline, because why not
    CGContextStrokeColor( context, CGColorMakeWithRGBABytes( 255, 255, 255, 255));
    CGContextStrokeWidth( context, 1.0);
    CGContextStroke( context);
}


static void   drawFourCircleSegments( CGContext *context,
                                      CACircle circle,
                                      float strokeWidth,
                                      unsigned char *texture,
                                      CGPaint *paint)
{
   CGRect     rect;

   rect  = CACircleGetOuterRect( circle);

   CGContextFillPaintableOptions( context, paint, rect, NULL);
   CGContextStrokePaintableOptions( context, paint, rect, strokeWidth, NULL);
   CGContextStrokeWidth( context, strokeWidth);

   CGContextBeginPath( context);
   CGContextMoveTo( context, CACircleGetCenter( circle));
   CGContextArcRadians( context, circle, CAAngleNorthWest, CAAngleNorthEast, CARotationDirectionClockWise);
   CGContextStroke( context);

   CGContextBeginPath( context);
   CGContextMoveTo( context, CACircleGetCenter( circle));
   CGContextArcRadians( context, circle, CAAngleNorthEast, CAAngleSouthEast, CARotationDirectionClockWise);
   CGContextStroke( context);

   CGContextBeginPath( context);
   CGContextMoveTo( context, CACircleGetCenter( circle));
   CGContextArcRadians( context, circle, CAAngleSouthEast, CAAngleSouthWest, CARotationDirectionClockWise);
   CGContextStroke( context);

   CGContextBeginPath( context);
   CGContextMoveTo( context, CACircleGetCenter( circle));
   CGContextArcRadians( context, circle, CAAngleSouthWest, CAAngleNorthWest, CARotationDirectionClockWise);
   CGContextStroke( context);
}



static BOOL   drawStuff( CGGraphicsLayer *layer, CGContext *context)
{
   CGRect              canvas;
   CGRect              rect;
   CACircle            circle;
   static CGImage      *image;


#define TEX_W  8
#define TEX_H  8

   static unsigned char   texture[ TEX_H * TEX_W * 4];
   if( ! texture[ 3])
   {
// 2x2 has some banding and doesn't look good
// 4x4 has very minor banding

      gradientFillRGBABytes( texture, TEX_W, TEX_H);
      image = [CGImage imageWithRGBABytes:texture
                               bitmapSize:CGPixelSizeMake( TEX_W, TEX_H)
                             textureFlags:0
                                  context:context];
   }


   canvas = [layer canvas];
   rect   = CGRectInset( canvas, 100, 100);
   circle = CGRectGetOuterCircle( rect);

   drawFourCircleSegments( context, circle, 3.0f, texture, image);

   // draws the square with "gouraud" shading, which is surprisingly
   // tricky in nanovg
   drawGradientRectangle( context, rect, image);

   // draw a small inner circle, because why not
   CGContextStrokeCircleWithColor( context, CGRectGetInnerCircle( rect),
                                 0.5f,
                                 CGColorMakeWhite());

   return( YES);
}


int   main( int argc, char *argv[])
{
   return( UIApplicationRunDrawCallbackHz( drawStuff,
                                           CGSizeMake( 400, 400),
                                           NULL,
                                           0.0f));
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
// █║▄▄▄█▄▄▄║H:  "╙▀███▄╓▄,█▌╓▄,,▄∩  ██p$ΦΦ▀▀▀▀▀ΦΦΦR≡≥▄╓ ║▄ ╙j▌` ▄▀╙╗ `╫
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

