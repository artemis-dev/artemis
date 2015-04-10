/**
 * @file   TCmdClassInfo.h
 * @brief  show artemis class information
 *
 * @date   Created       : 2015-04-09 21:00:48 JST
 *         Last Modified : Apr 10, 2015 18:20:01 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015
 */

#ifndef INCLUDE_GUARD_UUID_334DEB98_02AB_4177_8035_43428F2B6E25
#define INCLUDE_GUARD_UUID_334DEB98_02AB_4177_8035_43428F2B6E25

#include "TCatCmd.h"

namespace art {
   class TCmdClassInfo;
}

class art::TCmdClassInfo : public TCatCmd {
public:
   TCmdClassInfo();
   virtual ~TCmdClassInfo();

   TCmdClassInfo(const TCmdClassInfo& rhs);
   TCmdClassInfo& operator=(const TCmdClassInfo& rhs);

   virtual Long_t Cmd(std::vector<TString> args); // parse command line

   virtual Long_t Run(const char *name); // execute 

   virtual void Help();

protected:

private:

   ClassDef(TCmdClassInfo,1) // show class specific information
};

#endif // INCLUDE_GUARD_UUID_334DEB98_02AB_4177_8035_43428F2B6E25
