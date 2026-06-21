#include <mulle-geometry/mulle-geometry.h>
#include <stdio.h>


void   CGPointPrint( CGPoint point)
{
   mulle_printf( "%.2f %.2f", point.x, point.y);
}


void   CGRectPrint( CGRect rect)
{
   mulle_printf( "%.2f %.2f %.2f %.2f",
            rect.origin.x,
            rect.origin.y,
            rect.size.width,
            rect.size.height);
}



static void   test( CGRect frame1, CGRect bounds1,
                    CGRect frame2, CGRect bounds2,
                    CGRect rect)
{
   CGRect              converted;
   CGAffineTransform   xform;
   CGAffineTransform   xform1;
   CGAffineTransform   xform2;
   CGRect              reverse;

   xform1          = CGAffineTransformMakeProjection( frame1, bounds1);
   xform2          = CGAffineTransformMakeProjection( frame2, bounds2);
   xform           = CGAffineTransformConcat( xform2, xform1);
   converted       = CGRectApplyAffineTransformInline( rect, xform);

   xform1          = CGAffineTransformMakeInverseProjection( frame1, bounds1);
   xform2          = CGAffineTransformMakeInverseProjection( frame2, bounds2);
   xform           = CGAffineTransformConcat( xform1, xform2);
   reverse         = CGRectApplyAffineTransformInline( converted, xform);

   mulle_printf( "[f=");
   CGRectPrint( frame1);
   mulle_printf( " b=");
   CGRectPrint( bounds1);
   mulle_printf( "] ");
   mulle_printf( "[f=");
   CGRectPrint( frame2);
   mulle_printf( " b=");
   CGRectPrint( bounds2);
   mulle_printf( "] ");
   CGRectPrint( rect);
   mulle_printf( " -> ");
   CGRectPrint( converted);
   mulle_printf( " -> ");
   CGRectPrint( reverse);
   mulle_printf( "\n");
}


int   main( void)
{
   //
   // the 20 needs to be doubled and added to the 10 of first rect,
   // to get a 50 for 'x'
   //
   test( CGRectMake( 10, 0, 4, 4), CGRectMake( 0, 0, 2, 2),
         CGRectMake( 20, 0, 2, 2), CGRectMake( 0, 0, 1, 1),
         CGRectMake( 0, 1, 0.5, 1));

   return( 0);
}

