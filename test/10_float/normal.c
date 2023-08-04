#include <mulle-geometry/mulle-geometry.h>

#include <stdlib.h>

#define N_BUCKETS 100


int   main()
{
   long      i;
   long      hi, lo;
   int       j;
   CGFloat   x;
   double    total;
   double    buckets[ N_BUCKETS];
   CGFloat   mean;

   /* check that we get only values from 0.0 to 1.0
      and that the mean is about 0.5
   */
   total = 0;
   memset( buckets, 0, sizeof( buckets));

   for( i = 0; i < 1000000; i++)
   {
      x      = CGFloatMakeNormalDistribution();
      total += x;

      if( x < 0.0)
         return(1);
      if( x > 1.0)
         return( 2);

      j = (int) floor( x * N_BUCKETS);
      buckets[ j]++;
   }
   mean = total / i;
   if( mean > 0.55 || mean < 0.45)
   {
      fprintf( stderr, "mean: %f\n", mean);
      return( 3);
   }
   for( j = 0; j < N_BUCKETS; j++)
      fprintf( stderr, "[%d]=%f\n", j, buckets[ j]);
   return( 0);
}
