#include <mulle-geometry/mulle-geometry.h>
#include <stdio.h>


// this is our texture
#define IMAGE_WIDTH    9
#define IMAGE_HEIGHT   6


// this is our rect
#define X  100
#define Y  200
#define W  400
#define H  300


void   CGPointPrint( CGPoint point)
{
   printf( "%.2f %.2f", point.x, point.y);
}



void   CGSizePrint( CGSize size)
{
   printf( "%.2f %.2f", size.width, size.height);
}



static CGAffineTransform   bitmap_transform( CGSize imageSize)
{
   CGAffineTransform  scale;

   //
   // this rotates and maps the vertex coordinate back to texture space
   //
   // texture coordinates in a 2D texture are between 0 and 1.
   scale   = CGAffineTransformMakeScale( imageSize.width, imageSize.height);

   return( scale);
}


static CGAffineTransform   fragment_transform( CGAffineTransform paint, CGSize extent)
{
   CGAffineTransform  scale;
   CGAffineTransform  shader;

   //
   // this rotates and maps the vertex coordinate back to texture space
   //
   // texture coordinates in a 2D texture are between 0 and 1.
   scale   = CGAffineTransformMakeScale( 1.0 / extent.width, 1.0 / extent.height);
   shader  = CGAffineTransformConcat( paint, scale);

   return( shader);
}



static CGAffineTransform   paint_transform( CGPoint point, CGFloat angle)
{
   CGAffineTransform   rotate;
   CGAffineTransform   xform;
   CGAffineTransform   translate;

   rotate    = CGAffineTransformMakeRotation( angle);
   translate = CGAffineTransformMakeTranslation( point.x, point.y);
   xform     = CGAffineTransformConcat( rotate, translate);

   return( xform);
}


static CGAffineTransform  reverse_texture_mapper( CGPoint point,
                                                  CGFloat angle,
                                                  CGSize extent,
                                                  CGSize bitmapSize)
{
   CGAffineTransform   paint;
   CGAffineTransform   shader;
   CGAffineTransform   combined;
   CGAffineTransform   current;
   CGAffineTransform   fragment;
   CGAffineTransform   bitmap;
   CGAffineTransform   reverse;
   CGPoint             texturePoint;
   CGPoint             vertexPoint;
   CGPoint             bitmapPoint;

   // stand in for current transformation
   current  = CGAffineTransformMakeIdentity();

   // NVGpaint nvgImagePattern(NVGcontext* ctx,
   //                          float point_x, float point_y,
   //                          float extent_w, float extent_h,
   //                          float angle,
   //                          int image,
   //                          float alpha)
   paint    = paint_transform( point, angle);
   combined = CGAffineTransformConcat( current, paint);
   shader   = CGAffineTransformInvert( combined);

   // actual shader code is:
   // "     // Calculate color fron texture\n"
   // "     vec2 texturePoint = (shader * vec3( vertex,1.0)).xy / extent;\n"
   // "     vec4 color = texture2D( image, texturePoint);\n"

   fragment = fragment_transform( shader, extent);
   bitmap   = bitmap_transform( bitmapSize);

   combined = CGAffineTransformConcat( fragment, bitmap);
   reverse  = CGAffineTransformInvert( combined);

   return( reverse);
}




static void   test( CGPoint vertex, CGPoint point, CGFloat angle, CGSize extent, CGSize bitmapSize)
{
   CGAffineTransform   paint;
   CGAffineTransform   shader;
   CGAffineTransform   combined;
   CGAffineTransform   current;
   CGAffineTransform   fragment;
   CGAffineTransform   bitmap;
   CGAffineTransform   reverse;
   CGPoint             texturePoint;
   CGPoint             vertexPoint;
   CGPoint             bitmapPoint;

   printf( "{ v=");
   CGPointPrint( vertex);
   printf( ", p=");
   CGPointPrint( point);
   printf( ", e=");
   CGSizePrint( extent);
   printf( ", s=");
   CGSizePrint( bitmapSize);
   printf( " } ");

   // stand in for current transformation
   current       = CGAffineTransformMakeIdentity();

   // NVGpaint nvgImagePattern(NVGcontext* ctx,
   //                          float point_x, float point_y,
   //                          float extent_w, float extent_h,
   //                          float angle,
   //                          int image,
   //                          float alpha)
   paint         = paint_transform( point, angle);
   combined      = CGAffineTransformConcat( current, paint);

   // this is what gets passed to the shader
   shader        = CGAffineTransformInvert( combined);

   // actual shader code is:
   // "     // Calculate color fron texture\n"
   // "     vec2 texturePoint = (shader * vec3( vertex,1.0)).xy / extent;\n"
   // "     vec4 color = texture2D( image, texturePoint);\n"

   fragment      = fragment_transform( shader, extent);

   // fragment_shader applies vertex to get texture point
   // (0..1 / 0..1)
   texturePoint  = CGPointApplyAffineTransform( vertex, fragment);

   printf( " -> t=");
   CGPointPrint( texturePoint);

   // map this to our bitmap size
   bitmap        = bitmap_transform( bitmapSize);
   bitmapPoint   = CGPointApplyAffineTransform( texturePoint, bitmap);

   printf( " -> b=");
   CGPointPrint( bitmapPoint);

   // the reverse transforms a texture point between 0.0 1.0 back to vertex
   reverse       = CGAffineTransformInvert( fragment);

   vertexPoint = CGPointApplyAffineTransform( texturePoint, reverse);
   printf( " -> rt=");
   CGPointPrint( vertexPoint);

   // the reverse transforms a image point between 0..bitmapSize.width
   // back
   combined      = CGAffineTransformConcat( fragment, bitmap);
   reverse       = CGAffineTransformInvert( combined);

   vertexPoint   = CGPointApplyAffineTransform( bitmapPoint, reverse);
   printf( " -> ri=");
   CGPointPrint( vertexPoint);

   printf( "\n");
}



int   main( void)
{
   CGFloat             x, y;
   CGAffineTransform   xform;
   CGPoint             point1;
   CGPoint             point2;
   CGPoint             imagePoint1;
   CGPoint             imagePoint2;

   for( y = Y; y <= Y + H; y += H / 4.0)
      for( x = X; x <= X + W; x +=  W / 4.0)
      {
         test( CGPointMake( x, y),
               CGPointMake( X, Y),
               0,
               CGSizeMake( W, H),
               CGSizeMake( IMAGE_WIDTH, IMAGE_HEIGHT));
      }

   printf( "\n");
   printf( "\n");
   printf( "\n");

   // we use the reverse_texture_mapper to figure out
   // what we want to use for X, Y, W, H so that we only get a
   // subpart of the texture

   xform = reverse_texture_mapper( CGPointMake( X, Y),
                                   0,
                                   CGSizeMake( W, H),
                                   CGSizeMake( IMAGE_WIDTH, IMAGE_HEIGHT));

   for( y = 0; y <= IMAGE_HEIGHT; y += IMAGE_HEIGHT / 4.0)
      for( x = 0; x <= IMAGE_WIDTH; x += IMAGE_WIDTH / 4.0)
         {
            imagePoint1 = CGPointMake( x, y);
            point1      = CGPointApplyAffineTransform( imagePoint1, xform);

            printf( "i[ %.1f][ %.1f]=", y, x);
            CGPointPrint( imagePoint1);
            printf( " -> p=");
            CGPointPrint( point1);
            printf( "\n");
         }

   printf( "\n");
   printf( "\n");
   printf( "\n");


   xform = reverse_texture_mapper( CGPointMake( X, Y),
                                   0,
                                   CGSizeMake( W, H),
                                   CGSizeMake( IMAGE_WIDTH, IMAGE_HEIGHT));

   // we should be able to choose points in image

   imagePoint1 = CGPointMake( 1, 1);
   imagePoint2 = CGPointMake( imagePoint1.x + IMAGE_WIDTH / 3, imagePoint1.y + IMAGE_HEIGHT / 3);

   point1 = CGPointApplyAffineTransform( imagePoint1, xform);
   point2 = CGPointApplyAffineTransform( imagePoint2, xform);

   // two points in our texture, translated to X/Y
   printf( "b1=");
   CGPointPrint( imagePoint1);
   printf( " -> p1=");
   CGPointPrint( point1);

   printf( "\nb2=");
   CGPointPrint( imagePoint2);
   printf( " -> p2=");
   CGPointPrint( point2);
   printf( "\n\n");


   for( y = Y; y <= Y + H; y += H / 4.0)
      for( x = X; x <= X + W; x += W / 4.0)
      {
         test( CGPointMake( x, y),
               CGPointMake( X - (point1.x - X), Y - (point1.y - Y)),
               0,
               CGSizeMake( W * IMAGE_WIDTH  / (imagePoint2.x - imagePoint1.x),
                           H * IMAGE_HEIGHT / (imagePoint2.y - imagePoint1.y)),
               CGSizeMake( IMAGE_WIDTH, IMAGE_HEIGHT));
      }


   return( 0);
}

