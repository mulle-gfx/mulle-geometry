#include <mulle-geometry/mulle-geometry.h>


#include <stdio.h>


void   CGPointPrint( CGPoint point)
{
   printf( "%.2f %.2f", point.x, point.y);
}


void   CGRectPrint( CGRect rect)
{
   printf( "%.2f %.2f %.2f %.2f",
            rect.origin.x,
            rect.origin.y,
            rect.size.width,
            rect.size.height);
}



static void   test( CGRect frame, CGRect bounds, CGPoint location)
{
   CGPoint             contentLocation;
   CGAffineTransform   xform;
   CGAffineTransform   identity;
   CGPoint             reverse;

   xform           = CGAffineTransformMakeProjection( frame, bounds);
   contentLocation = CGPointApplyAffineTransformInline( location, xform);

   xform           = CGAffineTransformMakeInverseProjection( frame, bounds);
   reverse         = CGPointApplyAffineTransformInline( contentLocation, xform);

   printf( "[f=");
   CGRectPrint( frame);
   printf( " b=");
   CGRectPrint( bounds);
   printf( "] ");
   CGPointPrint( location);
   printf( " -> ");
   CGPointPrint( contentLocation);
   printf( " -> ");
   CGPointPrint( reverse);
   printf( "\n");
}


int   main( void)
{
   test( CGRectMake( 0, 0, 4, 4), CGRectMake( 0, 0, 4, 4), CGPointMake( 0, 0));
   test( CGRectMake( 0, 0, 4, 4), CGRectMake( 0, 0, 4, 4), CGPointMake( 1, 1));
   test( CGRectMake( 0, 0, 4, 4), CGRectMake( 0, 0, 4, 4), CGPointMake( 3, 3));

   printf("\n");

   // -1 bounds.x means rect is shifted to left, getting hit with a 0,
   // it will actually be a 1. Add frame.x, make it 2
   test( CGRectMake( 1, 0, 4, 4), CGRectMake( -1, 0, 4, 4), CGPointMake( 0, 0));

   printf("\n");

   // this is scaling up so expect 2
   test( CGRectMake( 0, 0, 4, 4), CGRectMake( 0, 0, 2, 2), CGPointMake( 1, 1));

   printf("\n");

   // we hit -1 which is scaled up, so -2, therefore we hit 2 (as two above)
   test( CGRectMake( 0, 0, 4, 4), CGRectMake( -1, 0, 2, 2), CGPointMake( 0, 0));

   printf("\n");

   // we hit -1 which is scaled up, so -2, therefore we hit 2 (as two above)
   // subtract frame from it though -> 1
   test( CGRectMake( -1, 0, 4, 4), CGRectMake( -1, 0, 2, 2), CGPointMake( 0, 0));

   return( 0);
}

