#include <mulle-geometry/mulle-geometry.h>


#include <stdio.h>


void   CGRectPrint( CGRect rect)
{
   mulle_printf( "%.2f %.2f %.2f %.2f",
            rect.origin.x,
            rect.origin.y,
            rect.size.width,
            rect.size.height);
}


static void   test( CGRect rect1, CGRect rect2)
{
   CGRect   rect3;

   rect3 = CGRectIntersection( rect1, rect2);

   mulle_printf( "[a=");
   CGRectPrint( rect1);
   mulle_printf( " b=");
   CGRectPrint( rect2);
   mulle_printf( " u=");
   CGRectPrint( rect3);
   mulle_printf( "]");
   mulle_printf( "\n");
}


int   main( void)
{
   test( CGRectMake( 1, 1, 2, 2), CGRectMake( 0, 0, 4, 4));
   test( CGRectMake( 1, 1, 2, 2), CGRectMake( 0, 0, 1, 1));
   test( CGRectMake( 0, 0, 4, 4), CGRectMake( 0.5, 0, -4.5, 4));

   return( 0);
}

