#include <mulle-geometry/mulle-geometry.h>

#include <stdio.h>


int   main( int argc, char *argv[])
{
   CAPolarPoint   a, b;

   a = CAPolarPointMakeWithRadiansAsAngle( -10, CADegreesToRadiansAsAngle( 45.0));
   b = CAPolarPointMakeWithRadiansAsAngle( +10, CADegreesToRadiansAsAngle( 45.0 + 180.0));

   printf( "%s\n", CAPolarPointEqualToPolarPoint( a, b) ? "YES" : "NO");

   return( 0);
}
