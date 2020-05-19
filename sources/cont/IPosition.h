/* @file IPosition.h
 * @brief description
 *
 * @date Create        : 2019-05-15 10:59:40 JST
 *       Last Modified : 2019-05-15 11:28:39 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */


#ifndef INCLUDE_GUARD_UUID_96947562_BE7E_4478_8A0C_2D927A654B18
#define INCLUDE_GUARD_UUID_96947562_BE7E_4478_8A0C_2D927A654B18


#include "Rtypes.h"

namespace art {
   class IPosition;
};

class TVector3;

class art::IPosition {
public:
   IPosition();
   virtual ~IPosition();

   virtual void Copy(IPosition& dest) const {
      dest.SetXYZ(X(),Y(),Z());
   }

   TVector3 Vec() const;
   virtual double X() const = 0;
   virtual double Y() const = 0;
   virtual double Z() const = 0;
   virtual double x() const { return X(); }
   virtual double y() const { return Y(); }
   virtual double z() const { return Z(); }

   virtual void SetX(double) = 0;
   virtual void SetY(double) = 0;
   virtual void SetZ(double) = 0;
   virtual void SetXYZ(double x, double y, double z) { SetX(x); SetY(y); SetZ(z); };
   virtual void SetXYZ(const TVector3& pos);

   ClassDef(IPosition,1)

}; // end of IPosition

#endif // #ifndef INCLUDE_GUARD_UUID_96947562_BE7E_4478_8A0C_2D927A654B18}
