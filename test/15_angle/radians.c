#include <mulle-geometry/mulle-geometry.h>

#include <stdio.h>


int   main( int argc, char *argv[])
{
   CAAngle      v_angle;
   CGFloat      v_deg;
   CGFloat      v_hour;
   CGFloat      v_rad2;
   CGFloat      v_rad;
   CGFloat      v_radhour;
   CARotation   v_rotA;
   CARotation   v_rotR;

   for( v_rad = -M_PI_f * 2.0; v_rad <= M_PI_f * 2.0 + 0.1; v_rad += M_PI_f * 0.25)
   {
      if( v_rad > -0.01 && v_rad < 0.01)
         v_rad = 0.0;
      if( v_rad > M_PI_f * 2.0)
         v_rad = M_PI_f * 2.0;

      v_angle   = CARadiansToAngle( v_rad);
      v_rotR    = CARadiansAsRotationToRotation( v_rad);
      v_rotA    = CARadiansAsAngleToRotation( v_rad);
      v_deg     = CARadiansAsAngleToDegrees( v_rad);

      v_rad2    = CAAngleToRadians( v_angle);
      v_hour    = CAAngleToHour( v_rad2);
      v_radhour = CAHourToAngle( v_hour);

      mulle_printf( "[%.1f rad] ang: %.3f rotA: %.3f rotR: %.3f deg: %.3f hour: %.3f (%.3f)\n",
                  v_rad,
                  v_angle, 
                  v_rotA,
                  v_rotR,
                  v_deg,
                  v_hour,
                  v_radhour);
   }
   return( 0);
}