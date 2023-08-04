#include <mulle-geometry/mulle-geometry.h>

#include <stdio.h>


int   main( int argc, char *argv[])
{
   int              angle;
   CGFloat   deg;
   CGFloat          ang;
   CGFloat          rad;
   CGFloat          rad2;

   for( angle = -720; angle <= 720; angle += 45)
   {
      deg  = CADegreesNormalize( (CGFloat) angle);
      ang  = CADegreesToAngle( deg);
      rad  = CAAngleToRadians( ang);
      rad2 = CARadiansToAngle( rad);

      printf( "a:%d dn:%.1f r:%.3f (%.1f) rn:%.1f (%.1f)\n",
                     angle, deg,
                     rad, CAAngleToDegrees( rad),
                     rad2, CAAngleToDegrees( rad2));
   }
   return( 0);
}
