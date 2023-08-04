#include <mulle-geometry/mulle-geometry.h>

#include <stdio.h>


int   main( int argc, char *argv[])
{
   CGFloat   angle;
   CGFloat   deg;
   CGFloat   hour;
   CGFloat   rad2;
   CGFloat   rad;
   CGFloat   radhour;
   CGFloat   rot;

   for( rad = -M_PI_f * 2.0; rad <= M_PI_f * 2.0 + 0.1; rad += M_PI_f * 0.25)
   {
      if( rad > -0.01 && rad < 0.01)
         rad = 0.0;
      if( rad > M_PI_f * 2.0)
         rad = M_PI_f * 2.0;

      angle   = CARadiansToAngle( rad);
      deg     = CARadiansToDegrees( angle);
      rot     = CARadiansToRotation( rad);
      rad2    = CAAngleToRadians( angle);
      hour    = CAAngleToHour( rad2);
      radhour = CAHourToAngle( hour);

      printf( "[%.1f rad] ang: %.3f rot: %.3f deg: %.3f hour: %.3f (%.3f)\n",
                  rad,
                  angle, 
                  rot,
                  deg,
                  hour,
                  radhour);
   }
   return( 0);
}