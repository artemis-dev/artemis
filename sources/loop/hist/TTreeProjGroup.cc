/**
 * @file   TTreeProjGroup.cc
 * @brief  Collection of the projections of TTree
 *
 * @date   Created       : 2014-03-03 17:01:02 JST
 *         Last Modified : Mar 05, 2014 17:52:17 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#include "TTreeProjGroup.h"
#include <TROOT.h>
#include <TH1.h>

using art::TTreeProjGroup;

ClassImp(TTreeProjGroup)


TTreeProjGroup::TTreeProjGroup()
: TDirectoryFile(), TAttCut()
{
}

TTreeProjGroup::TTreeProjGroup(const char *name, const char *title, const char *cut, TDirectory *mother)
   : TDirectoryFile(name,title,TTreeProjGroup::Class_Name(),mother),TAttCut(cut)
{
}

TTreeProjGroup::~TTreeProjGroup()
{
}

TTreeProjGroup::TTreeProjGroup(const TTreeProjGroup& rhs)
{
   MayNotUse("TTreeProjGroup");
}

TTreeProjGroup& TTreeProjGroup::operator=(const TTreeProjGroup& rhs)
{
   MayNotUse("operator=");
   return *this;
}


TObject *TTreeProjGroup::Clone(const char *newname) const
{
   TDirectory *saved = gDirectory;
   TDirectory *mother = GetMotherDir();
   printf("this (%s) mother = %p %s\n",GetName(),mother,mother?mother->GetName():"");
   if (!mother) mother = gROOT;
   TString name(newname);
   if (name.IsNull()) name = GetName();
   TTreeProjGroup *newgroup = new TTreeProjGroup(name,GetTitle(),GetCut(),mother);
   newgroup->cd();
   TList *list = GetList();
   TIter next(list);
   TObject *obj = NULL;
   while ((obj = next())) {
      if (obj->InheritsFrom(TH1::Class())) {
         // cloning histogram in usual way
         TObject *newobj = obj->Clone();
         ((TH1*)newobj)->SetDirectory(newgroup);
      } else if (obj->InheritsFrom(TTreeProjGroup::Class())) {
         // cloning TTreeProjGroup. (not directory since TDirectory does not support Clone())
         TTreeProjGroup *oldgroup =  (TTreeProjGroup*)obj;
         oldgroup->SetMother(newgroup);
         TNamed *obj = (TNamed*)oldgroup->Clone();
         oldgroup->SetMother(mother);
      } else {
         // I don't care about the other object for now :D
         newgroup->Add(obj->Clone());
      }
   }
   saved->cd();
   return newgroup;
}
