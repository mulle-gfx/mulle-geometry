#include <mulle-geometry/mulle-geometry.h>

#include <stdio.h>


int   main( int argc, char *argv[])
{
   CGPoint        a, b, c;
   CAPolarPoint   ap, bp;

   a  = CGPointMake( 18, 48);
   printf( "[%.1f, %.1f]\n", a.x, a.y);
   ap = CAPolarPointMakeWithPoint( a);
   printf( "[%.1f / %.1f]\n", ap.radius, ap.angle);
   b  = CGPointMakeWithPolarPoint( ap);
   printf( "[%.1f %.1f]\n", b.x, b.y);
   bp = CAPolarPointMakeWithPoint( b);
   printf( "[%.1f / %.1f]\n", bp.radius, bp.angle);
   c  = CGPointMakeWithPolarPoint( bp);
   printf( "[%.1f %.1f]\n", c.x, c.y);

   return( 0);
}
