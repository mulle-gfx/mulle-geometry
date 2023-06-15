#include <mulle-geometry/mulle-geometry.h>

#include <stdio.h>


#define STEP   50

int   main( int argc, char *argv[])
{
   CADirection    direction;
   CADirection    x;
   CADirection    y;
   int            i;

   for( direction = 0; direction < 16; direction++)
   {
      mulle_printf( "direction %s: [%C]", CADirectionUTF8String( direction),
                                          CADirectionArrowCharacter( direction));
      x = direction;
      for( i = 0; i < 8; i++)
      {
         y = CADirectionTurn45ClockWise( x);
         mulle_printf( " [%C]", CADirectionArrowCharacter( y));
         x = y;
      }
      mulle_printf( "\n");
   }

   return( 0);
}