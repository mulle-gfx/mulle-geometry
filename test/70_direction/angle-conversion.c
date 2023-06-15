#include <mulle-geometry/mulle-geometry.h>

#include <stdio.h>


#define STEP   50

int   main( int argc, char *argv[])
{
   CADirection     direction;
   CAAngleHour     hour;

   for( hour = 0.0; hour <= 12.0; hour += 0.5)
   {
      direction = CAAngleRadiansToDirection( CAAngleHourToRadians( hour));

      mulle_printf( "%.1f°: [%C] -> %.1f°\n",
                    hour,
                    CADirectionArrowCharacter( direction),
                    CAAngleRadiansToHour( CADirectionToAngleRadians( direction)));
   }

   return( 0);
}