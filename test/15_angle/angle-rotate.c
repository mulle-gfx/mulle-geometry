#include <mulle-geometry/mulle-geometry.h>

#include <stdio.h>



//
// check that CAAngleGetRotationTowardsAngle works fine
//
int   main( int argc, char *argv[])
{
   CAAngle               angle1;
   CAAngle               angle2;
   CAAngle               angle3;
   CARotation            rot1;
   CARotation            rot2;
   CARotation            diff;
   CARotationDirection   dir;
   CGFloat               hour1;
   CGFloat               hour2;
   CGFloat               hour3;

   for( rot1 = -1.0; rot1 <= 1.01; rot1 += 0.25)
   {
      angle1 = CARotationToAngle( rot1);
      for( rot2 = -1.0; rot2 <= 1.01; rot2 += 0.25)
      {
         for( dir = CARotationDirectionClockWise; dir >= CARotationDirectionCounterClockWise; dir /= 2)
         {
            angle2 = CARotationToAngle( rot2);

            diff   = CAAngleGetRotationTowardsAngle( angle1, angle2, dir);
            angle3 = CAAngleRotate( angle1, diff);

            hour1  = CAAngleToHour( angle1);
            hour2  = CAAngleToHour( angle2);
            hour3  = CAAngleToHour( angle3);

            mulle_printf( "hour1: %.3f to hour2: %.3f dir=%s -> rot: %.3f = hour3: %.3f\n",
                          hour1,
                          hour2,
                          CARotationDirectionUTF8String( dir),
                          diff,
                          hour3);
         }
      }
   }
   return( 0);
}