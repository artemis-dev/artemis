/**
 * @file   TEventDisplayProcessorHelper.cc
 * @brief  event display helper
 *
 * @date   Created       : 2016-07-13 15:35:58 JST
 *         Last Modified : 2017-06-02 19:08:22 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#include "TEventDisplayProcessorHelper.h"
#include "TEventCollection.h"
#include "TProcessor.h"
#include "TEventHeader.h"
#include <TCanvas.h>
#include <TPad.h>
#include <TPaveLabel.h>
#include <TSystem.h>
#include <TDatime.h>
#include <TVirtualPad.h>

using art::TEventDisplayProcessorHelper;

ClassImp(TEventDisplayProcessorHelper)

TEventDisplayProcessorHelper::TEventDisplayProcessorHelper()
: fCanvas(NULL), fMainPad(NULL), fTitleLabel(NULL), fDateLabel(NULL), fEventInfoLabel(NULL), fWaitTime(1000), fDisplayMode(kQuiet),
   fEventHeader(NULL),fDirNameFmt(""),fDirName(""),fFileNameFmt("")
{
}

TEventDisplayProcessorHelper::TEventDisplayProcessorHelper(const char *name, const char *title)
   : TNamed(name,title),
     fCanvas(NULL), fMainPad(NULL), fTitleLabel(NULL), fDateLabel(NULL), fEventInfoLabel(NULL), fWaitTime(1000), fDisplayMode(kQuiet),
     fEventHeader(NULL),fDirNameFmt(""),fDirName(""),fFileNameFmt("")
{
   
}

TEventDisplayProcessorHelper::~TEventDisplayProcessorHelper()
{
   if (fCanvas) fCanvas->Close();
}

TEventDisplayProcessorHelper::TEventDisplayProcessorHelper(const TEventDisplayProcessorHelper& rhs)
{
}

TEventDisplayProcessorHelper& TEventDisplayProcessorHelper::operator=(const TEventDisplayProcessorHelper& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}


void TEventDisplayProcessorHelper::Register(TProcessor *proc)
{
   proc->RegisterInputCollection("EventHeader","name of event header",fNameEventHeader,TString("eventheader"));
   proc->RegisterProcessorParameter("WaitTime","time wating for next draw (micro second)",fWaitTime,100000);
   proc->RegisterProcessorParameter("DisplayMode",Form("draw flag, %d  skip, %d draw, %d draw and save",kQuiet,kDrawOnly,kDrawSave),
                                    (Int_t&)fDisplayMode,(Int_t)kQuiet);

   proc->RegisterOptionalParameter("DirNameFmt","Name of directory to save the figures",fDirNameFmt,TString("figs/%s"));
   proc->RegisterOptionalParameter("FileNameFmt","Name of files",fFileNameFmt,TString("%s%04d_%05d.png"));
   fParent = proc;
}

void TEventDisplayProcessorHelper::Init(TEventCollection *col)
{
   
   if (fDisplayMode == kQuiet) return;
   fEventHeader = (TEventHeader**)col->GetObjectRef(fNameEventHeader);
   fCanvas =  new TCanvas(Form("c%s",fParent->GetName()),Form("c%s",fParent->GetName()),800,800);
   fCanvas->Clear();
   fCanvas->cd();
   if (!fTitleLabel) {
      fTitleLabel = new TPaveLabel(0.1,0.96,0.9,0.99,"");
      fTitleLabel->SetLabel(this->GetTitle());
   }
   if (!fDateLabel) {
      fDateLabel  = new TPaveLabel(0.5,0.92,0.9,0.95,"");
      fDateLabel->SetFillStyle(0);
   }
   if (!fEventInfoLabel) {
      fEventInfoLabel  = new TPaveLabel(0.1,0.92,0.5,0.95,"");
      fEventInfoLabel->SetFillStyle(0);
   }
   fMainPad = new TPad("graphs","graphs",0.05,0.01,0.95,0.91);
   fMainPad->Draw();
   fDateLabel->Draw();
   fTitleLabel->Draw();
   fEventInfoLabel->Draw();

   fDirName = Form(fDirNameFmt,GetName());
   if (fDisplayMode == kDrawSave) {
      gSystem->Exec(Form("mkdir -p %s",fDirName.Data()));
   }
}

void TEventDisplayProcessorHelper::Process()
{
   if (fDisplayMode == kQuiet) return;
   TDatime now;
   fDateLabel->SetLabel(now.AsString());
   if (fEventHeader) {
      fEventInfoLabel->SetLabel(Form("%s%04d %5lld evt",
                                     (*fEventHeader)->GetRunName().Data(),
                                     (*fEventHeader)->GetRunNumber(),
                                     (*fEventHeader)->GetEventNumber()));
   }
   Int_t i = 1;
   TVirtualPad *pad;
   while ((pad = fMainPad->GetPad(i)) != NULL) {
      pad->Update();
      pad->Modified();
      i++;
   }
   fMainPad->Update();
   fMainPad->Modified();
   fCanvas->Update();
   fCanvas->Modified();

   if (fDisplayMode == kDrawSave && fEventHeader) {
         TString filename = Form(fFileNameFmt.Data(),
                                 (*fEventHeader)->GetRunName().Data(),
                                 (*fEventHeader)->GetRunNumber(),
                                 (*fEventHeader)->GetEventNumber());
         fCanvas->SaveAs(Form("%s/%s",fDirName.Data(),filename.Data()));
   }
   usleep(fWaitTime);
}

void TEventDisplayProcessorHelper::SetWindowSize(UInt_t ww, UInt_t wh)
{
   fCanvas->SetWindowSize(ww,wh);
}
