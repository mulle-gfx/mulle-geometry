#include <mulle-geometry/mulle-geometry.h>

#include <stdio.h>


int   main( int argc, char *argv[])
{
   CAAngle        ang;
   CADirection    direction;
   unsigned int   i;
   auto CAAngle   directions[] =
   {
      CAAngleNNE,
      CAAngleNE ,
      CAAngleENE,
      CAAngleE  ,
      CAAngleESE,
      CAAngleSE ,
      CAAngleSSE,
      CAAngleS  ,
      CAAngleSSW,
      CAAngleSW ,
      CAAngleWSW,
      CAAngleW  ,
      CAAngleWNW,
      CAAngleNW ,
      CAAngleNNW,
      CAAngleN
   };
   static char   *names[] =
   {
      "CAAngleNNE",
      "CAAngleNE ",
      "CAAngleENE",
      "CAAngleE  ",
      "CAAngleESE",
      "CAAngleSE ",
      "CAAngleSSE",
      "CAAngleS  ",
      "CAAngleSSW",
      "CAAngleSW ",
      "CAAngleWSW",
      "CAAngleW  ",
      "CAAngleWNW",
      "CAAngleNW ",
      "CAAngleNNW",
      "CAAngleN  "
   };

   for( i = 0; i < sizeof(directions) / sizeof( directions[ 0]); i++)
   {
      ang       = directions[ i];
      direction = CAAngleToDirection( ang);
      mulle_printf( "%s : rad: %.3f (%.3f) direction: %s\n",
                     names[ i],
                     ang,
                     CAAngleToHour( ang),
                     CADirectionUTF8String( direction));
   }
   return( 0);
}
