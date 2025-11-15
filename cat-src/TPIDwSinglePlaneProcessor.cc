/**
 * @file   TPIDwSinglePlaneProcessor.cc
 * @brief  pid with mwdc single plane
 *
 * @date   Created       : 2018-07-12 17:39:49 JST
 *         Last Modified : 2018-08-06 20:11:45 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#include "TPIDwSinglePlaneProcessor.h"
#include <TMath.h>
#include <TMWDCHitData.h>
#include <TSimpleData.h>
#include <TTimingChargeData.h>
#include <TClonesArray.h>
#include <deque>
#include <TMWDCParameter.h>
#include <TMWDCPlaneInfo.h>

using art::TPIDwSinglePlaneProcessor;

ClassImp(TPIDwSinglePlaneProcessor)

namespace {
   class LessByTimestamp {
   public:
      bool operator() (const art::TDataObject* data1, const art::TDataObject *data2) {
         return data1->GetTimestamp() < data2->GetTimestamp();
      }
   };
}

   


TPIDwSinglePlaneProcessor::TPIDwSinglePlaneProcessor()
   : fPlanes(NULL), fTOFs(NULL), fOutput(NULL)
{
   RegisterInputCollection("PlaneName","name of input plane",fPlaneName,TString("dc51_x1"),
                           &fPlanes,TClonesArray::Class_Name(),TMWDCHitData::Class_Name());
   RegisterInputCollection("TofName","name of input tof",fTofName,TString("tof37"),
                           &fTOFs,TClonesArray::Class_Name(),TTimingChargeData::Class_Name());
   RegisterOutputCollection("OutputAQ","name of output pid",fOutputName,TString("aq"),
                            &fOutput,TClonesArray::Class_Name(),TSimpleData::Class_Name());
   RegisterOutputCollection("OutputZ","name of output z",fOutputZName,TString("z"),
                            &fOutputZ,TClonesArray::Class_Name(),TSimpleData::Class_Name());
   RegisterOutputCollection("Brho1","name of brho before focus",fOutputBrho1Name,TString("br35"),
                            &fBrho1,TClonesArray::Class_Name(),TSimpleData::Class_Name());
   RegisterOutputCollection("Brho2","name of brho before focus",fOutputBrho2Name,TString("br57"),
                            &fBrho2,TClonesArray::Class_Name(),TSimpleData::Class_Name());
   RegisterProcessorParameter("Brho0","two Brho0s before and after focus",fBrho0,DoubleVec_t(2));
   RegisterProcessorParameter("Length","two lengths before and after focus",fLength,DoubleVec_t(2, 23.2832));
   RegisterProcessorParameter("Dispersion","two dispersions before and after focus",fDispersion,DoubleVec_t(0));
   RegisterProcessorParameter("DegThick","degrader thickness (d = p0 + p1 * x)",fDegThick,DoubleVec_t(0));
}

TPIDwSinglePlaneProcessor::~TPIDwSinglePlaneProcessor()
{
}

TPIDwSinglePlaneProcessor::TPIDwSinglePlaneProcessor(const TPIDwSinglePlaneProcessor& rhs)
{
}

TPIDwSinglePlaneProcessor& TPIDwSinglePlaneProcessor::operator=(const TPIDwSinglePlaneProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TPIDwSinglePlaneProcessor::Init(TEventCollection *col)
{


   const Int_t delimIndex = fPlaneName.Last('_');
   const TString &mwdcname = fPlaneName(0,delimIndex);
   const TString &planename = fPlaneName(delimIndex+1,fPlaneName.Length());

   TMWDCParameter* mwdcprm = reinterpret_cast<TMWDCParameter*>(col->GetInfo(mwdcname));
   
   if (!mwdcprm) {
      SetStateError(TString::Format("mwdc parameter object '%s' not found",mwdcname.Data()));
      return;
   }

   fMWDCParam = mwdcprm->GetPlane(planename);

}

void TPIDwSinglePlaneProcessor::Process()
{
   fOutput->Clear("C");
   fOutputZ->Clear("C");
   fBrho1->Clear("C");
   fBrho2->Clear("C");
   if (!(*fPlanes)->GetEntriesFast() ||
       !(*fTOFs)->GetEntriesFast()) {
      return;
   }
   std::deque<TMWDCHitData*> mwdcHits;
   std::deque<TTimingChargeData*> tofHits;

   for (Int_t iHit = 0, nHits = (*fPlanes)->GetEntriesFast(); iHit < nHits; ++iHit) {
      mwdcHits.push_back(static_cast<TMWDCHitData*>((*fPlanes)->UncheckedAt(iHit)));
   }
   for (Int_t iTof = 0, nTof = (*fTOFs)->GetEntriesFast(); iTof < nTof; ++iTof) {
      tofHits.push_back(static_cast<TTimingChargeData*>((*fTOFs)->UncheckedAt(iTof)));
   }

   std::sort(mwdcHits.begin(),mwdcHits.end(),LessByTimestamp());
   std::sort(tofHits.begin(),tofHits.end(),LessByTimestamp());
//   Info("Process","");
   
   while (mwdcHits.size()) {
      TMWDCHitData* plane = mwdcHits.front();
      mwdcHits.pop_front();
      while (tofHits.size()) {
         TTimingChargeData* tof = tofHits.front();
         tofHits.pop_front();
//         printf("%f %f\n",plane->GetTimestamp(),tof->GetTimestamp());
         if ( tof->GetTimestamp() - plane->GetTimestamp() > TMath::Limits<Float_t>::Epsilon()) {
            // skip this mwdc hit if plane timestamp is ealier than tof timestamp
            break;
         }
         if (plane->GetTimestamp() - tof->GetTimestamp() > TMath::Limits<Float_t>::Epsilon()) {
            // skip this tof hit if tof timestamp is ealier than plane timestamp
            continue;
         }
         // found
         Double_t x = - ((plane->GetID() - fMWDCParam->GetCenter()) * fMWDCParam->GetCellSize()) * fMWDCParam->Cos();
//         printf("%d %f %f\n",plane->GetID(),x, fMWDCParam->GetCellSize());
         Double_t t = tof->GetTiming();
         Double_t aq, z, br35, br57;
         CalcZAQ(t,-x,z,aq, br35, br57);
         TSimpleData *dataaq = static_cast<TSimpleData*>(fOutput->ConstructedAt(fOutput->GetEntriesFast()));
         TSimpleData *dataz = static_cast<TSimpleData*>(fOutputZ->ConstructedAt(fOutputZ->GetEntriesFast()));
         TSimpleData *data35 = static_cast<TSimpleData*>(fBrho1->ConstructedAt(fBrho1->GetEntriesFast()));
         TSimpleData *data57 = static_cast<TSimpleData*>(fBrho2->ConstructedAt(fBrho2->GetEntriesFast()));
         dataaq->SetValue(aq);
         dataz->SetValue(z);
         data35->SetValue(br35);
         data57->SetValue(br57);
         break;
      }
   }
}

void TPIDwSinglePlaneProcessor::CalcZAQ(Double_t tof, Double_t x, Double_t& z, Double_t& aq, Double_t &br35, Double_t & br57)
{
   const Double_t c = TMath::C();
   const Double_t m = 931.494; // unit nucleus mass 

   // calculate brhos
   Double_t r35 = fBrho0[0] * ( 1 + x / fDispersion[0] / 100.) ;
   Double_t r57 = fBrho0[1] * ( 1 + x / fDispersion[1] / 100.) ;
   Double_t t = tof * 1e-9; // ns to sec
   
   // decompose beta
   Double_t r = r35 / r57 ;
   Double_t v1 = fLength[0]/c;
   Double_t v2 = fLength[1]/c;

   Double_t tt = t * r * TMath::Sqrt(1 + (r-1) * (r+1) * (v2 - v1/r) * (v2 + v1/r) / t /t);

   Double_t b35 = (v1 * tt + v2 * r * r * t) / (t * tt + v1*v2*r*r - v1*v2);
   Double_t b57 = (v1 * tt + v2 * r * r * t) / (r*r*t*t - v1*v1*r*r + v1*v1);
   Double_t g35 = 1. / sqrt( 1 - b35 * b35 );
   Double_t g57 = 1. / sqrt( 1 - b57 * b57 );

//   printf("b35 = %f, b57 = %f\n",b35, b57);
   
   Double_t d = fDegThick[0] + fDegThick[1] * x;
   Double_t aq35 = c * r35 / (b35*g35*m) * 1e-6;
   Double_t aq57 = c * r57 / (b57*g57*m) * 1e-6;
   z = (r35/b35 - r57/b57) * b35*b35 / d;
   aq = aq35;
   br35 = r35;
   br57 = r57;
//   printf("fDispersion[0] = %f, r35 = %f r57 = %f aq35 = %f, aq57 = %f\n",fDispersion[0],r35,r57,aq35,aq57);
}
