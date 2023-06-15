#include <mulle-geometry/mulle-geometry.h>
#include <mulle-fprintf/mulle-fprintf.h>


int   main( int argc, char *argv[])
{
   CADirection    direction;

   for( direction = 0; direction < 16; direction++)
   {
      mulle_printf( "direction %s: [%C]\n", CADirectionUTF8String( direction),
                                            CADirectionArrowCharacter( direction));
   }

   return( 0);
}