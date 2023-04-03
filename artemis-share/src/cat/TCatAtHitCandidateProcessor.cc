/* $Id:$ */
/**
 * @file   TCatAtHitCandidateProcessor.cc
 * @date   Created : Oct 26, 2013 04:26:22 JST
 *   Last Modified : Oct 26, 2013 04:41:58 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TCatAtHitCandidateProcessor.h"

using namespace art;

TCatAtHitCandidateProcessor::TCatAtHitCandidateProcessor()
{
   StringVec_t defInput; defInput.push_back("atsum");
   RegisterInputCollection("InputCollection","CAT hit data array name (TClonesArray)",
                           fInputColName,defInput);
   RegisterOutputCollection("OutputCollection","output collection name",
                            fOutputColName,TString("athit"));
}
TCatAtHitCandidateProcessor::~TCatAtHitCandidateProcessor()
{
   if (fPosition)  delete fPosition;
}

void art::TCatAtHitCandidateProcessor::Init(art::TEventCollection *col)
{
   TEventObject *obj = NULL;
   if (!(obj = col->Get(fInputColName.front()))) {
      printf("<TCatAtHitCandidateProcessor::Init> no such input %s\n",fInputColName.front().Data());
      return;
   }
   fCatHits = (TClonesArray**) obj->GetObjectRef();
   fPosition = new TClonesArray("TCatPosition");
   col->Add(fOutputColName,fPosition,fOutputIsTransparent);
}

void art::TCatAtHitCandidateProcessor::Process()
{
   if (!fCatHits) return;
   fPosition->Clear("C");
   Int_t nHits = (*fCatHits)->GetEntriesFast();
   for (Int_t iHit1 = 0; iHit1!=nHits; iHit1++) {
      TCatPulseShape *pulse1 = (TCatPulseShape*)fCatHits->At(iHit1);
      if (!pulse1.IsGood()) continue;
      for (Int_t iHit2 = iHit1+1; iHit2!=nHits; iHit2++) {
         TCatPulseShape *pulse2 = (TCatPulseShape*)fCatHits->At(iHit2);
         if (!pulse2.IsGood()) continue;
         if ((pulse1.GetID() < 60 && pulse2.GetID() < 60) ||
             (pulse1.GetID() > 339 && pulse2.GetID() > 339)) {
            if (abs(pulse2.GetID()-pulse1.GetID()) == 10) {
               // calculate position 
            }
         } else if (pulse1.GetID() > 70
         

      }
   }
}
