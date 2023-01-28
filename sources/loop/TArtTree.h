/* @file TArtTree.h
 * @brief description
 *
 * @date Create        : 2021-07-17 19:42:51 JST
 *       Last Modified : 2021-07-17 22:36:55 JST (ota)
 * @author:  <ota@ata03.atnet>
 */

#ifndef INCLUDE_GUARD_UUID_552D1ACF_1583_40A0_82BC_47674BD95155
#define INCLUDE_GUARD_UUID_552D1ACF_1583_40A0_82BC_47674BD95155

#include "TTree.h"

namespace art {
   class TArtTree;
}


class art::TArtTree : public TTree {
public:
   TArtTree();
   TArtTree (const char *name, const char *title, Int_t splitlevel=99, TDirectory *dir=gDirectory);
   

   virtual ~TArtTree();

   virtual Long64_t Draw(const char *varexp, const char *selection="", Option_t *option="", Long64_t nentries=kMaxEntries, Long64_t firstentry=0);
   virtual Long64_t Draw(const char *varexp, const TCut &selection, Option_t *option="", Long64_t nentries=kMaxEntries, Long64_t firstentry=0);
   virtual Long64_t Scan(const char *varexp="", const char *selection="", Option_t *option="", Long64_t nentries=kMaxEntries, Long64_t firstentry=0);
   virtual Int_t 	Write (const char *name=0, Int_t option=0, Int_t bufsize=0);
   virtual Int_t 	Write (const char *name=0, Int_t option=0, Int_t bufsize=0) const;

   virtual void SuspendLoop();
   virtual void ResumeLoop();
   

//   ClassDef(TArtTree,1)
}; // end of TArtTree

#endif // #ifndef INCLUDE_GUARD_UUID_552D1ACF_1583_40A0_82BC_47674BD95155}
