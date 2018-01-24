/**
 * @file   TCmdProcessorDescription.h
 * @brief  show Processor Description
 *
 * @date   Created       : 2018-01-24 05:29:14 JST
 *         Last Modified : 2018-01-24 07:29:32 JST (mas)
 * @author Shoichiro Masuoka <masuoka@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shoichiro Masuoka
 */


#ifndef INCLUDE_GUARD_UUID_7E6AFF89_AECC_484B_98B6_05F2C94DEE62
#define INCLUDE_GUARD_UUID_7E6AFF89_AECC_484B_98B6_05F2C94DEE62

#include "TCatCmd.h"

namespace art {
   class TCmdProcessorDescription;
}

class art::TCmdProcessorDescription : public TCatCmd {
public:
   TCmdProcessorDescription();
   virtual ~TCmdProcessorDescription();

   TCmdProcessorDescription(const TCmdProcessorDescription& rhs);
   TCmdProcessorDescription& operator=(const TCmdProcessorDescription& rhs);

   Long_t Cmd(vector<TString> tokens);
   
   void Help();

protected:

private:

   ClassDef(TCmdProcessorDescription,1) // show Processor Description
};

#endif // INCLUDE_GUARD_UUID_7E6AFF89_AECC_484B_98B6_05F2C94DEE62
