/* $Id:$ */
/**
 * @file   TCatParameterTable.cc
 * @date   Created : Oct 07, 2012 11:07:08 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatParameterTable.h"

#include <TMath.h>

TCatParameterTable::TCatParameterTable()
{
}
TCatParameterTable::~TCatParameterTable()
{
}

TCatParameterTable::TCatParameterTable(const char* filename, const char *format, Option_t *opt, Bool_t xIsFirst)
   : TGraph(filename,format,opt), fFilename(filename), fFormat(format), fXIsFirst(xIsFirst),
     fSplineXY(NULL), fSplineYX(NULL)
{
   if (!xIsFirst) {
      // shoud invert x and y
      for (Int_t i=0; i<fNpoints; i++) {
         Double_t temp = fX[i];
         fX[i] = fY[i];
         fY[i] = temp;
      }
   }
   
   BuildSpline();
}

void TCatParameterTable::ScaleY(const Double_t &scale)
{
   fScaleY = scale;
   for (Int_t ix = 0; ix != fNpoints; ix++) {
      fY[ix] *= fScaleY;
   }
   BuildSpline();
}

void TCatParameterTable::BuildSpline()
{
   if (fSplineXY) delete fSplineXY;
   if (fSplineYX) delete fSplineYX;

   // sorting to make spline function
   std::vector<Double_t> xsort(fNpoints);
   std::vector<Double_t> ysort(fNpoints);
   std::vector<Int_t> indxsort(fNpoints);
   TMath::Sort(fNpoints, fX, &indxsort[0], false);
   for (Int_t i = 0; i < fNpoints; ++i) {
      xsort[i] = fX[ indxsort[i] ];
      ysort[i] = fY[ indxsort[i] ];
   }

   // spline interpolation creating a new spline
   fSplineXY = new TSpline3("", &xsort[0], &ysort[0], fNpoints);
   fSplineYX = new TSpline3("", &ysort[0], &xsort[0], fNpoints);
   
}
