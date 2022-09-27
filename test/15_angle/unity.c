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
   CGFloat   revval;

   for( val = 0.0f; val <= 1.0f; val += 0.1f)
   {
      rad    = CAAngleUnityToRadians( val);
      revval = CAAngleRadiansToUnity( rad);

      printf( "val: %.3f rad: %.3f deg: %.3f (%.3f)\n",
                     val,
                     rad,
                     degrees( rad),
                     revval);
   }
   return( 0);
}
