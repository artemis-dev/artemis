/**
 * @file   TCmdGroup.h
 * @brief  command group
 *
 * @date   Created       : 2016-08-17 16:52:50 JST
 *         Last Modified : 2016-08-18 15:22:12 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_31DA924A_6153_40D8_B413_164FAC6B1FED
#define INCLUDE_GUARD_UUID_31DA924A_6153_40D8_B413_164FAC6B1FED

#include "TCatCmd.h"

namespace art {
   class TCmdGroup;
}

class TObjArray;

class art::TCmdGroup : public TCatCmd {
public:
   TCmdGroup();
   virtual ~TCmdGroup();

   virtual Long_t Cmd(vector<TString> args);


   TCmdGroup(const TCmdGroup& rhs);
   TCmdGroup& operator=(const TCmdGroup& rhs);

   virtual void Register(TCatCmd *cmd, const char *name = NULL, Bool_t replace = kFALSE);

   virtual void SetOptExactName(Bool_t flag) { fFlagExactName = flag; }

   virtual void Help();

   virtual void CmdHelp(TString cmd = "");

   
protected:

   TObjArray *fCmds;

   Bool_t fFlagExactName;
private:
   

   ClassDef(TCmdGroup,1) // command group
};

#endif // INCLUDE_GUARD_UUID_31DA924A_6153_40D8_B413_164FAC6B1FED
