/**
 * @file   TCatTrackDiffusedMinuit.h
 * @brief  implementation of tracking taking diffusion into account
 *
 * @date   Created       : 2015-08-26 10:50:40 JST
 *         Last Modified : 2016-02-02 16:35:14 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_EC7FD296_2FE7_40EE_A25C_729CF41C06D2
#define INCLUDE_GUARD_UUID_EC7FD296_2FE7_40EE_A25C_729CF41C06D2

#include "TCatTrackMinuit.h"

namespace art {
   class TCatTrackDiffusedMinuit;
   class TNArray;
}

class TFile;
class TH2;


class art::TCatTrackDiffusedMinuit : public TCatTrackMinuit {
public:
   TCatTrackDiffusedMinuit();
   virtual ~TCatTrackDiffusedMinuit();

   TCatTrackDiffusedMinuit(const TCatTrackDiffusedMinuit& rhs);
   TCatTrackDiffusedMinuit& operator=(const TCatTrackDiffusedMinuit& rhs);

   // override ecalculate square residual
   virtual Double_t CalcResidual(Double_t y0, Double_t z0, Double_t vx, Double_t vz, Double_t t, TClonesArray *output = 0);

   // register histogram
   Bool_t RegisterHistogram(const char* filename);
   Bool_t LoadResponseTable(const char* filename);
   Bool_t RegisterRange2EnergyLoss(TGraph *graph) { fRange2EnergyLoss = graph; return kTRUE; }
   

   // Set attachment
   void SetAttachment(Float_t attachment) { fAttachment = attachment; }
   void SetFieldCageOffset(Float_t offset) { fFieldCageOffset = offset; }

protected:
   TFile *fFile; //!
   std::vector<TH2*> fHist; //!
   TNArray *fResponseTable; // !
   TGraph  *fRange2EnergyLoss; // 

   Int_t fNumAngle; //! number of angle 
   Int_t fNumDistance; //! number of distance
   Double_t fStepAngle; //! step of angle
   Double_t fStepDistance; //! step of distance
   Double_t fStepDrift; // step of drift
   Double_t fStepRange; // step of range
   Double_t fOriginDrift; // origin of drift
   Double_t fOriginRange; // origin of range

   Int_t fMinBinDrift;
   Int_t fMaxBinDrift;
   Int_t fMinBinRange;
   Int_t fMaxBinRange;

   Float_t fAttachment; // attachment coefficient (cm^{-1})
   Float_t fFieldCageOffset; // offset of the mesh grid of field cage to the beam line

   
   
private:


   ClassDef(TCatTrackDiffusedMinuit,1) // implementation of tracking taking diffusion into account
};

#endif // INCLUDE_GUARD_UUID_EC7FD296_2FE7_40EE_A25C_729CF41C06D2
