#include <mulle-geometry/mulle-geometry.h>

#include <stdio.h>


int   main( int argc, char *argv[])
{
   CGPoint        a, b, c;
   CAPolarPoint   ap, bp;

   a  = CGPointMake( 18, 48);
   mulle_printf( "[%.1f, %.1f]\n", a.x, a.y);
   ap = CAPolarPointMakeWithPoint( a);
   mulle_printf( "[%.1f / %.1f]\n", ap.radius, ap.radians);
   b  = CGPointMakeWithPolarPoint( ap);
   mulle_printf( "[%.1f %.1f]\n", b.x, b.y);
   bp = CAPolarPointMakeWithPoint( b);
   mulle_printf( "[%.1f / %.1f]\n", bp.radius, bp.radians);
   c  = CGPointMakeWithPolarPoint( bp);
   mulle_printf( "[%.1f %.1f]\n", c.x, c.y);

   return( 0);
}
