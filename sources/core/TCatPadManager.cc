/* $Id:$ */
/**
 * @file   TCatPadManager.cc
 * @date   Created : Feb 06, 2012 19:06:29 JST
 *   Last Modified : 2016-04-17 05:56:35 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatPadManager.h"
#include <TPaveLabel.h>
#include <TList.h>
#include <TDatime.h>

TCatPadManager::TCatPadManager()
   : fCanvas(NULL), fMainPad(NULL), fCurrentPadId(0), fNumSubPads(0), fTitleLabel(NULL), fDateLabel(NULL)
{
   //CreateCanvas();
   fTitleLabel = new TPaveLabel(0.1,0.96,0.9,0.99,"");
   fDateLabel  = new TPaveLabel(0.7,0.01,0.9,0.03,"");
}
TCatPadManager::~TCatPadManager()
{
   if (fTitleLabel) delete fTitleLabel;
   if (fDateLabel) delete fDateLabel;
   
}

TCatPadManager* TCatPadManager::Instance()
{
   static TCatPadManager instance;
   return &instance;
}

void TCatPadManager::CreateCanvas()
{
   TDatime now;
   TString name = "";
   fCanvas = new TCanvas("artcanvas","canvas",800,800);
   fCanvas->Connect("Closed()","TCatPadManager",this,"Closed()");
   fDateLabel->Draw();
   fTitleLabel->Draw();
   fMainPad = new TPad("graphs","graphs",0.05,0.05,0.95,0.95);
   fMainPad->Draw();
   fMainPad->cd();
   fCurrentPadId = 0;
   fNumSubPads = 0;
}

void TCatPadManager::SetTitle(const char* title)
{
   Instance()->GetTitleLabel()->SetLabel(title);
}
void TCatPadManager::SetComment(const char* comment)
{
//   Instance()->GetTitleLabel()->SetLabel(title);
}

Int_t TCatPadManager::GetNumChild()
{
   return fNumSubPads;
}

Bool_t TCatPadManager::HasChild()
{
   return GetNumChild() ? kTRUE : kFALSE;
}

TVirtualPad *TCatPadManager::Next()
{
   GetCanvas();
   if (!HasChild()) {
      fCurrentPadId = 0;
      return fMainPad->cd(0);
   } else if (fCurrentPadId + 1 <= GetNumChild()) {
      fCurrentPadId++;
      return fMainPad->cd(fCurrentPadId);
   } else {
      return fMainPad->cd((fCurrentPadId=1));
   }
}

TVirtualPad *TCatPadManager::Current()
{
   return fMainPad->GetPad(fCurrentPadId);
}

TVirtualPad* TCatPadManager::Get(Int_t idx)
{
   return fMainPad->GetPad(idx);
}

TVirtualPad *TCatPadManager::GetCanvas() 
{
   TDatime now;
   if (!fMainPad) CreateCanvas();
   fDateLabel->SetLabel(now.AsString());
   fCanvas->Modified();
   fCanvas->Update();
      
   return fMainPad;
}


void TCatPadManager::Closed()
{
   fCanvas = 0;
   fMainPad = 0;
   fTitleLabel = 0;
   fDateLabel = 0;
   fCurrentPadId = 0;
   fNumSubPads = 0;
}
void TCatPadManager::Divide(Int_t nx, Int_t ny, 
                            Float_t xmargin, Float_t ymargin)
{
   GetCanvas()->Clear();
   fMainPad->Divide(nx,ny,xmargin,ymargin);
   fCurrentPadId = 0;
   fNumSubPads = nx * ny;
}
