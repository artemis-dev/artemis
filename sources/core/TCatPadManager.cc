/* $Id:$ */
/**
 * @file   TCatPadManager.cc
 * @date   Created : Feb 06, 2012 19:06:29 JST
 *   Last Modified : 2016-04-16 07:02:05 JST (ota)
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
{
   CreateCanvas();
}
TCatPadManager::~TCatPadManager()
{
}

TCatPadManager* TCatPadManager::Instance()
{
   static TCatPadManager instance;
   return &instance;
}

void TCatPadManager::CreateCanvas()
{
   TDatime now;
   fCanvas = new TCanvas("artcanvas","canvas",800,800);
   fCanvas->Connect("Closed()","TCatPadManager",this,"Closed()");
//   fTitleLabel = new TPaveLabel(0.1,0.96,0.9,0.99,name);
   fDateLabel  = new TPaveLabel(0.7,0.01,0.9,0.03,now.AsString());
   fDateLabel->Draw();
//   fTitleLabel->Draw();
   fMainPad = new TPad("graphs","graphs",0.05,0.05,0.95,0.95);
   fMainPad->Draw();
   fCurrentPadId = 0;
   fNumSubPads = 0;
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
