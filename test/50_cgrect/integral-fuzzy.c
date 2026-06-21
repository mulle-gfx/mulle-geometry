#include <mulle-geometry/mulle-geometry.h>


#include <stdio.h>


void   CGRectPrint( CGRect rect)
{
   mulle_printf( "%.3f %.3f %.3f %.3f",
            rect.origin.x,
            rect.origin.y,
            rect.size.width,
            rect.size.height);
}


static void   test( CGRect rect)
{
   CGRect   rect2;

   rect2 = CGRectIntegralFuzzy( rect, 0.01);

   mulle_printf( "[r=");
   CGRectPrint( rect);
   mulle_printf( " i=");
   CGRectPrint( rect2);
   mulle_printf( "]");
   mulle_printf( "\n");
}


static float   plus_values[] =
{
   +0,0
   +0.001,
   +0.1,
   +0.5,
   +0.9,
   +0.999,
   +1.0
};


static float   minus_values[] =
{
   -1.0
   -0.999,
   -0.9,
   -0.5,
   -0.1,
   -0.001,
};


static float   values[] =
{
   -1.0
   -0.999,
   -0.9,
   -0.5,
   -0.1,
   -0.001,
   +0,0
   +0.001,
   +0.1,
   +0.5,
   +0.9,
   +0.999,
   +1.0
};



int   main( void)
{
   unsigned int   x_i;
   unsigned int   y_i;
   unsigned int   w_i;
   unsigned int   h_i;

   for( h_i = 0; h_i < sizeof( values) / sizeof( values[ 0]); h_i++)
   {
      for( w_i = 0; w_i < sizeof( values) / sizeof( values[ 0]); w_i++)
      {
         for( y_i = 0; y_i < sizeof( minus_values) / sizeof( minus_values[ 0]); y_i++)
         {
            for( x_i = 0; x_i < sizeof( plus_values) / sizeof( plus_values[ 0]); x_i++)
            {
               test( CGRectMake( plus_values[ x_i], minus_values[ y_i], values[ w_i], values[ h_i]));
            }
            mulle_printf( "\n");
         }
         mulle_printf( "\n");
      }
      mulle_printf( "\n");
   }
   return( 0);
}

