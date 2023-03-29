/**
 * @file   TCatTrackDiffusedMinuitHelper.h
 * @brief  helper for tracking with diffused charge
 *
 * @date   Created       : 2016-07-22 09:40:35 JST
 *         Last Modified : 2018-02-04 13:16:34 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_B502220B_4F6B_43CE_81A7_138DA3B1D63D
#define INCLUDE_GUARD_UUID_B502220B_4F6B_43CE_81A7_138DA3B1D63D

#include "TCatTrackMinuitHelper.h"
#include <TVector2.h>

namespace art {
   class TCatTrackDiffusedMinuitHelper;
   class TNArray;
   class TCatTrackResult;
}

class TVector3;
class TClonesArray;

class art::TCatTrackDiffusedMinuitHelper : public TCatTrackMinuitHelper {
public:
   TCatTrackDiffusedMinuitHelper();
   virtual ~TCatTrackDiffusedMinuitHelper();

   TCatTrackDiffusedMinuitHelper(const TCatTrackDiffusedMinuitHelper& rhs);
   TCatTrackDiffusedMinuitHelper& operator=(const TCatTrackDiffusedMinuitHelper& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Register(TProcessor *);

   virtual Double_t CalcResidual(Double_t x0, Double_t y0, Double_t z0, Double_t vx, Double_t vy, Double_t vz);

   virtual Bool_t LoadResponseTable(const char *filename);
   virtual void GetResponse(Int_t id, const TVector3 &trackStart, const TVector3& unitDirection, const Double_t &range,
                            Int_t nStep, Double_t step, Double_t &charge, Double_t& closest, Double_t &rangeToEnd);

   virtual void SetAttachment(Double_t attachment) { fAttachment = attachment; }
   virtual void SetFieldCageOffset(Double_t fieldCageOffset) { fFieldCageOffset = fieldCageOffset; }
   virtual void SetResponseTableFileName(const char *name) { fResponseTableFileName = name; }

   virtual void GetTrackResult(TCatTrackResult *result);

   virtual void SetData(TObjArray *input);

protected:

   Double_t fAttachment;
   Double_t fFieldCageOffset;
   Double_t fIntegrateStep;

   TString fResponseTableFileName;

   std::vector<TVector2> fPadCenter;
   std::vector<TVector2> fPadVertex;

   TNArray *fResponseTable; //!
   TClonesArray *fPulseHelper; //!
   

private:

   ClassDef(TCatTrackDiffusedMinuitHelper,1) // helper for tracking with diffused charge
};

#endif // INCLUDE_GUARD_UUID_B502220B_4F6B_43CE_81A7_138DA3B1D63D
 
