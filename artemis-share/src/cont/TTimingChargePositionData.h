/* @file TTimingChargePositionData.h
 * @brief container for timing, charge and position data
 *
 * @date Create        : 2019-05-15 11:22:22 JST
 *       Last Modified : 2019-05-15 11:32:01 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */


#ifndef INCLUDE_GUARD_UUID_6A41918E_20CB_4C54_9E29_123E4CED940C
#define INCLUDE_GUARD_UUID_6A41918E_20CB_4C54_9E29_123E4CED940C

#include "TTimingChargeData.h"
#include "IPosition.h"


namespace art {
   class TTimingChargePositionData;
}

class art::TTimingChargePositionData : public TTimingChargeData, public IPosition {
public:
   TTimingChargePositionData();
   virtual ~TTimingChargePositionData();
   virtual void Copy(TObject& dest) const {
      TTimingChargeData::Copy(dest);
      TTimingChargePositionData *obj = static_cast<TTimingChargePositionData*>(&dest);
      obj->SetXYZ(X(),Y(),Z());
   }

   virtual void SetX(double x) { fX = x; }
   virtual void SetY(double y) { fY = y; }
   virtual void SetZ(double z) { fZ = z; }

   virtual double X() const { return fX; }
   virtual double Y() const { return fY; }
   virtual double Z() const { return fZ; }

protected:
   double fX;
   double fY;
   double fZ;


   ClassDef(TTimingChargePositionData, 1)
}; // end of TTimingChargePositionData

#endif // #ifndef INCLUDE_GUARD_UUID_6A41918E_20CB_4C54_9E29_123E4CED940C}
