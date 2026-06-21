#include <mulle-geometry/mulle-geometry.h>

#include <math.h>


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


static void    test( float a, float b)
{
   mulle_printf( "%g + %g = %g\n",  a, b, a + b);
   mulle_printf( "%g - %g = %g\n",  a, b, a - b);
   mulle_printf( "%g * %g = %g\n",  a, b, a * b);
   mulle_printf( "%g == %g = %s\n", a, b, a == b ? "YES" : "NO");
   mulle_printf( "%g > %g = %s\n",  a, b, a > b ? "YES" : "NO");
   mulle_printf( "%g < %g = %s\n",  a, b, a < b ? "YES" : "NO");
}

int   main()
{
   int   i, j;

   for( j = 0; j < N_VALUES; j++)
   {
      for( i = 0; i < N_VALUES; i++)
         test( values[ j], values[ i]);
      mulle_printf( "\n");
   }
   return( 0);
}
