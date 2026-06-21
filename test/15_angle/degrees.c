#include <mulle-geometry/mulle-geometry.h>

#include <stdio.h>


int   main( int argc, char *argv[])
{
   int       angle;
   CGFloat   ang;
   CGFloat   rot;
   CGFloat   deg;
   CGFloat   rad;
   CGFloat   hour;
   CGFloat   radhour;

   for( angle = -720; angle <= 720; angle += 45)
   {

      rot     = CADegreesToRotation( (CGFloat) angle);

      deg     = CARotationToDegrees( rot);
      rad     = CARotationToAngle( rot);
      hour    = CARotationToHour( rot);
      radhour = CAHourToAngle( hour);

      mulle_printf( "[%d°] rot: %.3f deg: %.3f rad:%.3f | hour: %.3f (%.3f)\n",
                  angle,
                  rot, deg, rad,
                  hour, radhour);
   }
   return( 0);
}