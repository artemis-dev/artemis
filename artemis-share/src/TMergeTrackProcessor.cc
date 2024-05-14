/**
 * @file   TMergeTrackProcessor.cc
 * @brief
 *
 * @date   Created:       2023-10-17 10:33:43 JST
 *         Last Modified: 2023-10-17 12:03:54 JST
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 *
 *    Copyright (C) 2013 OTA Shinsuke All rights reserved
 */


#include "TMergeTrackProcessor.h"
#include "TClonesArray.h"
#include "TMWDCTrackingResult.h"

using art::TMergeTrackProcessor;

ClassImp(TMergeTrackProcessor)


TMergeTrackProcessor::TMergeTrackProcessor()
{

   Register(fInputNames("InputCollections","list of input collections",std::vector<TString>()));
   Register(fOutputName("OutputCollection","name of output collection","track"));
            
}


TMergeTrackProcessor::~TMergeTrackProcessor()
{
}

void TMergeTrackProcessor::Init(TEventCollection *col)
{
   TClass *cls = nullptr;
   TClass *targetClass = TMWDCTrackingResult::Class();
   
   for (auto& name : fInputNames.Value()) {
      TClonesArray** arr = reinterpret_cast<TClonesArray**>(col->GetObjectRef(name));
      if (!arr) {
         SetStateError(Form("No such input collection %s",name.Data()));
         return;
      }
      TClass *newcls = (*arr)->GetClass();
      if (!newcls->InheritsFrom(targetClass)) {
         SetStateError(Form("Class does not inherits from TTrack : %s",newcls->GetName()));
         return;
      }
         
      if (!cls) {
         cls = (*arr)->GetClass();
      } else if (cls != newcls) {
         Warning("Init","Different TTrack will be merged into TTrack. Other data will be lost.: %s %s",cls->GetName(),newcls->GetName());
         cls = targetClass;
      }
      fInputs.push_back(arr);
   }

   fOutput = new TClonesArray(cls);

   col->Add(fOutputName.Value(),fOutput,fOutputIsTransparent);
}

void TMergeTrackProcessor::Process()
{
   fOutput->Clear("C");
   // assuming single hit (multihit with different timing should be implemented later)
   TMWDCTrackingResult* outtrack = nullptr;
   for (auto arr : fInputs) {
      for (int i = 0, n = (*arr)->GetEntriesFast(); i < n; ++i) {
         TMWDCTrackingResult* track = static_cast<TMWDCTrackingResult*>((*arr)->At(i));
         if (track->IsGood()) {
            outtrack = track;
            break;
         }
      }
      if (outtrack) break;
   }

   if (outtrack) {
      TMWDCTrackingResult* out = static_cast<TMWDCTrackingResult*>(fOutput->ConstructedAt(fOutput->GetEntriesFast()));
      outtrack->TTrack::Copy(*(TTrack*)out);
      out->SetSSR(outtrack->GetSSR());
   }
}



