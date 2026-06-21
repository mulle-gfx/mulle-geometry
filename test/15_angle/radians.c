#include <mulle-geometry/mulle-geometry.h>

#include <stdio.h>


int   main( int argc, char *argv[])
{
   CAAngle      angle;
   CGFloat      deg;
   CGFloat      hour;
   CGFloat      rad2;
   CGFloat      rad;
   CGFloat      radhour;
   CARotation   rotA;
   CARotation   rotR;

   for( rad = -M_PI_f * 2.0; rad <= M_PI_f * 2.0 + 0.1; rad += M_PI_f * 0.25)
   {
      if( rad > -0.01 && rad < 0.01)
         rad = 0.0;
      if( rad > M_PI_f * 2.0)
         rad = M_PI_f * 2.0;

      angle   = CARadiansToAngle( rad);
      rotR    = CARadiansAsRotationToRotation( rad);
      rotA    = CARadiansAsAngleToRotation( rad);
      deg     = CARadiansAsAngleToDegrees( rad);

      rad2    = CAAngleToRadians( angle);
      hour    = CAAngleToHour( rad2);
      radhour = CAHourToAngle( hour);

      mulle_printf( "[%.1f rad] ang: %.3f rotA: %.3f rotR: %.3f deg: %.3f hour: %.3f (%.3f)\n",
                  rad,
                  angle, 
                  rotA,
                  rotR,
                  deg,
                  hour,
                  radhour);
   }
   return( 0);
}