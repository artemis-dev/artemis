/**
 * @file   TTreeProjectionProcessor.cc
 * @brief  tree projection
 *
 * @date   Created       : 2014-03-05 22:30:06 JST
 *         Last Modified : Jun 15, 2014 11:11:57 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#include "TTreeProjectionProcessor.h"
#include <TTreeProjection.h>
#include <TTree.h>
#include <TFolder.h>
#include <TROOT.h>

using art::TTreeProjectionProcessor;

ClassImp(TTreeProjectionProcessor)

TTreeProjectionProcessor::TTreeProjectionProcessor()
: fTree(NULL),fTreeProj(NULL)
{
}

TTreeProjectionProcessor::~TTreeProjectionProcessor()
{
}

TTreeProjectionProcessor::TTreeProjectionProcessor(const TTreeProjectionProcessor& rhs)
{
}

TTreeProjectionProcessor& TTreeProjectionProcessor::operator=(const TTreeProjectionProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}


void TTreeProjectionProcessor::Init(TEventCollection *col)
{
   TParameterLoader::Init(col);
   if (IsError()) return;
   fTreeProj = static_cast<art::TTreeProjection*>(fParameter);
   

   // prepare tree
   fTree = new TTree("tmptree","tree for variables");
   fTree->SetDirectory(NULL);
   TIter *iter = col->GetIter();
   TEventObject *obj;
   while ((obj = (TEventObject*)iter->Next())) {
      fTree->Branch(obj->GetName(),obj->GetClass()->GetName(),obj->GetObjectRef(),3200000,0);
   }
   fTree->SetCircular(1);

   // sync with tree
   fTreeProj->Sync(fTree);

   TFolder *folder = (TFolder*) gROOT->FindObject("/artemis/loops/loop0");
   if (folder) {
      folder->Add(fTreeProj);
   }
      
}

void TTreeProjectionProcessor::Process()
{
   fTree->GetEntry(1);
   fTreeProj->Project();
}
