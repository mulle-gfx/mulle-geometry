#include <mulle-geometry/mulle-geometry.h>

#include <stdio.h>


int   main()
{
   int       i;
   CGFloat   x;
   CGFloat   y;

   for( i = -20; i <= 20; i++)
   {
      x = i / 10.0f;
      y = CGFloatModToUnity( x);
      printf( "CGFloatModToUnity( %g) = %g\n", x, y);
   }
   return( 0);
}
