/* $Id:$ */
/**
 * @file   TCatPadManager.cc
 * @date   Created : Feb 06, 2012 19:06:29 JST
 *   Last Modified : 2016-04-19 04:36:17 JST (nil)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatPadManager.h"
#include <TPaveLabel.h>
#include <TLatex.h>
#include <TList.h>
#include <TDatime.h>
#include <TFolder.h>
#include <TROOT.h>
#include <TRunInfo.h>

TCatPadManager::TCatPadManager()
   : fCanvas(NULL), fMainPad(NULL), fCurrentPadId(0), fNumSubPads(0), fTitleLabel(NULL), fDateLabel(NULL)
{
   //CreateCanvas();
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
   if (!fCanvas) {
      fCanvas = new TCanvas("artcanvas","canvas",800,800);
      fCanvas->Connect("Closed()","TCatPadManager",this,"Closed()");
   }
   fCanvas->Clear();
   fCanvas->cd();
   if (!fTitleLabel) {
      fTitleLabel = new TPaveLabel(0.1,0.96,0.9,0.99,"");
   }
   if (!fDateLabel) {
      fDateLabel  = new TPaveLabel(0.7,0.93,0.9,0.95,"");
      fDateLabel->SetBorderSize(0);
      fDateLabel->SetFillStyle(0);
   }
   fDateLabel->Draw();
   fTitleLabel->Draw();
   fMainPad = new TPad("graphs","graphs",0.05,0.01,0.95,0.91);
   fMainPad->Draw();
   fMainPad->cd();

   fMainPad->Connect("Closed()","TCatPadManager",this,"MainPadClosed()");
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
   GetCanvas();
   return fMainPad->cd(fCurrentPadId);
}

TVirtualPad* TCatPadManager::Get(Int_t idx)
{
   GetCanvas();
   return fMainPad->GetPad(idx);
}

TVirtualPad *TCatPadManager::GetCanvas() 
{
   if (!fMainPad) CreateCanvas();
   TDatime now;
   TFolder *folder = (TFolder*) gROOT->FindObject("/artemis/loops/loop0");
   if (folder) {
      TString header = folder->GetTitle();
      TList *runheader = (TList*)folder->FindObjectAny("runheader");
      if (runheader) {
         art::TRunInfo *info = static_cast<art::TRunInfo*>(runheader->Last());
         if (info) {
            header.Append("   ");
            header.Append(info->GetName());
            header.Append(Form("  (%d evt)",info->GetEventNumber()));
         }
      }
      fTitleLabel->SetLabel(header);
   }
   fDateLabel->SetLabel(now.AsString());
   fCanvas->Modified();
   fCanvas->Update();
      
   return fCanvas;
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

void TCatPadManager::MainPadClosed()
{
   fMainPad = 0;
   fCurrentPadId = 0;
   fNumSubPads = 0;
}
void TCatPadManager::Divide(Int_t nx, Int_t ny, 
                            Float_t xmargin, Float_t ymargin)
{
   GetCanvas();
   fMainPad->Clear();
   fMainPad->Divide(nx,ny,xmargin,ymargin);
   fCurrentPadId = 0;
   fNumSubPads = nx * ny;
}

void TCatPadManager::SetCurrentPadId(Int_t id)
{
   if (id < 1 || id > GetNumChild()) return;
   fCurrentPadId = id;
}
