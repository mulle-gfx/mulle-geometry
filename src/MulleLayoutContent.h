//
// Copyright (c) 2021 Nat!, Mulle kybernetiK
//
#ifndef mulle_layout_content_h__
#define mulle_layout_content_h__

#include "CGGeometry.h"


typedef enum
{
   MulleLayoutContentModeScaleToFill,
   MulleLayoutContentModeScaleAspectFit,
   MulleLayoutContentModeScaleAspectFill,

//   MulleLayoutContentModeRedraw,  // not supported

   MulleLayoutContentModeCenter,

   MulleLayoutContentModeTop,
   MulleLayoutContentModeBottom,
   MulleLayoutContentModeLeft,
   MulleLayoutContentModeRight,

   MulleLayoutContentModeTopLeft,
   MulleLayoutContentModeTopRight,
   MulleLayoutContentModeBottomLeft,
   MulleLayoutContentModeBottomRight
} MulleLayoutContentMode;
#define N_MulleLayoutContentModeNames 12


extern char            *MulleLayoutContentModeNames[ N_MulleLayoutContentModeNames];
char                   *MulleLayoutContentModeCStringDescription( MulleLayoutContentMode mode);
MulleLayoutContentMode  MulleLayoutContentModeParseCString( char *s);




CGFloat   MulleSizeGetAspectFitFactor( CGSize dstSize, CGSize srcSize);
CGFloat   MulleSizeGetAspectFillFactor( CGSize dstSize, CGSize srcSize);

CGRect  MulleSizeAspectLayoutInRect( CGSize size, CGRect rect, int fill);

//
// a rect of a certain size is positioned and possibly stretched into a
// given frame using contentMode. If the size is larger than the frame
// it may overlap (or shrink)
//
CGRect   MulleSizeApplyContentModeInRect( CGSize size,
                                          MulleLayoutContentMode contentMode,
                                          CGRect rect);



static inline CGPoint   MulleRectGetCenter( CGRect rect)
{
   return( CGPointMake( CGRectGetMidX( rect), CGRectGetMidY( rect)));
}


static inline CGRect   MulleLayoutRectCenterInRect( CGRect rect, CGRect bounds)
{
   CGPoint   center;

   center.x = CGRectGetMidX( bounds);
   center.y = CGRectGetMidY( bounds);

   rect.origin.x = center.x - rect.size.width / 2.0;
   rect.origin.y = center.y - rect.size.height / 2.0;

   return( rect);
}



// MulleSize_CenterInRect lesbarer ?
static inline CGPoint   MulleOriginOfSizeCenteredInRect( CGSize size, CGRect rect)
{
   CGPoint   center;

   center.x = CGRectGetMidX( rect) - size.width / 2.0;
   center.y = CGRectGetMidY( rect) - size.height / 2.0;

   return( center);
}

//void   MulleRectSplitVertically( CGRect frame, CGRect *frame1, CGRect *frame2)
//void   MulleRectSplitHorizontally( CGRect frame, CGRect *frame1, CGRect *frame2);

//
// take frame and partition into 'n' equal sized rects and spaced apart rects
// n must be >= 1. Sorta like CFRectDivide just
//


static inline CGRect   MulleRectCenterInRect( CGRect rect, CGRect otherRect)
{
   return( MulleSizeApplyContentModeInRect( rect.size, MulleLayoutContentModeCenter, otherRect));
}


// so adjust x, so that centers align
static inline CGRect   MulleRectAlignCenterInColumn( CGRect rect,
                                                     CGPoint otherCenter)
{
   rect.origin.x += otherCenter.x - CGRectGetMidX( rect);
   return( rect);
}


static inline CGRect   MulleRectAlignCenterInRow( CGRect rect,
                                                  CGPoint otherCenter)
{
   rect.origin.y += otherCenter.y - CGRectGetMidY( rect);
   return( rect);
}



#endif
