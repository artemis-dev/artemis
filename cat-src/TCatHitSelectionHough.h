/**
 * @file   TCatHitSelectionHough.h
 * @brief  hit selection by hough transformation
 *
 * @date   Created       : 2014-12-27 02:11:30 JST
 *         Last Modified : Dec 27, 2014 02:11:41 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_6B1066C8_F57A_4A0F_8402_7A4F88B3812E
#define INCLUDE_GUARD_UUID_6B1066C8_F57A_4A0F_8402_7A4F88B3812E

#include "TProcessor.h"

namespace art {
   class TCatHitSelectionHough;
}

class art::TCatHitSelectionHough : public TProcessor {
public:
   TCatHitSelectionHough();
   virtual ~TCatHitSelectionHough();

   TCatHitSelectionHough(const TCatHitSelectionHough& rhs);
   TCatHitSelectionHough& operator=(const TCatHitSelectionHough& rhs);

   void Process();


protected:

private:

   ClassDef(TCatHitSelectionHough,1) // hit selection by hough transformation
};

#endif // INCLUDE_GUARD_UUID_6B1066C8_F57A_4A0F_8402_7A4F88B3812E
