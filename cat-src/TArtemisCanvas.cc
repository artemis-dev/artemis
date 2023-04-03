/**
 * @file   TArtemisCanvas.cc
 * @brief  canvas for artemis object display
 *
 * @date   Created       : 2018-01-28 22:21:16 JST
 *         Last Modified : 2021-04-13 11:59:30 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#include "TArtemisCanvas.h"
#include <TCanvas.h>
#include <TPaveLabel.h>
#include <TLatex.h>
#include <TList.h>
#include <TDatime.h>
#include <TFolder.h>
#include <TROOT.h>
#include <TRunInfo.h>
#include <TTimeStamp.h>
#include "TSystem.h"

using art::TArtemisCanvas;

ClassImp(TArtemisCanvas)

namespace {
   const Int_t kDefaultSize = 600;
}

TArtemisCanvas::CanvasControl_t *TArtemisCanvas::fgCanvasControl = new TArtemisCanvas::CanvasControl_t(1,kFALSE);
std::vector<art::TArtemisCanvas*> TArtemisCanvas::fgCanvases;

TArtemisCanvas::TArtemisCanvas()
   : TNamed(), fCanvas(NULL), fMainPad(NULL), fCurrentID(0),fNumPads(0),
   fSize(), fDivision(), fPosition(), fUpdateInterval(0),
  fLastUpdate(0), fTitleLabel(NULL), fDateLabel(NULL)
{
   Init();
}

TArtemisCanvas::~TArtemisCanvas()
{
   if (fCanvas) fCanvas->Close();
}

TArtemisCanvas::TArtemisCanvas(const TArtemisCanvas& rhs)
{
}

TArtemisCanvas& TArtemisCanvas::operator=(const TArtemisCanvas& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TArtemisCanvas::Init()
{
   if (!fMainPad) {
      if (fDivision[0] <= 0) fDivision[0] = 1;
      if (fDivision[1] <= 0) fDivision[1] = 1;
      if (fSize[0] <= 0) fSize[0] = kDefaultSize;
      if (fSize[1] <= 0) fSize[1] = kDefaultSize;
      
      TDatime now;
      TString name = "";
      if (!fCanvas) {
         if (TString(GetName()).IsNull()) {
            fCanvas = new TCanvas;
            fCanvas->SetWindowPosition(fPosition[0],fPosition[1]);
            fCanvas->SetWindowSize(fSize[0],fSize[1]);
         } else {
            fCanvas = new TCanvas(GetName(),GetTitle(),fPosition[0],fPosition[1],fSize[0],fSize[1]);
         }
         fCanvas->Connect("Closed()",Class_Name(),this,"Closed()");
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
      name = TString::Format("%s_main",GetName());
      fMainPad = new TPad(name,name,0.05,0.01,0.95,0.91);
      fMainPad->Draw();
      fMainPad->Divide(fDivision[0],fDivision[1]);
      fMainPad->cd(1);
      
      fMainPad->Connect("Closed()",Class_Name(),this,"MainPadClosed()");
      fCurrentID = 1;
      fNumPads = fDivision[0] * fDivision[1];
   }
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
            header.Append(Form("  (%ld evt)",info->GetEventNumber()));
         }
      }
      fTitleLabel->SetLabel(header);
   }
   fDateLabel->SetLabel(now.AsString());
   fCanvas->Modified();
   fCanvas->Update();

   SetUpdateInterval(fUpdateInterval);
}
   
void TArtemisCanvas::Divide(Int_t nx, Int_t ny, Float_t xmargin, Float_t ymargin, Int_t color)
{
   fMainPad->Clear();
   if (nx <= 0) nx = 1;
   if (ny <= 0) ny = 1;
   fMainPad->Divide(nx,ny,xmargin,ymargin,color);
   fMainPad->cd(1);
   fDivision[0] = nx;
   fDivision[1] = ny;
   fCurrentID = 1;
   fNumPads = nx * ny;
}

TVirtualPad* TArtemisCanvas::cd(Int_t subpadnumber)
{
   if (fNumPads == 0) Init();
   if (subpadnumber < 1 || subpadnumber > fNumPads) return NULL;
   fCurrentID = subpadnumber;
   return fMainPad->cd(subpadnumber);
}
void TArtemisCanvas::Closed()
{
   fCanvas = 0;
   fMainPad = 0;
   fTitleLabel = 0;
   fDateLabel = 0;
   fCurrentID = 0;
   fNumPads = 0;
}

void TArtemisCanvas::MainPadClosed()
{
   fMainPad = 0;
   fCurrentID = 0;
   fNumPads = 0;
}
                               
void TArtemisCanvas::Update()
{
   TTimeStamp timestamp;
   fLastUpdate = timestamp.GetSec();
   if (!fMainPad || !fCanvas) Init();
   UpdateLabels();
   for (Int_t i=0; i< fNumPads; ++i) {
      fMainPad->GetPad(i+1)->Modified();
      fMainPad->GetPad(i+1)->Update();
   }
}

Bool_t TArtemisCanvas::AutoUpdate()
{
   if (fUpdateInterval == 0) {
      for (std::vector<art::TArtemisCanvas*>::iterator it = fgCanvases.begin(), itend = fgCanvases.end();
           it != itend; ++it) {
         if ((*it) == this) {
            fgCanvases.erase(it);
            return kTRUE;
         }
      }
   }
   // check update or not
   TTimeStamp timestamp;
   if (!fCanvas || !fMainPad) return kTRUE;
   if (timestamp.GetSec() >= GetLastUpdate() + GetUpdateInterval()) {
      Update();
   }
   return kTRUE;
}

void TArtemisCanvas::SetUpdateInterval(Int_t interval)
{
   if (interval < 0) interval = 0;
   fUpdateInterval = interval;
   std::vector<art::TArtemisCanvas*>::iterator itself = fgCanvases.end();
   for (std::vector<art::TArtemisCanvas*>::iterator it = fgCanvases.begin(), itend = fgCanvases.end();
        it != itend; ++it) {
      if ((*it) == this) {
         itself = it;
         break;
      }
   }
   if (!fUpdateInterval) {
      if (itself != fgCanvases.end()) {
         fgCanvases.erase(itself);
      }
   } else {
      if (itself == fgCanvases.end()) fgCanvases.push_back(this);

      if (fgCanvasControl->TasksCount() == fgCanvasControl->SuccessfulTasks()) {
         Control *task = new Control;
         fgCanvasControl->PushTask(*task,fgCanvases);
      }
   } 
}
                                       

void TArtemisCanvas::UpdateLabels()
{
   if (!fDateLabel) Init();
   // update date
   TDatime now;
   fDateLabel->SetLabel(now.AsString());
   // update title
   TFolder *folder = (TFolder*) gROOT->FindObject("/artemis/loops/loop0");
   if (folder) {
      TString header = folder->GetTitle();
      TList *runheader = (TList*)folder->FindObjectAny("runheader");
      if (runheader) {
         art::TRunInfo *info = static_cast<art::TRunInfo*>(runheader->Last());
         if (info) {
            header.Append("   ");
            header.Append(info->GetName());
            header.Append(Form("  (%ld evt)",info->GetEventNumber()));
         }
      }
      fTitleLabel->SetLabel(header);
   }
   
   fCanvas->Modified();
   fCanvas->Update();
}
