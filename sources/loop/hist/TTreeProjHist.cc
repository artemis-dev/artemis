/**
 * @file   TTreeProjHist.cc
 * @brief  Histgram difinition
 *
 * @date   Created       : 2023-06-28 07:44:51 JST
 *         Last Modified : 2024-06-11 19:52:12 JST
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 *
 *    (C) 2023 Shinsuke OTA
 */

#include "TTreeProjHist.h"
#include "TDirectory.h"

ClassImp(art::TTreeProjHist)

art::TTreeProjHist::TTreeProjHist(const char* name, const char* title, TH1* hist, const char* cut)
: TTreeProjComponent(name,title), TAttTreeProj(hist,cut)
{
   hist->SetName(name);
   hist->SetTitle(title);
   hist->SetDirectory(gDirectory);
}



void art::TTreeProjHist::Copy(TObject& obj) const
{
   TTreeProjComponent::Copy(*(TTreeProjComponent*)(&obj));
   TAttTreeProj::Copy(*(TAttTreeProj*)(&obj));
}


