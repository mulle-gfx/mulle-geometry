#include <mulle-geometry/mulle-geometry.h>


#include <stdio.h>


void   CGFloatPrint( CGFloat value)
{
   mulle_printf( "%.2f", value);
}


void   CGPointPrint( CGPoint origin)
{
   mulle_printf( "%.2f %.2f",
            origin.x,
            origin.y);
}


void   CGRectPrint( CGRect rect)
{
   mulle_printf( "%.2f %.2f %.2f %.2f",
            rect.origin.x,
            rect.origin.y,
            rect.size.width,
            rect.size.height);
}


static void   test( CGPoint center, CGFloat radius)
{
   CGRect   rect;

   rect = CACircleGetInnerRect( CACircleMakeWithCenterAndRadius( center, radius));

   mulle_printf( "[c=");
   CGPointPrint( center);
   mulle_printf( " r=");
   CGFloatPrint( radius);
   mulle_printf( " rect=");
   CGRectPrint( rect);
   mulle_printf( "]");
   mulle_printf( "\n");
}


int   main( void)
{
   test( CGPointMake( 0, 0), 0.0);
   test( CGPointMake( 0, 0), 30.0);
   test( CGPointMake( 1, 2), 3.0);

   return( 0);
}

