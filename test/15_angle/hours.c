#include <mulle-geometry/mulle-geometry.h>

#include <stdio.h>

int   main( int argc, char *argv[])
{
   CGFloat          val;
   CGFloat          ang;
   CGFloat      revhour;
   CGFloat   deg;

   for( val = 1.0f; val <= 12.01f; val += 1.0f)
   {
      if( val > 12.0f)
         val = 12.0f;

      ang     = CAHourToAngle( val);
      revhour = CAAngleToHour( ang);
      deg     = CAAngleToDegrees( ang);

      printf( "val: %.3f ang: %.3f deg: %.3f (%.3f)\n",
                     val,
                     ang,
                     deg,
                     revhour);
   }
   return( 0);
}
