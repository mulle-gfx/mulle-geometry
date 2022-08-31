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


static void   test( CGRect rect1, CGRect rect2)
{
   CGRect   rect3;

   rect3 = CGRectUnion( rect1, rect2);

   printf( "[a=");
   CGRectPrint( rect1);
   printf( " b=");
   CGRectPrint( rect2);
   printf( " u=");
   CGRectPrint( rect3);
   printf( "]");
   printf( "\n");
}


int   main( void)
{
   test( CGRectMake( 1, 1, 2, 2), CGRectMake( 0, 0, 4, 4));
   test( CGRectMake( 1, 1, 2, 2), CGRectMake( 0, 0, 1, 1));
   test( CGRectMake( 0, 0, 4, 4), CGRectMake( 0.5, 0, -4.5, 4));

   return( 0);
}

