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
      CGWindDirectionNNE,
      CGWindDirectionNE ,
      CGWindDirectionENE,
      CGWindDirectionE  ,
      CGWindDirectionESE,
      CGWindDirectionSE ,
      CGWindDirectionSSE,
      CGWindDirectionS  ,
      CGWindDirectionSSW,
      CGWindDirectionSW ,
      CGWindDirectionWSW,
      CGWindDirectionW  ,
      CGWindDirectionWNW,
      CGWindDirectionNW ,
      CGWindDirectionNNW,
      CGWindDirectionN
   };
   static char   *names[] =
   {
      "CGWindDirectionNNE",
      "CGWindDirectionNE ",
      "CGWindDirectionENE",
      "CGWindDirectionE  ",
      "CGWindDirectionESE",
      "CGWindDirectionSE ",
      "CGWindDirectionSSE",
      "CGWindDirectionS  ",
      "CGWindDirectionSSW",
      "CGWindDirectionSW ",
      "CGWindDirectionWSW",
      "CGWindDirectionW  ",
      "CGWindDirectionWNW",
      "CGWindDirectionNW ",
      "CGWindDirectionNNW",
      "CGWindDirectionN  "
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
