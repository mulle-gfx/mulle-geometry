#import "import.h"


#if 1
# define ROWS      2
# define COLUMNS   3
# define SPACING_H 30.0
# define SPACING_V 30.0
#else
# define ROWS      1
# define COLUMNS   1
# define SPACING_H 300.0
# define SPACING_V 100.0
# define ALL_SAME
#endif

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


static void   drawTriangleWithCenter( CGContext *context,
                                      CGPoint center,
                                      CGFloat sideWidth,
                                      CGColor color)
{
   CGFloat   height;
   CGFloat   halfWidth;
   CGFloat   topOffset;
   CGFloat   bottomOffset;
   CGPoint   p1;
   CGPoint   p2;
   CGPoint   p3;

   height       = sideWidth * 0.866f;  // sqrt(3)/2 for equilateral triangle
   halfWidth    = sideWidth * 0.5f;
   topOffset    = height * 0.667f;     // 2/3 of height above center
   bottomOffset = height * 0.333f;     // 1/3 of height below center
    
   p1 = CGPointMake( center.x, center.y - topOffset);           // Top point
   p2 = CGPointMake( center.x + halfWidth, center.y + bottomOffset); // Bottom right
   p3 = CGPointMake( center.x - halfWidth, center.y + bottomOffset); // Bottom left
        
   CGContextBeginPath( context);
   CGContextMoveTo( context, p1);
   CGContextLineTo( context, p2);
   CGContextLineTo( context, p3);
   CGContextClosePath( context);
   CGContextFillColor( context, color);
   CGContextFill( context);
}


static void   drawRotatedTriangleWithCenter( CGContext *context,
                                             CGPoint center,
                                             CGFloat sideWidth,
                                             CARotation rotation,
                                             CGColor color)
{
    CGFloat             height;
    CGFloat             halfWidth;
    CGFloat             topOffset;
    CGFloat             bottomOffset;
    CGAffineTransform   translateTransform;
    CGAffineTransform   rotateTransform;
    CGAffineTransform   combinedTransform;
    CGPoint             p1;
    CGPoint             p2;
    CGPoint             p3;
    
    height       = sideWidth * 0.866f; // sqrt(3)/2 for equilateral triangle
    halfWidth    = sideWidth * 0.5f;
    topOffset    = height * 0.667f; // 2/3 of height above center
    bottomOffset = height * 0.333f; // 1/3 of height below center
    
    translateTransform = CGAffineTransformTranslate(CGAffineTransformIdentity, CGPointNegate( center));
    rotateTransform    = CGAffineTransformRotate(CGAffineTransformIdentity, rotation);
    combinedTransform  = CGAffineTransformConcat(rotateTransform, translateTransform);
    translateTransform = CGAffineTransformTranslate(CGAffineTransformIdentity, center);
    combinedTransform  = CGAffineTransformConcat(rotateTransform, translateTransform);
    
    CGContextTransform( context, &combinedTransform);
        
    p1 = CGPointMake( center.x, center.y - topOffset);           // Top point
    p2 = CGPointMake( center.x + halfWidth, center.y + bottomOffset); // Bottom right
    p3 = CGPointMake( center.x - halfWidth, center.y + bottomOffset); // Bottom left
                
    CGContextBeginPath( context);
    CGContextMoveTo( context, p1);
    CGContextLineTo( context, p2);
    CGContextLineTo( context, p3);
    CGContextClosePath( context);
    CGContextFillColor( context, color);
    CGContextFill( context);
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


static char   *currentTransformUTF8String( CGContext *context)
{
   CGAffineTransform   xform;
   char                *s;
   xform = CGContextCurrentTransform( context);
   s     = CGAffineTransformUTF8String( &xform);
   return( s);
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
   CGAffineTransform   parent;
   CGAffineTransform   rotationTransform;
   char                *label;           
   double              factor;
   CGRect              bounds;
   CGRect              miniCanvas;
   CGAffineTransform   old;
   NSUInteger          mode;

   canvas = [layer canvas];

   CGContextBeginPath( context);
   CGContextRect( context, canvas);
   CGContextFillColor( context, CGColorMakeLightGray());
   CGContextFill( context);

   // subdivide into rows and columns
   CGRectPartitionIntoRowsAndColumns( CGRectInset( canvas, SPACING_H, SPACING_V), 
                                      CGSizeMake( SPACING_H, SPACING_V), 
                                      ROWS, 
                                      COLUMNS, 
                                      (CGRect *) rects);

   mode = ([context numberOfFrames] / 240) & 0x1;
   drawRectBehindText( context, CGRectInset( canvas, 0.0, 1.0), mode ? "xform + old" : "old + xform", CGContentModeTop);

   for( y = 0; y < ROWS; y++)
      for( x = 0; x < COLUMNS; x++)
      {
         rect  = rects[ y][ x];
         color = CGColorMakeWithHSL( (float) (x + y * COLUMNS) / (COLUMNS * ROWS), y ? 0.1 : 1.0, 0.5);

         CGContextBeginPath( context);
         CGContextRect( context, rect);
         CGContextFillColor( context, CGColorBySettingAlpha( color, 0.2));
         CGContextFill( context);

         factor     = _CACubicGetValue( CACubicLinear, ([context numberOfFrames] % 120) / 119.0);
         miniCanvas = CGRectMakeWithSize( rect.size);

         switch( x)
         {
         default : bounds = miniCanvas;
                   label  = "projection none"; 
                   break;
#ifndef ALL_SAME
         case 1  : bounds = CGRectMakeWithSize( CGSizeMake( miniCanvas.size.width * (0.34 + (factor * 0.66)),
                                                            miniCanvas.size.height * (0.34 + (factor * 0.66))));
                   label = "projection shrink"; 
                   break;

         case 2  : bounds = CGRectMakeWithSize( CGSizeMake( miniCanvas.size.width * (1.00 + (factor * 0.66)),
                                                            miniCanvas.size.height * (1.00 + (factor * 0.66))));
                   label  = "projection grow"; 
                   break;
#endif
         } 

         // draw bounds for debugging purposes
         CGContextSave( context);
         {
            parent = CGAffineTransformMakeTranslation( CGPointMake( rect.origin.x, rect.origin.y)); 
            CGContextTransform( context, &parent);

            CGContextBeginPath( context);
            CGContextRect( context, bounds);
            CGContextStrokeColor( context, CGColorBySettingAlpha( color, 0.5));
            CGContextStrokeWidth( context, 1.0);
            CGContextStroke( context);
         }
         CGContextRestore( context);

         // depending on the order of concatenation either 
         // the CGAffineTransformMakeProjection or the inverse is what we 
         // want in this case, we order parent -> local -> rotation so
         // use the inverse
         switch( y)
         {
#ifndef ALL_SAME
         case 0 :
            xform = CGAffineTransformMakeInverseProjection( miniCanvas, bounds);
            label = MulleObjC_asprintf( "inverse %s", label); 
            break;

         case 1 :
            xform = CGAffineTransformMakeProjection( miniCanvas, bounds);
            break;
#endif
         default :
            xform = CGAffineTransformMakeIdentity();
            break;
         }

         if( label)
            drawRectBehindText( context, CGRectInset( rect, 0.0, 1.0), label, CGContentModeBottom);

         //
         // from this point on we simulate what would happen in a 
         // CGGraphicsContext. The painting would be done in the "canvas"
         // which is usually 0,0,frame.width,frame.height. This canvas is then
         // projected via the "bounds" unto the "frame", but the frame.origin
         // undergoes another translation for its origin.
         //

         rotationTransform = CGAffineTransformMakeRotationAroundPoint( CGRectGetCenterPoint( miniCanvas), factor);

         CGContextSave( context);
         {
            CGContextReset( context);

            if( x == 1 && y == 0)
            {
               mulle_fprintf( stderr, "0. parent                : %s\n", CGAffineTransformUTF8String( &parent));
               mulle_fprintf( stderr, "0. xform                 : %s\n",  CGAffineTransformUTF8String( &xform));
               mulle_fprintf( stderr, "0. rotationTransform     : %s\n", CGAffineTransformUTF8String( &rotationTransform));
            }

            parent = CGAffineTransformMakeTranslation( CGPointMake( rect.origin.x, rect.origin.y)); 
            CGContextTransform( context, &parent);
            if( x == 1 && y == 0)
               mulle_fprintf( stderr, "1. parent                : %s\n", currentTransformUTF8String( context));
            CGContextTransform( context, &xform);
            if( x == 1 && y == 0)
               mulle_fprintf( stderr, "2. parent+xform          : %s\n", currentTransformUTF8String( context));
            CGContextTransform( context, &rotationTransform);
            if( x == 1 && y == 0)
               mulle_fprintf( stderr, "3. parent+xform+rotation : %s\n", currentTransformUTF8String( context));

            // draw canvas for debugging purposes
            {
               CGContextBeginPath( context);
               CGContextRect( context, miniCanvas);
               CGContextStrokeColor( context, CGColorBySettingAlpha( CGColorMakeGray(), 0.5));
               CGContextStrokeWidth( context, 1.0);
               CGContextStroke( context);
            }

            drawTriangleWithCenter( context, CGRectGetCenterPoint( miniCanvas), 20.0, color);

         }
         CGContextRestore( context);
      }

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

