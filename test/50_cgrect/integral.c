#include <mulle-geometry/mulle-geometry.h>


#include <stdio.h>


void   CGRectPrint( CGRect rect)
{
   printf( "%.2f %.2f %.2f %.2f",
            rect.origin.x,
            rect.origin.y,
            rect.size.width,
            rect.size.height);
}


static void   test( CGRect rect)
{
   CGRect   rect2;

   rect2 = CGRectIntegral( rect);

   printf( "[r=");
   CGRectPrint( rect);
   printf( " i=");
   CGRectPrint( rect2);
   printf( "]");
   printf( "\n");
}


int   main( void)
{
   test( CGRectMake( 0, 0, 4, 4));
   test( CGRectMake( 0.5, 0, -4.5, 4));

   return( 0);
}

