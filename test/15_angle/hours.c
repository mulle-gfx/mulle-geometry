#include <mulle-geometry/mulle-geometry.h>

#include <stdio.h>

CGFloat   degrees( CGFloat rad)
{
   CGFloat   deg;

   deg = CAAngleRadiansToDegrees( rad);
   if( deg < 0.0)
      deg += 360.0;
   return( deg);
}


int   main( int argc, char *argv[])
{
   CGFloat   val;
   CGFloat   rad;
   CGFloat   revhour;

   for( val = 0.0f; val <= 12.0f; val += 1.0f)
   {
      rad     = CAAngleHourToRadians( val);
      revhour = CAAngleRadiansToHour( rad);

      printf( "val: %.3f rad: %.3f deg: %.3f (%.3f)\n",
                     val,
                     rad,
                     degrees( rad),
                     revhour);
   }
   return( 0);
}
