/**
 * @file   TRangeTableHelper.cc
 * @brief  helper for range table
 *
 * @date   Created       : 2016-07-21 06:39:06 JST
 *         Last Modified : 2020-04-29 17:41:06 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#include "TRangeTableHelper.h"
#include <TProcessor.h>
#include <TGraph.h>
#include <TArtSystemOfUnit.h>
#include <TSpline.h>


using art::TRangeTableHelper;

ClassImp(TRangeTableHelper)

using TArtSystemOfUnit::um;


TRangeTableHelper::TRangeTableHelper()
: TNamed(), fDE2R(NULL), fR2DE(NULL), fR2E(NULL), fE2R(NULL)
{
}

TRangeTableHelper::TRangeTableHelper(const char *name, const char *title)
   : TNamed(name,title), fDE2R(NULL), fR2DE(NULL), fR2E(NULL), fE2R(NULL)
{
}

TRangeTableHelper::~TRangeTableHelper()
{
}

TRangeTableHelper::TRangeTableHelper(const TRangeTableHelper& rhs)
{
}

TRangeTableHelper& TRangeTableHelper::operator=(const TRangeTableHelper& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}


void TRangeTableHelper::Init(TEventCollection *col)
{
   fDE2R = new TGraph(fFileName,"%*lg %lg %*lg %lg");
   fR2DE = new TGraph(fDE2R->GetN(),fDE2R->GetY(),fDE2R->GetX());
   fE2R  = new TGraph(fFileName,"%lg %*lg %*lg %lg");
   fR2E  = new TGraph(fE2R->GetN(),fE2R->GetY(),fE2R->GetX());

   const int offset = 10;
   const int size = fDE2R->GetN() + offset;
   std::vector<double> range (size,0.);
   std::vector<double> energy(size,0.);
   std::vector<double> deltae(size,0.);
   for (int i = 0; i < offset; ++i) {
      double rangeval = (- offset + i + 1)*um;
      printf("%f\n",rangeval);
      
      range[i] = rangeval;
   }
   for (int i = 0; i < fDE2R->GetN(); ++i) {
      range[i+offset] = fDE2R->GetY()[i];
      energy[i+offset] = fE2R->GetX()[i];
      deltae[i+offset] = fDE2R->GetX()[i];
   }
#if 0
   fSpDE2R = new TSpline3("spDE2R",fDE2R->GetX(),fDE2R->GetY(),fDE2R->GetN());
   fSpE2R = new TSpline3("spDE2R",fE2R->GetX(),fE2R->GetY(),fE2R->GetN());
   fSpR2E = new TSpline3("spDE2R",fR2E->GetX(),fR2E->GetY(),fR2E->GetN());
   fSpR2DE = new TSpline3("spDE2R",fR2DE->GetX(),fR2DE->GetY(),fR2DE->GetN());
#else
   fSpDE2R = new TSpline3("spDE2R",&(deltae[0]),&(range[0]),size);
   fSpE2R = new TSpline3("spDE2R",&(energy[0]),&(range[0]),size);
   fSpR2E = new TSpline3("spDE2R",&(range[0]),&(energy[0]),size);
   fSpR2DE = new TSpline3("spDE2R",&(range[0]),&(deltae[0]),size);
#endif   
   if (col) {
      col->AddInfo(fRangeTableName,this);
   }
}

void TRangeTableHelper::Register(TProcessor *proc)
{
   proc->RegisterProcessorParameter("RangeTableFile","Name of range table file",fFileName,TString("path/to/file"));
   proc->RegisterProcessorParameter("RangeTable","name of range table",fRangeTableName,TString("rangeTable"));
}

