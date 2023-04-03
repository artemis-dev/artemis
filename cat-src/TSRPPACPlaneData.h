/**
 * @file   TSRPPACPlaneData.h
 * @brief  Data class for SR-PPAC
 *
 * @date   Created       : 2019-02-07 03:41:37 DST
 *         Last Modified : 2019-02-14 03:15:04 JST (ota)
 * @author Shoichiro Masuoka <masuoka@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2019 Shoichiro Masuoka
 */

#ifndef INCLUDE_GUARD_UUID_F524967E_7EB3_43FF_AF66_2ABA92841B89
#define INCLUDE_GUARD_UUID_F524967E_7EB3_43FF_AF66_2ABA92841B89

#include "TDataObject.h"
#include "ITiming.h"
#include "ICharge.h"
#include "TTimingChargeData.h"


namespace art {
   class TSRPPACPlaneData;
}

class art::TSRPPACPlaneData : public TTimingChargeData {
public:
   TSRPPACPlaneData();
   virtual ~TSRPPACPlaneData();

   TSRPPACPlaneData(const TSRPPACPlaneData& rhs);
   TSRPPACPlaneData& operator=(const TSRPPACPlaneData& rhs);

   void SetPosition(Double_t val) { fPosition = val; }
   Double_t GetPosition() const { return fPosition; }

   Bool_t IsProcessed() const { return kProcessed; }
   void SetProcessed(Bool_t val) { kProcessed = val; }

protected:
   Double_t fPosition;
   Bool_t kProcessed;
   
private:

   ClassDef(TSRPPACPlaneData,1) // Data class for SR-PPAC
};

#endif // INCLUDE_GUARD_UUID_F524967E_7EB3_43FF_AF66_2ABA92841B89
