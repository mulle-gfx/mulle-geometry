#include <mulle-geometry/mulle-geometry.h>


#include <stdio.h>


void   CGFloatPrint( CGFloat value)
{
   printf( "%.2f", value);
}


void   CGPointPrint( CGPoint origin)
{
   printf( "%.2f %.2f",
            origin.x,
            origin.y);
}


void   CGRectPrint( CGRect rect)
{
   printf( "%.2f %.2f %.2f %.2f",
            rect.origin.x,
            rect.origin.y,
            rect.size.width,
            rect.size.height);
}


static void   test( CGPoint center, CGFloat radius)
{
   CGRect   rect;

   rect = CACircleGetOuterRect( CACircleMakeWithCenterAndRadius( center, radius));

   printf( "[c=");
   CGPointPrint( center);
   printf( " r=");
   CGFloatPrint( radius);
   printf( " rect=");
   CGRectPrint( rect);
   printf( "]");
   printf( "\n");
}


int   main( void)
{
   test( CGPointMake( 0, 0), 0.0);
   test( CGPointMake( 0, 0), 30.0);
   test( CGPointMake( 100, 100), 30.0);
   test( CGPointMake( 1, 2), 3.0);

   return( 0);
}

