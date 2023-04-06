/**
 * @file   TCmdFiga.h
 * @brief  fit with gauss function
 *
 * @date   Created       : 2014-06-25 05:34:54 JST
 *         Last Modified : Jun 28, 2014 19:27:56 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_8211C650_B582_4AA9_8D91_0F5B610BD3B9
#define INCLUDE_GUARD_UUID_8211C650_B582_4AA9_8D91_0F5B610BD3B9

#include "TCatCmd.h"

namespace art {
   class TCmdFiga;
}

class TH1;

class art::TCmdFiga : public TCatCmd {
public:
   TCmdFiga();
   virtual ~TCmdFiga();

   TCmdFiga(const TCmdFiga& rhs);
   TCmdFiga& operator=(const TCmdFiga& rhs);

   Long_t Cmd(vector<TString> tokens);
   Long_t Run(TH1 *h, Double_t *c, Double_t *m, Double_t *s);
   void   Help();

protected:
   

private:

   ClassDef(TCmdFiga,1) // fit with gauss function
};

#endif // INCLUDE_GUARD_UUID_8211C650_B582_4AA9_8D91_0F5B610BD3B9
