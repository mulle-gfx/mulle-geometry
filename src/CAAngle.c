//
// Copyright (c) <|YEAR|> Nat!, Mulle kybernetiK
//
#include "CAAngle.h"

#include "include-private.h"


// ein bischen gefummelt (0.1), damit die "winde" hübscher passen
// das ganze ist eh nur pi mal daumen
#define DEVIATION  (3.0f / 4.1f)
//#define DEVIATION 0



// ein bischen gefummelt (0.1), damit die "winde" hübscher passen
// das ganze ist eh nur pi mal daumen
#define DEVIATION  (3.0f / 4.1f)
//#define DEVIATION 0


CADirection   CARotationToDirection( CARotation rotation)
{
   CGFloat       h;
   CADirection   dir;

   h = CARotationToHour( rotation);
   if( h >= 12.0f)
      h = 0.0f;

   // split into top and bottom sector,
   dir = 0;
   if( h > (9.0 + DEVIATION) || h < (3.0 - DEVIATION))
      dir = CADirectionTop;
   else
      if( h < (9.0 - DEVIATION) && h > (3.0 + DEVIATION))
         dir = CADirectionBottom;

   // then split into right and left sector
   if( h > (0.0 + DEVIATION) && h < (6.0 - DEVIATION))
      dir |= CADirectionRight;
   else
      if( h < (12.0 - DEVIATION) && h > (6.0 + DEVIATION))
         dir |= CADirectionLeft;

   return( dir);
}


CGFloat   CARadiansNormalize( CGFloat radians)
{
   return( CARadiansNormalizeInline( radians));
}
