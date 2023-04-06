/* $Id:$ */
/**
 * @file   TCatAtPedestalProcessor.cc
 * @date   Created : Feb 09, 2013 05:09:28 JST
 *   Last Modified : Feb 12, 2013 19:59:26 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TCatAtPedestalProcessor.h"
#include <TCategorizedData.h>
#include "TCatFadcRawData.h"
#include <TProfile.h>
#include <TF1.h>

art::TCatAtPedestalProcessor::TCatAtPedestalProcessor()
{
   StringVec_t defInput;
   defInput.push_back("atraw");
   RegisterInputCollection("InputCollection","fadc rawdata",
                           fInputColName,defInput);
   RegisterOutputCollection("OutputCollection","output collection name",
                            fOutputColName,TString("no_output"));
   RegisterProcessorParameter("MaxID","Maximum ID of pads",fMaxID,400);
   RegisterProcessorParameter("HistName","HistName",fHistName,TString("hCatPed"));
   RegisterProcessorParameter("MaxGeo","Maximum Geo of pads counted from",fMaxGeo,6);
}
art::TCatAtPedestalProcessor::~TCatAtPedestalProcessor()
{
}


void art::TCatAtPedestalProcessor::Init(art::TEventCollection *col)
{
   fCategorized = (TCategorizedData**)(col->Get(fInputColName.front())->GetObjectRef());
   if (!fCategorized) {
      printf("Initialization failed with %s\n",fInputColName.front().Data());
   }
   fCATHit = new TClonesArray("TCatFadcRawData");
   fHistPed = new TH2F(fHistName,fHistName,fMaxID,-0.5,fMaxID-0.5,4000,0.,4000.);
   printf("fMaxID = %d\n",fMaxID);
   TString name = fHistName;
   name.Append("GeoCh");
   int maxidgeo = (fMaxGeo+1) * 64;
   fHistPedGeoCh = new TH2F(name,fHistName,maxidgeo,-0.5,maxidgeo-0.5,4000,0.,4000.);
}

void art::TCatAtPedestalProcessor::Process()
{
   TIter next(fCATHit);
   TObject *obj = NULL;
   while ((obj = next())) {
      TCatFadcRawData *hit = (TCatFadcRawData*)obj;
      Int_t nSample = hit->GetNumSample();
      for (Int_t is = 0; is != nSample; is++) {
         fHistPed->Fill(hit->GetID(),(*hit)[is]);
         fHistPedGeoCh->Fill(hit->GetGeo()*64+hit->GetCh(),(*hit)[is]);
      }
   }
}

void art::TCatAtPedestalProcessor::EndOfRun()
{
}
