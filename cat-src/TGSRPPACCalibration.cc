/**
 * @file   TGSRPPACCalibration.cc
 * @brief  SRPPAC calibration GUI
 *
 * @date   Created      : 2019-08-30 18:25:57 JST (ota)
 *         Last Modified: 2019-08-30 19:04:57 JST (ota)
 * @author OTA Shinsuke <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2019 OTA Shinsuke
 */

//_________________________________
// TGSRPPACCalibration
//
// Provides a GUI-based user-friendly calibration tool for BLD-SRPPAC.

#include "TGSRPPACCalibration.h"

#include <limits>
#include <fstream>

#include <TGClient.h>
#include <TGLabel.h>
#include <TGButton.h>
#include <TGNumberEntry.h>
#include <TGDoubleSlider.h>
#include <TRootEmbeddedCanvas.h>
#include <TGFileDialog.h>
#include <TGInputDialog.h>
#include <TGMsgBox.h>
#include <TGComboBox.h>
#include <TGListBox.h>
#include <TCatCmdXval.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TObjArray.h>

using art::TGSRPPACCalibration;

ClassImp(TGSRPPACCalibration);

TString art::TGSRPPACCalibration::fgSRPPACParamDir = "prm/srppac";
TString art::TGSRPPACCalibration::fgCalibParamSubDir = "dq2dl";

TGSRPPACCalibration::TGSRPPACCalibration(const TGWindow *p, UInt_t w, UInt_t h, TObjArray *a)
   : fHt(NULL), fHtl(NULL), fHists(a), fLastDir(gSystem->ConcatFileName(gSystem->pwd(),fgSRPPACParamDir))
{
   // constructor
   if (!a) return;
   fHt = dynamic_cast<TH1D*>(a->First());
   if (!fHt) return;

   Build(p,w,h);
   DoDraw();
}

TGSRPPACCalibration::~TGSRPPACCalibration()
{
   if (fMain) fMain->CloseWindow();
   delete fHtl;
}

void TGSRPPACCalibration::Build(const TGWindow *p, UInt_t w, UInt_t h)
{
   // Build main frame

   const Double_t kMin = fHt->GetXaxis()->GetXmin();
   const Double_t kMax = fHt->GetXaxis()->GetXmax();

   fXRange[0] = fHt->GetXaxis()->GetBinLowEdge(fHt->GetXaxis()->GetFirst());
   fXRange[1] = fHt->GetXaxis()->GetBinLowEdge(fHt->GetXaxis()->GetLast() + 1);

   fMain = new TGMainFrame(p,w,h);
   fMain->SetClassHints("ROOT","art::TGSRPPACCalibration");
   fMain->SetCleanup(kDeepCleanup);

   fECanvas = new TRootEmbeddedCanvas("Ecanvas",fMain,
				      gStyle->GetCanvasDefW(),
				      gStyle->GetCanvasDefH());
   fMain->AddFrame(fECanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,
					       2, 2, 2, 1));

   TCanvas *fCanvas = fECanvas->GetCanvas();
   fCanvas->Divide(1,2);

   Int_t id = 0;

   TGHorizontalFrame *panel = new TGHorizontalFrame(fMain);

   TGHorizontalFrame *fcombo = new TGHorizontalFrame(panel);
   fComboSRPPAC = new TGComboBox(fcombo,id++);
   fComboSRPPAC->Resize(75,20);
   fcombo->AddFrame(fComboSRPPAC, new TGLayoutHints(kLHintsLeft | kLHintsCenterY,2,2,2,2));

   fComboPlane = new TGComboBox(fcombo,id++);
   fComboPlane->Resize(50,20);
   fcombo->AddFrame(fComboPlane, new TGLayoutHints(kLHintsLeft | kLHintsCenterY,2,2,2,2));

   TGPictureButton *prev = new TGPictureButton(fcombo,gClient->GetPicture("previous_t.xpm"),id++);
   fcombo->AddFrame(prev, new TGLayoutHints(kLHintsLeft | kLHintsCenterY,2,2,2,2));

   TGPictureButton *next = new TGPictureButton(fcombo,gClient->GetPicture("next_t.xpm"),id++);
   fcombo->AddFrame(next, new TGLayoutHints(kLHintsLeft | kLHintsCenterY,2,2,2,2));

   TGPictureButton *draw = new TGPictureButton(fcombo,gClient->GetPicture("tb_refresh.xpm"),id++);
   fcombo->AddFrame(draw, new TGLayoutHints(kLHintsLeft | kLHintsCenterY,2,2,2,2));

   TGLabel *minLabel = new TGLabel(fcombo, "Min:");
   fcombo->AddFrame(minLabel,new TGLayoutHints(kLHintsCenterY,2,2,2,2));
   fNEMin = new TGNumberEntryField(fcombo,id++,fXRange[0],
				   TGNumberFormat::kNESReal,
				   TGNumberFormat::kNEAAnyNumber,
				   TGNumberFormat::kNELLimitMinMax,
				   kMin,kMax);
   fNEMin->SetWidth(60);
   fcombo->AddFrame(fNEMin,new TGLayoutHints(kLHintsCenterY,2,2,2,2));

   TGLabel *maxLabel = new TGLabel(fcombo, "Max:");
   fcombo->AddFrame(maxLabel,new TGLayoutHints(kLHintsCenterY,2,2,2,2));
   fNEMax = new TGNumberEntryField(fcombo,id++,fXRange[1],
				   TGNumberFormat::kNESReal,
				   TGNumberFormat::kNEAAnyNumber,
				   TGNumberFormat::kNELLimitMinMax,
				   kMin,kMax);
   fNEMax->SetWidth(60);
   fcombo->AddFrame(fNEMax,new TGLayoutHints(kLHintsCenterY,2,2,2,2));

   fBXval = new TGPictureButton(fcombo,gClient->GetPicture("selection_t.xpm"),id++);
   fcombo->AddFrame(fBXval, new TGLayoutHints(kLHintsCenterY,2,2,2,2));

   fBReset = new TGPictureButton(fcombo,gClient->GetPicture("sm_delete.xpm"),id++);
   fcombo->AddFrame(fBReset, new TGLayoutHints(kLHintsCenterY,2,2,2,2));

   fBSaveSingle = new TGPictureButton(fcombo,gClient->GetPicture("filesaveas.xpm"),id++);
   fcombo->AddFrame(fBSaveSingle, new TGLayoutHints(kLHintsCenterY,2,2,2,2));
   fBSaveSRPPAC = new TGPictureButton(fcombo,gClient->GetPicture("fileopen.xpm"),id++);
   fcombo->AddFrame(fBSaveSRPPAC, new TGLayoutHints(kLHintsCenterY,2,2,2,2));

   panel->AddFrame(fcombo, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY,
					     2,2,2,2));
   fMain->AddFrame(panel, new TGLayoutHints(kLHintsExpandX,2,2,2,2));

   /* tooltips */
   fBXval->SetToolTipText("Adopt range from embedded canvas");
   draw->SetToolTipText("Redraw histograms");
   fBReset->SetToolTipText("Reset drift time range");
   fBSaveSingle->SetToolTipText("Save a parameter file for single plane");
   fBSaveSRPPAC->SetToolTipText("Save parameter files for a SRPPAC at the same time");
   prev->SetToolTipText("previous plane");
   next->SetToolTipText("next plane");

   /* signal & slot */
   fNEMin->Connect("ReturnPressed()","art::TGSRPPACCalibration",this,"UpdateByNE()");
   fNEMin->Connect("ReturnPressed()","art::TGSRPPACCalibration",this,"DoDraw()");
   fNEMin->Connect("TabPressed()","TGNumberEntryField",fNEMax,"RequestFocus()");
   fNEMax->Connect("ReturnPressed()","art::TGSRPPACCalibration",this,"UpdateByNE()");
   fNEMax->Connect("ReturnPressed()","art::TGSRPPACCalibration",this,"DoDraw()");
   fNEMax->Connect("TabPressed()","TGNumberEntryField",fNEMin,"RequestFocus()");

   fBXval->Connect("Clicked()","art::TGSRPPACCalibration",this,"SetRangeByXval()");
   draw->Connect("Clicked()","art::TGSRPPACCalibration",this,"DoDraw()");
   fBReset->Connect("Clicked()","art::TGSRPPACCalibration",this,"Reset()");
   fBSaveSingle->Connect("Clicked()","art::TGSRPPACCalibration",this,"Save()");
   fBSaveSRPPAC->Connect("Clicked()","art::TGSRPPACCalibration",this,"SaveSRPPAC()");

   fComboSRPPAC->Connect("Selected(Int_t)","art::TGSRPPACCalibration",this,"ChangeHist(Int_t)");
   fComboPlane->Connect("Selected(Int_t)","art::TGSRPPACCalibration",this,"ChangeHist(Int_t)");
   prev->Connect("Clicked()","art::TGSRPPACCalibration",this,"Prev()");
   next->Connect("Clicked()","art::TGSRPPACCalibration",this,"Next()");

   fMain->Connect("CloseWindow()","art::TGSRPPACCalibration",this,"CloseWindow()");

   {
      Int_t id = 0;
      TIter next(fHists);
      while (TObject* h = next()) {
	 const TString& hname = h->GetName();
	 const TString& mwdcname = hname(0,hname.Last('_'));
	 if (!fComboSRPPAC->FindEntry(mwdcname.Data())) fComboSRPPAC->AddEntry(mwdcname,id);
	 ++id;
      }
      fComboSRPPAC->Select(0,kFALSE);
      ChangeHist(0);
      Reset();
   }

   const TString &wname = TString("SRPPAC Plane Calibration");
   fMain->SetWindowName(wname);

   fMain->MapSubwindows();

   fMain->Resize(fMain->GetDefaultSize());

   fMain->MapWindow();
}

void TGSRPPACCalibration::ChangeHist(Int_t id)
{
   fID = id;
   fHt = static_cast<TH1D*>(fHists->At(id));
   const Double_t kMin = fHt->GetXaxis()->GetXmin();
   const Double_t kMax = fHt->GetXaxis()->GetXmax();

   fXRange[0] = fHt->GetXaxis()->GetBinLowEdge(fHt->GetXaxis()->GetFirst());
   fXRange[1] = fHt->GetXaxis()->GetBinLowEdge(fHt->GetXaxis()->GetLast() + 1);

   fNEMin->SetLimits(TGNumberFormat::kNELLimitMinMax,kMin,kMax);
   fNEMin->SetNumber(fXRange[0]);
   fNEMax->SetLimits(TGNumberFormat::kNELLimitMinMax,kMin,kMax);
   fNEMax->SetNumber(fXRange[1]);

   UpdateComboBox();
   DoDraw();
}

void TGSRPPACCalibration::UpdateComboBox()
{
   const TString& currentmwdcname = CurrentSRPPACName();
   const Int_t mwdcid = fComboSRPPAC->FindEntry(currentmwdcname)->EntryId();

   if (mwdcid != fComboSRPPAC->GetSelected() || !fComboPlane->GetNumberOfEntries()) {
      Int_t id = 0;
      TIter next(fHists);
      fComboPlane->RemoveAll();
      while (TObject* h = next()) {
	 const TString& hname = h->GetName();
	 const TString& mwdcname = hname(0,hname.Last('_'));
	 const TString& planename = hname(hname.Last('_') + 1,hname.Length());
	 if (!mwdcname.CompareTo(currentmwdcname)) fComboPlane->AddEntry(planename,id);
	 ++id;
      }
   }

   fComboSRPPAC->Select(mwdcid,kFALSE);
   fComboPlane->Select(fID,kFALSE);
}

TString TGSRPPACCalibration::CurrentSRPPACName()
{
   const TString& hname = fHt->GetName();
   return hname(0,hname.Last('_'));
}

TString TGSRPPACCalibration::CurrentPlaneName()
{
   const TString& hname = fHt->GetName();
   return hname(hname.Last('_')+1,hname.Length());
}

void TGSRPPACCalibration::Prev()
{
   if (!fID) return;
   const Int_t id = fID - 1;
   ChangeHist(id);
}

void TGSRPPACCalibration::Next()
{
   if (fID + 1 == fHists->GetEntriesFast()) return;
   const Int_t id = fID + 1;
   ChangeHist(id);
}

void TGSRPPACCalibration::DoDraw()
{
   // Draw current histogram on enbedded canvas,

   if (!fHt) return;

   TCanvas *canvas = fECanvas->GetCanvas();

   canvas->cd(1);
   fHt->GetXaxis()->SetRangeUser(fXRange[0],fXRange[1]);
   fHt->Draw();

   canvas->cd(2);
   GenerateHist();
   fHtl->Draw();

   canvas->Update();
}

void TGSRPPACCalibration::CloseWindow()
{
   // Close main frame
   if(fMain) fMain->CloseWindow();
   fMain = NULL;
}

void TGSRPPACCalibration::Update()
{
   // Apply change of the x range to the entry fields and the slider.

   fNEMin->SetNumber(fXRange[0]);
   fNEMax->SetNumber(fXRange[1]);

   fBReset->SetState(kButtonUp);
}

void TGSRPPACCalibration::UpdateByNE()
{
   // Apply change of the entry fields.

   const Double_t min = fNEMin->GetNumber();
   const Double_t max = fNEMax->GetNumber();
   fXRange[0] = fHt->GetXaxis()->GetBinLowEdge(fHt->GetXaxis()->FindBin(min));
   fXRange[1] = fHt->GetXaxis()->GetBinLowEdge(fHt->GetXaxis()->FindBin(max));
   Update();
}

void TGSRPPACCalibration::Reset()
{
   // Reset x range and redraw.

   fXRange[0] = fHt->GetXaxis()->GetXmin();
   fXRange[1] = fHt->GetXaxis()->GetXmax();
   Update();
   DoDraw();

   fBReset->SetState(kButtonDisabled);
}

void TGSRPPACCalibration::Save()
{
   // Invoke a dialog to save current parameter.

   TGFileInfo fileInfo;

   fileInfo.fIniDir = StrDup(fLastDir);
   fileInfo.fFilename = StrDup(TString::Format("%s.dat",fHt->GetName()));
   new TGFileDialog(gClient->GetDefaultRoot(),fMain,kFDSave,&fileInfo);

   if(!fileInfo.fFilename) return;

   fLastDir = gSystem->DirName(fileInfo.fFilename);
   SaveToFile(fileInfo.fFilename);
}

void TGSRPPACCalibration::SaveSRPPAC()
{
   // Invoke a dialog to save current parameters for SRPPAC.
   const TString &mwdcname = CurrentSRPPACName();
   const TString &prompt = TString::Format("Input setting name for %s.",
					   mwdcname.Data());
   char retstr[256];

   new TGInputDialog(gClient->GetDefaultRoot(),fMain,prompt.Data(),
		     "temp",retstr);
   if (!strcmp(retstr,"")) return;

   const TString &outDir = TString::Format("%s/%s/%s/%s/%s",
					   gSystem->pwd(),fgSRPPACParamDir.Data(),
					   mwdcname.Data(),fgCalibParamSubDir.Data(),
					   retstr);
   const TString &outHistDir = gSystem->ConcatFileName(outDir.Data(),"dt_hist");

   FileStat_t fileStat;
   if (!gSystem->GetPathInfo(outDir,fileStat)) {
      if (fileStat.fMode && kS_IFDIR) {
	 // directory exists at outDir
	 const TString &msg = TString::Format("Directory \"%s\" exists.\nContinue? (Existing files can be overwritten)",
					      outDir.Data());
	 Int_t ret;
	 new TGMsgBox(gClient->GetDefaultRoot(),fMain,"SaveSRPPAC",msg.Data(),kMBIconQuestion,kMBYes | kMBNo,&ret);
	 if (ret == kMBNo) return;
      } else {
	 // something exists at outDir
	 const TString &msg = TString::Format("File \"%s\" exists. Abort.",outDir.Data());
	 new TGMsgBox(gClient->GetDefaultRoot(),fMain,"SaveSRPPAC",msg.Data(),kMBIconExclamation,kMBClose);
	 return;
      }
   } else {
      // outDir not exists
      const TString &msg = TString::Format("Save parameters to \"%s/\".",outDir.Data());
      Int_t ret;
      new TGMsgBox(gClient->GetDefaultRoot(),fMain,"SaveSRPPAC",msg.Data(),kMBIconAsterisk,kMBOk | kMBCancel,&ret);
      if (ret == kMBCancel) return;

      if (!gSystem->mkdir(outDir.Data(),kTRUE)) {
	 Info("SaveSRPPAC","directory %s was created",outDir.Data());
      } else {
	 SysError("SaveSRPPAC","Failed to make directory at %s",outDir.Data());
	 return;
      }
   }

   if (gSystem->GetPathInfo(outHistDir,fileStat)) {
      if (!gSystem->mkdir(outHistDir.Data(),kTRUE)) {
	 Info("SaveSRPPAC","directory %s was created",outDir.Data());
      } else {
	 SysError("SaveSRPPAC","Failed to make directory at %s",outDir.Data());
	 return;
      }
   }

   fLastDir = outDir;

   Int_t id = 0;
   TIter next(fHists);
   while (TObject* h = next()) {
      const TString& hname = h->GetName();
      const TString& hmwdc = hname(0,hname.Last('_'));
      const TString& planename = hname(hname.Last('_') + 1,hname.Length());
      if (!hmwdc.CompareTo(mwdcname)) {
	 ChangeHist(id);
	 const TString &ofname = TString::Format("%s/%s.dat",
						 outDir.Data(),planename.Data());
	 SaveToFile(ofname); // output artemis parameter

	 const TString &ofname2 = TString::Format("%s/dt_%s.data",
						  outHistDir.Data(),planename.Data());
	 SaveToFile(ofname2,kTRUE); // output histogram (anapaw parameter)
      }
      ++id;
   }
}

void TGSRPPACCalibration::SetRangeByXval()
{
   fBXval->SetState(kButtonEngaged);
   DoDraw();
   TPad *pad = (TPad*)((TCanvas*)fECanvas->GetCanvas())->GetPad(1);
   Double_t xmin, xmax;
   if (TCatCmdXval::Instance()->Run(pad,&xmin)
       && TCatCmdXval::Instance()->Run(pad,&xmax)) {
      if (xmax < xmin) std::swap(xmin,xmax);
      fXRange[0] = xmin;
      fXRange[1] = xmax;
      Update();
      DoDraw();
   }
   fBXval->SetState(kButtonUp);
}

void TGSRPPACCalibration::GenerateHist()
{
   // Generate integral histogram.

   const Int_t minBin = fHt->GetXaxis()->FindBin(fXRange[0]);
   const Int_t maxBin = fHt->GetXaxis()->FindBin(fXRange[1]);
   const Int_t nBin = maxBin - minBin + 1; // bin number for hDXDT

   fXRange[0] = fHt->GetBinLowEdge(minBin);
   fXRange[1] = fHt->GetBinLowEdge(maxBin+1);

   const TString &title = TString::Format("%s_tl",fHt->GetTitle());
   if (fHtl) {
      fHtl->SetBins(nBin,fXRange[0],fXRange[1]);
      fHtl->SetName(title);
      fHtl->SetTitle(title);
   } else {
      fHtl = new TH1D(title,"dx/dt curve;TL;Length per WireSpacing",
		      nBin,fXRange[0],fXRange[1]);
   }

   Float_t sum = 0;
   for(Int_t ibin = 0; ibin != nBin; ++ibin){
      sum += fHt->GetBinContent(minBin + ibin);
      fHtl->SetBinContent(ibin+1,sum);
   }

   fHtl->Scale(1./sum);
}

void TGSRPPACCalibration::SaveToFile(const char* ofilename,Bool_t isAna)
{
   // Output current parameter to a file.

   const TH1D *const h = isAna ? fHt : fHtl;

   const Int_t nBin = h->GetNbinsX();
   Double_t *center = new Double_t[nBin];
   h->GetCenter(center);
   const Double_t *const content = h->GetArray();

   Double_t *x = NULL;
   Double_t *y = NULL;

   Int_t begin = 0;
   Int_t end = nBin;

   if (!isAna) {
      const Double_t epsilon = std::numeric_limits<Double_t>::epsilon();
      for(Int_t i=0;i!=nBin;++i){
	 if (!begin) {
	    if (content[i] >= epsilon) {
	       begin = i;
	    }
	    continue;
	 }
	 if (TMath::AreEqualAbs(content[i],1.,epsilon)) {
	    end = i+1;
	    break;
	 }
      }
   }

   const Int_t size = end - begin;
   x = new Double_t[size];
   y = new Double_t[size];
   std::copy(center+begin,center+end,x);
   std::copy(content+begin,content+end,y);

   ofstream ofs(ofilename);
   if (ofs.fail()) {
      SysError("SaveToFile","can't open file: %s",ofilename);
      return;
   }

   if (!isAna) ofs << size << std::endl;
   for(Int_t i =0; i!=size;++i)
   {
      ofs << x[i] << " " << y[i] << std::endl;
   }
   ofs.close();

   const TString str = isAna ? "Histogram" : "Parameter";
   Info("SaveToFile","%s was saved to %s",str.Data(),ofilename);

   delete [] x;
   delete [] y;
   delete [] center;
}
