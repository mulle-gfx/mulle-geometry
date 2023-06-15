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
                                          CADirectionBoxCharacter( direction));
      x = direction;
      for( i = 0; i < 4; i++)
      {
         y = CADirectionTurn90ClockWise( x);
         mulle_printf( " [%C]",CADirectionBoxCharacter( y));
         x = y;
      }
      mulle_printf( "\n");
   }

   return( 0);
}