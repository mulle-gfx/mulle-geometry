#include <mulle-geometry/mulle-geometry.h>

#include <stdio.h>


int   main( void)
{
   CACircle   a;
   CACircle   b;
   CGVector   normal;
   CGFloat    dist;
   CGFloat    overlap;

   // overlapping circles
   a = CACircleMake( 0, 0, 50);
   b = CACircleMake( 60, 0, 50);

   dist = CACircleDistanceToCircle( a, b);
   printf( "distance: %.1f\n", dist);
   printf( "intersects: %d\n", CACircleIntersectsCircle( a, b));

   overlap = CACircleOverlapWithCircle( a, b);
   printf( "overlap: %.1f\n", overlap);

   normal = CACircleCollisionNormal( a, b);
   printf( "normal: %.1f,%.1f\n", normal.dx, normal.dy);

   // non-overlapping circles
   b = CACircleMake( 200, 0, 50);

   printf( "intersects far: %d\n", CACircleIntersectsCircle( a, b));
   printf( "overlap far: %.1f\n", CACircleOverlapWithCircle( a, b));

   // touching circles (edge case)
   b = CACircleMake( 100, 0, 50);

   printf( "intersects touch: %d\n", CACircleIntersectsCircle( a, b));
   printf( "overlap touch: %.1f\n", CACircleOverlapWithCircle( a, b));

   // coincident circles
   b = CACircleMake( 0, 0, 50);

   normal = CACircleCollisionNormal( a, b);
   printf( "normal coincident: %.1f,%.1f\n", normal.dx, normal.dy);

   // diagonal overlap
   a = CACircleMake( 0, 0, 50);
   b = CACircleMake( 40, 30, 50);

   dist = CACircleDistanceToCircle( a, b);
   printf( "distance diagonal: %.1f\n", dist);
   printf( "intersects diagonal: %d\n", CACircleIntersectsCircle( a, b));

   normal = CACircleCollisionNormal( a, b);
   printf( "normal diagonal: %.1f,%.1f\n", normal.dx, normal.dy);

   return( 0);
}
