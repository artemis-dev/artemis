/**
 * @file   TTreeProjGroup.h
 * @brief  Collection of the projections of TTree
 *
 * @date   Created       : 2014-03-03 16:59:53 JST
 *         Last Modified : 2024-06-11 19:50:35 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_A9DD712D_B016_4016_81D2_A5CD7EA43842
#define INCLUDE_GUARD_UUID_A9DD712D_B016_4016_81D2_A5CD7EA43842

#include "TTreeProjComponent.h"
#include "TAttCut.h"
#include "TROOT.h"
#include "TDirectoryFile.h"
#include <memory>

namespace art {
   class TTreeProjGroup;
   class TTreeProjHist;
   class TAttTreeProj;
}

class TTree;


class art::TTreeProjGroup : public TTreeProjComponent, public TAttCut {
public:
   TTreeProjGroup();
   TTreeProjGroup(const char *name, const char *title, const char *cut = "", TTreeProjComponent *mother = 0);
   virtual ~TTreeProjGroup();

   TTreeProjGroup(const TTreeProjGroup& rhs);
   TTreeProjGroup& operator=(const TTreeProjGroup& rhs);
   virtual TDirectory* GetMotherDir() const { return fParent->IsGroup() ? ((TTreeProjGroup*)fParent)->GetDirectory() : gROOT; }
   virtual TDirectory* GetDirectory() const { return fDirectory; }
//   virtual T* GetList() const { return fChildren; }
   virtual bool IsGroup() { return true; }

   virtual void Add(TTreeProjHist* child) ;

   virtual void Sync(TTree*, std::vector<TAttTreeProj*>& projs);

   virtual TObject* Clone(const char *newname = "") const;

protected:
   TDirectoryFile *fDirectory; //!
   std::vector<TTreeProjHist*> fChildren; //!
//   TObjArray* fChildren; //! list of children

private:

   ClassDef(TTreeProjGroup,1) // Collection of the projections of TTree
};





#if 0

class art::TTreeProjGroup : public TDirectoryFile, public TAttCut {
public:
   TTreeProjGroup();
   TTreeProjGroup(const char *name, const char *title, const char *cut = "", TDirectory *mother = 0);
   virtual ~TTreeProjGroup();

   TTreeProjGroup(const TTreeProjGroup& rhs);
   TTreeProjGroup& operator=(const TTreeProjGroup& rhs);

   virtual TObject* Clone(const char *newname = "") const;

protected:

private:

   ClassDef(TTreeProjGroup,1) // Collection of the projections of TTree
};
#endif 
#endif // INCLUDE_GUARD_UUID_A9DD712D_B016_4016_81D2_A5CD7EA43842
