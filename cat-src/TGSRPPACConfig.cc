/**
 * @file   TGSRPPACConfig.cc
 * @brief  SRPPAC config GUI
 *
 * @date   Created      : 2019-08-30 18:27:18 JST (ota)
 *         Last Modified: 2019-08-30 18:27:34 JST (ota)
 * @author OTA Shinsuke <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 OTA Shinsuke
 */

#include "TGSRPPACConfig.h"

#include <TSystem.h>
#include <TStyle.h>
#include <TGClient.h>
#include <TGFrame.h>
#include <TGTab.h>
#include <TGStatusBar.h>
#include <TGButton.h>
#include <TGLabel.h>
#include <TGComboBox.h>
#include <TGLayout.h>
#include <TGTextEntry.h>

#include <algorithm>
#include <unistd.h>

#include <yaml-cpp/yaml.h>
#include <TSRPPACParameter.h>
#include <fstream>
#include <algorithm>

using art::TGSRPPACConfig;
using art::TGSRPPACConfigGroupFrame;

ClassImp(TGSRPPACConfig)

TGSRPPACConfig::TGSRPPACConfig()
{
   Build(gClient->GetDefaultRoot(),gStyle->GetCanvasDefW(),gStyle->GetCanvasDefH());
}

TGSRPPACConfig::TGSRPPACConfig(const TGWindow *p, UInt_t w, UInt_t h)
{
   // constructor
   Build(p,w,h);
}

namespace{
Bool_t cmp(const char* c1, const char* c2)
{
   return strcmp(c1,c2) < 0;
}

const TString kPrmDir = "prm/mwdc";
const TString kSubDir = "dt2dl";
const TString kConfigDir = "config";
}

void TGSRPPACConfig::Build(const TGWindow *p, UInt_t w, UInt_t h)
{
   fMain = new TGMainFrame(p,w,h);
   fMain->SetClassHints("ROOT","art::TGSRPPACConfig");
   fMain->SetCleanup(kDeepCleanup);

   fStatusBar = new TGStatusBar(fMain);
   fMain->AddFrame(fStatusBar, new TGLayoutHints(kLHintsBottom | kLHintsExpandX));

   {
      void* dirp = gSystem->OpenDirectory(kPrmDir);
      while (const char* entry = gSystem->GetDirEntry(dirp)) {
	 if (entry[0] == '.') continue;
	 if (!strcmp(entry,"config")) continue;
	 fSRPPACList.push_back(entry);
      }
   }

   {
      TString configDir = gSystem->ConcatFileName(kPrmDir,kConfigDir);
      void* dirp = gSystem->OpenDirectory(configDir);
      while (const char* entry = gSystem->GetDirEntry(dirp)) {
	 if (entry[0] == '.') continue;
	 TString name(entry);
	 if (!name.EndsWith(".yaml")) continue;
	 fConfigFileList.push_back(strdup(name));
      }
   }

   std::sort(fSRPPACList.begin(),fSRPPACList.end(),cmp);
   std::sort(fConfigFileList.begin(),fConfigFileList.end(),cmp);

   TGTab *tab = new TGTab(fMain);
   fMain->AddFrame(tab, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

   TGCompositeFrame *calibtab = tab->AddTab("calib");
   TGCompositeFrame *geometry = tab->AddTab("geometry");
   
   TGCompositeFrame *calib = new TGCompositeFrame(calibtab);
   calib->SetLayoutManager(new TGMatrixLayout(calib,0,2));
   calibtab->AddFrame(calib,new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));

   TGCanvas *gcan = new TGCanvas(geometry);
   geometry->AddFrame(gcan, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

   TGCompositeFrame *content = new TGCompositeFrame(gcan->GetViewPort());
   content->SetLayoutManager(new TGMatrixLayout(content,0,3));
   gcan->SetContainer(content);

   for (Int_t i = 0, n = fSRPPACList.size(); i!= n; ++i) {
      TGSRPPACConfigGroupFrame *frame = new TGSRPPACConfigGroupFrame(content,fSRPPACList[i]);
      frame->SetTitlePos(TGGroupFrame::kCenter);
      content->AddFrame(frame);
      frame->AddPlaneConfigEntries(fConfigFileList);
      frame->LoadGeometryYaml();
      frame->Connect("Message(char*)","TGStatusBar",fStatusBar,"SetText(const char*)");

      TGLabel *label = new TGLabel(calib,fSRPPACList[i]);
      calib->AddFrame(label, new TGLayoutHints(kLHintsExpandX));
      TGComboBox *combo = new TGComboBox(calib);
      combo->Resize(100,20);
      calib->AddFrame(combo, new TGLayoutHints(kLHintsCenterX));
      fCalibComboBoxes.push_back(combo);
//      combo->Connect("Selected(char*)","art::TGSRPPACConfigGroupFrame",this,"Modified()");
   }

   TGButton *save = new TGTextButton(calib,"Save");
   save->Resize(100,20);
   calib->AddFrame(save);

   TGButton *reset = new TGTextButton(calib,"Reset");
   reset->Resize(100,20);
   calib->AddFrame(reset);
   ResetCalibConfig();

   save->Connect("Clicked()","art::TGSRPPACConfig",this,"SaveCalibConfig()");
   reset->Connect("Clicked()","art::TGSRPPACConfig",this,"ResetCalibConfig()");

   fMain->Connect("CloseWindow()","art::TGSRPPACConfig",this,"CloseWindow()");

   const TString &wname = TString("SRPPAC Configurator");
   fMain->SetWindowName(wname);
   fMain->MapSubwindows();
   fMain->Resize(fMain->GetDefaultSize());
   fMain->MapWindow();
}

TGSRPPACConfig::~TGSRPPACConfig()
{
   fMain->Cleanup();
   delete fMain;
}

void TGSRPPACConfig::CloseWindow()
{
   if (fMain) fMain->CloseWindow();
}

void TGSRPPACConfig::SaveCalibConfig()
{
   fStatusBar->SetText("");
   TString wd = gSystem->pwd();

   for (Int_t i = 0, n = fSRPPACList.size(); i!=n; ++i) {
      TGComboBox *const combo = fCalibComboBoxes[i];
      TGTextLBEntry *entry = static_cast<TGTextLBEntry*>(combo->GetSelectedEntry());
      if (!entry) continue;
      TString mwdcDir = gSystem->ConcatFileName(kPrmDir,fSRPPACList[i]);
      TString subDir = gSystem->ConcatFileName(mwdcDir,kSubDir);
      TString from = entry->GetText()->Data();
      TString to   = "current";

      gSystem->cd(subDir);

      FileStat_t filestat;
      if(!gSystem->GetPathInfo(to,filestat)) {
	 // file exists
	 if (filestat.fIsLink) {
	    // symbolic link exists
	    gSystem->Unlink(to);
	 } else {
	    // unknown file exists
	    const TString &msg = TString::Format("Error: %s is not a symbolic link.",to.Data());
	    fStatusBar->SetText(msg);
	    gSystem->cd(wd);
	    return;
	 }
      } else {
	 // file not exists
      }
      gSystem->Symlink(from,to);
      gSystem->cd(wd);
   }

   fStatusBar->SetText("Calibration config was saved.");
}


void TGSRPPACConfig::ResetCalibConfig()
{
   fStatusBar->SetText("");
   for (Int_t i = 0, n = fSRPPACList.size(); i!=n; ++i) {
      TGComboBox *const combo = fCalibComboBoxes[i];
      combo->RemoveAll();
      TString mwdcdir = gSystem->ConcatFileName(kPrmDir,fSRPPACList[i]);
      TString prmDir = gSystem->ConcatFileName(mwdcdir,kSubDir);
      void* dirp = gSystem->OpenDirectory(prmDir);
      if (!dirp) continue;
      Int_t entry_id = 0;
      TString currentSetting = "";
      while (const char* entry = gSystem->GetDirEntry(dirp)) {
	 if (entry[0] == '.') continue;
	 if (!strcmp(entry,"current")) {
	    char buf[256];
	    ssize_t size = readlink(gSystem->ConcatFileName(prmDir,"current"),buf,256);
	    buf[size] = '\0';
	    currentSetting = gSystem->BaseName(buf);
	    continue;
	 }
	 combo->AddEntry(entry,entry_id++);
      }
      if (!currentSetting.Length()) continue;
      const Int_t current_id = combo->FindEntry(currentSetting)->EntryId();
      combo->Select(current_id,kFALSE);
   }
}


TGSRPPACConfigGroupFrame::TGSRPPACConfigGroupFrame(const TGWindow* p, const char* title)
   : TGGroupFrame(p,title,kRaisedFrame), fName(title), fGeometry(NULL)
{
   fContents = new TGCompositeFrame(this);
   fContents->SetLayoutManager(new TGMatrixLayout(fContents,0,2));
   AddFrame(fContents, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2,2,2,2));

   Int_t id = 0;

   fContents->AddFrame(new TGLabel(fContents,"Global X"));
   fXEntry = new TGTextEntry(fContents,"",id++);
   fXEntry->SetAlignment(kTextRight);
   fXEntry->Resize(80,20);
   fXEntry->SetToolTipText("X position in global coordinate [mm]");
   fContents->AddFrame(fXEntry);

   fContents->AddFrame(new TGLabel(fContents,"Global Y"));
   fYEntry = new TGTextEntry(fContents,"",id++);
   fYEntry->SetAlignment(kTextRight);
   fYEntry->Resize(80,20);
   fYEntry->SetToolTipText("Y position in global coordinate [mm]");
   fContents->AddFrame(fYEntry);

   fContents->AddFrame(new TGLabel(fContents,"Global Z"));
   fZEntry = new TGTextEntry(fContents,"",id++);
   fZEntry->SetAlignment(kTextRight);
   fZEntry->Resize(80,20);
   fZEntry->SetToolTipText("Z position in global coordinate [mm]");
   fContents->AddFrame(fZEntry);

   fContents->AddFrame(new TGLabel(fContents,"Angle"));
   fAngleEntry = new TGTextEntry(fContents,"",id++);
   fAngleEntry->SetAlignment(kTextRight);
   fAngleEntry->Resize(80,20);
   fAngleEntry->SetToolTipText("XY Rotation angle (counterclockwise) [deg.]");
   fContents->AddFrame(fAngleEntry);

   fContents->AddFrame(new TGLabel(fContents,"Turned"));
   fTurnedButton = new TGCheckButton(fContents,"",id++);
   fTurnedButton->SetToolTipText("check if SRPPAC is turned (opposite Z)");
   fTurnedButton->Resize(80,20);
   fContents->AddFrame(fTurnedButton);

   fContents->AddFrame(new TGLabel(fContents,"Plane Config."));
   fCBPlaneConfig = new TGComboBox(fContents,id++);
   fContents->AddFrame(fCBPlaneConfig);
   fCBPlaneConfig->Resize(80,20);

   fSaveButton = new TGTextButton(fContents,"Save",id++);
   fSaveButton->Resize(80,20);
   fContents->AddFrame(fSaveButton);

   fResetButton = new TGTextButton(fContents,"Reset",id++);
   fResetButton->Resize(80,20);
   fContents->AddFrame(fResetButton);

   fXEntry->Connect("TextChanged(char*)","art::TGSRPPACConfigGroupFrame",this,"Modified()");
   fYEntry->Connect("TextChanged(char*)","art::TGSRPPACConfigGroupFrame",this,"Modified()");
   fZEntry->Connect("TextChanged(char*)","art::TGSRPPACConfigGroupFrame",this,"Modified()");
   fAngleEntry->Connect("TextChanged(char*)","art::TGSRPPACConfigGroupFrame",this,"Modified()");
   fTurnedButton->Connect("Toggled(Bool_t)","art::TGSRPPACConfigGroupFrame",this,"Modified()");
   fCBPlaneConfig->Connect("Selected(char*)","art::TGSRPPACConfigGroupFrame",this,"Modified()");

   fSaveButton->Connect("Clicked()","art::TGSRPPACConfigGroupFrame",this,"Save()");
   fResetButton->Connect("Clicked()","art::TGSRPPACConfigGroupFrame",this,"Reset()");

   fContents->Resize();
   Resize();
}

TGSRPPACConfigGroupFrame::~TGSRPPACConfigGroupFrame()
{
   delete fGeometry;
}

void TGSRPPACConfigGroupFrame::AddPlaneConfigEntries(const std::vector<const char*>& fileList)
{
   for (Int_t i = 0, n=fileList.size(); i != n; ++i) {
      TString configName(fileList[i]);
      TString entry = configName.Remove(configName.Index(".yaml"));
      fCBPlaneConfig->AddEntry(entry,i);
   }
}

void TGSRPPACConfigGroupFrame::Save()
{
   if (!fIsModified) return;

   const TString &xstr = TString(fXEntry->GetText());
   if (xstr.IsFloat()) fGeometry->SetX(xstr.Atof());

   const TString &ystr = TString(fYEntry->GetText());
   if (ystr.IsFloat()) fGeometry->SetY(ystr.Atof());

   const TString &zstr = TString(fZEntry->GetText());
   if (zstr.IsFloat()) fGeometry->SetZ(zstr.Atof());

   const TString &astr = TString(fAngleEntry->GetText());
   if (astr.IsFloat()) fGeometry->SetAngle(astr.Atof());

   fGeometry->SetTurned(fTurnedButton->IsOn());

   if (const TGTextLBEntry *entry = static_cast<TGTextLBEntry*>(fCBPlaneConfig->GetSelectedEntry())) {
      const TGString gstr = entry->GetText();
      const TString &configDir = gSystem->ConcatFileName(kPrmDir,kConfigDir);
      const TString &configFile = gSystem->ConcatFileName(configDir,TString::Format("%s.yaml",gstr.Data()));
      fGeometry->SetConfigurationFile(configFile);
   }

   {
      const TString &prmDir = gSystem->ConcatFileName(kPrmDir,fName);
      const TString &geoFile = TString::Format("%s/geometry.yaml",prmDir.Data());
      std::ofstream ofs(geoFile);
      if (!ofs.fail()) {
	 fGeometry->OutputDescriptionYAML(ofs);
      }
   }

   const TString &msg = TString::Format("configuration for \'%s\' was saved.",fName.Data());
   Reset();
   Message(msg);
}

void TGSRPPACConfigGroupFrame::Reset()
{
   Message("");
   Rename(fName);
   fIsModified = kFALSE;

   fXEntry->SetText(TString::Format("%.1f",fGeometry->GetX()),kFALSE);
   fYEntry->SetText(TString::Format("%.1f",fGeometry->GetY()),kFALSE);
   fZEntry->SetText(TString::Format("%.1f",fGeometry->GetZ()),kFALSE);
   fAngleEntry->SetText(TString::Format("%.2f",fGeometry->GetAngle()),kFALSE);
   fTurnedButton->SetOn(fGeometry->IsTurned(),kFALSE);

   TString configFile = gSystem->BaseName(fGeometry->GetConfigurationFile());
   if (!configFile.EndsWith(".yaml")) return;

   configFile.Remove(configFile.Index(".yaml"));
   if(TGLBEntry *entry = fCBPlaneConfig->FindEntry(configFile)) {
      fCBPlaneConfig->Select(entry->EntryId(),kFALSE);
   }
}

void TGSRPPACConfigGroupFrame::LoadGeometryYaml()
{
   const TString &mwdcDir = gSystem->ConcatFileName(kPrmDir,fName);
   const TString &geoFile = TString::Format("%s/geometry.yaml",mwdcDir.Data());

   delete fGeometry;
   fGeometry = new TSRPPACParameter();
   if(!fGeometry->LoadFile(geoFile)) return;

   Reset();
}

void TGSRPPACConfigGroupFrame::Modified()
{
   Rename(TString::Format("%s (modified)",fName.Data()));
   fIsModified = kTRUE;
}
