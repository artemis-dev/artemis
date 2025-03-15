
#ifndef TSRPPACPLANEDATAV2V2_H
#define TSRPPACPLANEDATAV2V2_H

#include "TDataObject.h"
#include "ITiming.h"
#include "ICharge.h"
#include "TTimingChargeData.h"


namespace art {
   class TSRPPACPlaneDataV2;
}

class art::TSRPPACPlaneDataV2 : public TTimingChargeData {
public:
   TSRPPACPlaneDataV2();
   virtual ~TSRPPACPlaneDataV2();

   TSRPPACPlaneDataV2(const TSRPPACPlaneDataV2& rhs);
   TSRPPACPlaneDataV2& operator=(const TSRPPACPlaneDataV2& rhs);

   void SetPosition(Double_t val) { fPosition = val; }
   Double_t GetPosition() const { return fPosition; }

   Bool_t IsProcessed() const { return kProcessed; }
   void SetProcessed(Bool_t val) { kProcessed = val; }

   void SetConvCoeff(Double_t val) { fConvCoeff = val; }
   Double_t GetConvCoeff() const { return fConvCoeff; }

   void SetConvCoeffL(Double_t val) { fConvCoeffL = val; }
   Double_t GetConvCoeffL() const { return fConvCoeffL; }

   void SetConvCoeffR(Double_t val) { fConvCoeffR = val; }
   Double_t GetConvCoeffR() const { return fConvCoeffR; }

   void Clear(Option_t *opt);
   
protected:
   Double_t fPosition;
   Bool_t kProcessed;
   
   Double_t fConvCoeff;
   Double_t fConvCoeffL;
   Double_t fConvCoeffR;

private:

   ClassDef(TSRPPACPlaneDataV2,1) // Data class for SR-PPAC
};

#endif // TSRPPACPLANEDATAV2_H
