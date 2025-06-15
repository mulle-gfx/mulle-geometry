#include <mulle-geometry/mulle-geometry.h>


#include <stdio.h>

#define N_VALUES     7

static float   values[ N_VALUES] =
{
   -1.0f,
   0.0f,
   1.0f,
   -INFINITY,
   +INFINITY,
   NAN,
   -NAN
};


void   CGRectPrint( CGRect rect)
{
   printf( "%g %g %g %g",
            rect.origin.x,
            rect.origin.y,
            rect.size.width,
            rect.size.height);
}


static void   test( CGRect rect1, CGRect rect2)
{
   int   flag;

   flag = CGRectEqualToRect( rect1, rect2);
   if( flag)
   {
      CGRectPrint( rect1);
      printf( " == ");
      CGRectPrint( rect2);
      printf( " = %s\n", flag ? "YES" : "NO");
   }
}


int   main( void)
{
   int   i[ 8];

   for( i[ 7] = 0; i[ 7] < N_VALUES; i[ 7]++)
      for( i[ 6] = 0; i[ 6] < N_VALUES; i[ 6]++)
         for( i[ 5] = 0; i[ 5] < N_VALUES; i[ 5]++)
            for( i[ 4] = 0; i[ 4] < N_VALUES; i[ 4]++)
               for( i[ 3] = 0; i[ 3] < N_VALUES; i[ 3]++)
                  for( i[ 2] = 0; i[ 2] < N_VALUES; i[ 2]++)
                     for( i[ 1] = 0; i[ 1] < N_VALUES; i[ 1]++)
                        for( i[ 0] = 0; i[ 0] < N_VALUES; i[ 0]++)
                        {
                           test( CGRectData( values[ i[ 0]], values[ i[ 1]], values[ i[ 2]], values[ i[ 3]]),
                                 CGRectData( values[ i[ 4]], values[ i[ 5]], values[ i[ 6]], values[ i[ 7]]));
                        }

   return( 0);
}

