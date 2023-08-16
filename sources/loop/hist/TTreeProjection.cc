/**
 * @file   TTreeProjection.cc
 * @brief  Tree projection definitions
 *
 * @date   Created       : 2014-03-05 10:15:05 JST
 *         Last Modified : 2023-08-16 16:19:00 JST (fendo)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#include "TTreeProjection.h"
#include <TObjArray.h>
#include <yaml-cpp/yaml.h>
#include <TSystem.h>
#include <TObjString.h>
#include <TTreeProjGroup.h>
#include <TH1FTreeProj.h>
#include <TH2FTreeProj.h>
#include <TH3FTreeProj.h>
#include <TAxisTreeProj.h>
#include <TTree.h>
#include <fstream>
#include <sstream>

using art::TTreeProjection;

ClassImp(TTreeProjection)


// definition of constant strings for the key of each node
namespace {
   const char* kNodeKeyGroup = "group";
   const char* kNodeKeyInclude = "include";
   const char* kNodeKeyAlias = "alias";
   const char* kNodeKeyReplace = "replace";
   const char* kNodeKeyCut = "cut";
   const char* kNodeKeyName = "name";
   const char* kNodeKeyTitle = "title";
   const char* kNodeKeyType = "type";
   const char* kNodeKeyContents = "contents";
   const char* kNodeKeyClone = "clone";
   const char* kNodeKeySuffix = "suffix";
   const char* kNodeKeyX = "x";
   const char* kNodeKeyY = "y";
   const char* kNodeKeyZ = "z";
   const char* kNodeKeyAsync = "async";
}

TTreeProjection::TTreeProjection()
   :  fProjGroups(NULL), fIncludes(NULL), fAliases(NULL)
{
   fProjGroups = new TObjArray;
   fIncludes   = new TObjArray;
   fAliases    = new TObjArray;
}

TTreeProjection::~TTreeProjection()
{
   fProjGroups->Delete();
   delete fProjGroups;
   fIncludes->Delete();
   delete fIncludes;
   fAliases->Delete();
   delete fAliases;
}

TTreeProjection::TTreeProjection(const TTreeProjection& rhs)
{
   // not implemented
   MayNotUse("TTreeProjection");
}

TTreeProjection& TTreeProjection::operator=(const TTreeProjection& rhs)
{
   // not implemented
   MayNotUse("operator=");
   if (this != &rhs) {

   }
   return *this;
}

Bool_t TTreeProjection::LoadYAMLNode(const YAML::Node &node)
{
   // local variables
   const char *kMethodName = "LoadYAMLNode";
   // node shoud have at least group and may have 'include' and 'cut'
   const YAML::Node groupNode   = node[kNodeKeyGroup];
   const YAML::Node aliasNode   = node[kNodeKeyAlias];
   const YAML::Node includeNode = node[kNodeKeyInclude];

   Info(kMethodName,"loading tree projection");
   if (fBaseDir.IsNull()) {
      fBaseDir = gSystem->DirName(fCurrentFile);
   }

#if 0
   // node itself exists since the Node::operator[] ensure its existense
   if (!groupNode && !includeNode) {
   }
#endif
   if (!groupNode && !includeNode) {
      
      Error(kMethodName,"At least one group or include node should be contained");
      return kFALSE;
   }
   //======================================================================
   // include node
   //======================================================================
   if (includeNode) {
      printf("include\n");
      // prepare includes before continue
      TString filename;
      try {
         for (YAML::const_iterator it = includeNode.begin(); it != includeNode.end(); ++it) {
            std::string name;
            // load template
            if (it->Type() == YAML::NodeType::Map) {
               const YAML::Node replaceNode = (*it)[kNodeKeyReplace];
               const YAML::Node nameNode = (*it)[kNodeKeyName];
               if (nameNode.IsNull()) {
                  Error("LoadFile","no 'name' node is found");
                  return kFALSE;
               }
               name = nameNode.as<std::string>();
               if (replaceNode.IsNull()) {
                  Warning("LoadFile","no 'replace' node is found while it is expected in '%s'",name.c_str());
               }
               filename = gSystem->ConcatFileName(fBaseDir,name.c_str());
               std::ifstream fin(filename.Data());
               if (!fin.is_open()) {
                  Error("LoadFile", "Cannot open file: %s",filename.Data());
                  return kFALSE;
               }
               YAML::Node doc;
               TString lines;
               lines.ReadFile(fin);
               if (replaceNode) {
                  for (YAML::const_iterator it = replaceNode.begin(), itend = replaceNode.end();
                       it != itend; ++it) {
                     std::string key, value;
                     key = it->first.as<std::string>();
                     value = it->second.as<std::string>();
                     lines.ReplaceAll(TString::Format("@%s@",key.c_str()),value);
                  }
               }
               if (!LoadYAMLNode(YAML::Load(lines.Data()))) {
                  return kFALSE;
               }
            } else {
               // load file simply 
               printf("loading file\n");
               name = it->as<std::string>();
               filename = gSystem->ConcatFileName(fBaseDir,name.c_str());
               TString dirsaved = fBaseDir;
               
//            gSystem->FindFile(fSearchPath,filename);
               if (filename.IsNull()) {
                  Warning(kMethodName,"File '%s' in include node does not exist",name.c_str());
                  continue;
               }
               if (LoadYAMLFile(filename)) {
                  fIncludes->Add(new TObjString(filename));
               } else {
                  Error("LoadYAMLNode","Error while loading file '%s'",filename.Data());
                  return kFALSE;
               }
               fBaseDir = dirsaved;
            }
         }
      } catch (YAML::Exception &e){
         Error(kMethodName,"In %s, %s",filename.Data(),e.what());
         return kFALSE;
      }
   }

   //======================================================================
   // alias node
   //======================================================================
   if (aliasNode) {
      for (YAML::const_iterator it = aliasNode.begin(); it != aliasNode.end(); it++) {
         try {
            TString name = it->first.as<std::string>().c_str();
            TString title = it->second.as<std::string>().c_str();
            fAliases->Add(new TNamed(name,title));
         } catch (YAML::Exception &e) {
            Error(kMethodName,"%s",e.what());
            return kFALSE;
         }
      }
   }
   if (!groupNode) return kTRUE;
   //======================================================================
   // group node
   //======================================================================
   TDirectory *saved = gDirectory;
   for (YAML::const_iterator it = groupNode.begin(); it != groupNode.end(); it++) {
      saved->cd();
      TString name, title, cut, suffix;
      TTreeProjGroup *group = NULL;
      try {
         name = (*it)[kNodeKeyName].as<std::string>().c_str();
         title = (*it)[kNodeKeyTitle].as<std::string>().c_str();
      } catch (YAML::Exception &e) {
         Warning(kMethodName,"group skipped since name or title is not defined (%s)",e.what());
         continue;
      }
      if (const YAML::Node c = (*it)[kNodeKeyCut]) {
         cut = c.as<std::string>().c_str();
      }
      // check if the cloning the object or not
      const YAML::Node contents = (*it)[kNodeKeyContents];
      const YAML::Node clone = (*it)[kNodeKeyClone];
      const YAML::Node nodeSuffix = (*it)[kNodeKeySuffix];
      if (nodeSuffix) {
         suffix = nodeSuffix.as<std::string>().c_str();
      }
      if (clone) {
         if (TObject *obj = fProjGroups->FindObject(clone.as<std::string>().c_str())) {
            // cloning the existing group;
            group = dynamic_cast<TTreeProjGroup*>(obj->Clone(name));
            group->SetTitle(title);
            group->GetCut() += cut;
         } else {
            Warning(kMethodName,"group skipped since the group '%s' does not exist",name.Data());
            continue;
         }
      } else if (contents) {
         // new group is created
         Int_t nSame = 0;
         TString groupName = name;
         while (gDirectory->Get(groupName)) {
            ++nSame;
            groupName = Form("%s%d",name.Data(),nSame);
         }
         group = new TTreeProjGroup(groupName,title,cut);
      } else {
         // invalid declaration of group
         Warning(kMethodName,"group skipped since the group '%s' does not contain any clones or contents",
                 name.Data());
         continue;
      }
      
      // group should be created here
      Info(kMethodName,"new group '%s'",name.Data());
      fProjGroups->Add(group);
      if (!contents) {
         // changing name of histogram will destroy the structure of link
         TList *savedlist = new TList;
         TObjLink *link = group->GetList()->FirstLink();
         while (link){
            savedlist->Add(link->GetObject());
            link = link->Next();
         }
         link = savedlist->FirstLink();
         while (link) {
            TH1* h1 = dynamic_cast<TH1*>(link->GetObject());
            if (!h1) continue; // Object may be group. not implemented yet
            h1->SetName(TString(h1->GetName()).Append(suffix));
            link = link->Next();
         }
         saved->cd();
         continue;
      }
      group->cd();

      for (YAML::const_iterator itcont = contents.begin(); itcont != contents.end(); itcont++) {
         TString hname, htitle, hcut, hasync;
         TAttTreeProj *hproj = NULL;
         Int_t nDim = 0;
         const YAML::Node axisnode[3] =
            {(*itcont)[kNodeKeyX],
            (*itcont)[kNodeKeyY],
            (*itcont)[kNodeKeyZ]};
         try {
            hname = (*itcont)["name"].as<std::string>().c_str();
            htitle = (*itcont)["title"].as<std::string>().c_str();
         }  catch (YAML::Exception &e) {
            Warning(kMethodName,"projection skipped since name or title is not defined (%s)",e.what());
            continue;
         }
         if (const YAML::Node c = (*itcont)[kNodeKeyCut]) {
            hcut = c.as<std::string>().c_str();
         }
         if (const YAML::Node n = (*itcont)[kNodeKeyAsync]) {
            hasync = n.as<std::string>().c_str();
         }
         if (const YAML::Node n = (*itcont)[kNodeKeySuffix]) {
            hname.Append(n.as<std::string>().c_str());
         }
         // apped group suffix
         hname.Append(suffix);
         // check if the cloning the object or not
         if (const YAML::Node clone = (*itcont)[kNodeKeyClone]) {
            if (TObject *obj = group->FindObject(clone.as<std::string>().c_str())) {
               // cloning the existing group;
               hproj = dynamic_cast<TAttTreeProj*>(obj->Clone(hname));
               hproj->SetProjTitle(htitle);
               hproj->GetCut() += hcut;
               hproj->SetAxisAsync(hasync.Atoi());
            } else {
               Warning(kMethodName,"projection skipped since the group '%s' does not exist",name.Data());
               continue;
            }
         } else {
            // check if axisnode has enough number of inputs
            for (Int_t i = 0; i < 3; ++i) {
               if (axisnode[i] && axisnode[i].size() < 4) {
                  // insufficient number of input for axis
                  Error("LoadYAMLNode","\n    histgram '%s': insufficient number of axis input for '%c' (size = %d)\n",
                                                       hname.Data(),'x'+i,axisnode[i].size());
                  saved->cd();
                  return kFALSE;
               }
            }
         
            if (axisnode[2]) {
               // three dimensional not implimented yet
               if (!axisnode[0] || ! axisnode[1]) {
                  Warning(kMethodName,"projection '%s' in group '%s' skipped since x or y axis is not defined",
                          hname.Data(),name.Data());
                  continue;
               }
               hproj = new TH3FTreeProj(hname,htitle,hcut);
               hproj->SetAxisAsync(hasync.Atoi());
               nDim = 3;
            } else if (axisnode[1]) {
               if (!axisnode[0]) {
                  Warning(kMethodName,"projection '%s' in group '%s' skipped since x axis is not defined",
                          hname.Data(),name.Data());
                  continue;
               }
               hproj = new TH2FTreeProj(hname,htitle,hcut);
               hproj->SetAxisAsync(hasync.Atoi());
               nDim = 2;
            } else if (axisnode[0]) {
               // 1D histogram
               hproj = new TH1FTreeProj(hname,htitle,hcut);
               hproj->SetAxisAsync(hasync.Atoi());
               nDim = 1;
            } else {
               Warning(kMethodName,"projection '%s' in group '%s' skipped since no axis is defined",
                       hname.Data(),name.Data());
               continue;
            }
         }
         for (Int_t i=0; i!=nDim; i++) {
            const YAML::Node &a = axisnode[i];
            try {
               TCut axiscut;
               TString axistitle;
               if (a.size()>5) axiscut = a[5].as<std::string>().c_str();
               if (a.size()>4) axistitle = a[4].as<std::string>().c_str();
               TAxisTreeProj *axis = new TAxisTreeProj(a[0].as<std::string>().c_str(),
                                                       a[1].as<int>(),
                                                       a[2].as<double>(),
                                                       a[3].as<double>(),
                                                       axiscut);
               hproj->SetAxisDef(i,axis);
//               if (!axistitle.IsNull()) {
//                  switch (i) {
//                  case 0:
//                     hproj->SetXtitle(axistitle);
//                     break;
//                  case 1: 
//                     hproj->SetYtitle(axistitle);
//                     break;
//                  case 2:
//                     hproj->SetZtitle(axistitle);
//                     break;
//                  }
//               }
            } catch (YAML::Exception &e) {
               Warning(kMethodName,"projection '%s' in group '%s' skipped axis %d is invalid",
                       hname.Data(),name.Data(),i);
               delete hproj;
               hproj = NULL;
               break;
            }
         }
      }
   }
   saved->cd();
      
   return kTRUE;
}

Bool_t TTreeProjection::Sync(TTree *tree)
{
   Info("Sync","synchronizing");
   Int_t nAlias = fAliases->GetEntriesFast();
   for (Int_t i = 0; i!=nAlias; i++) {
      TNamed *named = (TNamed*)fAliases->At(i);
      tree->SetAlias(named->GetName(),named->GetTitle());
   }
   Int_t nGroup = fProjGroups->GetEntriesFast();
   for (Int_t i = 0; i != nGroup; i++) {
      TTreeProjGroup *group = (TTreeProjGroup*)fProjGroups->At(i);
      Sync(group,tree,group->GetCut());
   }
   return kFALSE;
}


Bool_t TTreeProjection::Sync(TTreeProjGroup *group, TTree *tree, TCut cut)
{
   TIter next(group->GetList());
   while (TObject *obj = next()) {
      if (obj->InheritsFrom(TTreeProjGroup::Class())) {
         TTreeProjGroup *group = (TTreeProjGroup*)obj;
         Sync((TTreeProjGroup*)obj,tree,cut + group->GetCut());
      } else if (obj->InheritsFrom(TH1::Class())) {
         Int_t nDim = ((TH1*)obj)->GetDimension();
         TAttTreeProj *proj = dynamic_cast<TAttTreeProj*>(obj);
         for (Int_t i=0; i!=nDim; i++) {
            TAxisTreeProj* axis = proj->GetAxisDef(i);
            TCut totalcut = axis->GetCut() + proj->GetCut() + cut;
            TTreeFormula *axisFormula =new TTreeFormula(TString::Format("V%c",'x'+i),axis->GetTitle(),tree);
            if (axisFormula->GetNdim() == 0) {
               axisFormula->Delete();
               proj = NULL;
               break;
            }
            axis->SetVariableFormula(axisFormula);
            if (totalcut != "") {
               TTreeFormula *cutFormula = new TTreeFormula(TString::Format("S%c",'x'+i),
                                                           axis->GetCut()+proj->GetCut()+cut,tree);
               if (cutFormula->GetNdim() == 0) {
                  cutFormula->Delete();
                  proj = NULL;
                  break;
               }
               axis->SetSelectionFormula(cutFormula);
            }
         }
         if (proj) {
            proj->Sync();
            fProjections.push_back(proj);
         }
      }
   }
   return kTRUE;
}

