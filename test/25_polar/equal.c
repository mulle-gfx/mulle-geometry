#include <mulle-geometry/mulle-geometry.h>

#include <stdio.h>


int   main( int argc, char *argv[])
{
   CAPolarPoint   a, b;

   a = CAPolarPointMakeWithRadians( -10, CADegreesToRadians( 45.0));
   b = CAPolarPointMakeWithRadians( +10, CADegreesToRadians( 45.0 + 180.0));

   printf( "%s\n", CAPolarPointEqualToPolarPoint( a, b) ? "YES" : "NO");

   return( 0);
}
