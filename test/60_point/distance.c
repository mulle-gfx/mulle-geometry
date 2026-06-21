#include <mulle-geometry/mulle-geometry.h>

#include <stdio.h>


#define STEP   50

int   main( int argc, char *argv[])
{
   CGPoint        a, b;
   CGFloat        r;
   int            x1, y1;
   int            x2, y2;

   for( x1 = -100; x1 <= 100; x1 += STEP)
      for( x2 = -100; x2 <= 100; x2 += STEP)
         for( y1 = -100; y1 <= 100; y1 += STEP)
            for( y2 = -100; y2 <= 100; y2 += STEP)
            {
               a = CGPointMake( x1, y1);
               b = CGPointMake( x2, y2);
               r = CGPointDistanceToPoint( a, b);
               mulle_printf( "[%.1f %.1f] to [%.1f %.1f] = %.1f\n",
                        a.x, a.y,
                        b.x, b.y,
                        r);
            }

   return( 0);
}