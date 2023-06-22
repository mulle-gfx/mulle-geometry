#include <mulle-geometry/mulle-geometry.h>

#include <stdio.h>


int   main( int argc, char *argv[])
{
   int       angle;
   CGFloat   deg;
   CGFloat   rad;
   CGFloat   rad2;

   for( angle = -720; angle <= 720; angle += 45)
   {
      deg  = CAAngleDegreesNormalize( (CGFloat) angle);

      rad  = CAAngleDegreesToRadians( (CGFloat) angle);
      rad2 = CAAngleRadiansNormalize( rad);

      printf( "a:%d dn:%.1f r:%.3f (%.1f) rn:%.1f (%.1f)\n",
                     angle, deg,
                     rad, CAAngleRadiansToDegrees( rad),
                     rad2, CAAngleRadiansToDegrees( rad2));
   }
   return( 0);
}
