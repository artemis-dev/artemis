/**
 * @file   TCatFunctionChargeResidual.h
 * @brief  chi2 function for tracking
 *
 * @date   Created       : 2016-09-18 07:30:20 JST
 *         Last Modified : 2017-05-18 10:45:10 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_285C32B8_612C_455A_AF70_13AEB6F501F5
#define INCLUDE_GUARD_UUID_285C32B8_612C_455A_AF70_13AEB6F501F5

#include "Math/IFunction.h"
#include <vector>
#include <TArtSystemOfUnit.h>
#include <TArtTypes.h>

namespace art {
   class TCatFunctionChargeResidual;
   class TCatDiffusedChargeResponseHelper;
   class TCatPulseShape;
}

class TObjArray;


class art::TCatFunctionChargeResidual : public ROOT::Math::IBaseFunctionMultiDim {
public:
   TCatFunctionChargeResidual();
   virtual ~TCatFunctionChargeResidual();

   TCatFunctionChargeResidual(const TCatFunctionChargeResidual& rhs);
   TCatFunctionChargeResidual& operator=(const TCatFunctionChargeResidual& rhs);

   void SetHits(const std::vector<TCatPulseShape*>& input) { fHits = input; }
   void SetResponseHelper(TCatDiffusedChargeResponseHelper *helper) { fResponseHelper = helper;}

   double GetCharge(int id) const;

   // override of IBaseFunctionMultiDim
   double DoEval(const double *x) const;
   unsigned int NDim() const { return fUseRange ? 7 : 6; }
   ROOT::Math::IBaseFunctionMultiDim* Clone() const;

   void SetRangeUnit(double scale = TArtSystemOfUnit::mm) { fRangeUnit = scale; }

   void SetUseTotalCharge(bool use = false) { fUseTotalCharge = use; }

   void SetSigmaCoeff(const DoubleVec_t coeff) {
      fSigmaCoeff = coeff;
   }

   Double_t GetRangeUnit() const { return fRangeUnit; }

   void SetVerboseLevel(int level) { fVerboseLevel = level; }

   void SetUseRange(bool useRange) { fUseRange = useRange; }
   
protected:

private:

   TCatDiffusedChargeResponseHelper *fResponseHelper; //!
   std::vector<TCatPulseShape*> fHits; //!

   Double_t fRangeUnit; // scale for range
   bool fUseTotalCharge; // use total charge to fit

   DoubleVec_t fSigmaCoeff; // sigma coefficient

   int fVerboseLevel; // verbose level

   bool fUseRange; // use range
   

//   ClassDef(TCatFunctionChargeResidual,1) // chi2 function for tracking
};

#endif // INCLUDE_GUARD_UUID_285C32B8_612C_455A_AF70_13AEB6F501F5
