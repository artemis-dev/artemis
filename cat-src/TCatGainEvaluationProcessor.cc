/**
 * @file   TCatGainEvaluationProcessor.cc
 * @brief  evaluate gain by summing up the charge in a row
 *
 * @date   Created       : 2014-07-17 19:25:16 JST
 *         Last Modified : 2016-11-02 10:04:40 JST (ota)
 * @author h307-3 <h307-3@ata01.atnet>
 *
 *    (C) 2014 h307-3
 */

#include "TCatGainEvaluationProcessor.h"
#include <TClonesArray.h>
#include <TCatPulseShape.h>
using art::TCatGainEvaluationProcessor;

ClassImp(TCatGainEvaluationProcessor)

TCatGainEvaluationProcessor::TCatGainEvaluationProcessor()
{
   RegisterInputCollection("Input","name of input collection",fInputName,TString("atpulse"),
                           &fInput,TClonesArray::Class_Name(),art::TCatPulseShape::Class_Name());
   RegisterOutputCollection("Output","name of output collection",fOutputName,TString("athitsrow"),
                            &fOutput,TClonesArray::Class_Name(),art::TCatPulseShape::Class_Name());
   RegisterProcessorParameter("IdMin","minimum id for searching hit",fIdMin,0);
   RegisterProcessorParameter("IdMax","maximum id for searching hit",fIdMax,415);
   RegisterProcessorParameter("OffsetMin","minimum offset for searching hit",fOffsetMin,(Float_t)-200.);
   RegisterProcessorParameter("OffsetMax","maximum offset for searching hit",fOffsetMax,(Float_t)50.);
}

TCatGainEvaluationProcessor::~TCatGainEvaluationProcessor()
{
}

TCatGainEvaluationProcessor::TCatGainEvaluationProcessor(const TCatGainEvaluationProcessor& rhs)
{
}

TCatGainEvaluationProcessor& TCatGainEvaluationProcessor::operator=(const TCatGainEvaluationProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TCatGainEvaluationProcessor::Process()
{
   fOutput->Clear("C");
   Int_t nHit = (*fInput)->GetEntriesFast();
   TObjArray hits;
   for (Int_t iHit=0; iHit!=nHit; iHit++) {
      art::TCatPulseShape *data = (art::TCatPulseShape*) (*fInput)->UncheckedAt(iHit);
      const Int_t &id = data->GetID();
      const Double_t offset = data->GetOffset();
      if (fIdMin <= id && id <= fIdMax &&
         fOffsetMin <= offset && offset <= fOffsetMax) {
         hits.Add(data);
      }
   }
   nHit = hits.GetEntriesFast();
   for (Int_t iRow = 0; iRow != 20; ++iRow) {
      Double_t q = 0.;
      Double_t xq = 0.;
      Double_t yq = 0.;
      Double_t zq = 0.;
      for (Int_t iHit = 0; iHit != nHit; ++iHit) {
         art::TCatPulseShape *pulse = (art::TCatPulseShape*) hits.UncheckedAt(iHit);
         if ((pulse->GetID()%20) == iRow) {
            Double_t charge = pulse->GetCharge();
            q += charge;
            xq += pulse->GetX() * charge;
            yq += pulse->GetOffset() * charge;
            zq += pulse->GetZ() * charge;
         }
      }
      art::TCatPulseShape *out = (art::TCatPulseShape*)fOutput->ConstructedAt(fOutput->GetEntriesFast());
      out->SetID(iRow);
      out->SetCharge(q);
      if (q > 0.) {
         out->SetOffset(yq/q);
         out->SetXZ(xq/q,zq/q);
         out->Validate();
      }  else {
         out->Invalidate();
      }
   }
   
}
