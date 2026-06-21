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


static void   test( CGRect rect, CARotation rotation)
{
   CGRect   rotated;

   rotated = CGRectRotate( rect, rotation);

   mulle_printf( "[r=");
   CGRectPrint( rect);
   mulle_printf( " rot=%.2f", rotation);
   mulle_printf( " result=");
   CGRectPrint( rotated);
   mulle_printf( "]");
   mulle_printf( "\n");
}


int   main( void)
{
   // Test with no rotation (should return same rect)
   test( CGRectMake( 0, 0, 4, 4), 0.0f);
   test( CGRectMake( 1, 2, 3, 4), 0.0f);

   mulle_printf("\n");

   // Test with 90 degree rotation (0.25 rotation)
   test( CGRectMake( 0, 0, 4, 4), 0.25f);
   test( CGRectMake( 1, 2, 3, 4), 0.25f);

   mulle_printf("\n");

   // Test with 180 degree rotation (0.5 rotation)
   test( CGRectMake( 0, 0, 4, 4), 0.5f);
   test( CGRectMake( 1, 2, 3, 4), 0.5f);

   mulle_printf("\n");

   // Test with 270 degree rotation (0.75 rotation)
   test( CGRectMake( 0, 0, 4, 4), 0.75f);
   test( CGRectMake( 1, 2, 3, 4), 0.75f);

   mulle_printf("\n");

   // Test with full rotation (1.0 rotation, should be same as 0)
   test( CGRectMake( 0, 0, 4, 4), 1.0f);
   test( CGRectMake( 1, 2, 3, 4), 1.0f);

   mulle_printf("\n");

   // Test with negative rotation
   test( CGRectMake( 0, 0, 4, 4), -0.25f);

   mulle_printf("\n");

   // Test with fractional rotation
   test( CGRectMake( 0, 0, 4, 4), 0.125f); // 45 degrees

   return( 0);
}
