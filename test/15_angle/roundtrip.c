#include <mulle-geometry/mulle-geometry.h>
#include <stdio.h>
#include <math.h>


static void   test_angle_rotation_roundtrip( void)
{
   int         i;
   CAAngle     angle;
   CARotation  rotation;
   CAAngle     back;

   for( i = 0; i < 16; i++)
   {
      angle    = (CAAngle) (i / 16.0f) * CA_2xPI;
      rotation = CAAngleToRotation( angle);
      back     = CARotationToAngle( rotation);
      if( fabsf( back - angle) > CA_EPSILON)
         mulle_printf( "FAIL angle_rotation[%d]: %.6f -> %.6f -> %.6f\n",
                        i, angle, rotation, back);
   }
   mulle_printf( "angle<->rotation: ok\n");
}


static void   test_angle_degrees_roundtrip( void)
{
   int       i;
   CGFloat   degrees;
   CAAngle   angle;
   CGFloat   back;

   for( i = 0; i < 360; i += 15)
   {
      degrees = (CGFloat) i;
      angle   = CADegreesToAngle( degrees);
      back    = CAAngleToDegrees( angle);
      if( fabsf( back - degrees) > 0.01f)
         mulle_printf( "FAIL degrees[%d]: %.2f -> %.6f -> %.2f\n",
                        i, degrees, angle, back);
   }
   mulle_printf( "degrees<->angle: ok\n");
}


static void   test_angle_radians_roundtrip( void)
{
   int       i;
   CAAngle   angle;
   CGFloat   radians;
   CAAngle   back;

   for( i = 0; i < 16; i++)
   {
      angle   = (CAAngle) (i / 16.0f) * CA_2xPI;
      radians = CAAngleToRadians( angle);
      back    = CARadiansToAngle( radians);
      if( fabsf( back - angle) > CA_EPSILON)
         mulle_printf( "FAIL radians[%d]: %.6f -> %.6f -> %.6f\n",
                        i, angle, radians, back);
   }
   mulle_printf( "angle<->radians: ok\n");
}


static void   test_rotation_degrees_roundtrip( void)
{
   int         i;
   CARotation  rotation;
   CGFloat     degrees;
   CARotation  back;

   for( i = 0; i < 12; i++)
   {
      rotation = (CARotation) i / 12.0f;
      degrees  = CARotationToDegrees( rotation);
      back     = CADegreesToRotation( degrees);
      if( fabsf( back - rotation) > CA_EPSILON)
         mulle_printf( "FAIL rot_degrees[%d]: %.6f -> %.2f -> %.6f\n",
                        i, rotation, degrees, back);
   }
   mulle_printf( "rotation<->degrees: ok\n");
}


static void   test_hour_rotation_roundtrip( void)
{
   int         i;
   CGFloat     hour;
   CARotation  rotation;
   CGFloat     back;

   for( i = 1; i <= 12; i++)
   {
      hour     = (CGFloat) i;
      rotation = CAHourToRotation( hour);
      back     = CARotationToHour( rotation);
      if( fabsf( back - hour) > 0.01f)
         mulle_printf( "FAIL hour[%d]: %.1f -> %.6f -> %.1f\n",
                        i, hour, rotation, back);
   }
   mulle_printf( "hour<->rotation: ok\n");
}


static void   test_vector_angle_roundtrip( void)
{
   int        i;
   CAAngle    angle;
   CGVector   v;
   CAAngle    back;

   for( i = 0; i < 16; i++)
   {
      angle = (CAAngle) (i / 16.0f) * CA_2xPI;
      v     = CGVectorMakeWithAngle( angle);
      back  = CGVectorGetAngle( v);
      if( fabsf( back - angle) > CA_EPSILON)
         mulle_printf( "FAIL vec_angle[%d]: %.6f -> (%.4f,%.4f) -> %.6f\n",
                        i, angle, v.dx, v.dy, back);
   }
   mulle_printf( "vector<->angle: ok\n");
}


static void   test_vector_rotation_roundtrip( void)
{
   int         i;
   CARotation  rotation;
   CGVector    v;
   CARotation  back;

   for( i = 0; i < 12; i++)
   {
      rotation = (CARotation) i / 12.0f;
      v        = CGVectorMakeWithRotation( rotation);
      back     = CGVectorGetRotation( v);
      if( fabsf( back - rotation) > CA_EPSILON)
         mulle_printf( "FAIL vec_rotation[%d]: %.6f -> (%.4f,%.4f) -> %.6f\n",
                        i, rotation, v.dx, v.dy, back);
   }
   mulle_printf( "vector<->rotation: ok\n");
}


static void   test_polar_point_roundtrip( void)
{
   int            i;
   CARotation     rotation;
   CAPolarPoint   polar;
   CGPoint        cart;
   CAPolarPoint   back;
   CAPolarPoint   expected;

   for( i = 0; i < 12; i++)
   {
      rotation = (CARotation) i / 12.0f;
      polar    = CAPolarPointMakeWithRotation( 100.0f, rotation);
      expected = CAPolarPointStandardize( polar);
      cart     = CGPointMakeWithPolarPoint( polar);
      back     = CAPolarPointMakeWithPoint( cart);
      back     = CAPolarPointStandardize( back);
      if( fabsf( back.radius - expected.radius) > 0.01f ||
          fabsf( back.radians - expected.radians) > CA_EPSILON)
         mulle_printf( "FAIL polar[%d]: rot=%.4f -> (%.2f,%.2f) -> (r=%.2f,a=%.6f) want (r=%.2f,a=%.6f)\n",
                        i, rotation, cart.x, cart.y,
                        back.radius, back.radians,
                        expected.radius, expected.radians);
   }
   mulle_printf( "polar<->cartesian: ok\n");
}


int   main( void)
{
   test_angle_rotation_roundtrip();
   test_angle_degrees_roundtrip();
   test_angle_radians_roundtrip();
   test_rotation_degrees_roundtrip();
   test_hour_rotation_roundtrip();
   test_vector_angle_roundtrip();
   test_vector_rotation_roundtrip();
   test_polar_point_roundtrip();
   return( 0);
}
