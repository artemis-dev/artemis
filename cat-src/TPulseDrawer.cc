/* $Id:$ */
/**
 * @file   TPulseDrawer.cc
 * @date   Created : 2015-03-17 19:22:44 JST
 *   Last Modified : 2015-04-23 14:33:03 JST by tokieda
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2015
 */
#include "TPulseDrawer.h"
#include "TCatPulseShape.h"

using art::TPulseDrawer;

TPulseDrawer::TPulseDrawer()
{
}

TPulseDrawer::TPulseDrawer(const char* name, const char* title, Int_t wtopx, Int_t wtopy, Int_t ww, Int_t wh)
   : TGraphDrawer(name,title,wtopx,wtopy,ww,wh), fViewID(-1)
{
}

TPulseDrawer::~TPulseDrawer()
{
}

void TPulseDrawer::SetPulse(TObject &obj)
{
   TCatPulseShape *pulse = (TCatPulseShape*) &obj;
   if (fViewID!=-1) if (pulse->GetID()!=fViewID) return;
   std::vector<Float_t>  &inSample  = pulse->GetSample();
   int num = inSample.size();
   fhGraph->Set(num-1);
   for (Int_t i=0; i!=num; i++) {
      fhGraph->SetPoint(i,i,inSample[i]);
   }
}
