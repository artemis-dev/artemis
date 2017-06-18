/**
 * @file   TCmdSlope.h
 * @brief  Obtain slope graphically
 *
 * @date   Created       : 2017-05-26 14:22:23 JST
 *         Last Modified : 2017-05-26 16:47:05 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2017 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_CBC330B9_7419_4ECD_A425_18AD0E4662B1
#define INCLUDE_GUARD_UUID_CBC330B9_7419_4ECD_A425_18AD0E4662B1

#include "TCatCmd.h"

namespace art {
   class TCmdSlope;
}

class TF1;

class art::TCmdSlope : public TCatCmd {
public:
   TCmdSlope();
   ~TCmdSlope();

   static const char* kFuncNameBase;
   static const char* kLabelNameBase;
   Long_t Cmd(vector<TString>);

private:
   TCmdSlope(const TCmdSlope&); // undefined
   TCmdSlope& operator=(const TCmdSlope&); // undefined

   ClassDef(TCmdSlope,1) // Obtain slope graphically
};

#endif // INCLUDE_GUARD_UUID_CBC330B9_7419_4ECD_A425_18AD0E4662B1
