/**
 * @file   TCmdBranchInfo.h
 * @brief  display branches in tree
 *
 * @date   Created       : 2015-04-09 14:37:19 JST
 *         Last Modified : Apr 10, 2015 18:21:39 JST
 * @author  Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015
 */

#ifndef INCLUDE_GUARD_UUID_081784A8_9DCE_4499_8850_61A023CAD166
#define INCLUDE_GUARD_UUID_081784A8_9DCE_4499_8850_61A023CAD166

#include "TCatCmd.h"

namespace art {
   class TCmdBranchInfo;
}

class TTree;
class TClass;
class TBranch;
class art::TCmdBranchInfo : public TCatCmd {
public:
   TCmdBranchInfo();
   virtual ~TCmdBranchInfo();

   TCmdBranchInfo(const TCmdBranchInfo& rhs);
   TCmdBranchInfo& operator=(const TCmdBranchInfo& rhs);
   
   virtual Long_t Cmd(vector<TString> args);
   virtual Long_t Run(TTree *tree);
   virtual void Help();

   TTree*  GetTree();
   TClass* GetConcreteClass(TTree* tree, TBranch *br, Bool_t* isClonesArray);

protected:

private:

   ClassDef(TCmdBranchInfo,1) // display branches in tree
};

#endif // INCLUDE_GUARD_UUID_081784A8_9DCE_4499_8850_61A023CAD166
