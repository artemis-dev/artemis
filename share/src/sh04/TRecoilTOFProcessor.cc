/**
 * @file   TRecoilTOFProcessor.cc
 * @brief
 *
 * @date   Created:       2013-12-12 16:23:36
 *         Last Modified: 2015-04-13 18:11:03 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TRecoilTOFProcessor.h"

#include <TClonesArray.h>

#include "TBeamParameter.h"
#include "TTimingData.h"
#include "TTwoSidedPlasticData.h"

using art::sh04::TRecoilTOFProcessor;

namespace {
   const Double_t kLengthF3FH9 = 74075.;
   const Double_t kLengthFH9Tgt = 12020. - 1155.;
}

// Default constructor
TRecoilTOFProcessor::TRecoilTOFProcessor()
   : fTOFL(NULL), fTOFR(NULL), fTOFS0D(NULL)
{
   RegisterOutputCollection("OutputL","output name for L",
			    fOutputLName,TString("tofL"),
			    &fTOFL,TClonesArray::Class_Name(),TTimingData::Class_Name());
   RegisterOutputCollection("OutputR","output name for R",
			    fOutputRName,TString("tofR"),
			    &fTOFR,TClonesArray::Class_Name(),TTimingData::Class_Name());
   RegisterOutputCollection("OutputS0D","output name for S0D",
			    fOutputS0DName,TString("tofS0D"),
			    &fTOFS0D,TClonesArray::Class_Name(),TTimingData::Class_Name());
   RegisterInputCollection("F3PlasticColName","collection name for F3 plastic",
			   fF3PlasticColName,TString("spla"),
			   &fF3Plastic,TClonesArray::Class_Name(),TTwoSidedPlasticData::Class_Name());
   RegisterProcessorParameter("F3PlasticDetID","detector ID for F3 plastic",
			      fF3PlasticDetID,0);
   RegisterInputCollection("FH9PlasticColName","collection name for FH9 plastic",
			   fFH9PlasticColName,TString("spla"),
      			   &fFH9Plastic,TClonesArray::Class_Name(),TTwoSidedPlasticData::Class_Name());
   RegisterProcessorParameter("FH9PlasticDetID","detector ID for FH9 plastic",
			      fFH9PlasticDetID,1);
   RegisterInputCollection("S0DPlasticColName","collection name for S0D plastic",
			   fS0DPlasticColName,TString("spla"),
      			   &fS0DPlastic,TClonesArray::Class_Name(),TTwoSidedPlasticData::Class_Name());
   RegisterProcessorParameter("S0DPlasticDetID","detector ID for S0D plastic",
			      fS0DPlasticDetID,1);
   RegisterInputCollection("TPlasticColName","collection name for trigger plastic",
			   fTPlaColName,TString("tpla"),
			   &fTPlastic,TClonesArray::Class_Name(),TTwoSidedPlasticData::Class_Name());
   RegisterProcessorParameter("TPlasticLDetID","detector ID for trigger plastic L",
			      fTPlaLDetID,0);
   RegisterProcessorParameter("TPlasticRDetID","detector ID for trigger plastic R",
			      fTPlaRDetID,1);
   RegisterProcessorParameter("BeamZ","reaction z-position",fBeamZ,0.f);
   RegisterProcessorParameter("BeamParameter","name of beam parameter",
			      fBeamName,TString("beam"));
   RegisterProcessorParameter("Shift","shift of TOF",
			      fShift, 0.f);
}

TRecoilTOFProcessor::~TRecoilTOFProcessor()
{
   delete fTOFL;
   delete fTOFR;
   delete fTOFS0D;
}

void TRecoilTOFProcessor::Init(TEventCollection *col)
{
   { /* Load beam parameter from UserInfo */
      const TBeamParameter *const beam = dynamic_cast<TBeamParameter*>(col->GetInfo(fBeamName));
      if (!beam) {
	 Error("Init", "No TBeamParameter named '%s' found", fBeamName.Data());
	 return;
      }
      const Int_t z       = beam->GetZ();
      const Int_t a       = beam->GetA();
      const Double_t brho = beam->GetBrho();
      Double_t mass       = z * P_MASS + (a-z) * N_MASS;

      if (z == 1 && a == 1) {
	 fTOffsetUS = -306.88;
	 fTOffsetL = 56.7;
	 fTOffsetR = 57.9;
      } else if (z == 8 && a == 14) {
	 mass -= a * 7.052;
	 fTOffsetUS = -299.0;
	 fTOffsetL = 58.66; // before cal. -> 59.9 (58.9)
	 fTOffsetR = 59.8; // before cal. -> 60.7 (59.9)
      } else if (z == 8 && a == 22) {
	 mass -= a * 7.365;
	 fTOffsetUS = -304.1;
	 fTOffsetL = 58.66;
	 fTOffsetR = 59.8;
      } else if (z == 8 && a == 24) {
	 mass -= a * 7.040;
	 fTOffsetUS = -296.4;
	 fTOffsetL = 58.66;
	 fTOffsetR = 59.8;
      }

      const Double_t alpha = z * brho * TMath::C() * 1e-6 / mass;
      const Double_t beta = alpha/TMath::Sqrt(1+alpha*alpha);
      fPrinciple = kLengthF3FH9/beta/(TMath::C()* 1e-6);
   }

   Info("Init","F3Pl: %s (DetID: %d), FH9Pl: %s (DetID: %d), S0DPl: %s (DetID: %d)",
	fF3PlasticColName.Data(),fF3PlasticDetID,
	fFH9PlasticColName.Data(),fFH9PlasticDetID,
	fS0DPlasticColName.Data(),fS0DPlasticDetID);
   Info("Init","TPla: %s (DetID L: %d, R: %d)",
	fTPlaColName.Data(),fTPlaLDetID,fTPlaRDetID);
   Info("Init","BeamZ: %f",fBeamZ);
   Info("Init","output: %s, %s, %s",
	fOutputLName.Data(),fOutputRName.Data(),fOutputS0DName.Data());
}

namespace {
inline const art::TTwoSidedPlasticData * GetPlastic(const TClonesArray *const ca,const Int_t detID)
{
   const Int_t n = ca->GetEntriesFast();
   for(Int_t i = 0; i!=n;++i) {
      const art::TTwoSidedPlasticData *const obj =
	 static_cast<art::TTwoSidedPlasticData*>(ca->At(i));
      if(obj->GetDetID() == detID) return obj;
   }
   return NULL;
}
}

void TRecoilTOFProcessor::Process()
{
   /* initialization */
   fTOFL->Clear("C");
   fTOFR->Clear("C");
   fTOFS0D->Clear("C");

   const TTwoSidedPlasticData *const f3pla  = GetPlastic(*fF3Plastic,fF3PlasticDetID);
   const TTwoSidedPlasticData *const fh9pla = GetPlastic(*fFH9Plastic,fFH9PlasticDetID);
   const TTwoSidedPlasticData *const s0dpla = GetPlastic(*fS0DPlastic,fS0DPlasticDetID);
   if (!f3pla || !fh9pla) return;

   /* TODO: refine multihit treatment. now simply taking the first hit */
   const TTwoSidedPlasticData *const tplal = GetPlastic(*fTPlastic,fTPlaLDetID);
   const TTwoSidedPlasticData *const tplar = GetPlastic(*fTPlastic,fTPlaRDetID);
   if (!tplal || !tplar) return;

   {
      const Double_t lengthRatio = (kLengthFH9Tgt + fBeamZ) / kLengthF3FH9;

      const Double_t tF3  = f3pla->GetTiming();
      const Double_t tFH9 = fh9pla->GetTiming();

      const Double_t tof39   = tFH9 - tF3 - fTOffsetUS + fPrinciple;
      const Double_t tof9tgt = tof39 * lengthRatio;
      const Double_t tTgt    = tFH9 + tof9tgt;

      const Double_t tTplaL = tplal->GetTiming();
      const Double_t tTplaR = tplar->GetTiming();

      TTimingData *const tOutL = reinterpret_cast<TTimingData*>(fTOFL->ConstructedAt(0));
      TTimingData *const tOutR = reinterpret_cast<TTimingData*>(fTOFR->ConstructedAt(0));

      *tOutL = tTplaL - tTgt + fTOffsetL + fShift;
      tOutL->SetDetID(0);
      *tOutR = tTplaR - tTgt + fTOffsetR + fShift;
      tOutR->SetDetID(0);

      if (s0dpla) {
	 const Double_t tS0D = s0dpla->GetTiming();
	 TTimingData *const tOutS0D = reinterpret_cast<TTimingData*>(fTOFS0D->ConstructedAt(0));
	 *tOutS0D = tS0D - tTgt;
	 tOutS0D->SetDetID(0);
      }
   }
}
