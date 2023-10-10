#include <mulle-geometry/mulle-geometry.h>

#include <stdio.h>

static void   test( CAAngle a, CAAngle b)
{
   CAAngle   c;

   c = CAAngleGetMidAngle( a, b);

   printf( "%g mid %g = %g\n",
            CAAngleToDegrees( a),
            CAAngleToDegrees( b),
            CAAngleToDegrees( c));
}


int   main( int argc, char *argv[])
{
   test( CADegreesToAngle( 40), CADegreesToAngle( 60));
   test( CADegreesToAngle( 60), CADegreesToAngle( 40));
   test( CARadiansToAngle( 0),  CARadiansToAngle( CA_2xPI));

   return( 0);
}