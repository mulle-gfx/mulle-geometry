//
// Copyright (c) 2021 Nat!, Mulle kybernetiK
//
#include "MulleLayoutContent.h"

#include "include-private.h"

#include <stdlib.h>
#include <string.h>


//
// +++++
// +++++  ++++
//        ++++
//
// this will produce the amount to scale srcSize in both dimensions
// so that everything remains visible in the same aspect
//
CGFloat   MulleSizeGetAspectFitFactor( CGSize dstSize, CGSize srcSize)
{
   CGFloat   srcAspectScale;
   CGFloat   dstAspectScale;

   srcAspectScale = srcSize.width / srcSize.height;
   dstAspectScale = dstSize.width / dstSize.height;

   if( srcAspectScale < dstAspectScale)
      return( dstSize.height / srcSize.height);
   return( dstSize.width / srcSize.width);
}


CGFloat   MulleSizeGetAspectFillFactor( CGSize dstSize, CGSize srcSize)
{
   CGFloat   srcAspectScale;
   CGFloat   dstAspectScale;

   srcAspectScale = srcSize.width / srcSize.height;
   dstAspectScale = dstSize.width / dstSize.height;

   if( srcAspectScale > dstAspectScale)
      return( dstSize.height / srcSize.height);
   return( dstSize.width / srcSize.width);
}


CGRect  MulleSizeAspectLayoutInRect( CGSize size, CGRect rect, int fill)
{
   CGFloat  aspectScale;
   CGRect   result;

   if( size.height == 0.0 || size.width == 0.0  || rect.size.height == 0.0)
      return( CGRectZero);

   if( fill)
      aspectScale = MulleSizeGetAspectFillFactor( rect.size, size);
   else
      aspectScale = MulleSizeGetAspectFitFactor( rect.size, size);

   result.size.width  = size.width * aspectScale;
   result.size.height = size.height * aspectScale;
   // now center
   result.origin.x = CGRectGetMidX( rect) - result.size.width / 2.0;
   result.origin.y = CGRectGetMidY( rect) - result.size.height / 2.0;
   return( result);
}

CGRect   MulleSizeApplyContentModeInRect( CGSize size,
                                          MulleLayoutContentMode contentMode,
                                          CGRect rect)
{
   CGRect   result;
   CGFloat  aspectScale;

   switch( contentMode)
   {
   case MulleLayoutContentModeScaleToFill :
      return( rect);

   case MulleLayoutContentModeScaleAspectFit :
      return( MulleSizeAspectLayoutInRect( size, rect, 0));

   case MulleLayoutContentModeScaleAspectFill :
      return( MulleSizeAspectLayoutInRect( size, rect, 1));

   case MulleLayoutContentModeCenter :
      result.origin.x = CGRectGetMidX( rect) - size.width / 2.0;
      result.origin.y = CGRectGetMidY( rect) - size.height / 2.0;
      result.size     = size;
      return( result);

   case MulleLayoutContentModeTop :
      result.origin.x = CGRectGetMidX( rect) - size.width / 2.0;
      result.origin.y = rect.origin.y;
      result.size     = size;
      return( result);

   case MulleLayoutContentModeBottom :
      result.origin.x = CGRectGetMidX( rect) - size.width / 2.0;
      result.origin.y = CGRectGetMaxY( rect) - size.height;
      result.size     = size;
      return( result);

   case MulleLayoutContentModeLeft :
      result.origin.x = rect.origin.x;
      result.origin.y = CGRectGetMidY( rect) - size.height / 2.0;
      result.size     = size;
      return( result);

   case MulleLayoutContentModeRight :
      result.origin.x = CGRectGetMaxX( rect) - size.width;
      result.origin.y = CGRectGetMidY( rect) - size.height / 2.0;
      result.size     = size;
      return( result);

   case MulleLayoutContentModeTopLeft :
      result.origin = rect.origin;
      result.size   = size;
      return( result);

   case MulleLayoutContentModeTopRight :
      result.origin.x = CGRectGetMaxX( rect) - size.width;
      result.origin.y = rect.origin.y;
      result.size     = size;
      return( result);

   case MulleLayoutContentModeBottomLeft :
      result.origin.x = rect.origin.x;
      result.origin.y = CGRectGetMaxY( rect) - size.height;
      result.size     = size;
      return( result);

   case MulleLayoutContentModeBottomRight :
      result.origin.x = CGRectGetMaxX( rect) - size.width;
      result.origin.y = CGRectGetMaxY( rect) - size.height;
      result.size     = size;
      return( result);
   }
   abort();
}


char  *MulleLayoutContentModeNames[] =
{
   "ScaleToFill",
   "ScaleAspectFit",
   "ScaleAspectFill",
//   "MulleLayoutContentModeRedraw",
   "Center",
   "Top",
   "Bottom",
   "Left",
   "Right",
   "TopLeft",
   "TopRight",
   "BottomLeft",
   "BottomRight",
};


char  *MulleLayoutContentModeCStringDescription( MulleLayoutContentMode mode)
{
   assert( mode >= MulleLayoutContentModeScaleToFill && mode <= MulleLayoutContentModeBottomRight);
   return( MulleLayoutContentModeNames[ mode]);
}


MulleLayoutContentMode  MulleLayoutContentModeParseCString( char *s)
{
   char   **sentinel;
   char   **p;
   char   *prefix;

   prefix = strstr( s, "ContentMode");
   if( prefix)
   {
      s = &s[ prefix - s];
      s = &s[ strlen( "ContentMode")];
   }

   p        = MulleLayoutContentModeNames;
   sentinel = &p[ N_MulleLayoutContentModeNames];
   while( p < sentinel)
   {
      if( ! strcmp( s, *p))
         return( (int) (p - MulleLayoutContentModeNames));
      ++p;
   }
   return( -1);
}
