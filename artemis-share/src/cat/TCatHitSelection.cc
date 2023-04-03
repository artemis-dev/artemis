/* $Id:$ */
/**
 * @file   TCatHitSelection.cc
 * @date   Created : Oct 26, 2013 04:26:31 JST
 *   Last Modified : Oct 26, 2013 05:34:18 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TCatHitSelection.h"
#include <TClonesArray.h>
#include <TCatPulseShape.h>
using art::TCatHitSelection;

TCatHitSelection::TCatHitSelection()
{
   StringVec_t defInput; defInput.push_back("at");
   RegisterInputCollection("InputCollection","pulse array",
                           fInputColName,defInput);
}
TCatHitSelection::~TCatHitSelection()
{
   
}


void TCatHitSelection::Init(TEventCollection *col)
{
   if (!col->Get(fInputColName.front().Data())) return;
   fCatHit    = (TClonesArray**)(col->Get(fInputColName.front().Data())->GetObjectRef());
}

void TCatHitSelection::Process()
{
   if (!fCatHit) return;
   Int_t nHit = (*fCatHit)->GetEntriesFast();
   TObject arrLeft;
   TObject arrRight;
   Int_t nHitLeft = 0;
   Int_t nHitRight = 0;
   // require the four hits with non-zero charge in side pad 
   for (Int_t i=0; i!=nHit; i++) {
      TCatPulseShape *pulse = (TCatPulseShape*) (*fCatHit)->At(i);
      if (pulse->GetID() < 60 && pulse->GetCharge() > 0) {
         nHitLeft++;
      } else if (pulse->GetID() > 339 && pulse->GetCharge() > 0) {
         nHitRight++;
      }
   }
   if ((nHitLeft < 4 && nHitRight < 4) ||
      nHitLeft > 8 || nHitRight > 8) {
      SetStopEvent();
      return;
   }
   
}


