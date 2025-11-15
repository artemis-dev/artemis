/**
 * @file   TWalkConverter.h
 * @brief  walk converter
 *
 * @date   Created       : 2014-06-17 03:06:51 JST
 *         Last Modified : 2014-06-20 16:28:59 JST (kawase)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_12CD27D8_1040_4547_B568_AED1098D31BA
#define INCLUDE_GUARD_UUID_12CD27D8_1040_4547_B568_AED1098D31BA

#include "TFixedNumberParameter.h"
#include <TMath.h>

namespace art {
   class TWalkConverter;
}

class art::TWalkConverter : public TFixedNumberParameter {
public:
   TWalkConverter();
   virtual ~TWalkConverter();

   TWalkConverter(const TWalkConverter& rhs);
   TWalkConverter& operator=(const TWalkConverter& rhs);

   virtual Double_t Convert(Double_t val) const { return fParam[0] / TMath::Power(val,fParam[1]); }
   virtual void Print(Option_t *opt = "") const;

   ClassDef(TWalkConverter,1) // TAffineConverter
};

#endif // INCLUDE_GUARD_UUID_12CD27D8_1040_4547_B568_AED1098D31BA
