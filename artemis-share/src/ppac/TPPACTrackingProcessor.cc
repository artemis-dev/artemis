/**
 * @file   TPPACTrackingProcessor.cc
 * @brief  ppac tracking processor
 *
 * @date   Created       : 2014-06-15 14:09:17 JST
 *         Last Modified : Jun 15, 2014 17:23:35 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#include "TPPACTrackingProcessor.h"
#include "TPPACData.h"
#include <TClonesArray.h>
#include <TTrack.h>

using art::TPPACTrackingProcessor;

ClassImp(TPPACTrackingProcessor)

TPPACTrackingProcessor::TPPACTrackingProcessor()
{
   StringVec_t defval;   defval.push_back("f3ppaca"); defval.push_back("f3ppacb");
   RegisterProcessorParameter("InputColName","array of input collection name",fInputColNames,defval);
   RegisterOutputCollection("OutputColName","name of coutput collection",fOutputColName,TString("f3"),
                            &fOutput,TClonesArray::Class_Name(),art::TTrack::Class_Name());
}

TPPACTrackingProcessor::~TPPACTrackingProcessor()
{
}

TPPACTrackingProcessor::TPPACTrackingProcessor(const TPPACTrackingProcessor& rhs)
{
}

TPPACTrackingProcessor& TPPACTrackingProcessor::operator=(const TPPACTrackingProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TPPACTrackingProcessor::Init(TEventCollection *col)
{
   for (StringVec_t::iterator it = fInputColNames.begin(); it != fInputColNames.end(); it++) {
      TObject **obj = (TObject**) col->GetObjectRef(*it);
      if (!obj) {
         SetStateError(TString::Format("No such input collection '%s'",it->Data()));
         return;
      }
      fInputs.push_back((TClonesArray**)(obj));
   }
   fNumPlane = fInputColNames.size();
}

void TPPACTrackingProcessor::Process()
{
   fOutput->Clear("C");
   std::vector<std::vector<TPPACData*> > trackhit;
   std::vector<TPPACData*> candidate;

   // assume single hit
   for (Int_t i=0; i!=fNumPlane; i++) {
      if (!(*fInputs[i])->GetEntriesFast()) continue;
      TPPACData* data = (TPPACData*) (*fInputs[i])->At(0);
      if (!data->IsValid()) continue;
      candidate.push_back(data);
   }
   if (candidate.size() < 2) return;
   trackhit.push_back(candidate);
   
   Int_t nTrack = trackhit.size();
   for (Int_t i=0; i!=nTrack; i++) {
      Track(trackhit[i],(TTrack*)fOutput->ConstructedAt(i));
   }
}


void TPPACTrackingProcessor::Track(std::vector<TPPACData*> hits, TTrack *track)
{
   Int_t n = hits.size();
   Double_t szz, sz, sxz, sx, syz, sy;
   szz = sz = sxz = sx = syz = sy = 0.;
   for (Int_t i=0; i!=n; i++) {
      Double_t x = hits[i]->GetX();
      Double_t y = hits[i]->GetY();
      Double_t z = hits[i]->GetZ();
      szz += z*z;
      sz += z;
      sx  += x;
      sy  += y;
      sxz += x*z;
      syz += y*z;
   }
   Double_t delta = 1./(n*szz - sz*sz);
   Double_t a0 = (n * sxz - sx * sz) * delta;
   Double_t x0 = (- sxz * sz + szz * sx) * delta;
   Double_t b0 = (n * syz - sy * sz) * delta;
   Double_t y0 = (- syz * sz + szz * sy) * delta;

   track->SetPos(x0,y0,0);
   track->SetAngle(a0,b0);
}
