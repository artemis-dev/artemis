/* @file TArtTree.cc
 * @brief artemis tree to prevent unexpected segmentation foault
 *
 * @date Create        : 2021-07-17 20:02:21 JST
 *       Last Modified : 2022-10-13 15:25:32 JST (ota)
 * @author:  <ota@ata03.atnet>
 */


#include "TArtTree.h"
#include "TLoopManager.h"
#include "TSystem.h"

ClassImp(art::TArtTree)

using art::TArtTree;

TArtTree::TArtTree()
{
   
 }

TArtTree::TArtTree (const char *name, const char *title, Int_t splitlevel, TDirectory *diry)
   : TTree(name,title,splitlevel,diry)
{
   
}

TArtTree::~TArtTree()
{
}

Long64_t TArtTree::Draw(const char *varexp, const char *selection, Option_t *option, Long64_t nentries, Long64_t firstentry)
{
   SuspendLoop();
   Long64_t ret =TTree::Draw(varexp,selection,option,nentries,firstentry);
   // ResumeLoop();
   return ret;
}

Long64_t TArtTree::Draw(const char *varexp, const TCut &selection, Option_t *option, Long64_t nentries, Long64_t firstentry)
{
   SuspendLoop();
   Long64_t ret =TTree::Draw(varexp,selection,option,nentries,firstentry);
   // ResumeLoop();
   return ret;
}
Long64_t TArtTree::Scan(const char *varexp, const char *selection, Option_t *option, Long64_t nentries, Long64_t firstentry)
{
   SuspendLoop();
   Long64_t ret = TTree::Scan(varexp,selection,option,nentries,firstentry);
   //   ResumeLoop();
   
   return ret;
}


//Int_t TArtTree::Write (const char *name, Int_t option, Int_t bufsize)
//{
//   TTree::Write(name,option,bufsize);
//}
//
//Int_t TArtTree::Write (const char *name, Int_t option, Int_t bufsize) const
//{
//   TTree::Write(name,option,bufsize);
//}
//
void TArtTree::SuspendLoop() {
   TLoop *loop = TLoopManager::Instance()->GetLoop(0);
   if (!loop) return;
   TLoopManager::Instance()->Suspend(0);
   while (loop->IsRunning()) {
      gSystem->Sleep(10);
   } 
}

void TArtTree::ResumeLoop() {
   if (TLoopManager::Instance()->GetLoop(0)) {
      if (TLoopManager::Instance()->GetLoop(0)->IsDone()) return;
      TLoopManager::Instance()->Resume(0);
   }
}
