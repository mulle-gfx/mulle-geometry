#include <mulle-geometry/mulle-geometry.h>

#include <stdio.h>


#define STEP   50

int   main( int argc, char *argv[])
{
   CADirection     direction;
   CGFloat     hour;

   for( hour = 0.5f; hour <= 12.01; hour += 0.5)
   {
      if( hour > 12.0f)
         hour = 12.0f;
      direction = CAAngleToDirection( CAHourToAngle( hour));

      mulle_printf( "%.1fh: [%C] -> %.1fh\n",
                    hour,
                    CADirectionArrowCharacter( direction),
                    CAAngleToHour( CADirectionToAngle( direction)));
   }

   return( 0);
}