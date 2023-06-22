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
   CGFloat        rad;
   CADirection    direction;
   unsigned int   i;
   auto CAAngleRadians   directions[] =
   {
      CAAngleRadiansNNE,
      CAAngleRadiansNE ,
      CAAngleRadiansENE,
      CAAngleRadiansE  ,
      CAAngleRadiansESE,
      CAAngleRadiansSE ,
      CAAngleRadiansSSE,
      CAAngleRadiansS  ,
      CAAngleRadiansSSW,
      CAAngleRadiansSW ,
      CAAngleRadiansWSW,
      CAAngleRadiansW  ,
      CAAngleRadiansWNW,
      CAAngleRadiansNW ,
      CAAngleRadiansNNW,
      CAAngleRadiansN
   };
   static char   *names[] =
   {
      "CAAngleRadiansNNE",
      "CAAngleRadiansNE ",
      "CAAngleRadiansENE",
      "CAAngleRadiansE  ",
      "CAAngleRadiansESE",
      "CAAngleRadiansSE ",
      "CAAngleRadiansSSE",
      "CAAngleRadiansS  ",
      "CAAngleRadiansSSW",
      "CAAngleRadiansSW ",
      "CAAngleRadiansWSW",
      "CAAngleRadiansW  ",
      "CAAngleRadiansWNW",
      "CAAngleRadiansNW ",
      "CAAngleRadiansNNW",
      "CAAngleRadiansN  "
   };

   for( i = 0; i < sizeof(directions) / sizeof( directions[ 0]); i++)
   {
      rad       = directions[ i];
      direction = CAAngleRadiansToDirection( rad);
      printf( "%s : rad: %.3f direction: %s\n",
                     names[ i],
                     rad,
                     CADirectionUTF8String( direction));
   }
   return( 0);
}
