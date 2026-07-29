#include <mulle-geometry/mulle-geometry.h>

#include <stdio.h>


int   main( int argc, char *argv[])
{
   int       v_angle;
   CGFloat   v_deg;
   CGFloat   v_ang;
   CGFloat   v_rad;
   CGFloat   v_rad2;

   for( v_angle = -720; v_angle <= 720; v_angle += 45)
   {
      v_deg  = CADegreesNormalize( (CGFloat) v_angle);
      v_ang  = CADegreesToAngle( v_deg);
      v_rad  = CAAngleToRadians( v_ang);
      v_rad2 = CARadiansToAngle( v_rad);

      mulle_printf( "a:%d dn:%.1f r:%.3f (%.1f) rn:%.1f (%.1f)\n",
                     v_angle, v_deg,
                     v_rad, CAAngleToDegrees( v_rad),
                     v_rad2, CAAngleToDegrees( v_rad2));
   }
   return( 0);
}
