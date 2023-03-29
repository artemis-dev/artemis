/**
 * @file   TCatAtRoughEventCounter.cc
 * @brief  rough event counter for tracking detector
 *
 * @date   Created       : 2015-02-17 13:06:33 JST
 *         Last Modified : 2017-12-23 21:20:23 JST (ota)
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *
 *    
 */

#include "TCatAtRoughEventCounter.h"
#include <TCatPulseShape.h>
#include <TClonesArray.h>
#include <TEventHeader.h>
#include <TROOT.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TH2Poly.h>
#include <TCatReadoutPadArray.h>
#include <stdio.h>
#include <TMath.h>
#include <string>
#include <sstream>
#include <TArtSystemOfUnit.h>
#include <TPaveStats.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#ifndef __CINT__
#include <iomanip>
#endif
#include <unistd.h>
#include <dirent.h>

using art::TCatAtRoughEventCounter;
using art::TCatReadoutPadArray;
using TArtSystemOfUnit::deg;

ClassImp(TCatAtRoughEventCounter)

TCatAtRoughEventCounter::TCatAtRoughEventCounter()
{
  StringVec_t def;
  def.push_back("athitcls");
  RegisterInputCollection("InputCollection","pulse data (TCatPulseShape)",
			  fInputColName, def);
  RegisterOutputCollection("OutputCollection","output pulse data (TCatPulseShape)",
			   fOutputColName,TString("atroughevt"));
  RegisterProcessorParameter("LogFilePath","Path of Log File",fLogFilePath,TString("../log"));
  RegisterProcessorParameter("LogFileName","Name of Log File",fLogFileName,TString("roughevtcounter.log"));
  RegisterProcessorParameter("DebugMode","Flag For Debug",fDebug,false);
  RegisterProcessorParameter("DetailDebugMode","Flag For Detail Debug",fDetailDebug,false);
  RegisterProcessorParameter("LeastSquareMode","Flag For Least Squares Method Mode (0: using TGraph pol1 fit)",fLSM,true);
  RegisterProcessorParameter("SetRecoilRange","Set Recoil Range for Event Selection",SetRecoilRange,(Float_t)20.);
  RegisterProcessorParameter("SelectCondition","Selection of Rough Event Condition",fSelectCondition,(Int_t)0);
  RegisterProcessorParameter("SetLowThresAngle","Set Low Threshold of Gated Angle",fAngle_lowth,(Float_t)0.);
  RegisterProcessorParameter("SetHiThresAngle","Set Hi Threshold of Gated Angle",fAngle_hith,(Float_t)180.);
  RegisterProcessorParameter("SetCutRecoilHits","Set Recoil Hits to cut",fCutRecoilHits,(Int_t)3.);
  RegisterProcessorParameter("ShowBeamMode","Flag For Beam Show",fShowBeam,false);
  RegisterProcessorParameter("ShowRecoilMode","Flag For Recoil Show",fShowRecoil,false);
  RegisterProcessorParameter("HistSaveFlag","Flag For Histgram Save",fSaveHist,false);
  RegisterProcessorParameter("DrawFigFlag","Flag of Draw Figure",fDrawFig,false);
  RegisterProcessorParameter("DrawPulseFlag","Flag of Draw Pulse",fDrawPulse,false);
  RegisterProcessorParameter("NoColorFigFlag","Flag of Fill No Color Figure",fNoColorFig,false);
  RegisterProcessorParameter("HistName","HistName",fHistName,fOutputColName);
  RegisterProcessorParameter("WaitTime","Waiting time (ms)",fWaitTime,200000);
  RegisterProcessorParameter("SaveFigFlag","Flag of Save Figure",fSaveFig,false);
  RegisterProcessorParameter("FigFilePath","Path of Figure File",fFigFilePath,TString("./"));
  RegisterProcessorParameter("AddFigFilePath","Add Path of Figure File",fAddFigFilePath,TString("./"));
  RegisterProcessorParameter("SaveEvtNum","Number of Save Event",fSaveEvt,-1);
  RegisterInputInfo("ReadoutPad","Name of Readout Pad",fReadoutPadName,TString("readoutPad"),&fPads,"art::TCatReadoutPadArray","");
  RegisterProcessorParameter("CanvasX","X Position of Canvas",fCanvasX,(Int_t)0);
  RegisterProcessorParameter("CanvasY","Y Position of Canvas",fCanvasY,(Int_t)0);
  RegisterProcessorParameter("CanvasW","Width of Canvas",fCanvasW,(Int_t)1000);
  RegisterProcessorParameter("CanvasH","Hight of Canvas",fCanvasH,(Int_t)1050);
  RegisterOptionalParameter("LowPressure","Calcurate range at 0.4-atm when true (default ture)",
                            fLowPressure,true);
  RegisterProcessorParameter("DriftVelocity","Drift velocity (cm/us)",fDriftVelocity,(Float_t)1.0);
  RegisterProcessorParameter("BeamMass","Mass of Beam (MeV)",fBeamMass,(Float_t)13971.287);
  RegisterProcessorParameter("BeamAtomicMass","Atomic Mass of Beam (amu)",fBeamAtomicMass,(Float_t)15.003066);
  RegisterProcessorParameter("BeamVelocity","Beam Velocity (MeV/u)",fBeamVelocity,(Float_t)100.);
  RegisterOptionalParameter("IgnoreDistanceToBeam","Ignore checking distance between recoil and beam when true (default false)",
                            fIgnoreDistanceToBeam,false);
  RegisterOptionalParameter("UseAllRecoilCluster","Use all the recoil clusters without choosing maximum length cluster when true (default false)",
                            fUseAllRecoilCluster,false);
  RegisterInputInfo("RunHeadersName","run headers",fNameRunHeaders,TString("runheader"),&fRunHeaders,"TList","TList");
  RegisterInputCollection("EventHeader","name of event header (default : eventheader)",fNameEventHeader,TString("eventheader"),
                             &fEventHeader,art::TEventHeader::Class_Name());
}

TCatAtRoughEventCounter::~TCatAtRoughEventCounter()
{

  delete fOutput;
  if (fDrawPulse) if (fPulseDrawer) delete fPulseDrawer;
  if (fSaveHist) {
    delete fhAngle;
    delete fhAngleRecoilE;
    delete fhAngleRecoilE_ex;
    delete fhAngleRecoilTKE;
    delete fhAngleRecoilRange;
    delete fhCMAngle;
    delete fhEx;
    delete fhAngleEx;
    delete fhRecoilNRange;
    delete fhRecoilNAngle;
  }
  if (fDrawFig) {
    delete fCanvas;
    delete fCanvas2;
    delete fHPadEvt;
    //if (fLSM) delete fhBeamRay;
    if (fLSM) delete fhBeamYRay;
    if (fLSM) delete fhRecoilRay;
    if (fLSM) delete fhRecoilYRay;
  }
}

TCatAtRoughEventCounter::TCatAtRoughEventCounter(const TCatAtRoughEventCounter& rhs)
{
  MayNotUse("TCatAtRoughEventCounter");
}

TCatAtRoughEventCounter& TCatAtRoughEventCounter::operator=(const TCatAtRoughEventCounter& rhs)
{
  MayNotUse("operator=");
  if (this != &rhs) {

  }
  return *this;
}

void TCatAtRoughEventCounter::Init(TEventCollection *col)
{
  TEventObject *obj = NULL;
  if (!(obj = col->Get(fInputColName.front()))) {
    Info("Init","No such input %s",fInputColName.front().Data());
    return;
  } 
  // Prepare Log/Fig File
  if (fDebug || fDetailDebug) {
     const Char_t *LogNamePath = Form("%s/%s",fLogFilePath.Data(),fLogFileName.Data());
     fp = fopen(LogNamePath,"w");
     if (fp == NULL) {
        Error("Init","Cannot open %s/%s",fLogFilePath.Data(),fLogFileName.Data());
        exit(1);
     }
  }
  fInput = (TClonesArray**) obj->GetObjectRef();
  fOutput = new TClonesArray("art::TCatPulseShape");
  col->Add(fOutputColName,fOutput,fOutputIsTransparent);
  fOutputBeam = new TClonesArray("art::TCatPulseShape");
  col->Add("atroughbeam",fOutputBeam,fOutputIsTransparent);
  fOutputRecoil = new TClonesArray("art::TCatPulseShape");
  col->Add("atroughrecoil",fOutputRecoil,fOutputIsTransparent);

  // Prepare Figure and Histgram
  if (fDrawPulse) {
     fFigFile = Form("%s/%s/%s",fFigFilePath.Data(),fOutputColName.Data(),fAddFigFilePath.Data());
     fPulseDrawer = new TPulseDrawer(GetName(),GetName(),fCanvasX,fCanvasY,fCanvasW,fCanvasH);
     fPulseDrawer->SetOptions(fWaitTime,fSaveEvt,fSaveFig,fFigFile);
     fPulseDrawer->SetRunHeader(fRunHeaders);
  }

  gStyle->SetStatX(0.900);
  gStyle->SetStatY(0.980);
  gStyle->SetStatH(0.159);
  gStyle->SetStatW(0.150);
  gStyle->SetTitleYOffset(1.5);

  if (fSaveHist) {
    fhAngle  = new TH1F("hAngle","hAngle;theta_lab[deg]",180,0.,180.);
    fhAngleRecoilE  = new TH2F("hAngleRecoilE","hAngleRecoilE;theta_lab[deg];Recoil E[a.u.]",180,0.,180.,600,-500.,5500.);
    fhAngleRecoilE_ex  = new TH2F("hAngleRecoilE_ex","hAngleRecoilE_ex;theta_lab[deg];Recoil E[a.u.]",180,0.,180.,600,0.,600.);
    fhAngleRecoilRange = new TH2F("hAngleRecoilRange","hAngleRecoilRange;theta_lab[deg];Recoil Range[mm]",90,0.,180.,150,0.,300.);
    fhAngleRecoilTKE = new TH2F("hAngleRecoilTKE","hAngleRecoilTKE;theta_lab[deg];Recoil TKE[MeV] Calcurated from Range",180,0.,180.,80,0.,0.8);
    fhCMAngle  = new TH1F("hCMAngle","hCMAngle;theta_cm[deg]",300,0.,30.);
    fhEx  = new TH1F("hEx","hEx;[MeV]",100,-50.,50.);
    fhAngleEx  = new TH2F("hAngleEx","hAngleEx;theta_lab[deg];Ex[MeV]",90,0.,180.,300,-30.,30.);
    fhRecoilNRange  = new TH2F("hRecoilNRange","hRecoilNRange;Number of Recoil Hits;Recoil Range[mm]",20,0.,20.,91,0.,91.);
    fhRecoilNAngle  = new TH2F("hRecoilNAngle","hRecoilNAngle;Number of Recoil Hits;theta_lab[deg]",20,0.,20.,180,0.,180.);
  }
  if (fDrawFig) {
    gDirectory = gROOT;
    if (fSaveFig) {
       fFigFile = Form("%s/%s/%s",fFigFilePath.Data(),fOutputColName.Data(),fAddFigFilePath.Data());
       DIR *dp = opendir(fFigFile);
       if (dp == NULL) {
          Error("Init","Cannot open %s",fFigFile.Data());
          exit(1);
       }
       Info("Init","\nfFigFile : %s",fFigFile.Data());
    }
    fCanvas  = new TCanvas(GetName(),GetName(),  0, 0,505,521);
    fCanvas->SetGrid();
    fCanvas2 = new TCanvas("canvas2",GetName(),515, 0,505,521);
    //fCanvas2->Divide(1,2);
    fCanvas2->SetGrid();

#if 1
    fHPadEvt = DrawPad(fNPad,fPadX,fPadY);
    fHPadEvt->SetName(fHistName);
#else
    if (!fPads) {
      Warning("Init","No such input fPads");
      fPads = (TCatReadoutPadArray*) col->GetInfo(fReadoutPadName);
    } else {
      Info("Init","pad array found at %p",fPads);
    }
#endif
    //if (fLSM) fhBeamRay = new TF1("hBeamRay","[0]*x+[1]",-50.,50.);
    if (fLSM) fhBeamYRay = new TF1("hBeamRay","[0]*x+[1]",-50.,50.);
    if (fLSM) fhRecoilRay = new TF1("hRecoilRay","[0]*x+[1]",-50.,50.);
    if (fLSM) fhRecoilYRay = new TF1("hRecoilYRay","[0]*x+[1]",-50.,50.);
    t = new TText(0.1,0.1,"");
  }
  
  Info("Init","\nLow Pressre : %d\nBeam Mass : %f MeV\nBeam Atomic Mass : %f amu\nBeam Velocity : %f MeV/u",
       fLowPressure,fBeamMass,fBeamAtomicMass,fBeamVelocity);
}

void TCatAtRoughEventCounter::Process()
{
   if ((*fRunHeaders)->Last()) {
      fRunInfo   = dynamic_cast<TRunInfo*>((*fRunHeaders)->Last());
      fRunName   = fRunInfo->GetRunName();
      fRunNumber = fRunInfo->GetRunNumber();
   } else {
      printf("Failed to call runname,number from runinfo\n");
   }
   fEvt = (*fEventHeader)->GetEventNumber();
   fOutput->Clear("C");
   fOutputBeam->Clear("C");
   fOutputRecoil->Clear("C");
   TClonesArray *input = *fInput;
   Int_t nHits = (input)->GetEntriesFast();
   std::vector<TCatPulseShape*> icls;
   std::vector<std::vector<TCatPulseShape*> > cls;
   Int_t cls_i_tmp = 0;
   Int_t ncls = 0;

   TCatPulseShape::SetSortType(TCatPulseShape::kTiming);
   TCatPulseShape::SetSortOrder(TCatPulseShape::kASC);
   (input)->Sort();
   (input)->Compress();
  
   if (fDebug) if (nHits) fprintf(fp,"-----------\n*Run: %s%04d, Evt = %d, nHits = %3d\n",fRunName.Data(),fRunNumber,fEvt,nHits);
   if (fDetailDebug) if (nHits) printf("[roughevtcount]   Evt = %6d, nHits = %3d\n",fEvt,nHits);

   for (Int_t iHit = 0; iHit != nHits; iHit++) {
      TCatPulseShape *hit = (TCatPulseShape*) (input)->At(iHit);
      Int_t cls_i = hit->GetAuxID();
      Int_t id = hit->GetID();
      Float_t off = hit->GetOffset();
      Double_t charge = hit->GetCharge();
      Int_t col = id / 10;
      Int_t row = id % 10;
      if ((id > 69 && id < 330) && (col%2 == 0)) {
         col = col - 1;
         row = row + 10;
      }

      if (iHit==0) ncls = 1;
      if ((iHit==0) || (cls_i_tmp==cls_i)) {
         icls.push_back(hit);
      } else {
         cls.push_back(icls);
         icls.clear();
         icls.push_back(hit);
         ncls++;
      }
      cls_i_tmp = cls_i;
      if (iHit == nHits-1) { // last hit
         cls.push_back(icls);
      }
    
      if (fDetailDebug) printf("cls_i = %3d, iHit = %3d, id = %3d, col = %2d, row = %2d, off = %4.0f, charge = %5.1f\n",cls_i,iHit,id,col,row,off,charge);
   }
   /*--------------------------------------------------------------------------*/

   // Count hits each column
   std::vector<TCatPulseShape*> irecoil;
   for (Int_t icls = 0; icls != ncls; icls++) {
      if ((!fLSM) || fDrawFig) {
         //fhBeamHits = new TGraph;
         fhBeamYHits = new TGraph;
         fhRecoilHits = new TGraph;
         fhRecoilYHits = new TGraph;
         //fhBeamHits->Clear();
         fhBeamYHits->Clear();
         fhRecoilHits->Clear();
         fhRecoilYHits->Clear();
      }
      if (fDrawFig) {
         //if (fLSM) fhBeamRay->ReleaseParameter(0);
         //if (fLSM) fhBeamRay->ReleaseParameter(1);
         //if (fLSM) fhBeamRay->Update();
         if (fLSM) fhBeamYRay->ReleaseParameter(0);
         if (fLSM) fhBeamYRay->ReleaseParameter(1);
         if (fLSM) fhBeamYRay->Update();
         if (fLSM) fhRecoilRay->ReleaseParameter(0);
         if (fLSM) fhRecoilRay->ReleaseParameter(1);
         if (fLSM) fhRecoilRay->Update();
         if (fLSM) fhRecoilYRay->ReleaseParameter(0);
         if (fLSM) fhRecoilYRay->ReleaseParameter(1);
         if (fLSM) fhRecoilYRay->Update();
         fHPadEvt->ClearBinContents();
      }
      Bool_t BeamIsGood = false;
      Bool_t IsGood = false;
      Bool_t IsGoodBeamEvent = false;
      Int_t nHitsCol[40] = {};
      Int_t nHitsRow[10] = {};
      Int_t nBeamCol = 0;
      Int_t nBeam = 0;
      Int_t BeamCol = 0;
      Int_t Region = -1;
      Bool_t fBeamCol[40] = {};
      std::vector<TCatPulseShape*>::iterator it = cls[icls].begin();
      std::vector<TCatPulseShape*>::iterator itend = cls[icls].end();
      Int_t cls_i = (*it)->GetAuxID();
      if (fDebug) fprintf(fp,"*********************************\n");
      //if (fDebug) fprintf(fp,"// Count hits each column\n");
      //if (fDebug) fprintf(fp,"icls = %d, cls_i = %d:\n",icls,cls_i);
      //if (fDetailDebug) printf("icls = %d, cls_i = %d:\n",icls,cls_i);
      for (; it != itend; it++) {
         Int_t id = (*it)->GetID();
         Int_t col = id / 10;
         Int_t row = id % 10;
         if ((id > 69 && id < 330) && (col%2 == 0)) {
            col = col - 1;
            row = row + 10;
         }
         nHitsCol[col]++;
         if (id < 70 || id >= 340) nHitsRow[row]++;
      }
      /*--------------------------------------------------------------------------*/

      // Count Beam
      //if (fDebug) fprintf(fp,"// Count Beam\n");
      Int_t min_BeamCol = 39;
      Int_t max_BeamCol = 0;
      for (Int_t iCol=0; iCol<40; iCol++) {
         fBeamCol[iCol] = false;
         //if (fDebug) if (nHitsCol[iCol]>0) fprintf(fp,"iCol = %d, nHitsCol[%d] = %d\n",iCol,iCol,nHitsCol[iCol]);
         if ((((iCol<=6) || (iCol>=33)) && (nHitsCol[iCol]>=7)) 
             || (((iCol>=7) && (iCol<=31)) && (nHitsCol[iCol]>=17))) {
            nBeamCol++;
            fBeamCol[iCol] = true;
      
            if (min_BeamCol > iCol) min_BeamCol = iCol;
            if (max_BeamCol < iCol) max_BeamCol = iCol;
      
            if (nHitsCol[BeamCol] <= nHitsCol[iCol]) {
               BeamCol = iCol;
               if ((iCol>=9) && (iCol<=29)) {
                  Region = 0;
               } else if (((iCol>=5) && (iCol<=7)) || ((iCol>=31) && (iCol<=34))) {
                  Region = 1;
               } else {
                  Region = 2;
               }
            }
            if (iCol!=0) {
               if ((iCol<=7) || (iCol>=34)) {
                  if (!(fBeamCol[iCol-1])) nBeam++;
               } else {
                  if (!(fBeamCol[iCol-2])) nBeam++;
               }
            } else {
               nBeam++;
            }
            //if (fDebug) fprintf(fp,"iCol = %d, nBeamCol = %2d, fBeamCol[%d] = %d, nBeam = %2d\n",iCol,nBeamCol,iCol,fBeamCol[iCol],nBeam);
         }
      }
      
      //if (fDebug) fprintf(fp,"nBeam = %2d, nBeamCol = %2d, Region = %d, min_BeamCol = %2d, max_BeamCol = %2d\n",nBeam,nBeamCol,Region,min_BeamCol,max_BeamCol);
      //if (fDetailDebug) printf("icls = %2d, cls_i = %2d, nBeam = %2d, nBeamCol = %2d\n",icls,cls_i,nBeam,nBeamCol);
      /*--------------------------------------------------------------------------*/
      
      // Add Beam Go Through Col
      //if (fDebug) fprintf(fp,"// Add Beam Go Through Col\n");
      Bool_t IsRecoilRegionBeam = false;
      for (Int_t iCol=0; iCol<40; iCol++) {
         fBeamCol[iCol] = false;
         if (fDebug) if (nHitsCol[iCol]>=4) fprintf(fp,"iCol = %d, nHitsCol[%d] = %d\n",iCol,iCol,nHitsCol[iCol]);
         if ((nHitsCol[iCol]>=4) && (!fBeamCol[iCol])) {
            nBeamCol++;
            fBeamCol[iCol] = true;
            if (min_BeamCol > iCol) min_BeamCol = iCol;
            if (max_BeamCol < iCol) max_BeamCol = iCol;
         }

         if ((iCol<=7 || iCol>=33) && fBeamCol[iCol]) {
            IsRecoilRegionBeam = true;
            if (fDebug) fprintf(fp,"iCol = %d, fBeamCol[%d] = %d\n",iCol,iCol,fBeamCol[iCol]);
            break;
         }
      }

      Int_t nRowRecoilRegion = 0;
      for (Int_t iRow=0; iRow<10; iRow++) {
         if (fDebug) fprintf(fp,"nHitsRow[%d] = %d\n",iRow,nHitsRow[iRow]);
         if (nHitsRow[iRow] > 0) nRowRecoilRegion++;
      }
      //if (fDebug) fprintf(fp,"nBeam = %2d, nBeamCol = %2d, Region = %d, min_BeamCol = %2d, max_BeamCol = %2d\n",nBeam,nBeamCol,Region,min_BeamCol,max_BeamCol);
      //if (fDetailDebug) printf("icls = %2d, cls_i = %2d, nBeam = %2d, nBeamCol = %2d\n",icls,cls_i,nBeam,nBeamCol);
      /*--------------------------------------------------------------------------*/

      if (fDebug) fprintf(fp,"nRowRecoilRegion = %d\n",nRowRecoilRegion);
      //if (IsRecoilRegionBeam) {
      if (nRowRecoilRegion>=10) {
         if (fDebug) fprintf(fp,"This event has beam in recoil region.\n");
         continue;
      }

      /*--------------------------------------------------------------------------*/
      
      std::vector<TCatPulseShape*> beam;
      std::vector<TCatPulseShape*> recoil_cand;
      Int_t nBeamHitsRow[20] = {};
      //Float_t off0 = 0.;
      Double_t x0 = 0.;
      Double_t z0 = 0.;
      it = cls[icls].begin();
      itend = cls[icls].end();
      //if (fDebug) fprintf(fp,"icls = %d, cls_i = %d:\n",icls,cls_i);
      for (; it != itend; it++) {
         Int_t id = (*it)->GetID();
         Double_t x = (*it)->GetX();
         Double_t z = (*it)->GetZ();
         //Float_t off = (*it)->GetOffset(); 
         Double_t q = (*it)->GetCharge();
         Int_t col = id / 10;
         Int_t row = id % 10;
         if ((id > 69 && id < 330) && (col%2 == 0)) {
            col = col - 1;
            row = row + 10;
         }
         if (fBeamCol[col]) {
            //if (q<=0) continue;
            beam.push_back((*it));

            //if (fDrawFig) fHPadEvt->Fill(x,z);
            if (fDrawFig) fHPadEvt->Fill(x,z,q);

            nBeamHitsRow[row]++;
            //if (fDebug) fprintf(fp,"Beam       :cls_i = %2d, id = %3d, col = %2d, row = %2d, off = %6.1f, q = %5.1f, nBeamHitsRow[%2d] = %2d\n",cls_i,id,col,row,off,q,row,nBeamHitsRow[row]);
         } else {
            recoil_cand.push_back((*it));
            //if (fDebug) fprintf(fp,"Recoil_cand:cls_i = %2d, id = %3d, col = %2d, row = %2d, off = %6.1f, q = %5.1f\n",cls_i,id,col,row,off,q);
         }
      }
      /*Beam----------------------------------------------------------------------*/
      if (fDebug) fprintf(fp,"\n*Beam :\n");
      //if (0) {
      Int_t nBeamHits = beam.size();
      if (nBeamHits==0) {
         if (fDebug) fprintf(fp,"Skip: No Beam Hits\n");      
         continue;
      }
      //
      //// Calcurate Beam Average-Offset
      //if (fDebug) fprintf(fp,"// Calcurate Beam Average-Offset\n");
      //Float_t sum_off = 0;
      //it = beam.begin();
      //itend = beam.end();
      //for (; it != itend; it++) {
      //   Int_t id = (*it)->GetID();
      //   Float_t off = (*it)->GetOffset();
      //   Int_t col = id / 10;
      //   Int_t row = id % 10;
      //   if ((id > 69 && id < 330) && (col%2 == 0)) {
      //      col = col - 1;
      //      row = row + 10;
      //   }
      //   sum_off += off;
      //   if (row==0) {
      //      off0 = off;
      //   }
      //}
      //Float_t BeamOff = sum_off / nBeamHits;
      //if (fDebug) fprintf(fp,"icls = %d, Beamoff = %6.1f\n",icls,BeamOff);
      ///*--------------------------------------------------------------------------*/
      //
      //// Derive Linear Equation of Beam (x,z)
      //if (fDebug) fprintf(fp,"// Derive Linear Equation of Beam\n");
      //if (fDebug) fprintf(fp,"// nBeamHitsRow: 2 -> charge division, >2 -> center of charge\n");
      //Double_t x_b[20],z_b[20],q_b[20];
      //Double_t sqx[20];
      //for (Int_t i=0; i<20; i++) {
      //   x_b[i] = -1000.;
      //   z_b[i] = -1000.;
      //   q_b[i] = 0.;
      //   sqx[i] = 0.;
      //}
      //Double_t Q_b = 0.;
      //Double_t Sx  = 0.;
      //Double_t Sz  = 0.;
      //Double_t Sxx = 0.;
      //Double_t Sxz = 0.;
      //Double_t a_bz  = 0.;
      //Double_t b_bz  = 0.;
      //Double_t p0 =  0.;
      //Double_t p1 =  0.;
      //Int_t    n   = 0;
      //Int_t    first_col = -1;
      //const Double_t gap = 0.15 / TMath::Sqrt(3) * 2;
      it = beam.begin();
      itend = beam.end();
      //if (fDebug) fprintf(fp,"icls = %d, cls_i = %d:\n",icls,cls_i);
      TObjArray cls_array;
      for (; it != itend; it++) {
         cls_array.Add((*it));
      }
      TCatPulseShape::SetSortType(TCatPulseShape::kID);
      TCatPulseShape::SetSortOrder(TCatPulseShape::kASC);
      cls_array.Sort();    
      //nBeamHits = cls_array.GetEntriesFast();
      //if (fDebug) fprintf(fp,"cls_ i = %d, nBeamHits = %d\n",cls_i,nBeamHits);
      //for (Int_t iHit=0; iHit<nBeamHits; iHit++) {
      //   TCatPulseShape *hit = (TCatPulseShape*) cls_array[iHit];
      //   Int_t id = hit->GetID();
      //   Int_t col = id / 10;
      //   Int_t row = id % 10;
      //   if ((id > 69 && id < 330) && (col%2 == 0)) {
      //      col = col - 1;
      //      row = row + 10;
      //   }
      //   Double_t x = hit->GetX();
      //   Double_t z = hit->GetZ();
      //   Double_t q = hit->GetCharge();
      //
      //
      //   if (fDebug) fprintf(fp,"cls_i = %2d, id = %3d, row = %2d, nBeamHitsRow[%2d] = %2d\n",cls_i,id,row,row,nBeamHitsRow[row]);
      //   if (fDetailDebug) printf("cls_i = %2d, id = %3d, row = %2d, nBeamHitsRow[%2d] = %2d\n",cls_i,id,row,row,nBeamHitsRow[row]);
      //   if (nBeamHitsRow[row]==2) { // Charge Division
      //      if (x_b[row]==-1000.) {
      //         first_col = col;
      //         x_b[row] = x;
      //         z_b[row] = z;
      //         q_b[row] = q;
      //      } else {
      //         if ((first_col>=6) && (first_col<=33)) { // 5 mm pad
      //            Double_t z_bh = (5./2 * TMath::Sqrt(3) / 3) / 2;
      //            Double_t x_bh = (5./2 + gap) / 2;
      //            if ((first_col%4==3) || (first_col==6) || (first_col==33)) { // first_col = 6,7,11,15,19,23,27,31,33
      //               if (row%2==0) { /* 1\//\2 */
      //                  if (fDebug) fprintf(fp,"/* first_col : %2d,row :even, 1\\//\\2 */\n",first_col);
      //                  x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
      //                  z_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * z_bh + (z_b[row] - z_bh);
      //               } else if (row%2==1) { /* 1/\\/2 */
      //                  if (fDebug) fprintf(fp,"/* first_col : %2d,row : odd, 1/\\\\/2 */\n",first_col);
      //                  x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
      //                  z_b[row] = ((q - q_b[row]) / (q + q_b[row])) * z_bh + (z - z_bh);
      //               }
      //            } else if ((first_col%4==1) && (first_col!=33)) { // first_col = 9,13,17,21,25,29
      //               if (row%2==0) { /* 1/\\/2 */
      //                  if (fDebug) fprintf(fp,"/* first_col : %2d,row :even, 1/\\\\/2 */\n",first_col);
      //                  x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
      //                  z_b[row] = ((q - q_b[row]) / (q + q_b[row])) * z_bh + (z - z_bh);
      //               } else if (row%2==1) { /* 1\//\2 */
      //                  if (fDebug) fprintf(fp,"/* first_col : %2d,row : odd, 1\\//\\2 */\n",first_col);
      //                  x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
      //                  z_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * z_bh + (z_b[row] - z_bh);
      //               }
      //            }
      //         } else if ((first_col<=5) || (first_col>=34)) { // 10 mm pad
      //            Double_t z_bh = (10./2 * TMath::Sqrt(3) / 3) / 2;
      //            Double_t x_bh = (10./2 + gap) / 2;
      //            if ((first_col<=5) && (first_col%2==0)) { // first_col = 0,2,4
      //               if (row%2==0) { /* 1\//\2 */
      //                  if (fDebug) fprintf(fp,"/* first_col :even,row :even, 1\\//\\2 */\n");
      //                  x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
      //                  z_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * z_bh + (z_b[row] - z_bh);
      //               } else if (row%2==1) { /* 1/\\/2 */
      //                  if (fDebug) fprintf(fp,"/* first_col :even,row : odd, 1/\\\\/2 */\n");
      //                  x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
      //                  z_b[row] = ((q - q_b[row]) / (q + q_b[row])) * z_bh + (z - z_bh);
      //               }
      //            } else if ((first_col<=5) && (first_col%2==1)) { // first_col = 1,3,5
      //               if (row%2==0) { /* 1/\\/2 */
      //                  if (fDebug) fprintf(fp,"/* first_col : odd,row :even, 1/\\\\/2 */\n");
      //                  x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
      //                  z_b[row] = ((q - q_b[row]) / (q + q_b[row])) * z_bh + (z - z_bh);
      //               } else if (row%2==1) { /* 1\//\2 */
      //                  if (fDebug) fprintf(fp,"/* first_col : odd,row : odd, 1\\//\\2 */\n");
      //                  x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
      //                  z_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * z_bh + (z_b[row] - z_bh);
      //               }
      //            } else if ((first_col>=34) && (first_col%2==1)) { // first_col = 35,37,39
      //               if (row%2==0) { /* 1\//\2 */
      //                  if (fDebug) fprintf(fp,"/* first_col :even,row :even, 1\\//\\2 */\n");
      //                  x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
      //                  z_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * z_bh + (z_b[row] - z_bh);
      //               } else if (row%2==1) { /* 1/\\/2 */
      //                  if (fDebug) fprintf(fp,"/* first_col :even,row : odd, 1/\\\\/2 */\n");
      //                  x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
      //                  z_b[row] = ((q - q_b[row]) / (q + q_b[row])) * z_bh + (z - z_bh);
      //               }
      //            } else if ((first_col>=34) && (first_col%2==0)) { // first_col = 34,36,38
      //               if (row%2==0) { /* 1/\\/2 */
      //                  if (fDebug) fprintf(fp,"/* first_col : odd,row :even, 1/\\\\/2 */\n");
      //                  x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
      //                  z_b[row] = ((q - q_b[row]) / (q + q_b[row])) * z_bh + (z - z_bh);
      //               } else if (row%2==1) { /* 1\//\2 */
      //                  if (fDebug) fprintf(fp,"/* first_col : odd,row : odd, 1\\//\\2 */\n");
      //                  x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
      //                  z_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * z_bh + (z_b[row] - z_bh);
      //               }
      //            }
      //         }
      //         q_b[row] += q;
      //      }
      //
      //      if (fDetailDebug) fprintf(fp,"x_b[%2d] = %5.1f, z_b[%2d] = %5.1f, q_b[%2d] = %5.1f, x = %5.1f, z = %5.1f, q = %5.1f\n",row,x_b[row],row,z_b[row],row,q_b[row],x,z,q);
      //
      //   } else if (nBeamHitsRow[row]>=3) { // Center of Charge
      //      sqx[row] += q * x;
      //      z_b[row] =  z; 
      //      q_b[row] += q;
      //
      //      if (fDetailDebug) fprintf(fp,"x_b[%2d] = %5.1f, z_b[%2d] = %5.1f, q_b[%2d] = %5.1f, x = %5.1f, z = %5.1f, q = %5.1f, sqx[%2d] = %5.1f\n",row,x_b[row],row,z_b[row],row,q_b[row],x,z,q,row,sqx[row]);
      //
      //   } else if (nBeamHitsRow[row]==1) {
      //      x_b[row] = x;
      //      z_b[row] = z;
      //      q_b[row] = q;
      //
      //      if (fDetailDebug) fprintf(fp,"x_b[%2d] = %5.1f, z_b[%2d] = %5.1f, q_b[%2d] = %5.1f, x = %5.1f, z = %5.1f, q = %5.1f\n",row,x_b[row],row,z_b[row],row,q_b[row],x,z,q);
      //
      //   }
      //}
      //
      //if (fDebug) fprintf(fp,"\n* Only row = 0,1,18,19 join Beam fitting\n");      
      //for (Int_t iRow=0; iRow<20; iRow++) {
      //   if (nBeamHitsRow[iRow]>=3) x_b[iRow] = sqx[iRow] / q_b[iRow];
      //   if ((x_b[iRow]==-1000.) || (nBeamHitsRow[iRow]<=1)) continue;
      //   if (!((iRow==0) || (iRow==1) || (iRow==18) || (iRow==19))) continue;
      //   n++;
      //   Sx += x_b[iRow];
      //   Sz += z_b[iRow];
      //   Sxx += x_b[iRow]*x_b[iRow];
      //   Sxz += x_b[iRow] * z_b[iRow];
      //   Q_b += q_b[iRow];
      //
      //   //if ((!fLSM) || fDrawFig) fhBeamHits->SetPoint(n-1,x_b[iRow],z_b[iRow]);
      //
      //   if (fDetailDebug) fprintf(fp,"x_b[%2d] = %5.1f, z_b[%2d] = %5.1f, q_b[%2d] = %5.1f\n",iRow,x_b[iRow],iRow,z_b[iRow],iRow,q_b[iRow]);
      //   if (fDetailDebug) fprintf(fp,"Sx = %5.1f, Sz = %5.1f, Sxx = %5.1f, Sxz = %5.1f\n",Sx,Sz,Sxx,Sxz);
      //}
      //if (n<=2) {
      //   if (fDebug) fprintf(fp,"Skip: n <= 2\n");      
      //   continue;
      //}
      //
      //if (fLSM) {
      //   if (fDebug) fprintf(fp,"Beam: Least Square Method selected.\n");
      //   Double_t d   = 0;
      //   Double_t X_b = 0;
      //
      //   d = n * Sxx - Sx * Sx;
      //   if (d!=0) {
      //      a_bz= (n * Sxz - Sx * Sz) / d;
      //      b_bz= (Sxx * Sz - Sxz * Sx) / d;
      //      X_b = (z_b[0] - b_bz) / a_bz;
      //   } else {
      //      a_bz= b_bz= 0;
      //      X_b = x_b[0];
      //   }
      //   if (fDebug) fprintf(fp,"x_b[0] = %5.1f, Q_b = %5.1f, a_bz= %5.1f, b_bz= %5.1f\n",x_b[0],Q_b,a_bz,b_bz);
      //
      //   //if (fDrawFig) {
      //   //   if (a_bz!=0.) {
      //   //      fhBeamRay->SetParameter(0,a_bz);
      //   //      fhBeamRay->SetParameter(1,b_bz);
      //   //   }
      //   //}
      //
      //   if (TMath::Abs(a_bz) >= 4.) {
      //      BeamIsGood = true;
      //   } else {
      //      if (fDebug) fprintf(fp,"Skip: |a_bz| < 4.\n");
      //      continue;
      //   }
      //
      //   for (Int_t iRow=0; iRow<20; iRow++) {
      //      if (x_b[iRow]==-1000.) continue;
      //      if (a_bz==0.) continue;
      //      Double_t resXa_bz = x_b[iRow] - (z_b[iRow] - b_bz) / a_bz;
      //      if (fDebug) fprintf(fp,"iRow = %2d, x_b[%2d] = %5.1f, resXa_bz = %5.1f\n",iRow,iRow,x_b[iRow],resXa_bz);
      //   }
      //} else {
      //   if (fDebug) fprintf(fp,"Beam: TGraph pol1 Fitting selected.\n");
      //
      //   Double_t chi2 = 0.;
      //   if (fDetailDebug) printf("\n Evt = %d, cls = %d, n = %d",fEvt,icls,n);
      //   //fhBeamHits->Fit("pol1");
      //   p0 = fhBeamHits->GetFunction("pol1")->GetParameter(0);
      //   p1 = fhBeamHits->GetFunction("pol1")->GetParameter(1);
      //   chi2 = fhBeamHits->GetFunction("pol1")->GetChisquare();
      //   if (fDebug) fprintf(fp,"p0 = %5.1f, p1 = %5.1f, chi2 = %5.1f\n",p0,p1,chi2);
      //   if (TMath::Abs(p0) >= 4.) {
      //      BeamIsGood = true;
      //   } else {
      //      if (fDebug) fprintf(fp,"Skip: |p0| < 4.\n");
      //      continue;
      //   }
      //
      //   for (Int_t iRow=0; iRow<20; iRow++) {
      //      if (x_b[iRow]==-1000.) continue;
      //      Double_t resXp0 = x_b[iRow] - (z_b[iRow] - p1) / p0;
      //      if (fDebug) fprintf(fp,"iRow = %2d, x_b[%2d] = %5.1f, resXp0 = %5.1f\n",iRow,iRow,x_b[iRow],resXp0);
      //   }
      //}
      //z0 = -40.;
      //x0 = (z0 - b_bz) / a_bz;
      ///*--------------------------------------------------------------------------*/

      // Derive Linear Equation of Beam (z,y)
      if (fDebug) fprintf(fp,"// Derive Linear Equation of Beam (z,y)\n");
      //if (fDebug) fprintf(fp,"Only row = 0,1,18,19 join Beam fitting\n");      
      //Double_t y0   = 0.;
      //Double_t z_by = 0.;
      Double_t z_bx = 0.;
      Double_t y_b  = 0.;
      //Double_t Sy   = 0.;
      //Double_t Szy  = 0.;
      //Double_t a_by = 0.;
      //Double_t b_by = 0.;
      //Double_t Szz  = 0.;
      //Sz  = 0.;
      Int_t n   = 0;
      //
      nBeamHits = cls_array.GetEntriesFast();
      if (fDebug) fprintf(fp,"nBeamHits = %d\n",nBeamHits);
      for (Int_t iHit=0; iHit<nBeamHits; iHit++) {
         TCatPulseShape *hit = (TCatPulseShape*) cls_array[iHit];
      //   Int_t id = hit->GetID();
      //   Int_t col = id / 10;
      //   Int_t row = id % 10;
      //   if ((id > 69 && id < 330) && (col%2 == 0)) {
      //      col = col - 1;
      //      row = row + 10;
      //   }
         Double_t x = hit->GetX();
      //   Double_t z = hit->GetZ();
      //   Double_t q = hit->GetCharge();
         Float_t  off = hit->GetOffset();
      //   Float_t  diffoff = off - BeamOff;
      //
      //   if (!(TMath::Abs(z)>=35.)) continue;
      //
         // 0.70 cm/us = 0.70 x 10^(-1) mm/ns @ phys1802
         //y_b  = (off - BeamOff) * 20. * (fDriftVelocity / 100.);
         y_b  = off * 20. * (fDriftVelocity / 100.);
      //   z_by  = z;
         z_bx  = x;
      //
      //   if (fDetailDebug) fprintf(fp,"iHit  = %2d, id  = %3d, x = %5.1f, z = %5.1f, off  = %5.1f, y_b = %5.1f, q = %6.1f, diffoff  = %4.1f\n",iHit,id,x,z,off,y_b,q,diffoff);
      //
      //   if ((z_by==0.) || (y_b==0.)) continue;
         n++;
      //   Sz += z_by;
      //   Sy += y_b;
      //   Szz += z_by * z_by;
      //   Szy += z_by * y_b;
         //if ((!fLSM) || fDrawFig) fhBeamYHits->SetPoint(n-1,z_by,y_b);
         //if ((!fLSM) || fDrawFig) fhBeamYHits->SetPoint(n,z_by,y_b);
         if ((!fLSM) || fDrawFig) fhBeamYHits->SetPoint(n,z_bx,y_b);
         //if (fDetailDebug) fprintf(fp,"z_by = %5.1f, y_b = %6.1f\n",z_by,y_b);
         if (fDetailDebug) fprintf(fp,"z_by = %5.1f, y_b = %6.1f\n",z_bx,y_b);
      }
      //
      //if (fLSM) {
      //   if (fDebug) fprintf(fp,"Beam: Least Square Method selected.\n");
      //   Double_t d   = 0;
      //
      //   d = n * Szz - Sz * Sz;
      //   if (d!=0) {
      //      a_by = (n * Szy - Sz * Sy) / d;
      //      b_by = (Szz * Sy - Szy * Sz) / d;
      //   } else {
      //      a_by = b_by = 0;
      //   }
      //   if (fDebug) fprintf(fp,"a_by = %5.1f, b_by = %5.1f\n",a_by,b_by);
      //
      //   if (fDrawFig) {
      //      if (a_by!=0.) {
      //         fhBeamYRay->SetParameter(0,a_by);
      //         fhBeamYRay->SetParameter(1,b_by);
      //      }
      //   }
      //
      //   if (TMath::Abs(a_by) != 0.) {
      //      BeamIsGood = true;
      //   } else {
      //      if (fDebug) fprintf(fp,"Skip: |a_by| = 0.\n");
      //      BeamIsGood = false;
      //      continue;
      //   }
      //}
      //y0 = a_by * z0 + b_by;
      BeamIsGood = true;
      
      /*Recoil--------------------------------------------------------------------*/
      if (fDebug) fprintf(fp,"\n*Recoil :\n");
      Int_t nRecoilHits = recoil_cand.size();
      if (nRecoilHits==0) {
         if (fDebug) fprintf(fp,"Skip: No Recoil Candidate Hits\n");      
         continue;
      }

      if (fDebug) fprintf(fp,"recoil %d hits:\n",nRecoilHits);

      std::vector<std::vector<TCatPulseShape*> > recoil;
      Int_t nRecoilCls = 0;
      TObjArray recoil_cand_array;
      it = recoil_cand.begin();
      itend = recoil_cand.end();
      for (; it != itend; it++) {
         recoil_cand_array.Add((*it));
      }

      TCatPulseShape::SetSortType(TCatPulseShape::kID);
      //TCatPulseShape::SetSortType(TCatPulseShape::kTiming);
      TCatPulseShape::SetSortOrder(TCatPulseShape::kASC);
      recoil_cand_array.Sort();
      nRecoilHits = recoil_cand_array.GetEntriesFast();

      if (fDebug || fDetailDebug) {
         for (Int_t iHit=0; iHit < nRecoilHits; iHit++) {
            TCatPulseShape *hit = (TCatPulseShape*) recoil_cand_array[iHit];
            Int_t id = hit->GetID();
            Float_t off = hit->GetOffset();
            if (fDebug) fprintf(fp,"id = %3d, off = %6.1f\n",id,off);      
         }
      }

      Bool_t fFirstFail = false;
      Bool_t fSwitch = true;
      Int_t fail_iHit = -1;
      for (Int_t iHit=0; iHit < nRecoilHits;) {
         TCatPulseShape *hit = (TCatPulseShape*) recoil_cand_array[iHit];
         Bool_t IsAdd = false;
         if (!hit) {
            if (fDetailDebug) fprintf(fp,"Skip: No Object iHit = %d\n",iHit);
            for (Int_t jHit=0; jHit < nRecoilHits; jHit++) {
               TCatPulseShape *hit_tmp = (TCatPulseShape*) recoil_cand_array[jHit];
               if (!hit_tmp) {
                  if (fDetailDebug) fprintf(fp,"Skip: No Object jHit = %d\n",jHit);
                  continue;
               }
               Int_t id_tmp = hit_tmp->GetID();
               if (fDebug) fprintf(fp,"jHit = %d, id_tmp = %d\n",jHit,id_tmp);
            }
            if (fDetailDebug) printf("Skip: No Object iHit = %d\n",iHit);
            iHit++;
            continue;
         }
         Int_t id = hit->GetID();
         Int_t col = id / 10;
         Int_t row = id % 10;
         if ((id > 69 && id < 330) && (col%2 == 0)) {
            col = col - 1;
            row = row + 10;
         }
         Float_t off = hit->GetOffset();
         //Float_t diffoff = off - BeamOff;
         //if (fDetailDebug) fprintf(fp,"iHit  = %d, id = %3d, col = %2d, row = %2d, off = %6.1f, diffoff = %6.1f\n",iHit,id,col,row,off,diffoff);      
      

         Int_t iHit2 = iHit + 1;
         for (; iHit2 < nRecoilHits;) {
            if (iHit==iHit2) continue;
            TCatPulseShape *hit2 = (TCatPulseShape*) recoil_cand_array[iHit2];
            if (!hit2) {
               if (fDetailDebug) fprintf(fp,"Skip: No Object iHit2 = %d\n",iHit2);      
               iHit2++;
               continue;
            }
            Int_t id2 = hit2->GetID();
            Float_t off2 = hit2->GetOffset();
            Int_t col2 = id2 / 10;
            Int_t row2 = id2 % 10;
            if ((id2 > 69 && id2 < 330) && (col2%2 == 0)) {
               col2 = col2 - 1;
               row2 = row2 + 10;
            }

            Int_t diffcol = TMath::Abs(col - col2);
            Int_t diffrow = TMath::Abs(row - row2);
            //Float_t diffoff2 = off2 - BeamOff;
            Float_t diffoff = off - off2;

            //if (fDetailDebug) fprintf(fp,"iHit2 = %d, id2 = %3d, col2 = %2d, row2 = %2d, off2 = %6.1f, diffoff2 = %6.1f\n",iHit2,id2,col2,row2,off2,diffoff2);      

            if (col2<=5) {
               if ((col>=9) && (col<34)) diffcol = (diffcol-4) + 2;
               if (col>=34) diffcol = (diffcol-29) + 14;
            } else if (col2==6) {
               if ((col>=9) && (col<34)) diffcol = (diffcol+1)/2;
               if (col>=34) diffcol = (diffcol-28) + 15;
            } else if (col2==7) {
               if (col<=5) diffcol = (diffcol-3) + 2;
               if ((col>=9) && (col<34)) diffcol = diffcol/2;
               if (col>=34) diffcol = (diffcol-27) + 15;
            } else if ((col2>=9) && (col2<=29)) {
               if (col<=5) diffcol = (diffcol-24)/2 + 12;
               if (col==6) diffcol = (diffcol+1)/2;
               if ((col>=7) && (col<34)) diffcol = diffcol/2;
               if (col>=34) diffcol = (diffcol-25)/2 + 15;
            } else if (col2==31) {
               if (col<=5) diffcol = (diffcol-24)/2 + 12;
               if (col==6) diffcol = 13;
               if ((col>=7) && (col<34)) diffcol = diffcol/2;
               if (col>=34) diffcol = (diffcol-3) + 1;
            } else if (col2==33) {
               if (col<=5) diffcol = (diffcol-24)/2 + 12;
               if (col==6) diffcol = 14;
               if ((col>=7) && (col<34)) diffcol = diffcol/2;
            } else if (col2>=34) {
               if (col<=5) diffcol = (diffcol-25)/2 + 12;
               if (col==6) diffcol = 14;
               if (col==33) diffcol = 1;
               if ((col>=7) && (col<=29)) diffcol = (diffcol+1)/2;
            }

            // col=4,5, col2=7
            if ((id==40) || (id==50)) {
               if ((id2==70) || (id==71)) diffrow = 0;
               if ((id2==72) || (id==73)) diffrow = 1;
               if ((id2==74) || (id==75)) diffrow = 2;
               if ((id2==76) || (id==77)) diffrow = 3;
               if ((id2==78) || (id==79)) diffrow = 4;
               if ((id2==80) || (id==81)) diffrow = 5;
               if ((id2==82) || (id==83)) diffrow = 6;
               if ((id2==84) || (id==85)) diffrow = 7;
               if ((id2==86) || (id==87)) diffrow = 8;
               if ((id2==88) || (id==89)) diffrow = 9;
            }
            if ((id==41) || (id==51)) {
               if ((id2==70) || (id==71)) diffrow = 1;
               if ((id2==72) || (id==73)) diffrow = 0;
               if ((id2==74) || (id==75)) diffrow = 1;
               if ((id2==76) || (id==77)) diffrow = 2;
               if ((id2==78) || (id==79)) diffrow = 3;
               if ((id2==80) || (id==81)) diffrow = 4;
               if ((id2==82) || (id==83)) diffrow = 5;
               if ((id2==84) || (id==85)) diffrow = 6;
               if ((id2==86) || (id==87)) diffrow = 7;
               if ((id2==88) || (id==89)) diffrow = 8;
            }
            if ((id==42) || (id==52)) {
               if ((id2==70) || (id==71)) diffrow = 2;
               if ((id2==72) || (id==73)) diffrow = 1;
               if ((id2==74) || (id==75)) diffrow = 0;
               if ((id2==76) || (id==77)) diffrow = 1;
               if ((id2==78) || (id==79)) diffrow = 2;
               if ((id2==80) || (id==81)) diffrow = 3;
               if ((id2==82) || (id==83)) diffrow = 4;
               if ((id2==84) || (id==85)) diffrow = 5;
               if ((id2==86) || (id==87)) diffrow = 6;
               if ((id2==88) || (id==89)) diffrow = 7;
            }
            if ((id==43) || (id==53)) {
               if ((id2==70) || (id==71)) diffrow = 3;
               if ((id2==72) || (id==73)) diffrow = 2;
               if ((id2==74) || (id==75)) diffrow = 1;
               if ((id2==76) || (id==77)) diffrow = 0;
               if ((id2==78) || (id==79)) diffrow = 1;
               if ((id2==80) || (id==81)) diffrow = 2;
               if ((id2==82) || (id==83)) diffrow = 3;
               if ((id2==84) || (id==85)) diffrow = 4;
               if ((id2==86) || (id==87)) diffrow = 5;
               if ((id2==88) || (id==89)) diffrow = 6;
            }
            if ((id==44) || (id==54)) {
               if ((id2==70) || (id==71)) diffrow = 4;
               if ((id2==72) || (id==73)) diffrow = 3;
               if ((id2==74) || (id==75)) diffrow = 2;
               if ((id2==76) || (id==77)) diffrow = 1;
               if ((id2==78) || (id==79)) diffrow = 0;
               if ((id2==80) || (id==81)) diffrow = 1;
               if ((id2==82) || (id==83)) diffrow = 2;
               if ((id2==84) || (id==85)) diffrow = 3;
               if ((id2==86) || (id==87)) diffrow = 4;
               if ((id2==88) || (id==89)) diffrow = 5;
            }
            if ((id==45) || (id==55)) {
               if ((id2==70) || (id==71)) diffrow = 5;
               if ((id2==72) || (id==73)) diffrow = 4;
               if ((id2==74) || (id==75)) diffrow = 3;
               if ((id2==76) || (id==77)) diffrow = 2;
               if ((id2==78) || (id==79)) diffrow = 1;
               if ((id2==80) || (id==81)) diffrow = 0;
               if ((id2==82) || (id==83)) diffrow = 1;
               if ((id2==84) || (id==85)) diffrow = 2;
               if ((id2==86) || (id==87)) diffrow = 3;
               if ((id2==88) || (id==89)) diffrow = 4;
            }
            if ((id==46) || (id==56)) {
               if ((id2==70) || (id==71)) diffrow = 6;
               if ((id2==72) || (id==73)) diffrow = 5;
               if ((id2==74) || (id==75)) diffrow = 4;
               if ((id2==76) || (id==77)) diffrow = 3;
               if ((id2==78) || (id==79)) diffrow = 2;
               if ((id2==80) || (id==81)) diffrow = 1;
               if ((id2==82) || (id==83)) diffrow = 0;
               if ((id2==84) || (id==85)) diffrow = 1;
               if ((id2==86) || (id==87)) diffrow = 2;
               if ((id2==88) || (id==89)) diffrow = 3;
            }
            if ((id==47) || (id==57)) {
               if ((id2==70) || (id==71)) diffrow = 7;
               if ((id2==72) || (id==73)) diffrow = 6;
               if ((id2==74) || (id==75)) diffrow = 5;
               if ((id2==76) || (id==77)) diffrow = 4;
               if ((id2==78) || (id==79)) diffrow = 3;
               if ((id2==80) || (id==81)) diffrow = 2;
               if ((id2==82) || (id==83)) diffrow = 1;
               if ((id2==84) || (id==85)) diffrow = 0;
               if ((id2==86) || (id==87)) diffrow = 1;
               if ((id2==88) || (id==89)) diffrow = 2;
            }
            if ((id==48) || (id==58)) {
               if ((id2==70) || (id==71)) diffrow = 8;
               if ((id2==72) || (id==73)) diffrow = 7;
               if ((id2==74) || (id==75)) diffrow = 6;
               if ((id2==76) || (id==77)) diffrow = 5;
               if ((id2==78) || (id==79)) diffrow = 4;
               if ((id2==80) || (id==81)) diffrow = 3;
               if ((id2==82) || (id==83)) diffrow = 2;
               if ((id2==84) || (id==85)) diffrow = 1;
               if ((id2==86) || (id==87)) diffrow = 0;
               if ((id2==88) || (id==89)) diffrow = 1;
            }
            if ((id==49) || (id==59)) {
               if ((id2==70) || (id==71)) diffrow = 9;
               if ((id2==72) || (id==73)) diffrow = 8;
               if ((id2==74) || (id==75)) diffrow = 7;
               if ((id2==76) || (id==77)) diffrow = 6;
               if ((id2==78) || (id==79)) diffrow = 5;
               if ((id2==80) || (id==81)) diffrow = 4;
               if ((id2==82) || (id==83)) diffrow = 3;
               if ((id2==84) || (id==85)) diffrow = 2;
               if ((id2==86) || (id==87)) diffrow = 1;
               if ((id2==88) || (id==89)) diffrow = 0;
            }

            // col=5, col2=9
            if (id==50) {
               if ((id2==90)  || (id==91))  diffrow = 0;
               if ((id2==92)  || (id==93))  diffrow = 1;
               if ((id2==94)  || (id==95))  diffrow = 2;
               if ((id2==96)  || (id==97))  diffrow = 3;
               if ((id2==98)  || (id==99))  diffrow = 4;
               if ((id2==100) || (id==101)) diffrow = 5;
               if ((id2==102) || (id==103)) diffrow = 6;
               if ((id2==104) || (id==105)) diffrow = 7;
               if ((id2==106) || (id==107)) diffrow = 8;
               if ((id2==108) || (id==109)) diffrow = 9;
            }
            if (id==51) {
               if ((id2==90)  || (id==91))  diffrow = 1;
               if ((id2==92)  || (id==93))  diffrow = 0;
               if ((id2==94)  || (id==95))  diffrow = 1;
               if ((id2==96)  || (id==97))  diffrow = 2;
               if ((id2==98)  || (id==99))  diffrow = 3;
               if ((id2==100) || (id==101)) diffrow = 4;
               if ((id2==102) || (id==103)) diffrow = 5;
               if ((id2==104) || (id==105)) diffrow = 6;
               if ((id2==106) || (id==107)) diffrow = 7;
               if ((id2==108) || (id==109)) diffrow = 8;
            }
            if (id==52) {
               if ((id2==90)  || (id==91))  diffrow = 2;
               if ((id2==92)  || (id==93))  diffrow = 1;
               if ((id2==94)  || (id==95))  diffrow = 0;
               if ((id2==96)  || (id==97))  diffrow = 1;
               if ((id2==98)  || (id==99))  diffrow = 2;
               if ((id2==100) || (id==101)) diffrow = 3;
               if ((id2==102) || (id==103)) diffrow = 4;
               if ((id2==104) || (id==105)) diffrow = 5;
               if ((id2==106) || (id==107)) diffrow = 6;
               if ((id2==108) || (id==109)) diffrow = 7;
            }
            if (id==53) {
               if ((id2==90)  || (id==91))  diffrow = 3;
               if ((id2==92)  || (id==93))  diffrow = 2;
               if ((id2==94)  || (id==95))  diffrow = 1;
               if ((id2==96)  || (id==97))  diffrow = 0;
               if ((id2==98)  || (id==99))  diffrow = 1;
               if ((id2==100) || (id==101)) diffrow = 2;
               if ((id2==102) || (id==103)) diffrow = 3;
               if ((id2==104) || (id==105)) diffrow = 4;
               if ((id2==106) || (id==107)) diffrow = 5;
               if ((id2==108) || (id==109)) diffrow = 6;
            }
            if (id==54) {
               if ((id2==90)  || (id==91))  diffrow = 4;
               if ((id2==92)  || (id==93))  diffrow = 3;
               if ((id2==94)  || (id==95))  diffrow = 2;
               if ((id2==96)  || (id==97))  diffrow = 1;
               if ((id2==98)  || (id==99))  diffrow = 0;
               if ((id2==100) || (id==101)) diffrow = 1;
               if ((id2==102) || (id==103)) diffrow = 2;
               if ((id2==104) || (id==105)) diffrow = 3;
               if ((id2==106) || (id==107)) diffrow = 4;
               if ((id2==108) || (id==109)) diffrow = 5;
            }
            if (id==55) {
               if ((id2==90)  || (id==91))  diffrow = 5;
               if ((id2==92)  || (id==93))  diffrow = 4;
               if ((id2==94)  || (id==95))  diffrow = 3;
               if ((id2==96)  || (id==97))  diffrow = 2;
               if ((id2==98)  || (id==99))  diffrow = 1;
               if ((id2==100) || (id==101)) diffrow = 0;
               if ((id2==102) || (id==103)) diffrow = 1;
               if ((id2==104) || (id==105)) diffrow = 2;
               if ((id2==106) || (id==107)) diffrow = 3;
               if ((id2==108) || (id==109)) diffrow = 4;
            }
            if (id==56) {
               if ((id2==90)  || (id==91))  diffrow = 6;
               if ((id2==92)  || (id==93))  diffrow = 5;
               if ((id2==94)  || (id==95))  diffrow = 4;
               if ((id2==96)  || (id==97))  diffrow = 3;
               if ((id2==98)  || (id==99))  diffrow = 2;
               if ((id2==100) || (id==101)) diffrow = 1;
               if ((id2==102) || (id==103)) diffrow = 0;
               if ((id2==104) || (id==105)) diffrow = 1;
               if ((id2==106) || (id==107)) diffrow = 2;
               if ((id2==108) || (id==109)) diffrow = 3;
            }
            if (id==57) {
               if ((id2==90)  || (id==91))  diffrow = 7;
               if ((id2==92)  || (id==93))  diffrow = 6;
               if ((id2==94)  || (id==95))  diffrow = 5;
               if ((id2==96)  || (id==97))  diffrow = 4;
               if ((id2==98)  || (id==99))  diffrow = 3;
               if ((id2==100) || (id==101)) diffrow = 2;
               if ((id2==102) || (id==103)) diffrow = 1;
               if ((id2==104) || (id==105)) diffrow = 0;
               if ((id2==106) || (id==107)) diffrow = 1;
               if ((id2==108) || (id==109)) diffrow = 2;
            }
            if (id==58) {
               if ((id2==90)  || (id==91))  diffrow = 8;
               if ((id2==92)  || (id==93))  diffrow = 7;
               if ((id2==94)  || (id==95))  diffrow = 6;
               if ((id2==96)  || (id==97))  diffrow = 5;
               if ((id2==98)  || (id==99))  diffrow = 4;
               if ((id2==100) || (id==101)) diffrow = 3;
               if ((id2==102) || (id==103)) diffrow = 2;
               if ((id2==104) || (id==105)) diffrow = 1;
               if ((id2==106) || (id==107)) diffrow = 0;
               if ((id2==108) || (id==109)) diffrow = 1;
            }
            if (id==59) {
               if ((id2==90)  || (id==91))  diffrow = 9;
               if ((id2==92)  || (id==93))  diffrow = 8;
               if ((id2==94)  || (id==95))  diffrow = 7;
               if ((id2==96)  || (id==97))  diffrow = 6;
               if ((id2==98)  || (id==99))  diffrow = 5;
               if ((id2==100) || (id==101)) diffrow = 4;
               if ((id2==102) || (id==103)) diffrow = 3;
               if ((id2==104) || (id==105)) diffrow = 2;
               if ((id2==106) || (id==107)) diffrow = 1;
               if ((id2==108) || (id==109)) diffrow = 0;
            }

            // col=6, col2=7,9
            if (id==60) {
               if ((id2==70) || (id2==90)) diffrow = 1;
               if ((id2>=71) || (id2>=91)) diffrow = diffrow - 1;
            }
            if (id==61) {
               if ((id2==70) || (id2==90)) diffrow = 2;
               if ((id2==71) || (id2==91)) diffrow = 1;
               if ((id2>=72) || (id2>=92)) diffrow = diffrow - 1;
            }
            if (id==62) {
               if ((id2==70) || (id2==90)) diffrow = 5;
               if ((id2==71) || (id2==91)) diffrow = 4;
               if ((id2==72) || (id2==92)) diffrow = 3;
               if ((id2==73) || (id2==93)) diffrow = 2;
               if ((id2==74) || (id2==94)) diffrow = 1;
               if ((id2>=75) || (id2>=95)) diffrow = diffrow - 3;
            }
            if (id==63) {
               if ((id2==70) || (id2==90)) diffrow = 6;
               if ((id2==71) || (id2==91)) diffrow = 5;
               if ((id2==72) || (id2==92)) diffrow = 4;
               if ((id2==73) || (id2==93)) diffrow = 3;
               if ((id2==74) || (id2==94)) diffrow = 2;
               if ((id2==75) || (id2==95)) diffrow = 1;
               if ((id2>=76) || (id2>=96)) diffrow = diffrow - 3;
            }
            if (id==64) {
               if ((id2==70) || (id2==90)) diffrow = 9;
               if ((id2==71) || (id2==91)) diffrow = 8;
               if ((id2==72) || (id2==92)) diffrow = 7;
               if ((id2==73) || (id2==93)) diffrow = 6;
               if ((id2==74) || (id2==94)) diffrow = 5;
               if ((id2==75) || (id2==95)) diffrow = 4;
               if ((id2==76) || (id2==96)) diffrow = 3;
               if ((id2==77) || (id2==97)) diffrow = 2;
               if ((id2==78) || (id2==98)) diffrow = 1;
               if ((id2>=79) || (id2>=99)) diffrow = diffrow - 5;
            }
            if (id==65) {
               if ((id2==70) || (id2==90))  diffrow = 10;
               if ((id2==71) || (id2==91))  diffrow =  9;
               if ((id2==72) || (id2==92))  diffrow =  8;
               if ((id2==73) || (id2==93))  diffrow =  7;
               if ((id2==74) || (id2==94))  diffrow =  6;
               if ((id2==75) || (id2==95))  diffrow =  5;
               if ((id2==76) || (id2==96))  diffrow =  4;
               if ((id2==77) || (id2==97))  diffrow =  3;
               if ((id2==78) || (id2==98))  diffrow =  2;
               if ((id2==79) || (id2==99))  diffrow =  1;
               if ((id2>=80) || (id2>=100)) diffrow = diffrow - 5;
            }
            if (id==66) {
               if ((id2==70) || (id2==90))  diffrow = 13;
               if ((id2==71) || (id2==91))  diffrow = 12;
               if ((id2==72) || (id2==92))  diffrow = 11;
               if ((id2==73) || (id2==93))  diffrow = 10;
               if ((id2==74) || (id2==94))  diffrow =  9;
               if ((id2==75) || (id2==95))  diffrow =  8;
               if ((id2==76) || (id2==96))  diffrow =  7;
               if ((id2==77) || (id2==97))  diffrow =  6;
               if ((id2==78) || (id2==98))  diffrow =  5;
               if ((id2==79) || (id2==99))  diffrow =  4;
               if ((id2==80) || (id2==100)) diffrow =  3;
               if ((id2==81) || (id2==101)) diffrow =  2;
               if ((id2==82) || (id2==102)) diffrow =  1;
               if ((id2>=83) || (id2>=103)) diffrow = diffrow - 7;
            }
            if (id==67) {
               if ((id2==70) || (id2==90))  diffrow = 14;
               if ((id2==71) || (id2==91))  diffrow = 13;
               if ((id2==72) || (id2==92))  diffrow = 12;
               if ((id2==73) || (id2==93))  diffrow = 11;
               if ((id2==74) || (id2==94))  diffrow = 10;
               if ((id2==75) || (id2==95))  diffrow =  9;
               if ((id2==76) || (id2==96))  diffrow =  8;
               if ((id2==77) || (id2==97))  diffrow =  7;
               if ((id2==78) || (id2==98))  diffrow =  6;
               if ((id2==79) || (id2==99))  diffrow =  5;
               if ((id2==80) || (id2==100)) diffrow =  4;
               if ((id2==81) || (id2==101)) diffrow =  3;
               if ((id2==82) || (id2==102)) diffrow =  2;
               if ((id2==83) || (id2==103)) diffrow =  1;
               if ((id2>=84) || (id2>=104)) diffrow = diffrow - 7;
            }
            if (id==68) {
               if ((id2==70) || (id2==90))  diffrow = 17;
               if ((id2==71) || (id2==91))  diffrow = 16;
               if ((id2==72) || (id2==92))  diffrow = 15;
               if ((id2==73) || (id2==93))  diffrow = 14;
               if ((id2==74) || (id2==94))  diffrow = 13;
               if ((id2==75) || (id2==95))  diffrow = 12;
               if ((id2==76) || (id2==96))  diffrow = 11;
               if ((id2==77) || (id2==97))  diffrow = 10;
               if ((id2==78) || (id2==98))  diffrow =  9;
               if ((id2==79) || (id2==99))  diffrow =  8;
               if ((id2==80) || (id2==100)) diffrow =  7;
               if ((id2==81) || (id2==101)) diffrow =  6;
               if ((id2==82) || (id2==102)) diffrow =  5;
               if ((id2==83) || (id2==103)) diffrow =  4;
               if ((id2==84) || (id2==104)) diffrow =  3;
               if ((id2==85) || (id2==105)) diffrow =  2;
               if ((id2==86) || (id2==106)) diffrow =  1;
               if ((id2>=87) || (id2>=107)) diffrow = diffrow - 9;
            }
            if (id==69) {
               if ((id2==70) || (id2==90))  diffrow = 18;
               if ((id2==71) || (id2==91))  diffrow = 17;
               if ((id2==72) || (id2==92))  diffrow = 16;
               if ((id2==73) || (id2==93))  diffrow = 15;
               if ((id2==74) || (id2==94))  diffrow = 14;
               if ((id2==75) || (id2==95))  diffrow = 13;
               if ((id2==76) || (id2==96))  diffrow = 12;
               if ((id2==77) || (id2==97))  diffrow = 11;
               if ((id2==78) || (id2==98))  diffrow = 10;
               if ((id2==79) || (id2==99))  diffrow =  9;
               if ((id2==80) || (id2==100)) diffrow =  8;
               if ((id2==81) || (id2==101)) diffrow =  7;
               if ((id2==82) || (id2==102)) diffrow =  6;
               if ((id2==83) || (id2==103)) diffrow =  5;
               if ((id2==84) || (id2==104)) diffrow =  4;
               if ((id2==85) || (id2==105)) diffrow =  3;
               if ((id2==86) || (id2==106)) diffrow =  2;
               if ((id2==87) || (id2==107)) diffrow =  1;
               if ((id2>=88) || (id2>=108)) diffrow = diffrow - 9;
            }

            // col=29,31, col2=34
            if ((id==290) || (id==310)) {
               if (id2==340) diffrow =  0;
               if (id2==341) diffrow =  2;
               if (id2==342) diffrow =  4;
               if (id2==343) diffrow =  6;
               if (id2==344) diffrow =  8;
               if (id2==345) diffrow = 10;
               if (id2==346) diffrow = 12;
               if (id2==347) diffrow = 14;
               if (id2==348) diffrow = 16;
               if (id2==349) diffrow = 18;
            }
            if ((id==291) || (id==311)) {
               if (id2==340) diffrow =  0;
               if (id2==341) diffrow =  1;
               if (id2==342) diffrow =  3;
               if (id2==343) diffrow =  5;
               if (id2==344) diffrow =  7;
               if (id2==345) diffrow =  9;
               if (id2==346) diffrow = 11;
               if (id2==347) diffrow = 13;
               if (id2==348) diffrow = 15;
               if (id2==349) diffrow = 17;
            }
            if ((id==292) || (id==312)) {
               if (id2==340) diffrow =  1;
               if (id2==341) diffrow =  0;
               if (id2==342) diffrow =  2;
               if (id2==343) diffrow =  4;
               if (id2==344) diffrow =  6;
               if (id2==345) diffrow =  8;
               if (id2==346) diffrow = 10;
               if (id2==347) diffrow = 12;
               if (id2==348) diffrow = 14;
               if (id2==349) diffrow = 16;
            }
            if ((id==293) || (id==313)) {
               if (id2==340) diffrow =  2;
               if (id2==341) diffrow =  0;
               if (id2==342) diffrow =  1;
               if (id2==343) diffrow =  3;
               if (id2==344) diffrow =  5;
               if (id2==345) diffrow =  7;
               if (id2==346) diffrow =  9;
               if (id2==347) diffrow = 11;
               if (id2==348) diffrow = 13;
               if (id2==349) diffrow = 15;
            }
            if ((id==294) || (id==314)) {
               if (id2==340) diffrow =  3;
               if (id2==341) diffrow =  1;
               if (id2==342) diffrow =  0;
               if (id2==343) diffrow =  2;
               if (id2==344) diffrow =  4;
               if (id2==345) diffrow =  6;
               if (id2==346) diffrow =  8;
               if (id2==347) diffrow = 10;
               if (id2==348) diffrow = 12;
               if (id2==349) diffrow = 14;
            }
            if ((id==295) || (id==315)) {
               if (id2==340) diffrow =  4;
               if (id2==341) diffrow =  2;
               if (id2==342) diffrow =  0;
               if (id2==343) diffrow =  1;
               if (id2==344) diffrow =  3;
               if (id2==345) diffrow =  5;
               if (id2==346) diffrow =  7;
               if (id2==347) diffrow =  9;
               if (id2==348) diffrow = 11;
               if (id2==349) diffrow = 13;
            }
            if ((id==296) || (id==316)) {
               if (id2==340) diffrow =  5;
               if (id2==341) diffrow =  3;
               if (id2==342) diffrow =  1;
               if (id2==343) diffrow =  0;
               if (id2==344) diffrow =  2;
               if (id2==345) diffrow =  4;
               if (id2==346) diffrow =  6;
               if (id2==347) diffrow =  8;
               if (id2==348) diffrow = 10;
               if (id2==349) diffrow = 12;
            }
            if ((id==297) || (id==317)) {
               if (id2==340) diffrow =  6;
               if (id2==341) diffrow =  4;
               if (id2==342) diffrow =  2;
               if (id2==343) diffrow =  0;
               if (id2==344) diffrow =  1;
               if (id2==345) diffrow =  3;
               if (id2==346) diffrow =  5;
               if (id2==347) diffrow =  7;
               if (id2==348) diffrow =  9;
               if (id2==349) diffrow = 11;
            }
            if ((id==298) || (id==318)) {
               if (id2==340) diffrow =  7;
               if (id2==341) diffrow =  5;
               if (id2==342) diffrow =  3;
               if (id2==343) diffrow =  1;
               if (id2==344) diffrow =  0;
               if (id2==345) diffrow =  2;
               if (id2==346) diffrow =  4;
               if (id2==347) diffrow =  6;
               if (id2==348) diffrow =  8;
               if (id2==349) diffrow = 10;
            }
            if ((id==299) || (id==319)) {
               if (id2==340) diffrow =  8;
               if (id2==341) diffrow =  6;
               if (id2==342) diffrow =  4;
               if (id2==343) diffrow =  2;
               if (id2==344) diffrow =  0;
               if (id2==345) diffrow =  1;
               if (id2==346) diffrow =  3;
               if (id2==347) diffrow =  5;
               if (id2==348) diffrow =  7;
               if (id2==349) diffrow =  9;
            }
            if ((id==300) || (id==320)) {
               if (id2==340) diffrow =  9;
               if (id2==341) diffrow =  7;
               if (id2==342) diffrow =  5;
               if (id2==343) diffrow =  3;
               if (id2==344) diffrow =  1;
               if (id2==345) diffrow =  0;
               if (id2==346) diffrow =  2;
               if (id2==347) diffrow =  4;
               if (id2==348) diffrow =  6;
               if (id2==349) diffrow =  8;
            }
            if ((id==301) || (id==321)) {
               if (id2==340) diffrow = 10;
               if (id2==341) diffrow =  8;
               if (id2==342) diffrow =  6;
               if (id2==343) diffrow =  4;
               if (id2==344) diffrow =  2;
               if (id2==345) diffrow =  0;
               if (id2==346) diffrow =  1;
               if (id2==347) diffrow =  3;
               if (id2==348) diffrow =  5;
               if (id2==349) diffrow =  7;
            }
            if ((id==300) || (id==322)) {
               if (id2==340) diffrow = 11;
               if (id2==341) diffrow =  9;
               if (id2==342) diffrow =  7;
               if (id2==343) diffrow =  5;
               if (id2==344) diffrow =  3;
               if (id2==345) diffrow =  1;
               if (id2==346) diffrow =  0;
               if (id2==347) diffrow =  2;
               if (id2==348) diffrow =  4;
               if (id2==349) diffrow =  6;
            }
            if ((id==300) || (id==323)) {
               if (id2==340) diffrow = 12;
               if (id2==341) diffrow = 10;
               if (id2==342) diffrow =  8;
               if (id2==343) diffrow =  6;
               if (id2==344) diffrow =  4;
               if (id2==345) diffrow =  2;
               if (id2==346) diffrow =  0;
               if (id2==347) diffrow =  2;
               if (id2==348) diffrow =  4;
               if (id2==349) diffrow =  6;
            }
            if ((id==300) || (id==324)) {
               if (id2==340) diffrow = 13;
               if (id2==341) diffrow = 11;
               if (id2==342) diffrow =  9;
               if (id2==343) diffrow =  7;
               if (id2==344) diffrow =  5;
               if (id2==345) diffrow =  3;
               if (id2==346) diffrow =  1;
               if (id2==347) diffrow =  0;
               if (id2==348) diffrow =  2;
               if (id2==349) diffrow =  4;
            }
            if ((id==300) || (id==325)) {
               if (id2==340) diffrow = 14;
               if (id2==341) diffrow = 12;
               if (id2==342) diffrow = 10;
               if (id2==343) diffrow =  8;
               if (id2==344) diffrow =  6;
               if (id2==345) diffrow =  4;
               if (id2==346) diffrow =  2;
               if (id2==347) diffrow =  0;
               if (id2==348) diffrow =  1;
               if (id2==349) diffrow =  3;
            }
            if ((id==300) || (id==326)) {
               if (id2==340) diffrow = 15;
               if (id2==341) diffrow = 13;
               if (id2==342) diffrow = 11;
               if (id2==343) diffrow =  9;
               if (id2==344) diffrow =  7;
               if (id2==345) diffrow =  5;
               if (id2==346) diffrow =  3;
               if (id2==347) diffrow =  1;
               if (id2==348) diffrow =  0;
               if (id2==349) diffrow =  2;
            }
            if ((id==300) || (id==327)) {
               if (id2==340) diffrow = 16;
               if (id2==341) diffrow = 14;
               if (id2==342) diffrow = 12;
               if (id2==343) diffrow = 10;
               if (id2==344) diffrow =  8;
               if (id2==345) diffrow =  6;
               if (id2==346) diffrow =  4;
               if (id2==347) diffrow =  2;
               if (id2==348) diffrow =  0;
               if (id2==349) diffrow =  1;
            }
            if ((id==300) || (id==328)) {
               if (id2==340) diffrow = 17;
               if (id2==341) diffrow = 16;
               if (id2==342) diffrow = 13;
               if (id2==343) diffrow = 12;
               if (id2==344) diffrow =  9;
               if (id2==345) diffrow =  8;
               if (id2==346) diffrow =  5;
               if (id2==347) diffrow =  4;
               if (id2==348) diffrow =  1;
               if (id2==349) diffrow =  0;
            }
            if ((id==300) || (id==329)) {
               if (id2==340) diffrow = 18;
               if (id2==341) diffrow = 16;
               if (id2==342) diffrow = 14;
               if (id2==343) diffrow = 12;
               if (id2==344) diffrow = 10;
               if (id2==345) diffrow =  8;
               if (id2==346) diffrow =  6;
               if (id2==347) diffrow =  4;
               if (id2==348) diffrow =  2;
               if (id2==349) diffrow =  0;
            }

            // col=31, col2=33
            if (id==310) {
               if (id2==330) diffrow =  1;
               if (id2==331) diffrow =  2;
               if (id2==332) diffrow =  5;
               if (id2==333) diffrow =  6;
               if (id2==334) diffrow =  9;
               if (id2==335) diffrow = 10;
               if (id2==336) diffrow = 13;
               if (id2==337) diffrow = 14;
               if (id2==338) diffrow = 17;
               if (id2==339) diffrow = 18;
            }
            if (id==311) {
               if (id2==330) diffrow =  0;
               if (id2==331) diffrow =  1;
               if (id2==332) diffrow =  4;
               if (id2==333) diffrow =  5;
               if (id2==334) diffrow =  8;
               if (id2==335) diffrow =  9;
               if (id2==336) diffrow = 12;
               if (id2==337) diffrow = 13;
               if (id2==338) diffrow = 16;
               if (id2==339) diffrow = 17;
            }
            if (id==312) {
               if (id2==330) diffrow =  1;
               if (id2==331) diffrow =  0;
               if (id2==332) diffrow =  3;
               if (id2==333) diffrow =  4;
               if (id2==334) diffrow =  7;
               if (id2==335) diffrow =  8;
               if (id2==336) diffrow = 11;
               if (id2==337) diffrow = 12;
               if (id2==338) diffrow = 15;
               if (id2==339) diffrow = 16;
            }
            if (id==313) {
               if (id2==330) diffrow =  2;
               if (id2==331) diffrow =  1;
               if (id2==332) diffrow =  2;
               if (id2==333) diffrow =  3;
               if (id2==334) diffrow =  6;
               if (id2==335) diffrow =  7;
               if (id2==336) diffrow = 10;
               if (id2==337) diffrow = 11;
               if (id2==338) diffrow = 14;
               if (id2==339) diffrow = 15;
            }
            if (id==314) {
               if (id2==330) diffrow =  3;
               if (id2==331) diffrow =  2;
               if (id2==332) diffrow =  1;
               if (id2==333) diffrow =  2;
               if (id2==334) diffrow =  5;
               if (id2==335) diffrow =  6;
               if (id2==336) diffrow =  9;
               if (id2==337) diffrow = 10;
               if (id2==338) diffrow = 13;
               if (id2==339) diffrow = 14;
            }
            if (id==315) {
               if (id2==330) diffrow =  4;
               if (id2==331) diffrow =  3;
               if (id2==332) diffrow =  0;
               if (id2==333) diffrow =  1;
               if (id2==334) diffrow =  4;
               if (id2==335) diffrow =  5;
               if (id2==336) diffrow =  8;
               if (id2==337) diffrow =  9;
               if (id2==338) diffrow = 12;
               if (id2==339) diffrow = 13;
            }
            if (id==316) {
               if (id2==330) diffrow =  5;
               if (id2==331) diffrow =  4;
               if (id2==332) diffrow =  1;
               if (id2==333) diffrow =  0;
               if (id2==334) diffrow =  3;
               if (id2==335) diffrow =  4;
               if (id2==336) diffrow =  7;
               if (id2==337) diffrow =  8;
               if (id2==338) diffrow = 11;
               if (id2==339) diffrow = 12;
            }
            if (id==317) {
               if (id2==330) diffrow =  6;
               if (id2==331) diffrow =  5;
               if (id2==332) diffrow =  0;
               if (id2==333) diffrow =  1;
               if (id2==334) diffrow =  2;
               if (id2==335) diffrow =  3;
               if (id2==336) diffrow =  6;
               if (id2==337) diffrow =  7;
               if (id2==338) diffrow = 10;
               if (id2==339) diffrow = 11;
            }
            if (id==318) {
               if (id2==330) diffrow =  7;
               if (id2==331) diffrow =  6;
               if (id2==332) diffrow =  3;
               if (id2==333) diffrow =  2;
               if (id2==334) diffrow =  1;
               if (id2==335) diffrow =  2;
               if (id2==336) diffrow =  5;
               if (id2==337) diffrow =  6;
               if (id2==338) diffrow =  9;
               if (id2==339) diffrow = 10;
            }
            if (id==319) {
               if (id2==330) diffrow =  8;
               if (id2==331) diffrow =  7;
               if (id2==332) diffrow =  4;
               if (id2==333) diffrow =  3;
               if (id2==334) diffrow =  0;
               if (id2==335) diffrow =  1;
               if (id2==336) diffrow =  4;
               if (id2==337) diffrow =  5;
               if (id2==338) diffrow =  8;
               if (id2==339) diffrow =  9;
            }
            if (id==320) {
               if (id2==330) diffrow =  9;
               if (id2==331) diffrow =  8;
               if (id2==332) diffrow =  5;
               if (id2==333) diffrow =  4;
               if (id2==334) diffrow =  1;
               if (id2==335) diffrow =  0;
               if (id2==336) diffrow =  3;
               if (id2==337) diffrow =  4;
               if (id2==338) diffrow =  7;
               if (id2==339) diffrow =  8;
            }
            if (id==321) {
               if (id2==330) diffrow = 10;
               if (id2==331) diffrow =  9;
               if (id2==332) diffrow =  6;
               if (id2==333) diffrow =  5;
               if (id2==334) diffrow =  2;
               if (id2==335) diffrow =  1;
               if (id2==336) diffrow =  2;
               if (id2==337) diffrow =  3;
               if (id2==338) diffrow =  6;
               if (id2==339) diffrow =  7;
            }
            if (id==322) {
               if (id2==330) diffrow = 11;
               if (id2==331) diffrow = 10;
               if (id2==332) diffrow =  7;
               if (id2==333) diffrow =  6;
               if (id2==334) diffrow =  3;
               if (id2==335) diffrow =  2;
               if (id2==336) diffrow =  1;
               if (id2==337) diffrow =  2;
               if (id2==338) diffrow =  5;
               if (id2==339) diffrow =  6;
            }
            if (id==323) {
               if (id2==330) diffrow = 12;
               if (id2==331) diffrow = 11;
               if (id2==332) diffrow =  8;
               if (id2==333) diffrow =  7;
               if (id2==334) diffrow =  4;
               if (id2==335) diffrow =  3;
               if (id2==336) diffrow =  0;
               if (id2==337) diffrow =  1;
               if (id2==338) diffrow =  4;
               if (id2==339) diffrow =  5;
            }
            if (id==324) {
               if (id2==330) diffrow = 13;
               if (id2==331) diffrow = 12;
               if (id2==332) diffrow =  9;
               if (id2==333) diffrow =  8;
               if (id2==334) diffrow =  5;
               if (id2==335) diffrow =  4;
               if (id2==336) diffrow =  1;
               if (id2==337) diffrow =  0;
               if (id2==338) diffrow =  3;
               if (id2==339) diffrow =  4;
            }
            if (id==325) {
               if (id2==330) diffrow = 14;
               if (id2==331) diffrow = 13;
               if (id2==332) diffrow = 10;
               if (id2==333) diffrow =  9;
               if (id2==334) diffrow =  6;
               if (id2==335) diffrow =  5;
               if (id2==336) diffrow =  2;
               if (id2==337) diffrow =  1;
               if (id2==338) diffrow =  2;
               if (id2==339) diffrow =  3;
            }
            if (id==326) {
               if (id2==330) diffrow = 15;
               if (id2==331) diffrow = 14;
               if (id2==332) diffrow = 11;
               if (id2==333) diffrow = 10;
               if (id2==334) diffrow =  7;
               if (id2==335) diffrow =  6;
               if (id2==336) diffrow =  3;
               if (id2==337) diffrow =  2;
               if (id2==338) diffrow =  1;
               if (id2==339) diffrow =  2;
            }
            if (id==327) {
               if (id2==330) diffrow = 16;
               if (id2==331) diffrow = 15;
               if (id2==332) diffrow = 12;
               if (id2==333) diffrow = 11;
               if (id2==334) diffrow =  8;
               if (id2==335) diffrow =  7;
               if (id2==336) diffrow =  4;
               if (id2==337) diffrow =  3;
               if (id2==338) diffrow =  0;
               if (id2==339) diffrow =  1;
            }
            if (id==328) {
               if (id2==330) diffrow = 17;
               if (id2==331) diffrow = 16;
               if (id2==332) diffrow = 13;
               if (id2==333) diffrow = 12;
               if (id2==334) diffrow =  9;
               if (id2==335) diffrow =  8;
               if (id2==336) diffrow =  5;
               if (id2==337) diffrow =  4;
               if (id2==338) diffrow =  1;
               if (id2==339) diffrow =  0;
            }
            if (id==329) {
               if (id2==330) diffrow = 18;
               if (id2==331) diffrow = 17;
               if (id2==332) diffrow = 14;
               if (id2==333) diffrow = 13;
               if (id2==334) diffrow = 10;
               if (id2==335) diffrow =  9;
               if (id2==336) diffrow =  6;
               if (id2==337) diffrow =  5;
               if (id2==338) diffrow =  2;
               if (id2==339) diffrow =  1;
            }

            // Col=31, col2=34
            if (id==310) {
               if (id2==340) diffrow =  1;
               if (id2==341) diffrow =  2;
               if (id2==342) diffrow =  5;
               if (id2==343) diffrow =  6;
               if (id2==344) diffrow =  9;
               if (id2==345) diffrow = 10;
               if (id2==346) diffrow = 13;
               if (id2==347) diffrow = 14;
               if (id2==348) diffrow = 17;
               if (id2==349) diffrow = 18;
            }
            if (id==311) {
               if (id2==340) diffrow =  0;
               if (id2==341) diffrow =  1;
               if (id2==342) diffrow =  4;
               if (id2==343) diffrow =  5;
               if (id2==344) diffrow =  8;
               if (id2==345) diffrow =  9;
               if (id2==346) diffrow = 12;
               if (id2==347) diffrow = 13;
               if (id2==348) diffrow = 16;
               if (id2==349) diffrow = 17;
            }
            if (id==312) {
               if (id2==340) diffrow =  1;
               if (id2==341) diffrow =  0;
               if (id2==342) diffrow =  3;
               if (id2==343) diffrow =  4;
               if (id2==344) diffrow =  7;
               if (id2==345) diffrow =  8;
               if (id2==346) diffrow = 11;
               if (id2==347) diffrow = 12;
               if (id2==348) diffrow = 15;
               if (id2==349) diffrow = 16;
            }
            if (id==313) {
               if (id2==340) diffrow =  2;
               if (id2==341) diffrow =  1;
               if (id2==342) diffrow =  2;
               if (id2==343) diffrow =  3;
               if (id2==344) diffrow =  6;
               if (id2==345) diffrow =  7;
               if (id2==346) diffrow = 10;
               if (id2==347) diffrow = 11;
               if (id2==348) diffrow = 14;
               if (id2==349) diffrow = 15;
            }
            if (id==314) {
               if (id2==340) diffrow =  3;
               if (id2==341) diffrow =  2;
               if (id2==342) diffrow =  1;
               if (id2==343) diffrow =  2;
               if (id2==344) diffrow =  5;
               if (id2==345) diffrow =  6;
               if (id2==346) diffrow =  9;
               if (id2==347) diffrow = 10;
               if (id2==348) diffrow = 13;
               if (id2==349) diffrow = 14;
            }
            if (id==315) {
               if (id2==340) diffrow =  4;
               if (id2==341) diffrow =  3;
               if (id2==342) diffrow =  0;
               if (id2==343) diffrow =  1;
               if (id2==344) diffrow =  4;
               if (id2==345) diffrow =  5;
               if (id2==346) diffrow =  8;
               if (id2==347) diffrow =  9;
               if (id2==348) diffrow = 12;
               if (id2==349) diffrow = 13;
            }
            if (id==316) {
               if (id2==340) diffrow =  5;
               if (id2==341) diffrow =  4;
               if (id2==342) diffrow =  1;
               if (id2==343) diffrow =  0;
               if (id2==344) diffrow =  3;
               if (id2==345) diffrow =  4;
               if (id2==346) diffrow =  7;
               if (id2==347) diffrow =  8;
               if (id2==348) diffrow = 11;
               if (id2==349) diffrow = 12;
            }
            if (id==317) {
               if (id2==340) diffrow =  6;
               if (id2==341) diffrow =  5;
               if (id2==342) diffrow =  0;
               if (id2==343) diffrow =  1;
               if (id2==344) diffrow =  2;
               if (id2==345) diffrow =  3;
               if (id2==346) diffrow =  6;
               if (id2==347) diffrow =  7;
               if (id2==348) diffrow = 10;
               if (id2==349) diffrow = 11;
            }
            if (id==318) {
               if (id2==340) diffrow =  7;
               if (id2==341) diffrow =  6;
               if (id2==342) diffrow =  3;
               if (id2==343) diffrow =  2;
               if (id2==344) diffrow =  1;
               if (id2==345) diffrow =  2;
               if (id2==346) diffrow =  5;
               if (id2==347) diffrow =  6;
               if (id2==348) diffrow =  9;
               if (id2==349) diffrow = 10;
            }
            if (id==319) {
               if (id2==340) diffrow =  8;
               if (id2==341) diffrow =  7;
               if (id2==342) diffrow =  4;
               if (id2==343) diffrow =  3;
               if (id2==344) diffrow =  0;
               if (id2==345) diffrow =  1;
               if (id2==346) diffrow =  4;
               if (id2==347) diffrow =  5;
               if (id2==348) diffrow =  8;
               if (id2==349) diffrow =  9;
            }
            if (id==320) {
               if (id2==340) diffrow =  9;
               if (id2==341) diffrow =  8;
               if (id2==342) diffrow =  5;
               if (id2==343) diffrow =  4;
               if (id2==344) diffrow =  1;
               if (id2==345) diffrow =  0;
               if (id2==346) diffrow =  3;
               if (id2==347) diffrow =  4;
               if (id2==348) diffrow =  7;
               if (id2==349) diffrow =  8;
            }
            if (id==321) {
               if (id2==340) diffrow = 10;
               if (id2==341) diffrow =  9;
               if (id2==342) diffrow =  6;
               if (id2==343) diffrow =  5;
               if (id2==344) diffrow =  2;
               if (id2==345) diffrow =  1;
               if (id2==346) diffrow =  2;
               if (id2==347) diffrow =  3;
               if (id2==348) diffrow =  6;
               if (id2==349) diffrow =  7;
            }
            if (id==322) {
               if (id2==340) diffrow = 11;
               if (id2==341) diffrow = 10;
               if (id2==342) diffrow =  7;
               if (id2==343) diffrow =  6;
               if (id2==344) diffrow =  3;
               if (id2==345) diffrow =  2;
               if (id2==346) diffrow =  1;
               if (id2==347) diffrow =  2;
               if (id2==348) diffrow =  5;
               if (id2==349) diffrow =  6;
            }
            if (id==323) {
               if (id2==340) diffrow = 12;
               if (id2==341) diffrow = 11;
               if (id2==342) diffrow =  8;
               if (id2==343) diffrow =  7;
               if (id2==344) diffrow =  4;
               if (id2==345) diffrow =  3;
               if (id2==346) diffrow =  0;
               if (id2==347) diffrow =  1;
               if (id2==348) diffrow =  4;
               if (id2==349) diffrow =  5;
            }
            if (id==324) {
               if (id2==340) diffrow = 13;
               if (id2==341) diffrow = 12;
               if (id2==342) diffrow =  9;
               if (id2==343) diffrow =  8;
               if (id2==344) diffrow =  5;
               if (id2==345) diffrow =  4;
               if (id2==346) diffrow =  1;
               if (id2==347) diffrow =  0;
               if (id2==348) diffrow =  3;
               if (id2==349) diffrow =  4;
            }
            if (id==325) {
               if (id2==340) diffrow = 14;
               if (id2==341) diffrow = 13;
               if (id2==342) diffrow = 10;
               if (id2==343) diffrow =  9;
               if (id2==344) diffrow =  6;
               if (id2==345) diffrow =  5;
               if (id2==346) diffrow =  2;
               if (id2==347) diffrow =  1;
               if (id2==348) diffrow =  2;
               if (id2==349) diffrow =  3;
            }
            if (id==326) {
               if (id2==340) diffrow = 15;
               if (id2==341) diffrow = 14;
               if (id2==342) diffrow = 11;
               if (id2==343) diffrow = 10;
               if (id2==344) diffrow =  7;
               if (id2==345) diffrow =  6;
               if (id2==346) diffrow =  3;
               if (id2==347) diffrow =  2;
               if (id2==348) diffrow =  1;
               if (id2==349) diffrow =  2;
            }
            if (id==327) {
               if (id2==340) diffrow = 16;
               if (id2==341) diffrow = 15;
               if (id2==342) diffrow = 12;
               if (id2==343) diffrow = 11;
               if (id2==344) diffrow =  8;
               if (id2==345) diffrow =  7;
               if (id2==346) diffrow =  4;
               if (id2==347) diffrow =  3;
               if (id2==348) diffrow =  0;
               if (id2==349) diffrow =  1;
            }
            if (id==328) {
               if (id2==340) diffrow = 17;
               if (id2==341) diffrow = 16;
               if (id2==342) diffrow = 13;
               if (id2==343) diffrow = 12;
               if (id2==344) diffrow =  9;
               if (id2==345) diffrow =  8;
               if (id2==346) diffrow =  5;
               if (id2==347) diffrow =  4;
               if (id2==348) diffrow =  1;
               if (id2==349) diffrow =  0;
            }
            if (id==329) {
               if (id2==340) diffrow = 18;
               if (id2==341) diffrow = 17;
               if (id2==342) diffrow = 14;
               if (id2==343) diffrow = 13;
               if (id2==344) diffrow = 10;
               if (id2==345) diffrow =  9;
               if (id2==346) diffrow =  6;
               if (id2==347) diffrow =  5;
               if (id2==348) diffrow =  2;
               if (id2==349) diffrow =  1;
            }
	
            if ( // CAUTION : condition has something about row2 or row 
               // col = 4
               // col2 = 6, diffcol = 2
               (((id==40) || (id==41)) && ((id2==60) || (id2==61))) || (((id==42) || (id==43)) && ((id2==62) || (id2==63))) || 
               (((id==44) || (id==45)) && ((id2==64) || (id2==65))) || (((id==46) || (id==47)) && ((id2==66) || (id2==67))) ||
               (((id==48) || (id==49)) && ((id2==68) || (id2==69))) || 

               // col = 5
               // col2 = 6, diffcol = 1
               (((id==50) || (id==51)) && ((id2==60) || (id2==61))) || (((id==52) || (id==53)) && ((id2==62) || (id2==63))) || 
               (((id==54) || (id==55)) && ((id2==64) || (id2==65))) || (((id==56) || (id==57)) && ((id2==66) || (id2==67))) ||
               (((id==58) || (id==59)) && ((id2==68) || (id2==69))) || 
               // col2 = 7, diffcol = 1
               ((col==5) && (col2==7) && (diffrow==0)) ||
               // col2 = 9, diffcol = 2
               ((id==50) && (id2==90)) || (((id==51) || (id==52)) && ((id2==93) || (id2==94))) ||
               (((id==53) || (id==54)) && ((id2==97) || (id2==98))) || (((id==55) || (id==56)) && ((id2==101) || (id2==102))) ||
               (((id==57) || (id==58)) && ((id2==105) || (id2==106))) || ((id==59) && (id2==109)) ||

               // col = 6
               // col2 = 6, diffcol = 0
               (((col==6) && (row%2==0)) && ((col2==6) && (row2>=row)) && (diffrow<=1)) ||
               (((col==6) && (row%2!=0)) && ((col2==6) && (row2<=row)) && (diffrow<=1)) ||
               // col2 = 7, diffcol = 1
               (((id2>=70) && (id2<=72)) && (id==60)) || (((id2>=71) && (id2<=73)) && (id==61)) || 
               (((id2>=74) && (id2<=76)) && (id==62)) || (((id2>=75) && (id2<=77)) && (id==63)) || 
               (((id2>=78) && (id2<=80)) && (id==64)) || (((id2>=79) && (id2<=81)) && (id==65)) || 
               (((id2>=82) && (id2<=84)) && (id==66)) || (((id2>=83) && (id2<=85)) && (id==67)) || 
               (((id2>=86) && (id2<=88)) && (id==68)) || (((id2>=87) && (id2<=89)) && (id==69)) || 
               // col2 = 9, diffcol = 2
               (((id==60) || (id==61)) && ((id2==91) || (id2==92))) || (((id==62) || (id==63)) && ((id2==95) || (id2==96))) || 
               (((id==64) || (id==65)) && ((id2==99) || (id2==100))) || (((id==66) || (id==67)) && ((id2==103) || (id2==104))) || 
               (((id==68) || (id==69)) && ((id2==107) || (id2==108))) ||

               // col = 29
               // col2 = 33, diffcol = 2
               (((id==291) || (id==292)) && ((id2==330) || (id2==331))) || (((id==295) || (id==296)) && ((id2==332) || (id2==333))) ||
               (((id==299) || (id==300)) && ((id2==334) || (id2==335))) || (((id==303) || (id==304)) && ((id2==336) || (id2==337))) ||
               (((id==307) || (id==308)) && ((id2==338) || (id2==339))) ||
               // col2 = 34, diffcol = 2
               ((id==290) && (id2==340)) || (((id==293) || (id==294)) && ((id2==341) || (id2==342))) || 
               (((id==297) || (id==298)) && ((id2==343) || (id2==344))) || (((id==301) || (id==302)) && ((id2==345) || (id2==346))) ||
               (((id==305) || (id==306)) && ((id2==347) || (id2==348))) || ((id==309) && (id2==349)) ||

               // col = 31
               // col2 = 33, diffcol = 1
               (((id>=310) && (id<=312)) && (id2==330)) || (((id>=311) && (id<=313)) && (id2==331)) || 
               (((id>=314) && (id<=316)) && (id2==332)) || (((id>=315) && (id<=317)) && (id2==333)) || 
               (((id>=318) && (id<=320)) && (id2==334)) || (((id>=319) && (id<=321)) && (id2==335)) || 
               (((id>=322) && (id<=324)) && (id2==336)) || (((id>=323) && (id<=325)) && (id2==337)) || 
               (((id>=326) && (id<=328)) && (id2==338)) || (((id>=327) && (id<=329)) && (id2==339)) || 
               // col2 = 34, diffcol = 1
               ((col==5) && (col2==7) && (diffrow==0)) ||

               // col = 33
               // col2 = 33, diffcol = 0
               (((col==33) && (row%2==0)) && ((col2==33) && (row2>=row)) && (diffrow<=1)) ||
               (((col==33) && (row%2!=0)) && ((col2==33) && (row2<=row)) && (diffrow<=1)) ||
               // col2 = 34, diffcol = 1
               (((col==33) && (row%2==0)) && ((col2==34) && (row2>=row)) && (diffrow<=1)) ||
               (((col==33) && (row%2!=0)) && ((col2==34) && (row2<=row)) && (diffrow<=1)) ||
               // col2 = 35, diffcol = 2
               (((col==33) && (row%2==0)) && ((col2==35) && (row2>=row)) && (diffrow<=1)) ||
               (((col==33) && (row%2!=0)) && ((col2==35) && (row2<=row)) && (diffrow<=1)) ||

               // col <= 5, col2 <= 5
               ((col<=5) && (col2<=5) &&
                (((diffcol<=1) && (diffrow<=1)) || 
                 ((diffcol==2) && (diffrow<=1) && 
                  (((col%2==0) && (((row%2==0) && (row2>=row)) || ((row%2!=0) && (row2<=row)))) ||     // col = 0,2,4
                   ((col%2!=0) && (((row%2==0) && (row2<=row)) || ((row%2!=0) && (row2>=row)))))))) || // col = 1,3,5

               // 34 <= col, 34 <= col2
               ((col>=34) && (col2>=34) &&
                (((diffcol<=1) && (diffrow<=1)) || 
                 ((diffcol==2) && (diffrow<=1) && 
                  (((col%2==0) && (((row%2==0) && (row2<=row)) || ((row%2!=0) && (row2>=row)))) ||     // col = 34,36,38
                   ((col%2!=0) && (((row%2==0) && (row2>=row)) || ((row%2!=0) && (row2<=row)))))))) || // col = 35,37,39

               // 7 <= col <= 31, 7 <= col2 <= 31 
               (((col>=7) && (col<=31)) && ((col2>=7) && (col2<=31)) && 
                (((diffcol<=1) && (diffrow<=1)) || 
                 ((diffcol==2) && (diffrow<=1) && 
                  (((col%4==3) && (((row%2==0) && (row2>=row)) || ((row%2!=0) && (row2<=row)))) ||  // col = 7,11,15,19,23,27,31
                   ((col%4==1) && (((row%2==0) && (row2<=row)) || ((row%2!=0) && (row2>=row)))))))) // col = 9,13,17,21,25,29
               && (TMath::Abs(diffoff) < 5.))
            {
               if (fDebug) fprintf(fp,"id = %3d, col = %2d, row = %2d, off = %6.1f, id2 = %3d, col2 = %2d, row2 = %2d, off2 = %6.1f, diffcol = %d, diffrow = %d, diffoff = %6.1f\n",id,col,row,off,id2,col2,row2,off2,diffcol,diffrow,diffoff);

               IsAdd = true;
               irecoil.push_back(hit);
               irecoil.push_back(hit2);
               if (fDebug) fprintf(fp,"irecoil: iHit  = %d, added id  = %d\n",iHit,id);
               if (fDebug) fprintf(fp,"irecoil: iHit2 = %d, added id2 = %d\n",iHit2,id2);
               if (fDebug) fprintf(fp,"iHit = %d, iHit2 = %d, iHit2 loop break\n",iHit,iHit2);
               break;
            } else {
               if (fDebug) fprintf(fp,"fail: id = %3d, col = %2d, row = %2d, off = %6.1f, id2 = %3d, col2 = %2d, row2 = %2d, off2 = %6.1f, diffcol = %d, diffrow = %d, diffoff = %6.1f\n",id,col,row,off,id2,col2,row2,off2,diffcol,diffrow,diffoff);
               if ((!fFirstFail) && (fSwitch)) {
                  fail_iHit = iHit2;
                  fFirstFail = true;
                  fSwitch = false;
                  if (fDebug) fprintf(fp,"set fail_iHit = %d\n",fail_iHit);
               }
               iHit2++;
            }
         }

         if (IsAdd) {
            if (fDebug) fprintf(fp,"recoil_cand_array: deleted id = %d, iHit = %d -> ",id,iHit);
            recoil_cand_array.RemoveAt(iHit);
            iHit = iHit2;
            if (fDebug) fprintf(fp,"%d\n",iHit);
         } else {
            if (fDebug) fprintf(fp,"fail: iHit = %d, id = %d, reached last id2\n",iHit,id);
            if (fDebug) fprintf(fp,"recoil_cand_array: deleted id = %d, iHit = %d -> ",id,iHit);
            recoil_cand_array.RemoveAt(iHit);
            if (fDebug) fprintf(fp,"%d\n",iHit);
            if (irecoil.size()>0) {
               if (fDebug) fprintf(fp,"irecoil size = %zd\n",irecoil.size());
               std::sort(irecoil.begin(),irecoil.end());
               irecoil.erase(std::unique(irecoil.begin(),irecoil.end()),irecoil.end());
               recoil.push_back(irecoil);
               irecoil.clear();
               if (fDebug) fprintf(fp,"switch: recoil cls %d",nRecoilCls);
               nRecoilCls++;
               if (fDebug) fprintf(fp," -> %d\n",nRecoilCls);
               fSwitch = true;
            } else {
               if (fDebug) fprintf(fp,"iHit = %d, iHit2 = %d\n",iHit,iHit2);
            }
            if (fFirstFail) {
               iHit = fail_iHit;
               fFirstFail = false;
               fail_iHit = -1;
               if (fDebug) fprintf(fp,"iHit = fail_iHit = %d\n",iHit);
            } else {
               if (iHit!=(nRecoilHits-1)) {
                  iHit++;
                  if (fDebug) fprintf(fp,"iHit = %d\n",iHit);
               } else { // all finished
                  if (fDebug) fprintf(fp,"iHit = %d, finish\n",iHit);
                  break;
               }
            }
         }
      }
      /*--------------------------------------------------------------------------*/

      // Check remained hits in recoil_cand_array
      for (Int_t iHit=0; iHit < nRecoilHits; iHit++) {
         TCatPulseShape *hit = (TCatPulseShape*) recoil_cand_array[iHit];
         if (!hit) continue;
         Int_t id = hit->GetID();
         Float_t off = hit->GetOffset();
         if (fDebug) fprintf(fp,"id = %d, off = %6.1f\n",id,off);
      }
      /*--------------------------------------------------------------------------*/

      // Check whether recoil hits connect with beam hits
      if (fDebug) fprintf(fp,"\n*nRecoilCls = %d\n",nRecoilCls);
      if (fDetailDebug) printf("\n*nRecoilCls = %d\n",nRecoilCls);
      Bool_t RecoilIsGood = false;
      for (Int_t ircls=0; ircls<nRecoilCls; ircls++) {
         Bool_t iRecoilIsGood = false;
         //Float_t min_diffoff = 1000.;
         //Int_t first_recoil_hit_id = -1;
         nRecoilHits = recoil[ircls].size();
         if (fDebug) fprintf(fp,"recoil cls[%d] size = %d\n",ircls,nRecoilHits);
         if (fDetailDebug) printf("recoil cls[%d] size = %d\n",ircls,nRecoilHits);

         it    = recoil[ircls].begin();
         itend = recoil[ircls].end();
         for (; it != itend; it++) {
            Int_t id = (*it)->GetID();
            Float_t off = (*it)->GetOffset();
            if (fDebug) fprintf(fp,"ircls = %d, id = %3d, off = %6.1f\n",ircls,id,off);
            if (fDetailDebug) printf("ircls = %d, id = %3d, off = %6.1f\n",ircls,id,off);
         }

         if (nRecoilHits<=fCutRecoilHits) {
            if (fDebug) fprintf(fp,"Clear: Recoil Cluster[%d] Size <= %d\n",ircls,fCutRecoilHits);
            if (fDetailDebug) printf("Clear: Recoil Cluster[%d] Size <= %d\n",ircls,fCutRecoilHits);
            recoil[ircls].clear();
         }

         it    = recoil[ircls].begin();
         itend = recoil[ircls].end();
         for (; it != itend; it++) {
            Int_t id = (*it)->GetID();
            Float_t off = (*it)->GetOffset();
            Int_t col = id / 10;
            Int_t row = id % 10;
            if ((id > 69 && id < 330) && (col%2 == 0)) {
               col = col - 1;
               row = row + 10;
            }
            Double_t x1 = (*it)->GetX();
            Double_t z1 = (*it)->GetZ();
            //Float_t diffoff = off - BeamOff;

            //if (fDetailDebug) fprintf(fp,"Recoil: id  = %3d, x1 = %5.1f, z1 = %7.3f, off  = %6.1f, diffoff  = %5.1f\n",id,x1,z1,off,diffoff);

            std::vector<TCatPulseShape*>::iterator it2    = beam.begin();
            std::vector<TCatPulseShape*>::iterator it2end = beam.end();
            for (; it2 != it2end; it2++) {
               Int_t id2 = (*it2)->GetID();
               Float_t off2 = (*it2)->GetOffset();
               Int_t col2 = id2 / 10;
               Int_t row2 = id2 % 10;
               if ((id2 > 69 && id2 < 330) && (col2%2 == 0)) {
                  col2 = col2 - 1;
                  row2 = row2 + 10;
               }
               Double_t x2 = (*it2)->GetX();
               Double_t z2 = (*it2)->GetZ();
               //Float_t diffoff2 = off2 - BeamOff;

               //if (fDetailDebug) fprintf(fp,"Beam  : id2 = %3d, x2 = %5.1f, z2 = %7.3f, off2 = %6.1f, diffoff2 = %5.1f\n",id2,x2,z2,off2,diffoff2);

               Double_t l = TMath::Sqrt((x1 - x2)*(x1 - x2) + (z1 - z2)*(z1 - z2));

               if (
                  ((TMath::Abs(x1)==25.0) && (l<13.0)) || // col =  4 or 35
                  ((TMath::Abs(x1)==20.0) && (l<16.0)) || // col =  5 or 34
                  ((TMath::Abs(x1)==17.5) && (l< 9.0)) || // col =  6 or 33
                  ((TMath::Abs(x1)==15.0) && (l<11.0)) || // col =  7 or 31
                  ((TMath::Abs(x1)==12.5) && (l<11.0)) || // col =  9 or 29
                  ((TMath::Abs(x1)>=30.0) && (l<13.0)) || // col <= 3, 36 <= col, 10 mm Pad
                  ((TMath::Abs(x1)<=10.0) && (l< 6.0))    // 11 <= col <= 27, 5 mm Pad
                  )
               {
                  if (fDetailDebug) fprintf(fp,"l = %6.3f mm, Recoil is Good, break\n",l);
                  iRecoilIsGood = true;
                  RecoilIsGood = true;
                  break;
               } else if (fIgnoreDistanceToBeam) {
                  if (fDetailDebug) fprintf(fp,"ignored fail: Recoil id = %3d, off = %6.1f, Beam id2 = %3d, off2 = %6.1f\n",id,off,id2,off2);
                  iRecoilIsGood = true;
                  RecoilIsGood = true;
                  break;
               } else {
                  if (fDetailDebug) fprintf(fp,"fail: Recoil id = %3d, off = %6.1f, Beam id2 = %3d, off2 = %6.1f\n",id,off,id2,off2);
               }
            }
            //if (iRecoilIsGood) break;
            //if (iRecoilIsGood) {
            //   if (TMath::Abs(min_diffoff) > TMath::Abs(diffoff)) {
            //      min_diffoff = diffoff;
            //      first_recoil_hit_id = id;
            //      if (fDetailDebug) fprintf(fp,"min_diffoff = %6.1f, first_recoil_hit_id = %3d\n",min_diffoff,first_recoil_hit_id);
            //   }
            //}
         }
         if (!iRecoilIsGood) {
            //if (fDebug) fprintf(fp,"Clear ircls = %d: Not Good Hit Pattern Between Recoil and Beam, min_BeamCol = %2d, max_BeamCol = %2d\n",ircls,min_BeamCol,max_BeamCol);
            recoil[ircls].clear();
         }
      }

      if (!RecoilIsGood) {
         if (fDebug) fprintf(fp,"Skip: Not Good Hit Pattern Between Recoil and Beam\n");
         continue;
      }
      /*--------------------------------------------------------------------------*/
    
      // Calcurate max length between recoil and beam
      Int_t max_RecoilHits = 0;
      Int_t max_rcls = -1;
      Double_t max_length = 0;
      Double_t max_X = 0;
      Double_t max_X_off = 0;
      Bool_t IsPenetrating = false;
      for (Int_t ircls=0; ircls<nRecoilCls; ircls++) {
         TObjArray irecoil_array;
         irecoil_array.Clear();
         it    = recoil[ircls].begin();
         itend = recoil[ircls].end();
         for (; it != itend; it++) {
            irecoil_array.Add((*it));
         }
         TCatPulseShape::SetSortType(TCatPulseShape::kTiming);
         TCatPulseShape::SetSortOrder(TCatPulseShape::kASC);
         irecoil_array.Sort();
      
         Double_t max_rcls_length = 0;
         Double_t max_rcls_X = 0;
         Double_t max_rcls_X_off = 0;
         Double_t BeamX = 0;
         Bool_t rcls_IsPenetrating = false;
         nRecoilHits = irecoil_array.GetEntriesFast();
         if (fDebug) fprintf(fp,"recoil[%d] %d hits remained\n",ircls,nRecoilHits);
         if (nRecoilHits<=fCutRecoilHits) {
            if (fDebug) fprintf(fp,"Skip: Recoil Hits <= %d\n",fCutRecoilHits);
            continue;
         }
         for (Int_t iHit=0; iHit < nRecoilHits; iHit++) {
            TCatPulseShape *hit = (TCatPulseShape*) irecoil_array[iHit];
            Double_t x = hit->GetX();
            Double_t z = hit->GetZ();
            Float_t off = hit->GetOffset();
            Double_t q = hit->GetCharge();
            if (fDebug) fprintf(fp,"id = %3d, off = %6.1f, x = %5.1f, z = %5.1f, q = %5.1f\n",hit->GetID(),off,x,z,q);
            //if (fLSM) {
            //   BeamX = (z - b_bz) / a_bz;
            //} else {
            //   BeamX = (z - p1) / p0;
            //}
            Double_t lx = TMath::Sqrt((x - BeamX)*(x - BeamX));
            if (max_rcls_length < lx) {
               max_rcls_length = lx;
               max_rcls_X = x;
               max_rcls_X_off = off;
            }
            if (TMath::Abs(x)>=40.) rcls_IsPenetrating = true;
         }
         if (fDetailDebug) fprintf(fp,"max_rcls_length = %5.1f\n",max_rcls_length);    

         if (max_length < max_rcls_length) {
            max_rcls = ircls;
            max_length = max_rcls_length;
            max_X = max_rcls_X;
            max_X_off = max_rcls_X_off;
            IsPenetrating = rcls_IsPenetrating;
         } else if (max_length == max_rcls_length) {
            if (max_RecoilHits < nRecoilHits) {
               max_rcls = ircls;
               max_X = max_rcls_X;
               max_X_off = max_rcls_X_off;
               max_RecoilHits = nRecoilHits;
               IsPenetrating = rcls_IsPenetrating;
            }
         }
      }

      if (fDebug) fprintf(fp,"max_rcls = %d, max_length = %5.1f, max_X = %5.1f\n",max_rcls,max_length,max_X);

      TObjArray recoil_array;
      if (!fUseAllRecoilCluster) {
         // default
         // use the cluster with the maximum length
         it    = recoil[max_rcls].begin();
         itend = recoil[max_rcls].end();
         for (; it != itend; it++) {
            Double_t x = (*it)->GetX();
            Double_t z = (*it)->GetZ();
            Double_t q = (*it)->GetCharge();
            recoil_array.Add((*it));
            //if (fDrawFig) fHPadEvt->Fill(x,z);
            if (fDrawFig) fHPadEvt->Fill(x,z,q);
         }
      } else {
         // optional
         // use all the clusters
         for (Int_t ircls=0; ircls<nRecoilCls; ircls++) {
            it    = recoil[ircls].begin();
            itend = recoil[ircls].end();
            for (; it != itend; it++) {
               Double_t x = (*it)->GetX();
               Double_t z = (*it)->GetZ();
               Double_t q = (*it)->GetCharge();
               recoil_array.Add((*it));
               //if (fDrawFig) fHPadEvt->Fill(x,z);
               if (fDrawFig) fHPadEvt->Fill(x,z,q);
            }
         }
      }
      /*--------------------------------------------------------------------------*/

      // Derive Linear Equation of Recoil (x,z)
      if (fDebug) fprintf(fp,"// Derive Linear Equation of Recoil (x,z)\n");
      nRecoilHits = recoil_array.GetEntriesFast();

      Double_t x_r = 0.;
      Double_t z_r = 0.;
      Double_t q_r = 0.;
      Double_t Sx  = 0.;
      Double_t Sz  = 0.;
      Double_t Sxx = 0.;
      Double_t Sxz = 0.;
      Double_t a_rz   = 0.;
      Double_t b_rz   = 0.;
      n   = 0;
      for (Int_t iHit=0; iHit < nRecoilHits; iHit++) {
         TCatPulseShape *hit = (TCatPulseShape*) recoil_array[iHit];
         Int_t id = hit->GetID();
         Int_t col = id / 10;
         Int_t row = id % 10;
         if ((id > 69 && id < 330) && (col%2 == 0)) {
            col = col - 1;
            row = row + 10;
         }
         Double_t x1 = hit->GetX();
         Double_t z1 = hit->GetZ();
         Double_t q1 = hit->GetCharge();
         //Float_t off = hit->GetOffset();
         //Float_t diffoff = off - BeamOff;

         // 0.70 cm/us = 0.70 x 10^(-1) mm/ns @ phys1802
         //Float_t RecoilY = diffoff * 20. * (fDriftVelocity / 100. );
         //if (fSaveHist) fhDiffRecoilY->Fill(x1,RecoilY);

         for (Int_t iHit2=0; iHit2 < nRecoilHits; iHit2++) {
            if (iHit>=iHit2) continue;
            TCatPulseShape *hit2 = (TCatPulseShape*) recoil_array[iHit2];
            Int_t id2 = hit2->GetID();
            Int_t col2 = id2 / 10;
            Int_t row2 = id2 % 10;
            if ((id2 > 69 && id2 < 330) && (col2%2 == 0)) {
               col2 = col2 - 1;
               row2 = row2 + 10;
            }
            Double_t x2 = hit2->GetX();
            Double_t z2 = hit2->GetZ();
            Double_t q2 = hit2->GetCharge();
            //Float_t off2 = hit2->GetOffset();
            //Float_t diffoff2 = off2 - BeamOff;

            Double_t l = TMath::Sqrt((x1 - x2)*(x1 - x2) + (z1 - z2)*(z1 - z2));

            Double_t gap = 0.15 / TMath::Sqrt(3) * 2;
            if ((TMath::Abs(x1)>=20.0) && (l< 6.0)) { // 10 mm Pad
               Double_t z_rh = (10./2 * TMath::Sqrt(3) / 3) / 2;
               Double_t x_rh = (10./2 + gap) / 2;
               //if (fDetailDebug) fprintf(fp,"iHit  = %2d, id  = %3d, x1 = %6.3f, z1 = %6.3f, off  = %6.1f, q1 = %6.3f, diffoff  = %6.1f\n",iHit,id,x1,z1,off,q1,diffoff);
               //if (fDetailDebug) fprintf(fp,"iHit2 = %2d, id2 = %3d, x2 = %6.3f, z2 = %6.3f, off2 = %6.1f, q2 = %6.3f, diffoff2 = %6.1f\n",iHit2,id2,x2,z2,off2,q2,diffoff2);      
               if (fDetailDebug) fprintf(fp,"id = %3d: id2 = %3d, l = %6.3f mm, pair hit was found in 10 mm pad area\n",id,id2,l);
               if (x1==x2) {
                  x_rh = 10./2;
                  if (id<id2) {
                     if (fDebug) fprintf(fp,"col=col2, id<id2\n");
                     x_r = ((q2 - q1) / (q2 + q1)) * x_rh + x1;
                     z_r =  (z2 - z1 + gap) / 2 + z1;
                  } else {
                     if (fDebug) fprintf(fp,"col=col2, id>id2\n");
                     x_r = ((q1 - q2) / (q1 + q2)) * x_rh + x1;
                     z_r =  (z1 - z2 + gap) / 2 + z2;
                  }
               } else {
                  if (id<id2) {
                     if ((col<=5) && (col%2==0)) { // col = 0,2,4
                        if (row%2==0) { /* 1\//\2 */
                           if (fDebug) fprintf(fp,"/* col :even,row :even, 1\\//\\2 */\n");
                           x_r = ((q1 - q2) / (q1 + q2)) * x_rh + (x2 - x_rh);
                           z_r = ((q1 - q2) / (q1 + q2)) * z_rh + (z1 - z_rh);
                        } else if (row%2==1) { /* 1/\\/2 */
                           if (fDebug) fprintf(fp,"/* col :even,row : odd, 1/\\\\/2 */\n");
                           x_r = ((q1 - q2) / (q1 + q2)) * x_rh + (x2 - x_rh);
                           z_r = ((q2 - q1) / (q2 + q1)) * z_rh + (z2 - z_rh);
                        }
                     } else if ((col<=5) && (col%2==1)) { // col = 1,3,5
                        if (row%2==0) { /* 1/\\/2 */
                           if (fDebug) fprintf(fp,"/* col : odd,row :even, 1/\\\\/2 */\n");
                           x_r = ((q1 - q2) / (q1 + q2)) * x_rh + (x2 - x_rh);
                           z_r = ((q2 - q1) / (q2 + q1)) * z_rh + (z2 - z_rh);
                        } else if (row%2==1) { /* 1\//\2 */
                           if (fDebug) fprintf(fp,"/* col : odd,row : odd, 1\\//\\2 */\n");
                           x_r = ((q1 - q2) / (q1 + q2)) * x_rh + (x2 - x_rh);
                           z_r = ((q1 - q2) / (q1 + q2)) * z_rh + (z1 - z_rh);
                        }
                     } else if ((col>=34) && (col%2==1)) { // col = 35,37,39
                        if (row%2==0) { /* 1\//\2 */
                           if (fDebug) fprintf(fp,"/* col :even,row :even, 1\\//\\2 */\n");
                           x_r = ((q1 - q2) / (q1 + q2)) * x_rh + (x2 - x_rh);
                           z_r = ((q1 - q2) / (q1 + q2)) * z_rh + (z1 - z_rh);
                        } else if (row%2==1) { /* 1/\\/2 */
                           if (fDebug) fprintf(fp,"/* col :even,row : odd, 1/\\\\/2 */\n");
                           x_r = ((q1 - q2) / (q1 + q2)) * x_rh + (x2 - x_rh);
                           z_r = ((q2 - q1) / (q2 + q1)) * z_rh + (z2 - z_rh);
                        }
                     } else if ((col>=34) && (col%2==0)) { // col = 34,36,38
                        if (row%2==0) { /* 1/\\/2 */
                           if (fDebug) fprintf(fp,"/* col : odd,row :even, 1/\\\\/2 */\n");
                           x_r = ((q1 - q2) / (q1 + q2)) * x_rh + (x2 - x_rh);
                           z_r = ((q2 - q1) / (q2 + q1)) * z_rh + (z2 - z_rh);
                        } else if (row%2==1) { /* 1\//\2 */
                           if (fDebug) fprintf(fp,"/* col : odd,row : odd, 1\\//\\2 */\n");
                           x_r = ((q1 - q2) / (q1 + q2)) * x_rh + (x2 - x_rh);
                           z_r = ((q1 - q2) / (q1 + q2)) * z_rh + (z1 - z_rh);
                        }
                     }
                  } else { // id2<id
                     if ((col2<=5) && (col2%2==0)) { // col2 = 0,2,4
                        if (row2%2==0) { /* 2\//\1 */
                           if (fDebug) fprintf(fp,"/* col2:even,row2:even, 2\\//\\1 */\n");
                           x_r = ((q2 - q1) / (q2 + q1)) * x_rh + (x1 - x_rh);
                           z_r = ((q2 - q1) / (q2 + q1)) * z_rh + (z2 - z_rh);
                        } else if (row2%2==1) { /* 2/\\/1 */
                           if (fDebug) fprintf(fp,"/* col2:even,row2: odd, 2/\\\\/1 */\n");
                           x_r = ((q2 - q1) / (q2 + q1)) * x_rh + (x1 - x_rh);
                           z_r = ((q1 - q2) / (q1 + q2)) * z_rh + (z1 - z_rh);
                        }
                     } else if ((col2<=5) && (col2%2==1)) { // col2 = 1,3,5
                        if (row2%2==0) { /* 2/\\/1 */
                           if (fDebug) fprintf(fp,"/* col2: odd,row2:even, 2/\\\\/1 */\n");
                           x_r = ((q2 - q1) / (q2 + q1)) * x_rh + (x1 - x_rh);
                           z_r = ((q1 - q2) / (q1 + q2)) * z_rh + (z1 - z_rh);
                        } else if (row2%2==1) { /* 2\//\1 */
                           if (fDebug) fprintf(fp,"/* col2: odd,row2: odd, 2\\//\\1 */\n");
                           x_r = ((q2 - q1) / (q2 + q1)) * x_rh + (x1 - x_rh);
                           z_r = ((q2 - q1) / (q2 + q1)) * z_rh + (z2 - z_rh);
                        }
                     } else if ((col2>=34) && (col2%2==1)) { // col2 = 35,37,39
                        if (row2%2==0) { /* 2\//\1 */
                           if (fDebug) fprintf(fp,"/* col2:even,row2:even, 2\\//\\1 */\n");
                           x_r = ((q2 - q1) / (q2 + q1)) * x_rh + (x1 - x_rh);
                           z_r = ((q2 - q1) / (q2 + q1)) * z_rh + (z2 - z_rh);
                        } else if (row2%2==1) { /* 2/\\/1 */
                           if (fDebug) fprintf(fp,"/* col2:even,row2: odd, 2/\\\\/1 */\n");
                           x_r = ((q2 - q1) / (q2 + q1)) * x_rh + (x1 - x_rh);
                           z_r = ((q1 - q2) / (q1 + q2)) * z_rh + (z1 - z_rh);
                        }
                     } else if ((col2>=5) && (col2%2==0)) { // col2 = 34,36,38
                        if (row2%2==0) { /* 2/\\/1 */
                           if (fDebug) fprintf(fp,"/* col2: odd,row2:even, 2/\\\\/1 */\n");
                           x_r = ((q2 - q1) / (q2 + q1)) * x_rh + (x1 - x_rh);
                           z_r = ((q1 - q2) / (q1 + q2)) * z_rh + (z1 - z_rh);
                        } else if (row2%2==1) { /* 2\//\1 */
                           if (fDebug) fprintf(fp,"/* col2: odd,row2: odd, 2\\//\\1 */\n");
                           x_r = ((q2 - q1) / (q2 + q1)) * x_rh + (x1 - x_rh);
                           z_r = ((q2 - q1) / (q2 + q1)) * z_rh + (z2 - z_rh);
                        }
                     }
                  }
               }
               if (x_r==0.) continue;
               if ((!fLSM) || fDrawFig) fhRecoilHits->SetPoint(n,x_r,z_r);
               n++;
               Sx += x_r;
               Sz += z_r;
               Sxx += x_r * x_r;
               Sxz += x_r * z_r;
               if (fDebug) fprintf(fp,"id = %3d, id2 = %3d, x_r = %6.3f, z_r = %6.3f\n",id,id2,x_r,z_r);
            }
	
            if ((TMath::Abs(x1)< 20.0) && (l< 3.0)) { //  5 mm Pad
               Double_t z_rh = (5./2 * TMath::Sqrt(3) / 3) / 2;
               Double_t x_rh = (5./2 + gap) / 2;
               //if (fDetailDebug) fprintf(fp,"iHit  = %2d, id  = %3d, x1 = %6.3f, z1 = %6.3f, off  = %6.1f, q1 = %6.3f, diffoff  = %6.1f\n",iHit,id,x1,z1,off,q1,diffoff);
               //if (fDetailDebug) fprintf(fp,"iHit2 = %2d, id2 = %3d, x2 = %6.3f, z2 = %6.3f, off2 = %6.1f, q2 = %6.3f, diffoff2 = %6.1f\n",iHit2,id2,x2,z2,off2,q2,diffoff2);      
               if (fDetailDebug) fprintf(fp,"id = %3d: id2 = %3d, l = %6.3f mm, pair hit was found in  5 mm pad area\n",id,id2,l);
               if (x1==x2) {
                  x_rh = 5./2;
                  if (id<id2) {
                     if (fDebug) fprintf(fp,"col=col2, id<id2\n");
                     x_r = ((q2 - q1) / (q2 + q1)) * x_rh + x1;
                     z_r =  (z2 - z1 + gap) / 2 + z1;
                  } else {
                     if (fDebug) fprintf(fp,"col=col2, id>id2\n");
                     x_r = ((q1 - q2) / (q1 + q2)) * x_rh + x1;
                     z_r =  (z1 - z2 + gap) / 2 + z2;
                  }
               } else {
                  if (id<id2) {
                     if ((col%4==3) || (col==6) || (col==33)) { // col = 6,7,11,15,19,23,27,31,33
                        if (row%2==0) { /* 1\//\2 */
                           if (fDebug) fprintf(fp,"/* col : %2d,row :even, 1\\//\\2 */\n",col);
                           x_r = ((q1 - q2) / (q1 + q2)) * x_rh + (x2 - x_rh);
                           z_r = ((q1 - q2) / (q1 + q2)) * z_rh + (z1 - z_rh);
                        } else if (row%2==1) { /* 1/\\/2 */
                           if (fDebug) fprintf(fp,"/* col : %2d,row : odd, 1/\\\\/2 */\n",col);
                           x_r = ((q1 - q2) / (q1 + q2)) * x_rh + (x2 - x_rh);
                           z_r = ((q2 - q1) / (q2 + q1)) * z_rh + (z2 - z_rh);
                        }
                     } else if ((col%4==1) && (col!=33)) { // col = 9,13,17,21,25,29
                        if (row%2==0) { /* 1/\\/2 */
                           if (fDebug) fprintf(fp,"/* col : %2d,row :even, 1/\\\\/2 */\n",col);
                           x_r = ((q1 - q2) / (q1 + q2)) * x_rh + (x2 - x_rh);
                           z_r = ((q2 - q1) / (q2 + q1)) * z_rh + (z2 - z_rh);
                        } else if (row%2==1) { /* 1\//\2 */
                           if (fDebug) fprintf(fp,"/* col : %2d,row : odd, 1\\//\\2 */\n",col);
                           x_r = ((q1 - q2) / (q1 + q2)) * x_rh + (x2 - x_rh);
                           z_r = ((q1 - q2) / (q1 + q2)) * z_rh + (z1 - z_rh);
                        }
                     }
                  } else { // id2<id
                     if ((col%4==3) || (col==6) || (col==33)) { // col2 = 6,7,11,15,19,23,27,31,33
                        if (row2%2==0) { /* 2\//\1 */
                           if (fDebug) fprintf(fp,"/* col2: %2d,row2:even, 2\\//\\1 */\n",col2);
                           x_r = ((q2 - q1) / (q2 + q1)) * x_rh + (x1 - x_rh);
                           z_r = ((q2 - q1) / (q2 + q1)) * z_rh + (z2 - z_rh);
                        } else if (row2%2==1) { /* 2/\\/1 */
                           if (fDebug) fprintf(fp,"/* col2: %2d,row2: odd, 2/\\\\/1 */\n",col2);
                           x_r = ((q2 - q1) / (q2 + q1)) * x_rh + (x1 - x_rh);
                           z_r = ((q1 - q2) / (q1 + q2)) * z_rh + (z1 - z_rh);
                        }
                     } else if ((col%4==1) && (col!=33)) { // col2 = 9,13,17,21,25,29
                        if (row2%2==0) { /* 2/\\/1 */
                           if (fDebug) fprintf(fp,"/* col2: %2d,row2:even, 2/\\\\/1 */\n",col2);
                           x_r = ((q2 - q1) / (q2 + q1)) * x_rh + (x1 - x_rh);
                           z_r = ((q1 - q2) / (q1 + q2)) * z_rh + (z1 - z_rh);
                        } else if (row2%2==1) { /* 2\//\1 */
                           if (fDebug) fprintf(fp,"/* col2: %2d,row2: odd, 2\\//\\1 */\n",col2);
                           x_r = ((q2 - q1) / (q2 + q1)) * x_rh + (x1 - x_rh);
                           z_r = ((q2 - q1) / (q2 + q1)) * z_rh + (z2 - z_rh);
                        }
                     }
                  }
               }
               if (x_r==0.) continue;
               if ((!fLSM) || fDrawFig) fhRecoilHits->SetPoint(n,x_r,z_r);
               n++;
               Sx += x_r;
               Sz += z_r;
               Sxx += x_r * x_r;
               Sxz += x_r * z_r;
               if (fDebug) fprintf(fp,"id = %3d, id2 = %3d, x_r = %6.3f, z_r = %6.3f\n",id,id2,x_r,z_r);
            }
         }
         q_r += q1;
      }

      if (n<2) {
         if (fDebug) fprintf(fp,"n < 2\n");
         RecoilIsGood = false;
         continue;
      }

      if (fLSM) {
         if (fDebug) fprintf(fp,"Recoil: Least Square Method selected.\n");
         Double_t d   = 0;

         d = n * Sxx - Sx * Sx;
         if (d!=0) {
            a_rz = (n * Sxz - Sx * Sz) / d;
            b_rz = (Sxx * Sz - Sxz * Sx) / d;
         } else {
            a_rz = b_rz = 0;
         }
         if (fDebug) fprintf(fp,"q_r = %5.1f, a_rz = %5.1f, b_rz = %5.1f\n",q_r,a_rz,b_rz);

         if (fDrawFig) {
            if (a_rz!=0.) {
               fhRecoilRay->SetParameter(0,a_rz);
               fhRecoilRay->SetParameter(1,b_rz);
            }
         }

         if (TMath::Abs(a_rz) != 0.) {
            RecoilIsGood = true;
         } else {
            if (fDebug) fprintf(fp,"Skip: |a_rz| = 0.\n");
            RecoilIsGood = false;
            continue;
         }
      }
      /*--------------------------------------------------------------------------*/

      // Assumption: beam angle = 0. deg, beam position (x,z,y) = (0.,0.,y) mm 
      // modified 2016/06/02
      x0 = 0.;
      Double_t y0 = -50.;
      Double_t b_by = -50.;

      /*--------------------------------------------------------------------------*/

      // Calcurate Vertex Position
      //Double_t x_v = (b_rz - b_bz) / (a_bz - a_rz);
      //Double_t z_v = (a_bz * b_rz - a_rz * b_bz) / (a_bz - a_rz);
      //Double_t y_v = a_by * z_v + b_by;
      // Assumption: beam angle = 0. deg, beam position (x,z,y) = (0.,0.,y) mm
      // vertex position (x,y) = (0.,y) mm
      // modified 2016/06/02
      Double_t x_v = 0.;
      Double_t y_v = b_by;
      Double_t z_v = b_rz;
      // modified 2016/08/24

      // moved to around L.2850
      //if (fDebug) fprintf(fp,"x_v = %6.3f, z_v = %6.3f, y_v = %6.3f\n",x_v,z_v,y_v);
      //if ((TMath::Abs(x_v)>=50.) || (TMath::Abs(z_v)>=43.)) {
      //   if (fDebug) fprintf(fp,"Skip: Vertex is out of CAT active range\n");
      //   continue;
      //}
      /*--------------------------------------------------------------------------*/

      // Derive Linear Equation of Recoil (x,y)
      if (fDebug) fprintf(fp,"// Derive Linear Equation of Recoil (x,y)\n");
      nRecoilHits = recoil_array.GetEntriesFast();

#if 1
      Double_t y_r  = 0.;
      Double_t a_ry = 0.;
      Double_t b_ry = 0.;
      Double_t Sxy  = 0.;
      x_r = 0.;
      Sx  = 0.;
      Double_t Sy   = 0.;
      Sxx = 0.;
      n   = 0;

      // 0.70 cm/us = 0.70 x 10^(-1) mm/ns @ phys1802
      //{ // n = 0 : 
      //   Sx += x_v;
      //   Sy += y_v;
      //   Sxx += x_v * x_v;
      //   Sxy += x_v * y_v;
      //   if ((!fLSM) || fDrawFig) fhRecoilYHits->SetPoint(0,x_v,y_v);
      //   if (fDebug) fprintf(fp,"n = 0, x_v = %5.1f, y_v = %5.1f\n",x_v,y_v);
      //}

      for (Int_t iHit=0; iHit < nRecoilHits; iHit++) {
         TCatPulseShape *hit = (TCatPulseShape*) recoil_array[iHit];
         Int_t id = hit->GetID();
         Int_t col = id / 10;
         Int_t row = id % 10;
         if ((id > 69 && id < 330) && (col%2 == 0)) {
            col = col - 1;
            row = row + 10;
         }
         Double_t x = hit->GetX();
         Double_t z = hit->GetZ();
         Double_t q = hit->GetCharge();
         Float_t  off = hit->GetOffset();
         //Float_t  diffoff = off - BeamOff;

         // 0.70 cm/us = 0.70 x 10^(-2) mm/ns @ phys1802
         //y_r  = (off - BeamOff) * 20. * (fDriftVelocity / 100.);
         y_r  = off * 20. * (fDriftVelocity / 100.);
         x_r  = x;

         //n++;
         if ((!fLSM) || fDrawFig) fhRecoilYHits->SetPoint(n,x_r,y_r);
         if (fDebug) fprintf(fp,"iHit = %2d, id = %3d, x = %5.1f, z = %5.1f, off = %6.1f, y_r = %6.1f, q = %6.1f\n",iHit,id,x,z,off,y_r,q);
         n++;

         // Only hit of max_X joins fitting
         //if (x!=max_X) continue;

         Sx += x_r;
         Sy += y_r;
         Sxx += x_r * x_r;
         Sxy += x_r * y_r;
         if (fDetailDebug) fprintf(fp,"x_r = %5.1f, y_r = %6.1f\n",x_r,y_r);
      }

      if (fLSM) {
         if (fDebug) fprintf(fp,"Recoil: Least Square Method selected.\n");
         Double_t d   = 0;
      
         d = n * Sxx - Sx * Sx;
         if (d!=0) {
            a_ry = (n * Sxy - Sx * Sy) / d;
            b_ry = (Sxx * Sy - Sxy * Sx) / d;
         } else {
            a_ry = b_ry = 0;
         }
         if (fDebug) fprintf(fp,"a_ry = %5.1f, b_ry = %5.1f\n",a_ry,b_ry);

         if (fDrawFig) {
            if (a_ry!=0.) {
               fhRecoilYRay->SetParameter(0,a_ry);
               fhRecoilYRay->SetParameter(1,b_ry);
            }
         }

         if (TMath::Abs(a_ry) != 0.) {
            RecoilIsGood = true;
         } else {
            if (fDebug) fprintf(fp,"Skip: |a_ry| = 0.\n");
            RecoilIsGood = false;
            continue;
         }
      }
#else
      //// Only hits of max_X and vertex join
      //Double_t a_ry,b_ry,max_Y;
      //
      //// 0.70 cm/us = 0.70 x 10^(-1) mm/ns @ phys1802
      //max_Y  = (max_X_off - BeamOff) * 20. * (fDriftVelocity / 100.);
      //
      //a_ry = (max_Y - y_v) / (max_X - x_v);
      //b_ry = y_v - a_ry * x_v;
      //
      //if (fDebug) fprintf(fp,"max_X = %5.1f, max_Y = %5.1f, x_v = %5.1f, y_v = %5.1f, a_ry = %4.1f, b_ry = %4.1f\n",max_X,max_Y,x_v,y_v,a_ry,b_ry);
      //
      //if (fDrawFig) {
      //   if (a_ry!=0.) {
      //      fhRecoilYRay->SetParameter(0,a_ry);
      //      fhRecoilYRay->SetParameter(1,b_ry);
      //   }
      //}
      //
      //Double_t y_r;
      //n = 0;
      ////if ((!fLSM) || fDrawFig) fhRecoilYHits->SetPoint(0,x_v,y_v);
      //
      //for (Int_t iHit=0; iHit < nRecoilHits; iHit++) {
      //   TCatPulseShape *hit = (TCatPulseShape*) recoil_array[iHit];
      //   Int_t id = hit->GetID();
      //   Int_t col = id / 10;
      //   Int_t row = id % 10;
      //   if ((id > 69 && id < 330) && (col%2 == 0)) {
      //      col = col - 1;
      //      row = row + 10;
      //   }
      //   Double_t x = hit->GetX();
      //   Double_t z = hit->GetZ();
      //   Double_t q = hit->GetCharge();
      //   Float_t  off = hit->GetOffset();
      //   Float_t  diffoff = off - BeamOff;
      //
      //   // 0.70 cm/us = 0.70 x 10^(-1) mm/ns @ phys1802
      //   y_r  = (off - BeamOff) * 20. * (fDriftVelocity / 100.);
      //   x_r  = x;
      //
      //   n++;
      //   if ((!fLSM) || fDrawFig) fhRecoilYHits->SetPoint(n,x_r,y_r);
      //   if (fDebug) fprintf(fp,"iHit = %2d, id = %3d, x = %5.1f, z = %5.1f, off = %6.1f, y_r = %6.1f, q = %6.1f, diffoff = %5.1f\n",iHit,id,x,z,off,y_r,q,diffoff);
      //}
      //
      //
      //if (TMath::Abs(a_ry) != 0.) {
      //   RecoilIsGood = true;
      //} else {
      //   if (fDebug) fprintf(fp,"Skip: |a_ry| = 0.\n");
      //   RecoilIsGood = false;
      //   continue;
      //}
#endif
      /*--------------------------------------------------------------------------*/
      
      y_v = b_ry;

      // modified 2016/08/24
      y0 = y_v;
      // moved from L.2700
      if (fDebug) fprintf(fp,"x_v = %6.3f, z_v = %6.3f, y_v = %6.3f\n",x_v,z_v,y_v);
      if ((TMath::Abs(x_v)>=50.) || (TMath::Abs(z_v)>=43.)) {
         if (fDebug) fprintf(fp,"Skip: Vertex is out of CAT active range\n");
         continue;
      }
      /*--------------------------------------------------------------------------*/
      
      // Calcuration Recoil Range
      Double_t max_Z;
      max_Z = a_rz * max_X + b_rz;
      Double_t max_Y;
      max_Y = a_ry * max_X + b_ry;
      Double_t recoil_range = TMath::Sqrt((max_X - x_v)*(max_X - x_v) + (max_Y - y_v)*(max_Y - y_v) + (max_Z - z_v)*(max_Z - z_v));

      //// Calcuration Recoil Energy from Range
      Double_t recoil_TKE = -1.0E+9;
      if (fLowPressure) { // 0.4 atm
         if (recoil_range<520.) {
            recoil_TKE = (1.0e-8)*(recoil_range*recoil_range*recoil_range) + (-1.0e-5)*(recoil_range*recoil_range) + 0.0078*recoil_range + 0.0018;
         } else {
            recoil_TKE = (8.0e-12)*(recoil_range*recoil_range*recoil_range) + (-2.0e-7)*(recoil_range*recoil_range) + 0.0019*recoil_range + 1.1185;
         }
      } else { // 1 atm
         if (recoil_range<60.) {
            recoil_TKE = (-8.0e-5)*(recoil_range*recoil_range) + 0.0202*recoil_range - 0.0078;
         } else {
            recoil_TKE = 0.0089*recoil_range + 0.3817;
         }
      }

      if (fDebug) fprintf(fp,"recoil_range = %6.3f, recoil_TKE = %6.3f,q_r = %6.3f, max_X = %6.3f, max_Y = %6.3f, max_Z = %6.3f\n",
                          recoil_range,recoil_TKE,q_r,max_X,max_Y,max_Z);
      //if (fSaveHist) fhRecoilRangeQ->Fill(recoil_range,q_r);
      /*--------------------------------------------------------------------------*/

      // Combine recoil with beam
      std::vector<TCatPulseShape*> event;
      it    = beam.begin();
      itend = beam.end();
      for (; it != itend; it++) {
         event.push_back((*it));
      }
      nRecoilHits = recoil_array.GetEntriesFast();
      for (Int_t iHit=0; iHit < nRecoilHits; iHit++) {
         TCatPulseShape *hit = (TCatPulseShape*) recoil_array[iHit];
         event.push_back(hit);
      }
    
      if (fDebug) fprintf(fp,"Results: cls[%d] size = %zd -> %zd\n",icls,cls[icls].size(),event.size());
      if (fDetailDebug) printf("Results: cls[%d] size = %zd -> %zd\n",icls,cls[icls].size(),event.size());
      /*--------------------------------------------------------------------------*/
    
      switch(fSelectCondition) {
      case 0: // Recoil Range >= SetRecoilRange, and Recoil is penetrating reading pad
         if (fDebug) fprintf(fp,"case 0 was selected\n");
         if (fDetailDebug) printf("case 0 was selected\n");
         if ((BeamIsGood) && (recoil_range>=SetRecoilRange) && (IsPenetrating)) IsGood = true;
         break;
      case 1: // Recoil Range >= SetRecoilRange, and Recoil is NOT penetrating reading pad
         if (fDebug) fprintf(fp,"case 1 was selected\n");
         if (fDetailDebug) printf("case 1 was selected\n");
         if ((BeamIsGood) && (recoil_range>=SetRecoilRange) && (!IsPenetrating)) IsGood = true;
         break;
      case 2: // Recoil Range >= SetRecoilRange
         if (fDebug) fprintf(fp,"case 2 was selected\n");
         if (fDetailDebug) printf("case 2 was selected\n");
         if ((BeamIsGood) && (recoil_range>=SetRecoilRange) && (RecoilIsGood)) IsGood = true;
         break;
      case 3: // Recoil Range >= 5.0 mm
         if (fDebug) fprintf(fp,"case 3 was selected\n");
         if (fDetailDebug) printf("case 3 was selected\n");
         if ((BeamIsGood) && (recoil_range>=5.)) IsGood = true;
         break;
      case 4: // Good Beam Event
         if (fDebug) fprintf(fp,"case 3 was selected\n");
         if (fDetailDebug) printf("case 3 was selected\n");
         if (BeamIsGood) IsGood = true;
         IsGoodBeamEvent = true;
         break;
      default: // Not Good Event
         if (fDebug) fprintf(fp,"Not Good Event, No Save\n");
         if (fDetailDebug) printf("Not Good Event, No Save\n");
         break;
      }
      if (!IsGood) continue;
      /*--------------------------------------------------------------------------*/

      // Kinematics Calcuration
      // Lab. Angle
      TVector3 Vvertex(x_v,y_v,z_v);
      TVector3 Vbeam(TMath::Abs(x_v - x0),TMath::Abs(y_v - y0),TMath::Abs(z_v - z0));
      TVector3 Vrecoil((max_X - x_v),(max_Y - y_v),(max_Z - z_v));
      if (fDebug) fprintf(fp,"Vvertex = (%6.3f,%6.3f,%6.3f), Vbeam = (%6.3f,%6.3f,%6.3f), Vrecoil = (%6.3f,%6.3f,%6.3f)\n",x_v,y_v,z_v,TMath::Abs(x_v - x0),TMath::Abs(y_v - y0),TMath::Abs(z_v - z0),(max_X - x_v),(max_Y - y_v),(max_Z - z_v));
      if (fDebug) fprintf(fp,"beam0 = (%6.3f,%6.3f,%6.3f), recoil_range recoil = (%6.3f,%6.3f,%6.3f)\n",x0,y0,z0,max_X,max_Y,max_Z);
      Double_t angle = 0.;
      Double_t unit = 180. / TMath::Pi();
      //angle = TMath::ATan(TMath::Abs ((a_bz- a_rz)/(1 + a_bz* a_rz))) * unit;
      angle = Vrecoil.Angle(Vbeam) * unit;

      if (!((angle>=fAngle_lowth) && (angle<=fAngle_hith))) continue;

      if (fDebug) fprintf(fp,"angle = %0.3f, angle/unit = %0.3f, q_r = %0.3f, a_rz= %0.3f, a_ry = %0.3f\n",angle,angle/unit,q_r,a_rz,a_ry);
      //if (fDebug) fprintf(fp,"angle = %0.3f, angle/unit = %0.3f, q_r = %0.3f, a_bz = %0.3f, a_by = %0.3f, a_rz= %0.3f, a_ry = %0.3f\n",angle,angle/unit,q_r,a_bz,a_by,a_rz,a_ry);
      if (fSaveHist) fhAngle->Fill(angle);
      if (fSaveHist) fhAngleRecoilE->Fill(angle,q_r);
      if (fSaveHist) fhAngleRecoilE_ex->Fill(angle,q_r);
      if (fSaveHist) fhAngleRecoilRange->Fill(angle,recoil_range);
      if (fSaveHist) fhAngleRecoilTKE->Fill(angle,recoil_TKE);

      Double_t E_A,p_A,T_A,E_2H,p_2H,E_2Hrec,p_2Hrec,T_2Hrec,angle_lab,beta;
      Double_t phi_A,phi_2H,theta_A,theta_2H;
      const Double_t m_A   = fBeamMass;       // mass[MeV] of A
      const Double_t m_2H  =  1875.628;       // mass[MeV] of  2H
      p_2H = 0;
      E_2H = m_2H;
      angle_lab = Vrecoil.Angle(Vbeam);

      phi_A = Vbeam.Phi();
      phi_2H  = Vrecoil.Phi();
      theta_A = Vbeam.Theta();
      theta_2H  = Vrecoil.Theta();
      if (fDebug) fprintf(fp,"phi_A = %6.3f, theta_A = %6.3f\n",phi_A,theta_A);

      // temporary
      T_A   = fBeamAtomicMass * fBeamVelocity; // kinetic energy of A beam T MeV/u
      //T_A   = Q_b; // kinetic energy of A beam 
      //T_2Hrec = 0.5; // kinetic energy of 2H 1 MeV
      T_2Hrec = recoil_TKE; // kinetic energy of 2H 1 MeV
      //T_2Hrec = q_r; // kinetic energy of 2H 

      E_A = m_A + T_A;
      p_A = TMath::Sqrt(E_A * E_A - m_A * m_A);

      E_2Hrec  = m_2H + T_2Hrec;
      p_2Hrec  = TMath::Sqrt(E_2Hrec * E_2Hrec - m_2H * m_2H);

      beta = p_A / (E_A + m_2H);

      // Lorentz Boost
      Double_t Ecm_A,pcm_A,Ecm_2H,pcm_2H,Ecm_2Hrec,pcm_2Hrec,Ex;
      Double_t angle_cm;
      TVector3 Pbeam((p_A * TMath::Sin(theta_A) * TMath::Sin(phi_A)),(p_A * TMath::Sin(theta_A) * TMath::Cos(phi_A)),(p_A * TMath::Cos(theta_A)));
      TVector3 Precoil((p_2H * TMath::Sin(theta_2H) * TMath::Sin(phi_2H)),(p_2H * TMath::Sin(theta_2H) * TMath::Cos(phi_2H)),(p_2H * TMath::Cos(theta_2H)));
      TLorentzVector V4beam_cm(Pbeam,E_A);
      TLorentzVector V4recoil_cm(Precoil,E_2H);
      V4beam_cm.Boost(0,0,beta);
      V4recoil_cm.Boost(0,0,beta);
      TVector3 Vbeam_cm(V4beam_cm.Px(),V4beam_cm.Py(),V4beam_cm.Pz());
      TVector3 Vrecoil_cm(V4recoil_cm.Px(),V4recoil_cm.Py(),V4recoil_cm.Pz());

      Ecm_A = V4beam_cm.E();
      pcm_A = Vbeam_cm.Mag();

      pcm_2H = pcm_A;
      Ecm_2H = TMath::Sqrt(m_2H * m_2H + pcm_2H * pcm_2H);

      Ecm_2Hrec = V4recoil_cm.E();
      pcm_2Hrec = Vrecoil_cm.Mag();

      angle_cm = Vrecoil_cm.Angle(Vbeam_cm) * unit;
      if (fSaveHist) fhCMAngle->Fill(angle_cm);

      if (fDebug) fprintf(fp,"beta = %6.3f, E_A = %8.2f MeV, p_A = %8.2f MeV/c, T_A = %8.2f MeV, E_2Hrec = %8.2f MeV, p_2Hrec = %8.2f MeV/c, T_2Hrec = %8.2f MeV\n",beta,E_A,p_A,T_A,E_2Hrec,p_2Hrec,T_2Hrec);
      if (fDebug) fprintf(fp,"Ecm_A   = %8.2f MeV, pcm_A   = %8.2f MeV/c\n",Ecm_A,pcm_A);
      if (fDebug) fprintf(fp,"Ecm_2H    = %8.2f MeV, pcm_2H    = %8.2f MeV/c\n",Ecm_2H,pcm_2H);
      if (fDebug) fprintf(fp,"Ecm_2Hrec = %8.2f MeV, pcm_2Hrec = %8.2f MeV/c\n",Ecm_2Hrec,pcm_2Hrec);
      if (fDebug) fprintf(fp,"angle_cm = %8.2f\n",angle_cm);

      Ex = TMath::Sqrt((E_A + m_2H - E_2Hrec) * (E_A + m_2H - E_2Hrec) - ((p_A - p_2Hrec * TMath::Cos(angle_lab)) * (p_A - p_2Hrec * TMath::Cos(angle_lab)) + (p_2Hrec * TMath::Sin(angle_lab)) * (p_2Hrec * TMath::Sin(angle_lab)))) - m_A;

      if (fDebug) fprintf(fp,"Ex = %6.3f\n",Ex);
      if (fSaveHist) fhEx->Fill(Ex);

      if (fSaveHist) fhAngleEx->Fill(angle,Ex);
      /*--------------------------------------------------------------------------*/

      if (fSaveHist) {
         fhRecoilNRange->Fill(nRecoilHits,recoil_range);
         fhRecoilNAngle->Fill(nRecoilHits,angle);
      }
      /*--------------------------------------------------------------------------*/

      // Save
      if (!IsGoodBeamEvent) {
         if ((!fShowRecoil) && (!fShowBeam)) {
            // event : after combined beam with recoil
            if (fDebug) fprintf(fp,"Save event = beam + recoil, icls = %d, cls_i = %d\n",icls,cls_i);
            if (fDetailDebug) printf("Save event = beam + recoil, icls = %d, cls_i = %d\n",icls,cls_i);
            std::vector<TCatPulseShape*>::iterator it    = event.begin();
            std::vector<TCatPulseShape*>::iterator itend = event.end();
            for (; it != itend; it++) {
               TCatPulseShape *out = (TCatPulseShape*) fOutput->ConstructedAt(fOutput->GetEntriesFast());
               (*it)->Copy(*out);
	    
               Int_t num = 0;
               num = (*it)->GetNumSample();
               out->GetSample().resize(num);
               out->GetClock().resize(num);
               std::copy((*it)->GetSample().begin(),(*it)->GetSample().end(),out->GetSample().begin());
               for (Int_t i=0; i != num; i++) {
                  out->GetClock()[i] = (Double_t)i;
               }
               out->SetNumSample(num);
               if (fDrawPulse) {
                  TObject *hitobj = out;
                  fPulseDrawer->SetPulse(*hitobj);
                  Int_t iHit = std::distance(event.begin(),it);
                  fPulseDrawer->SetGraphRange(0.,100.,-2.,60.);
                  fPulseDrawer->SetEvent(fEvt,iHit);
                  //fPulseDrawer->SetViewID(fSaveID);
                  fPulseDrawer->SetTextNDC(Form("Evt = %5d, id = %3d, offset = %5.1f, cls = %2d",fEvt,out->GetID(),out->GetOffset(),cls_i),1);
                  fPulseDrawer->SetTextNDC(Form("nHits = %3d, iHit = %3d, nSample = %3d, charge = %5.2f",nHits,iHit,num,out->GetCharge()),5,0.10,0.91);
                  fPulseDrawer->SetTextNDC(GetName(),5,0.10,0.01);
                  fPulseDrawer->Draw();
               }
            }

            // Save only Beam hits
            it    = beam.begin();
            itend = beam.end();
            for (; it != itend; it++) {
               TCatPulseShape *out = (TCatPulseShape*) fOutputBeam->ConstructedAt(fOutputBeam->GetEntriesFast());
               (*it)->Copy(*out);
           
               Int_t num = 0;
               num = (*it)->GetNumSample();
               out->GetSample().resize(num);
               out->GetClock().resize(num);
               std::copy((*it)->GetSample().begin(),(*it)->GetSample().end(),out->GetSample().begin());
               for (Int_t i=0; i != num; i++) {
                  out->GetClock()[i] = (Double_t)i;
               }
               out->SetNumSample(num);
            }

            // Save only Recoil hits
            for (Int_t iHit=0; iHit < nRecoilHits; iHit++) {
               TCatPulseShape *hit = (TCatPulseShape*) recoil_array[iHit];
               TCatPulseShape *out = (TCatPulseShape*) fOutputRecoil->ConstructedAt(fOutputRecoil->GetEntriesFast());
               hit->Copy(*out);
           
               Int_t num = 0;
               num = hit->GetNumSample();
               out->GetSample().resize(num);
               out->GetClock().resize(num);
               std::copy(hit->GetSample().begin(),hit->GetSample().end(),out->GetSample().begin());
               for (Int_t i=0; i != num; i++) {
                  out->GetClock()[i] = (Double_t)i;
               }
               out->SetNumSample(num);
            }
         } else if ((fShowRecoil) || (fShowBeam)) {
            if (!fShowBeam) {
               // Save only recoil hits
               if (fDebug) fprintf(fp,"Save recoil, icls = %d, cls_i = %d\n",icls,cls_i);
               if (fDetailDebug) printf("Save recoil, icls = %d, cls_i = %d\n",icls,cls_i);
               for (Int_t iHit=0; iHit < nRecoilHits; iHit++) {
                  TCatPulseShape *hit = (TCatPulseShape*) recoil_array[iHit];
                  TCatPulseShape *out = (TCatPulseShape*) fOutputRecoil->ConstructedAt(fOutputRecoil->GetEntriesFast());
                  hit->Copy(*out);
                
                  Int_t num = 0;
                  num = hit->GetNumSample();
                  out->GetSample().resize(num);
                  out->GetClock().resize(num);
                  std::copy(hit->GetSample().begin(),hit->GetSample().end(),out->GetSample().begin());
                  for (Int_t i=0; i != num; i++) {
                     out->GetClock()[i] = (Double_t)i;
                  }
                  out->SetNumSample(num);
                
                  if (fDrawPulse) {
                     TObject *hitobj = out;
                     fPulseDrawer->SetPulse(*hitobj);
                     fPulseDrawer->SetEvent(fEvt,iHit);
                     fPulseDrawer->SetGraphRange(0.,100.,-2.,60.);
                     //fPulseDrawer->SetViewID(fSaveID);
                     fPulseDrawer->SetTextNDC(Form("Evt = %5d, id = %3d, offset = %5.1f, cls = %2d",fEvt,out->GetID(),out->GetOffset(),cls_i),1);
                     fPulseDrawer->SetTextNDC(Form("nHits = %3d, iHit = %3d, nSample = %3d, charge = %5.2f",nHits,iHit,num,out->GetCharge()),5,0.10,0.91);
                     fPulseDrawer->SetTextNDC(GetName(),5,0.10,0.01);
                     fPulseDrawer->Draw();
                  }
               }
            } else if (!fShowRecoil) {
               // Save only beam hits
               if (fDebug) fprintf(fp,"Save beam, icls = %d, cls_i = %d\n",icls,cls_i);
               if (fDetailDebug) printf("Save beam, icls = %d, cls_i = %d\n",icls,cls_i);
               it    = beam.begin();
               itend = beam.end();
               for (; it != itend; it++) {
                  TCatPulseShape *out = (TCatPulseShape*) fOutputBeam->ConstructedAt(fOutputBeam->GetEntriesFast());
                  (*it)->Copy(*out);
                
                  Int_t num = 0;
                  num = (*it)->GetNumSample();
                  out->GetSample().resize(num);
                  out->GetClock().resize(num);
                  std::copy((*it)->GetSample().begin(),(*it)->GetSample().end(),out->GetSample().begin());
                  for (Int_t i=0; i != num; i++) {
                     out->GetClock()[i] = (Double_t)i;
                  }
                  out->SetNumSample(num);
                
                  if (fDrawPulse) {
                     TObject *hitobj = out;
                     fPulseDrawer->SetPulse(*hitobj);
                     Int_t iHit = std::distance(beam.begin(),it);
                     fPulseDrawer->SetEvent(fEvt,iHit);
                     fPulseDrawer->SetGraphRange(0.,100.,-2.,60.);
                     //fPulseDrawer->SetViewID(fSaveID);
                     fPulseDrawer->SetTextNDC(Form("Evt = %5d, id = %3d, offset = %5.1f, cls = %2d",fEvt,out->GetID(),out->GetOffset(),cls_i),1);
                     fPulseDrawer->SetTextNDC(Form("nHits = %3d, iHit = %3d, nSample = %3d, charge = %5.2f",nHits,iHit,num,out->GetCharge()),5,0.10,0.91);
                     fPulseDrawer->SetTextNDC(GetName(),5,0.10,0.01);
                     fPulseDrawer->Draw();
                  }
               }
            }
         }
      } else { // Good Beam Event
         if (fDebug) fprintf(fp,"Save Good Beam Event, icls = %d, cls_i = %d\n",icls,cls_i);
         if (fDetailDebug) printf("Save Good Beam Event, icls = %d, cls_i = %d\n",icls,cls_i);
         it    = cls[icls].begin();
         itend = cls[icls].end();
         for (; it != itend; it++) {
            TCatPulseShape *out = (TCatPulseShape*) fOutput->ConstructedAt(fOutput->GetEntriesFast());
            (*it)->Copy(*out);
	  
            Int_t num = 0;
            num = (*it)->GetNumSample();
            out->GetSample().resize(num);
            out->GetClock().resize(num);
            std::copy((*it)->GetSample().begin(),(*it)->GetSample().end(),out->GetSample().begin());
            for (Int_t i=0; i != num; i++) {
               out->GetClock()[i] = (Double_t)i;
            }
            out->SetNumSample(num);
         }
      }
      /*--------------------------------------------------------------------------*/

      // Draw Figure (x,z)
      if (fDrawFig) {
         fCanvas->cd();
         //fhBeamHits->SetMarkerStyle(8);
         fhRecoilHits->SetMarkerStyle(8);
         fHPadEvt->SetMaximum(500.);
         fHPadEvt->SetMinimum(0.);
         if (fLSM) {
            //fhBeamRay->SetLineColor(kBlack);
            fhRecoilRay->SetLineColor(kBlack);
         } else {
            //fhBeamHits->GetFunction("pol1")->SetLineColor(kBlack);
            fhRecoilHits->GetFunction("pol1")->SetLineColor(kBlack);
         }
         t->SetTextColor(kBlue);
         t->SetTextSize(0.03);
         if (fSaveFig) {
            std::stringstream out_name;
            {
               std::string FigNamePath(fFigFile);
               if (fSaveEvt != -1) {
                  if (fEvt == fSaveEvt) {
                     out_name << FigNamePath << fOutputColName.Data() << "_" << fRunName << std::setw(4) << std::setfill('0') << fRunNumber << "_Evt" << fEvt << "_cls" << cls_i << "_zx" << ".png";
                     if (!fNoColorFig) {
                        fHPadEvt->Draw("colz");
                     } else {
                        fHPadEvt->Draw();
                     }
                     //fhBeamHits->Draw("P same");
                     //if (fLSM) fhBeamRay->Draw("same");
                     fhRecoilHits->Draw("P same");
                     if (fLSM) fhRecoilRay->Draw("same");
                     t->DrawTextNDC(0.1,0.91,Form("Evt = %5d, cls = %3d",fEvt,cls_i));
                     t->DrawTextNDC(0.01,0.04,Form("Vertex: (x,z,y) = (%5.1f,%5.1f,%5.1f), theta_lab = %5.1f deg.",x_v,z_v,y_v,angle));
                     t->DrawTextNDC(0.01,0.01,Form("Recoil %2d hits: range = %5.1f mm, Ex = %5.1f MeV",nRecoilHits,recoil_range,Ex));
                     gPad->SaveAs(out_name.str().c_str());
                     fCanvas->Update();
                     fCanvas->Modified();
                     if (fWaitTime>0) usleep(fWaitTime*100000); // [0.1sec]
                  }
               } else {
                  out_name << FigNamePath << fOutputColName.Data() << "_" << fRunName << std::setw(4) << std::setfill('0') << fRunNumber << "_Evt" << fEvt << "_cls" << cls_i << "_zx" << ".png";
                  if (!fNoColorFig) {
                     fHPadEvt->Draw("colz");
                  } else {
                     fHPadEvt->Draw();
                  }
                  //fhBeamHits->Draw("P same");
                  //if (fLSM) fhBeamRay->Draw("same");
                  fhRecoilHits->Draw("P same");
                  if (fLSM) fhRecoilRay->Draw("same");
                  t->DrawTextNDC(0.1,0.91,Form("Evt = %5d, cls = %3d",fEvt,cls_i));
                  t->DrawTextNDC(0.01,0.04,Form("Vertex: (x,z,y) = (%5.1f,%5.1f,%5.1f), theta_lab = %5.1f deg.",x_v,z_v,y_v,angle));
                  t->DrawTextNDC(0.01,0.01,Form("Recoil %2d hits: range = %5.1f mm, Ex = %5.1f MeV",nRecoilHits,recoil_range,Ex));
                  gPad->SaveAs(out_name.str().c_str());
                  fCanvas->Update();
                  fCanvas->Modified();
                  if (fWaitTime>0) usleep(fWaitTime*100000); // [0.1sec]
               }
            }
         } else { // not save fig.
            if (fSaveEvt != -1) {
               if (fEvt == fSaveEvt) {
                  if (!fNoColorFig) {
                     fHPadEvt->Draw("colz");
                  } else {
                     fHPadEvt->Draw();
                  }
                  //fhBeamHits->Draw("P same");
                  //if (fLSM) fhBeamRay->Draw("same");
                  fhRecoilHits->Draw("P same");
                  if (n>0) if (fLSM) fhRecoilRay->Draw("same");
                  t->DrawTextNDC(0.1,0.91,Form("Evt = %5d, cls = %3d",fEvt,cls_i));
                  t->DrawTextNDC(0.01,0.04,Form("Vertex: (x,z,y) = (%5.1f,%5.1f,%5.1f), theta_lab = %5.1f deg.",x_v,z_v,y_v,angle));
                  t->DrawTextNDC(0.01,0.01,Form("Recoil %2d hits: range = %5.1f mm, Ex = %5.1f MeV",nRecoilHits,recoil_range,Ex));
                  fCanvas->Update();
                  fCanvas->Modified();
                  if (fWaitTime>0) usleep(fWaitTime*100000); // [0.1sec]
               }
            } else {
               if (!fNoColorFig) {
                  fHPadEvt->Draw("colz");
               } else {
                  fHPadEvt->Draw();
               }
               //fhBeamHits->Draw("P same");
               //if (fLSM) fhBeamRay->Draw("same");
               fhRecoilHits->Draw("P same");
               if (n>0) if (fLSM) fhRecoilRay->Draw("same");
               t->DrawTextNDC(0.1,0.91,Form("Evt = %5d, cls = %3d",fEvt,cls_i));
               t->DrawTextNDC(0.01,0.04,Form("Vertex: (x,z,y) = (%5.1f,%5.1f,%5.1f), theta_lab = %5.1f deg.",x_v,z_v,y_v,angle));
               t->DrawTextNDC(0.01,0.01,Form("Recoil %2d hits: range = %5.1f mm, Ex = %5.1f MeV",nRecoilHits,recoil_range,Ex));
               fCanvas->Update();
               fCanvas->Modified();
               if (fWaitTime>0) usleep(fWaitTime*100000); // [0.1sec]
            }
         }
      }
      /*--------------------------------------------------------------------------*/

      // Draw Figure Beam(z,y)
      //if (fDrawFig) {
      //   //fCanvas2->cd(1);
      //   fCanvas2->cd();
      //   fhBeamYHits->GetXaxis()->SetTitle("z[mm]");
      //   fhBeamYHits->GetYaxis()->SetTitle("y[mm]");
      //   fhBeamYHits->SetMarkerStyle(8);
      //   if (fLSM) {
      //      fhBeamYRay->SetLineColor(kBlack);
      //   } else {
      //      fhBeamYHits->GetFunction("pol1")->SetLineColor(kBlack);
      //   }
      //   t->SetTextColor(kBlue);
      //   t->SetTextSize(0.05);
      //   if (fSaveFig) {
      //      std::stringstream out_name;
      //      {
      //         std::string FigNamePath(fFigFile);
      //         if (fSaveEvt != -1) {
      //            if (fEvt == fSaveEvt) {
      //               out_name << FigNamePath << fOutputColName.Data() << "_" << fRunName << std::setw(4) << std::setfill('0') << fRunNumber << "_Evt" << fEvt << "_cls" << cls_i << "_zy" << ".png";
      //               fhBeamYHits->Draw("AP");
      //               if (fLSM) fhBeamYRay->Draw("same");
      //               t->DrawTextNDC(0.1,0.91,Form("Evt = %5d, cls = %3d",fEvt,cls_i));
      //               t->DrawTextNDC(0.1,0.04,Form("Beam: y = %5.1f z + %5.1f",a_by,b_by));
      //               gPad->SaveAs(out_name.str().c_str());
      //               fCanvas2->Update();
      //               fCanvas2->Modified();
      //               if (fWaitTime>0) usleep(fWaitTime*100000); // [0.1sec]
      //            }
      //         } else {
      //            out_name << FigNamePath << fOutputColName.Data() << "_" << fRunName << std::setw(4) << std::setfill('0') << fRunNumber << "_Evt" << fEvt << "_cls" << cls_i << "_zy" << ".png";                  fhBeamYHits->Draw("AP");
      //            if (fLSM) fhBeamYRay->Draw("same");
      //            t->DrawTextNDC(0.1,0.91,Form("Evt = %5d, cls = %3d",fEvt,cls_i));
      //            t->DrawTextNDC(0.6,0.91,Form("Beam: y = %5.1f z + %5.1f",a_by,b_by));
      //            gPad->SaveAs(out_name.str().c_str());
      //            fCanvas2->Update();
      //            fCanvas2->Modified();
      //            if (fWaitTime>0) usleep(fWaitTime*100000); // [0.1sec]
      //         }
      //      }
      //   } else { // not save fig.
      //      if (fSaveEvt != -1) {
      //         if (fEvt == fSaveEvt) {
      //            fhBeamYHits->Draw("AP");
      //            if (fLSM) fhBeamYRay->Draw("same");
      //            t->DrawTextNDC(0.1,0.91,Form("Evt = %5d, cls = %3d",fEvt,cls_i));
      //            t->DrawTextNDC(0.6,0.91,Form("Beam: y = %5.1f z + %5.1f",a_by,b_by));
      //            fCanvas2->Update();
      //            fCanvas2->Modified();
      //            if (fWaitTime>0) usleep(fWaitTime*100000); // [0.1sec]
      //         }
      //      } else {
      //         fhBeamYHits->Draw("AP");
      //         if (fLSM) fhBeamYRay->Draw("same");
      //         t->DrawTextNDC(0.1,0.91,Form("Evt = %5d, cls = %3d",fEvt,cls_i));
      //         t->DrawTextNDC(0.6,0.91,Form("Beam: y = %5.1f z + %5.1f",a_by,b_by));
      //         fCanvas2->Update();
      //         fCanvas2->Modified();
      //         if (fWaitTime>0) usleep(fWaitTime*100000); // [0.1sec]
      //      }
      //      }
      //}
      /*--------------------------------------------------------------------------*/

      // Draw Figure Recoil(x,y)
      if (fDrawFig) {
         //fCanvas2->cd(2);
         fCanvas2->cd();
         fhRecoilYHits->SetMarkerStyle(8);
         fhBeamYHits->SetMarkerStyle(8);
         fhBeamYHits->SetMarkerColor(kRed);
         fhBeamYHits->GetXaxis()->SetTitle("x[mm]");
         fhBeamYHits->GetYaxis()->SetTitle("y[mm]");
         fhBeamYHits->GetHistogram()->SetBins(100,-50.,50.);
         //fhRecoilYHits->GetXaxis()->SetTitle("x[mm]");
         //fhRecoilYHits->GetYaxis()->SetTitle("y[mm]");
         if (fLSM) {
            fhRecoilYRay->SetLineColor(kBlack);
         } else {
            fhRecoilYHits->GetFunction("pol1")->SetLineColor(kBlack);
         }
         t->SetTextColor(kBlue);
         t->SetTextSize(0.03);
         if (fSaveFig) {
            std::stringstream out_name;
            {
               std::string FigNamePath(fFigFile);
               if (fSaveEvt != -1) {
                  if (fEvt == fSaveEvt) {
                     out_name << FigNamePath << fOutputColName.Data() << "_" << fRunName << std::setw(4) << std::setfill('0') << fRunNumber << "_Evt" << fEvt << "_cls" << cls_i << "_xy" << ".png";
                     fhBeamYHits->Draw("AP");
                     if (fLSM) fhRecoilYRay->Draw("same");
                     fhRecoilYHits->Draw("P same");
                     t->DrawTextNDC(0.1,0.91,Form("Evt = %5d, cls = %3d",fEvt,cls_i));
                     t->DrawTextNDC(0.1,0.04,Form("Recoil: y = %5.1f x + %5.1f",a_ry,b_ry));
                     gPad->SaveAs(out_name.str().c_str());
                     fCanvas2->Update();
                     fCanvas2->Modified();
                     if (fWaitTime>0) usleep(fWaitTime*100000); // [0.1sec]
                  }
               } else {
                  out_name << FigNamePath << fOutputColName.Data() << "_" << fRunName << std::setw(4) << std::setfill('0') << fRunNumber << "_Evt" << fEvt << "_cls" << cls_i << "_xy" << ".png";
                  fhBeamYHits->Draw("AP");
                  if (fLSM) fhRecoilYRay->Draw("same");
                  fhRecoilYHits->Draw("P same");
                  t->DrawTextNDC(0.1,0.91,Form("Evt = %5d, cls = %3d",fEvt,cls_i));
                  t->DrawTextNDC(0.1,0.04,Form("Recoil: y = %5.1f x + %5.1f",a_ry,b_ry));
                  gPad->SaveAs(out_name.str().c_str());
                  fCanvas2->Update();
                  fCanvas2->Modified();
                  if (fWaitTime>0) usleep(fWaitTime*100000); // [0.1sec]
               }
            }
         } else { // not save fig.
            if (fSaveEvt != -1) {
               if (fEvt == fSaveEvt) {
                  fhBeamYHits->Draw("AP");
                  if (fLSM) fhRecoilYRay->Draw("same");
                  fhRecoilYHits->Draw("P same");
                  t->DrawTextNDC(0.1,0.91,Form("Evt = %5d, cls = %3d",fEvt,cls_i));
                  t->DrawTextNDC(0.1,0.04,Form("Recoil: y = %5.1f x + %5.1f",a_ry,b_ry));
                  fCanvas2->Update();
                  fCanvas2->Modified();
                  if (fWaitTime>0) usleep(fWaitTime*100000); // [0.1sec]
               }
            } else {
               fhBeamYHits->Draw("AP");
               if (fLSM) fhRecoilYRay->Draw("same");
               fhRecoilYHits->Draw("P same");
               t->DrawTextNDC(0.1,0.91,Form("Evt = %5d, cls = %3d",fEvt,cls_i));
               t->DrawTextNDC(0.1,0.04,Form("Recoil: y = %5.1f x + %5.1f",a_ry,b_ry));
               fCanvas2->Update();
               fCanvas2->Modified();
               if (fWaitTime>0) usleep(fWaitTime*100000); // [0.1sec]
            }
         }
      }
      /*--------------------------------------------------------------------------*/
   }
}

TH2Poly* TCatAtRoughEventCounter::DrawPad(Int_t &nPads, Double_t &padX, Double_t &padY) 
{
   Double_t gap = 0.15 / TMath::Sqrt(3) * 2;
   Double_t l = 10. * TMath::Sqrt(3)/2. * 2/3.;
   Double_t r =  (10-gap) * TMath::Sqrt(3)/2. * 2/3.;
   Double_t ls = 5. * TMath::Sqrt(3)/2. * 2/3.;
   Double_t rs = (5-gap) * TMath::Sqrt(3)/2. * 2/3.;
   Int_t iPads = 0;
   Int_t fNumPads = 400;
   TH2Poly *hPad = new TH2Poly("hPad",";x[mm];z[mm]",-50.,50.,-50.,50.);
  
   // large pad
   for (Int_t iCol = 0; iCol < 6; iCol ++) {
      for (Int_t i=0; i<10; i++) {
         Double_t x[4] = { r*cos(-30*deg),
                           r*cos(90*deg),
                           r*cos(210*deg),
                           r*cos(-30.*deg) };
         Double_t y[4] = { r*sin(-30*deg),
                           r*sin(90*deg),
                           r*sin(210*deg),
                           r*sin(-30.*deg) };
         for (Int_t j=0;j<4;j++) {
            padX = (iCol+0.5)*5.-47.5;
            x[j] += padX;
            if ((iCol%2) == 0) {
               if ((i+1)%2) {
                  padY = -l/2.+(l*3.)*(i/2-2.);
                  y[j] = -l/2.-y[j]+(l*3.)*(i/2 - 2.) ;
               } else {
                  padY = l/2.+(l*3.)*(i/2-2.);
                  y[j] = +l/2.+y[j]+(l*3.)*(i/2 - 2.) ;
               }
            } else {
               if ((i+1)%2) {
                  padY = l/2.+(l*3.)*(i/2-2.5);
                  y[j] = +l/2.+y[j]+(l*3.)*(i/2 - 2.5) ;
               } else {
                  padY =-l/2.+(l*3.)*(i/2-1.5);
                  y[j] = -l/2.-y[j]+(l*3.)*(i/2 - 1.5) ;
               }
            }
         }
         Int_t neighbor[3];
         neighbor[0] = iPads-10 > -1 ? iPads-10: -1;
         neighbor[1] = iPads+10 < fNumPads? iPads+10:-1;
         neighbor[2] = -1;
         hPad->AddBin(4,x,y);
         iPads++;
      }
   }
   for (Int_t iCol = 0; iCol < 15; iCol ++) {
      for (Int_t i=0; i<20; i++) {
         Double_t x[4] = { rs*cos(-30*deg),
                           rs*cos(90*deg),
                           rs*cos(210*deg),
                           rs*cos(-30.*deg) };
         Double_t y[4] = { rs*sin(-30*deg),
                           rs*sin(90*deg),
                           rs*sin(210*deg),
                           rs*sin(-30.*deg) };
         if (iCol == 0 || iCol == 14) {
            if ((i%4) == 0 || (i%4) == 3) continue;
         }
         for (Int_t j=0;j<4;j++) {
            padX = 5.*((Double_t)iCol)/2.-17.5;
            x[j] += padX;
            if ((iCol%2) != 0) {
               if ((i+1)%2) {
                  padY = -ls/2.+(ls*3.)*(i/2-4.5);
                  y[j] = -ls/2.-y[j]+(ls*3.)*(i/2 - 4.5) ;
               } else {
                  padY = ls/2.+(ls*3.)*(i/2-4.5);
                  y[j] = +ls/2.+y[j]+(ls*3.)*(i/2 - 4.5) ;
               }
            } else {
               if ((i+1)%2) {
                  padY = ls/2.+(ls*3.)*(i/2-5.);
                  y[j] = +ls/2.+y[j]+(ls*3.)*(i/2 - 5.) ;
               } else {
                  padY =-ls/2.+(ls*3.)*(i/2-4.);
                  y[j] = -ls/2.-y[j]+(ls*3.)*(i/2 - 4.) ;
               }
            }
         }
         Int_t neighbor[3];
         if (iPads < 70 ) {
            neighbor[0] = iPads-10;
            neighbor[1] = iPads + 10 + (iPads%10)  + ((iPads%2)?0:1);
            neighbor[2] = -1;
         } else if (iPads < 90) {
            neighbor[0] = iPads + 20;
            switch ((iPads-70)%4) {
            case 0:
               neighbor[1] = (iPads - 70)/2 + 50;
               break;
            case 1:
               neighbor[1] = (iPads - 71)/2 + 60;
               break;
            case 2:
               neighbor[1] = (iPads - 70)/2 + 60;
               break;
            case 3:
               neighbor[1] = (iPads - 71)/2 + 50;
               break;
            }
            neighbor[2] = -1;
         } else if (iPads >= 330) {
            neighbor[0] = iPads+10;
            neighbor[1] = iPads -20  + (iPads%10) + ((iPads%2)?0:1);
            neighbor[2] = -1;
         } else if (iPads >= 310) {
            neighbor[0] = iPads - 20;
            switch ((iPads-310)%4) {
            case 0:
               neighbor[1] = (iPads - 310)/2 + 340;
               break;
            case 1:
               neighbor[1] = (iPads - 311)/2 + 330;
               break;
            case 2:
               neighbor[1] = (iPads - 310)/2 + 330;
               break;
            case 3:
               neighbor[1] = (iPads - 311)/2 + 340;
               break;
            }
            neighbor[2] = -1;
         } else {
            neighbor[0] = iPads-20 > -1 ? iPads-20: -1;
            neighbor[1] = iPads+20 < fNumPads? iPads+20:-1;
            neighbor[2] = -1;
         }
         hPad->AddBin(4,x,y);
         iPads++;
      }
   }
   for (Int_t iCol = 0; iCol < 6; iCol ++) {
      for (Int_t i=0; i<10; i++) {
         Double_t x[4] = { r*cos(-30*deg),
                           r*cos(90*deg),
                           r*cos(210*deg),
                           r*cos(-30.*deg) };
         Double_t y[4] = { r*sin(-30*deg),
                           r*sin(90*deg),
                           r*sin(210*deg),
                           r*sin(-30.*deg) };
         for (Int_t j=0;j<4;j++) {
            padX = (iCol+0.5)*5.+17.5;
            x[j] += padX;
            if ((iCol%2) != 0) {
               if ((i+1)%2) {
                  padY = -l/2.+(l*3.)*(i/2-2.);
                  y[j] = -l/2.-y[j]+(l*3.)*(i/2 - 2.) ;
               } else {
                  padY = l/2.+(l*3.)*(i/2-2.);
                  y[j] = +l/2.+y[j]+(l*3.)*(i/2 - 2.) ;
               }
            } else {
               if ((i+1)%2) {
                  padY = l/2.+(l*3.)*(i/2-2.5);
                  y[j] = +l/2.+y[j]+(l*3.)*(i/2 - 2.5) ;
               } else {
                  padY =-l/2.+(l*3.)*(i/2-1.5);
                  y[j] = -l/2.-y[j]+(l*3.)*(i/2 - 1.5) ;
               }
            }
         }
         Int_t neighbor[3];
         neighbor[0] = iPads-10;
         neighbor[1] = iPads+10 < fNumPads? iPads+10:-1;
         neighbor[2] = -1;
         hPad->AddBin(4,x,y);
         iPads++;
      }
   }
   printf("created %d pads\n",iPads);
   return hPad;
}
