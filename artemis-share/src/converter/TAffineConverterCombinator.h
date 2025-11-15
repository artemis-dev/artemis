/**
 * @file   TAffineConverterCombinator.h
 * @brief  combinate affine converter
 *
 * @date   Created       : 2017-08-04 08:43:22 JST
 *         Last Modified : 2017-08-04 08:52:29 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2017 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_A795177F_2C4C_4A7B_819D_57F3A3D89B87
#define INCLUDE_GUARD_UUID_A795177F_2C4C_4A7B_819D_57F3A3D89B87

#include "TAffineConverterArrayGenerator.h"

namespace art {
   class TAffineConverterCombinator;
}

class art::TAffineConverterCombinator : public TAffineConverterArrayGenerator {
public:
   TAffineConverterCombinator();
   virtual ~TAffineConverterCombinator();

   TAffineConverterCombinator(const TAffineConverterCombinator& rhs);
   TAffineConverterCombinator& operator=(const TAffineConverterCombinator& rhs);

   virtual void Init(TEventCollection *col);

protected:
   TString fNameOutput;


private:

   ClassDef(TAffineConverterCombinator,1) // combinate affine converter
};

#endif // INCLUDE_GUARD_UUID_A795177F_2C4C_4A7B_819D_57F3A3D89B87
