/**
 * @file   TSRPPACParameter.h
 * @brief  Parameter class for SR-PPAC
 *
 * @date   Created       : 2019-02-04 03:21:00 DST
 *         Last Modified : 2019-02-16 08:11:00 JST (ota)
 * @author Shoichiro Masuoka <masuoka@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2019 Shoichiro Masuoka
 */

#ifndef INCLUDE_GUARD_UUID_06FE1779_1863_4E6E_8619_610D47C19F23
#define INCLUDE_GUARD_UUID_06FE1779_1863_4E6E_8619_610D47C19F23

#include "TParameterObject.h"

namespace art {
   class TSRPPACParameter;
}

class art::TSRPPACParameter : public TParameterObject {
public:
   TSRPPACParameter();
   virtual ~TSRPPACParameter();
//   virtual Bool_t LoadYAMLNode(const YAML::Node &node);
   
   Int_t GetNStrip() const { return fNStrip; }
   Double_t GetStripWidth() const { return fStripWidth; }
   Double_t GetCenter() const { return fCenter; }
   Double_t GetDetOffset() const { return fDetOffset; }
   Double_t GetZ() const { return fZ; }
   Bool_t IsTurned() const { return fTurned; }
   
protected:
   Int_t fNStrip;
   Double_t fStripWidth;
   Double_t fCenter;
   Double_t fDetOffset;
   Double_t fZ;
   Bool_t fTurned;
   
private:

   ClassDef(TSRPPACParameter,1) // Parameter class for SR-PPAC
};

#endif // INCLUDE_GUARD_UUID_06FE1779_1863_4E6E_8619_610D47C19F23
