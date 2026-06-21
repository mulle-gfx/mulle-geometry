#include <mulle-geometry/mulle-geometry.h>

#include <stdio.h>


int   main( int argc, char *argv[])
{
   CAAngle      angle;
   CGFloat      hour;
   CARotation   rot;
   CARotation   back;

   for( rot = -1.0; rot <= 1.01; rot += 0.25)
   {
      angle = CARotationToAngle( rot);
      hour  = CAAngleToHour( angle);
      back  = CAAngleToRotation( angle);

      mulle_printf( "[%.2f rot] ang: %.3f (%.2f) hour: %.3f\n",
                  rot,
                  angle, 
                  back,
                  hour);
   }
   return( 0);
}