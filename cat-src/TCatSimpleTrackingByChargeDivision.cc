/**
 * 
 * @file   TCatSimpleTrackingByChargeDivision.cc
 * @date   Created : Aug 25, 2016 10:25:36 JST
 *   Last Modified : 2016-09-01 11:19:14 JST by tokieda
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2016
 */
#include "TCatSimpleTrackingByChargeDivision.h"
#include <TCatPulseShape.h>
#include <TClonesArray.h>
#include <TEventHeader.h>
#include <TROOT.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TH2Poly.h>
#include <stdio.h>
#include <TMath.h>
#include <string>
#include <sstream>
#include <TArtSystemOfUnit.h>
#include <TPaveStats.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <iomanip>
#include <unistd.h>
#include <dirent.h>
using art::TCatSimpleTrackingByChargeDivision;
using TArtSystemOfUnit::deg;
   
ClassImp(TCatSimpleTrackingByChargeDivision)
   
TCatSimpleTrackingByChargeDivision::TCatSimpleTrackingByChargeDivision()
{
  StringVec_t def;
  def.push_back("athitcls");
  RegisterInputCollection("InputCollection","pulse data (TCatPulseShape)",
        		  fInputColName, def);
  StringVec_t def2;
  def2.push_back("atbeamcand");
  RegisterInputCollection("InputBeamCollection","pulse data (TCatPulseShape)",
        		  fInputBeamColName, def2);
  RegisterOutputCollection("OutputCollection","output pulse data (TCatPulseShape)",
        		   fOutputColName,TString("atsmtrack"));
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
  RegisterInputInfo("RunHeadersName","run headers",fNameRunHeaders,TString("runheader"),&fRunHeaders,"TList","TList");
  RegisterInputCollection("EventHeader","name of event header (default : eventheader)",fNameEventHeader,TString("eventheader"),
                             &fEventHeader,art::TEventHeader::Class_Name());
}  
   
TCatSimpleTrackingByChargeDivision::~TCatSimpleTrackingByChargeDivision()
{  
   
   delete fOutput;
   if (fDrawPulse) if (fPulseDrawer) delete fPulseDrawer;
   if (fSaveHist) {
      if (fLSM) delete fhLSMXZDistance;
      if (fLSM) delete fhLSMXYDistance;
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
      if (fLSM) delete fhBeamYRay;
      if (fLSM) delete fhRecoilRay;
      if (fLSM) delete fhRecoilYRay;
   }
}   

TCatSimpleTrackingByChargeDivision& TCatSimpleTrackingByChargeDivision::operator=(const TCatSimpleTrackingByChargeDivision& rhs)
{  
  MayNotUse("operator=");
   if (this != &rhs) {

   }
   return *this;
}



void TCatSimpleTrackingByChargeDivision::Init(TEventCollection *col)
{   
   TEventObject *obj = NULL;
   if (!(obj = col->Get(fInputColName.front()))) {
      Info("Init","No such input %s",fInputColName.front().Data());
      return;
   } 
   TEventObject *objbeam = NULL;
   if (!(objbeam = col->Get(fInputBeamColName.front()))) {
      Info("Init","No such input %s",fInputBeamColName.front().Data());
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
   fInputBeam = (TClonesArray**) objbeam->GetObjectRef();
   fOutput = new TClonesArray("art::TCatPulseShape");
   col->Add(fOutputColName,fOutput,fOutputIsTransparent);
   fOutputBeam = new TClonesArray("art::TCatPulseShape");
   col->Add("atroughbeam",fOutputBeam,fOutputIsTransparent);

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
      if (fLSM) fhLSMXZDistance = new TH1F("hLSMXZDistance","hLSMXZDistance;LSM XZ Distance [mm]",50,0.,50.);
      if (fLSM) fhLSMXYDistance = new TH1F("hLSMXYDistance","hLSMXYDistance;LSM XY Distance [mm]",50,0.,50.);
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
      fCanvas2->SetGrid();
      
      fHPadEvt = DrawPad(fNPad,fPadX,fPadY);
      fHPadEvt->SetName(fHistName);

      if (fLSM) fhBeamYRay = new TF1("hBeamRay","[0]*x+[1]",-50.,50.);
      if (fLSM) fhRecoilRay = new TF1("hRecoilRay","[0]*x+[1]",-50.,50.);
      if (fLSM) fhRecoilYRay = new TF1("hRecoilYRay","[0]*x+[1]",-50.,50.);
      t = new TText(0.1,0.1,"");
   }

   Info("Init","\nLow Pressre : %d\nBeam Mass : %f MeV\nBeam Atomic Mass : %f amu\nBeam Velocity : %f MeV/u",
        fLowPressure,fBeamMass,fBeamAtomicMass,fBeamVelocity);
}

void TCatSimpleTrackingByChargeDivision::Process()
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
   //fOutputRecoil->Clear("C");
   TClonesArray *input = *fInput;
   Int_t nHits = (input)->GetEntriesFast();
   TClonesArray *inputbeam = *fInputBeam;
   Int_t nBeamHits = (inputbeam)->GetEntriesFast();
   std::vector<TCatPulseShape*> hits;
   Int_t cls_i = 0;
   Int_t n   = 0;

   if ((!fLSM) || fDrawFig) {
      fhBeamYHits = new TGraph;
      fhRecoilHits = new TGraph;
      fhRecoilYHits = new TGraph;
      fhBeamYHits->Clear();
      fhRecoilHits->Clear();
      fhRecoilYHits->Clear();
   }
   if (fDrawFig) {
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
   
   TCatPulseShape::SetSortType(TCatPulseShape::kTiming);
   TCatPulseShape::SetSortOrder(TCatPulseShape::kASC);
   (input)->Sort();
   (input)->Compress();
   (inputbeam)->Sort();
   (inputbeam)->Compress();
   
   if (fDebug) if (nHits) fprintf(fp,"-----------\n*Run: %s%04d, Evt = %d, nHits = %3d\n",fRunName.Data(),fRunNumber,fEvt,nHits);
   if (fDetailDebug) if (nHits) printf("[simpletrackingbychargedivision]   Evt = %6d, nHits = %3d\n",fEvt,nHits);
   
   Double_t x0 = 0.;
   Double_t z0 = 0.;
   //*Beam----------------------------------------------------------------------*/

   if (fDebug) fprintf(fp,"\n*Beam :\n");
   for (Int_t iHit = 0; iHit != nBeamHits; iHit++) {
      TCatPulseShape *hit = (TCatPulseShape*) (inputbeam)->At(iHit);
      Double_t z_bx = 0.;
      Double_t y_b  = 0.;
      y_b  = hit->GetOffset() * 20. * (fDriftVelocity / 100.);
      z_bx  = hit->GetX();
      if ((!fLSM) || fDrawFig) fhBeamYHits->SetPoint(n,z_bx,y_b);
      if (fDrawFig) fHPadEvt->Fill(hit->GetX(),hit->GetZ(),hit->GetCharge());
      n++;
   }

   //////if (0) {
   ////Int_t nBeamHits = beam.size();
   ////if (nBeamHits==0) {
   ////   if (fDebug) fprintf(fp,"Skip: No Beam Hits\n");      
   ////   continue;
   ////}
   ////
   ////// Calcurate Beam Average-Offset
   ////if (fDebug) fprintf(fp,"// Calcurate Beam Average-Offset\n");
   ////Float_t sum_off = 0;
   ////it = beam.begin();
   ////itend = beam.end();
   ////for (; it != itend; it++) {
   ////   Int_t id = (*it)->GetID();
   ////   Float_t off = (*it)->GetOffset();
   ////   Int_t col = id / 10;
   ////   Int_t row = id % 10;
   ////   if ((id > 69 && id < 330) && (col%2 == 0)) {
   ////      col = col - 1;
   ////      row = row + 10;
   ////   }
   ////   sum_off += off;
   ////   if (row==0) {
   ////      off0 = off;
   ////   }
   ////}
   ////Float_t BeamOff = sum_off / nBeamHits;
   ////if (fDebug) fprintf(fp,"icls = %d, Beamoff = %6.1f\n",icls,BeamOff);
   /////*--------------------------------------------------------------------------*/
   ////
   ////// Derive Linear Equation of Beam (x,z)
   ////if (fDebug) fprintf(fp,"// Derive Linear Equation of Beam\n");
   ////if (fDebug) fprintf(fp,"// nBeamHitsRow: 2 -> charge division, >2 -> center of charge\n");
   ////Double_t x_b[20],z_b[20],q_b[20];
   ////Double_t sqx[20];
   ////for (Int_t i=0; i<20; i++) {
   ////   x_b[i] = -1000.;
   ////   z_b[i] = -1000.;
   ////   q_b[i] = 0.;
   ////   sqx[i] = 0.;
   ////}
   ////Double_t Q_b = 0.;
   ////Double_t Sx  = 0.;
   ////Double_t Sz  = 0.;
   ////Double_t Sxx = 0.;
   ////Double_t Sxz = 0.;
   ////Double_t a_bz  = 0.;
   ////Double_t b_bz  = 0.;
   ////Double_t p0 =  0.;
   ////Double_t p1 =  0.;
   ////Int_t    n   = 0;
   ////Int_t    first_col = -1;
   ////const Double_t gap = 0.15 / TMath::Sqrt(3) * 2;
   //it = beam.begin();
   //itend = beam.end();
   ////if (fDebug) fprintf(fp,"icls = %d, cls_i = %d:\n",icls,cls_i);
   //TObjArray cls_array;
   //for (; it != itend; it++) {
   //   cls_array.Add((*it));
   //}
   //TCatPulseShape::SetSortType(TCatPulseShape::kID);
   //TCatPulseShape::SetSortOrder(TCatPulseShape::kASC);
   //cls_array.Sort();    
   ////nBeamHits = cls_array.GetEntriesFast();
   ////if (fDebug) fprintf(fp,"cls_ i = %d, nBeamHits = %d\n",cls_i,nBeamHits);
   ////for (Int_t iHit=0; iHit<nBeamHits; iHit++) {
   ////   TCatPulseShape *hit = (TCatPulseShape*) cls_array[iHit];
   ////   Int_t id = hit->GetID();
   ////   Int_t col = id / 10;
   ////   Int_t row = id % 10;
   ////   if ((id > 69 && id < 330) && (col%2 == 0)) {
   ////      col = col - 1;
   ////      row = row + 10;
   ////   }
   ////   Double_t x = hit->GetX();
   ////   Double_t z = hit->GetZ();
   ////   Double_t q = hit->GetCharge();
   ////
   ////
   ////   if (fDebug) fprintf(fp,"cls_i = %2d, id = %3d, row = %2d, nBeamHitsRow[%2d] = %2d\n",cls_i,id,row,row,nBeamHitsRow[row]);
   ////   if (fDetailDebug) printf("cls_i = %2d, id = %3d, row = %2d, nBeamHitsRow[%2d] = %2d\n",cls_i,id,row,row,nBeamHitsRow[row]);
   ////   if (nBeamHitsRow[row]==2) { // Charge Division
   ////      if (x_b[row]==-1000.) {
   ////         first_col = col;
   ////         x_b[row] = x;
   ////         z_b[row] = z;
   ////         q_b[row] = q;
   ////      } else {
   ////         if ((first_col>=6) && (first_col<=33)) { // 5 mm pad
   ////            Double_t z_bh = (5./2 * TMath::Sqrt(3) / 3) / 2;
   ////            Double_t x_bh = (5./2 + gap) / 2;
   ////            if ((first_col%4==3) || (first_col==6) || (first_col==33)) { // first_col = 6,7,11,15,19,23,27,31,33
   ////               if (row%2==0) { /* 1\//\2 */
   ////                  if (fDebug) fprintf(fp,"/* first_col : %2d,row :even, 1\\//\\2 */\n",first_col);
   ////                  x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
   ////                  z_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * z_bh + (z_b[row] - z_bh);
   ////               } else if (row%2==1) { /* 1/\\/2 */
   ////                  if (fDebug) fprintf(fp,"/* first_col : %2d,row : odd, 1/\\\\/2 */\n",first_col);
   ////                  x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
   ////                  z_b[row] = ((q - q_b[row]) / (q + q_b[row])) * z_bh + (z - z_bh);
   ////               }
   ////            } else if ((first_col%4==1) && (first_col!=33)) { // first_col = 9,13,17,21,25,29
   ////               if (row%2==0) { /* 1/\\/2 */
   ////                  if (fDebug) fprintf(fp,"/* first_col : %2d,row :even, 1/\\\\/2 */\n",first_col);
   ////                  x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
   ////                  z_b[row] = ((q - q_b[row]) / (q + q_b[row])) * z_bh + (z - z_bh);
   ////               } else if (row%2==1) { /* 1\//\2 */
   ////                  if (fDebug) fprintf(fp,"/* first_col : %2d,row : odd, 1\\//\\2 */\n",first_col);
   ////                  x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
   ////                  z_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * z_bh + (z_b[row] - z_bh);
   ////               }
   ////            }
   ////         } else if ((first_col<=5) || (first_col>=34)) { // 10 mm pad
   ////            Double_t z_bh = (10./2 * TMath::Sqrt(3) / 3) / 2;
   ////            Double_t x_bh = (10./2 + gap) / 2;
   ////            if ((first_col<=5) && (first_col%2==0)) { // first_col = 0,2,4
   ////               if (row%2==0) { /* 1\//\2 */
   ////                  if (fDebug) fprintf(fp,"/* first_col :even,row :even, 1\\//\\2 */\n");
   ////                  x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
   ////                  z_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * z_bh + (z_b[row] - z_bh);
   ////               } else if (row%2==1) { /* 1/\\/2 */
   ////                  if (fDebug) fprintf(fp,"/* first_col :even,row : odd, 1/\\\\/2 */\n");
   ////                  x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
   ////                  z_b[row] = ((q - q_b[row]) / (q + q_b[row])) * z_bh + (z - z_bh);
   ////               }
   ////            } else if ((first_col<=5) && (first_col%2==1)) { // first_col = 1,3,5
   ////               if (row%2==0) { /* 1/\\/2 */
   ////                  if (fDebug) fprintf(fp,"/* first_col : odd,row :even, 1/\\\\/2 */\n");
   ////                  x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
   ////                  z_b[row] = ((q - q_b[row]) / (q + q_b[row])) * z_bh + (z - z_bh);
   ////               } else if (row%2==1) { /* 1\//\2 */
   ////                  if (fDebug) fprintf(fp,"/* first_col : odd,row : odd, 1\\//\\2 */\n");
   ////                  x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
   ////                  z_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * z_bh + (z_b[row] - z_bh);
   ////               }
   ////            } else if ((first_col>=34) && (first_col%2==1)) { // first_col = 35,37,39
   ////               if (row%2==0) { /* 1\//\2 */
   ////                  if (fDebug) fprintf(fp,"/* first_col :even,row :even, 1\\//\\2 */\n");
   ////                  x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
   ////                  z_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * z_bh + (z_b[row] - z_bh);
   ////               } else if (row%2==1) { /* 1/\\/2 */
   ////                  if (fDebug) fprintf(fp,"/* first_col :even,row : odd, 1/\\\\/2 */\n");
   ////                  x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
   ////                  z_b[row] = ((q - q_b[row]) / (q + q_b[row])) * z_bh + (z - z_bh);
   ////               }
   ////            } else if ((first_col>=34) && (first_col%2==0)) { // first_col = 34,36,38
   ////               if (row%2==0) { /* 1/\\/2 */
   ////                  if (fDebug) fprintf(fp,"/* first_col : odd,row :even, 1/\\\\/2 */\n");
   ////                  x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
   ////                  z_b[row] = ((q - q_b[row]) / (q + q_b[row])) * z_bh + (z - z_bh);
   ////               } else if (row%2==1) { /* 1\//\2 */
   ////                  if (fDebug) fprintf(fp,"/* first_col : odd,row : odd, 1\\//\\2 */\n");
   ////                  x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
   ////                  z_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * z_bh + (z_b[row] - z_bh);
   ////               }
   ////            }
   ////         }
   ////         q_b[row] += q;
   ////      }
   ////
   ////      if (fDetailDebug) fprintf(fp,"x_b[%2d] = %5.1f, z_b[%2d] = %5.1f, q_b[%2d] = %5.1f, x = %5.1f, z = %5.1f, q = %5.1f\n",row,x_b[row],row,z_b[row],row,q_b[row],x,z,q);
   ////
   ////   } else if (nBeamHitsRow[row]>=3) { // Center of Charge
   ////      sqx[row] += q * x;
   ////      z_b[row] =  z; 
   ////      q_b[row] += q;
   ////
   ////      if (fDetailDebug) fprintf(fp,"x_b[%2d] = %5.1f, z_b[%2d] = %5.1f, q_b[%2d] = %5.1f, x = %5.1f, z = %5.1f, q = %5.1f, sqx[%2d] = %5.1f\n",row,x_b[row],row,z_b[row],row,q_b[row],x,z,q,row,sqx[row]);
   ////
   ////   } else if (nBeamHitsRow[row]==1) {
   ////      x_b[row] = x;
   ////      z_b[row] = z;
   ////      q_b[row] = q;
   ////
   ////      if (fDetailDebug) fprintf(fp,"x_b[%2d] = %5.1f, z_b[%2d] = %5.1f, q_b[%2d] = %5.1f, x = %5.1f, z = %5.1f, q = %5.1f\n",row,x_b[row],row,z_b[row],row,q_b[row],x,z,q);
   ////
   ////   }
   ////}
   ////
   ////if (fDebug) fprintf(fp,"\n* Only row = 0,1,18,19 join Beam fitting\n");      
   ////for (Int_t iRow=0; iRow<20; iRow++) {
   ////   if (nBeamHitsRow[iRow]>=3) x_b[iRow] = sqx[iRow] / q_b[iRow];
   ////   if ((x_b[iRow]==-1000.) || (nBeamHitsRow[iRow]<=1)) continue;
   ////   if (!((iRow==0) || (iRow==1) || (iRow==18) || (iRow==19))) continue;
   ////   n++;
   ////   Sx += x_b[iRow];
   ////   Sz += z_b[iRow];
   ////   Sxx += x_b[iRow]*x_b[iRow];
   ////   Sxz += x_b[iRow] * z_b[iRow];
   ////   Q_b += q_b[iRow];
   ////
   ////   //if ((!fLSM) || fDrawFig) fhBeamHits->SetPoint(n-1,x_b[iRow],z_b[iRow]);
   ////
   ////   if (fDetailDebug) fprintf(fp,"x_b[%2d] = %5.1f, z_b[%2d] = %5.1f, q_b[%2d] = %5.1f\n",iRow,x_b[iRow],iRow,z_b[iRow],iRow,q_b[iRow]);
   ////   if (fDetailDebug) fprintf(fp,"Sx = %5.1f, Sz = %5.1f, Sxx = %5.1f, Sxz = %5.1f\n",Sx,Sz,Sxx,Sxz);
   ////}
   ////if (n<=2) {
   ////   if (fDebug) fprintf(fp,"Skip: n <= 2\n");      
   ////   continue;
   ////}
   ////
   ////if (fLSM) {
   ////   if (fDebug) fprintf(fp,"Beam: Least Square Method selected.\n");
   ////   Double_t d   = 0;
   ////   Double_t X_b = 0;
   ////
   ////   d = n * Sxx - Sx * Sx;
   ////   if (d!=0) {
   ////      a_bz= (n * Sxz - Sx * Sz) / d;
   ////      b_bz= (Sxx * Sz - Sxz * Sx) / d;
   ////      X_b = (z_b[0] - b_bz) / a_bz;
   ////   } else {
   ////      a_bz= b_bz= 0;
   ////      X_b = x_b[0];
   ////   }
   ////   if (fDebug) fprintf(fp,"x_b[0] = %5.1f, Q_b = %5.1f, a_bz= %5.1f, b_bz= %5.1f\n",x_b[0],Q_b,a_bz,b_bz);
   ////
   ////   //if (fDrawFig) {
   ////   //   if (a_bz!=0.) {
   ////   //      fhBeamRay->SetParameter(0,a_bz);
   ////   //      fhBeamRay->SetParameter(1,b_bz);
   ////   //   }
   ////   //}
   ////
   ////   if (TMath::Abs(a_bz) >= 4.) {
   ////      BeamIsGood = true;
   ////   } else {
   ////      if (fDebug) fprintf(fp,"Skip: |a_bz| < 4.\n");
   ////      continue;
   ////   }
   ////
   ////   for (Int_t iRow=0; iRow<20; iRow++) {
   ////      if (x_b[iRow]==-1000.) continue;
   ////      if (a_bz==0.) continue;
   ////      Double_t resXa_bz = x_b[iRow] - (z_b[iRow] - b_bz) / a_bz;
   ////      if (fDebug) fprintf(fp,"iRow = %2d, x_b[%2d] = %5.1f, resXa_bz = %5.1f\n",iRow,iRow,x_b[iRow],resXa_bz);
   ////   }
   ////} else {
   ////   if (fDebug) fprintf(fp,"Beam: TGraph pol1 Fitting selected.\n");
   ////
   ////   Double_t chi2 = 0.;
   ////   if (fDetailDebug) printf("\n Evt = %d, cls = %d, n = %d",fEvt,icls,n);
   ////   //fhBeamHits->Fit("pol1");
   ////   p0 = fhBeamHits->GetFunction("pol1")->GetParameter(0);
   ////   p1 = fhBeamHits->GetFunction("pol1")->GetParameter(1);
   ////   chi2 = fhBeamHits->GetFunction("pol1")->GetChisquare();
   ////   if (fDebug) fprintf(fp,"p0 = %5.1f, p1 = %5.1f, chi2 = %5.1f\n",p0,p1,chi2);
   ////   if (TMath::Abs(p0) >= 4.) {
   ////      BeamIsGood = true;
   ////   } else {
   ////      if (fDebug) fprintf(fp,"Skip: |p0| < 4.\n");
   ////      continue;
   ////   }
   ////
   ////   for (Int_t iRow=0; iRow<20; iRow++) {
   ////      if (x_b[iRow]==-1000.) continue;
   ////      Double_t resXp0 = x_b[iRow] - (z_b[iRow] - p1) / p0;
   ////      if (fDebug) fprintf(fp,"iRow = %2d, x_b[%2d] = %5.1f, resXp0 = %5.1f\n",iRow,iRow,x_b[iRow],resXp0);
   ////   }
   ////}
   ////z0 = -40.;
   ////x0 = (z0 - b_bz) / a_bz;
   /////*--------------------------------------------------------------------------*/
   //
   //// Derive Linear Equation of Beam (z,y)
   //if (fDebug) fprintf(fp,"// Derive Linear Equation of Beam (z,y)\n");
   ////if (fDebug) fprintf(fp,"Only row = 0,1,18,19 join Beam fitting\n");      
   ////Double_t y0   = 0.;
   ////Double_t z_by = 0.;
   //Double_t z_bx = 0.;
   //Double_t y_b  = 0.;
   ////Double_t Sy   = 0.;
   ////Double_t Szy  = 0.;
   ////Double_t a_by = 0.;
   ////Double_t b_by = 0.;
   ////Double_t Szz  = 0.;
   ////Sz  = 0.;
   //Int_t n   = 0;
   ////
   //nBeamHits = cls_array.GetEntriesFast();
   //if (fDebug) fprintf(fp,"nBeamHits = %d\n",nBeamHits);
   //for (Int_t iHit=0; iHit<nBeamHits; iHit++) {
   //   TCatPulseShape *hit = (TCatPulseShape*) cls_array[iHit];
   //   //   Int_t id = hit->GetID();
   //   //   Int_t col = id / 10;
   //   //   Int_t row = id % 10;
   //   //   if ((id > 69 && id < 330) && (col%2 == 0)) {
   //   //      col = col - 1;
   //   //      row = row + 10;
   //   //   }
   //   Double_t x = hit->GetX();
   //   //   Double_t z = hit->GetZ();
   //   //   Double_t q = hit->GetCharge();
   //   Float_t  off = hit->GetOffset();
   //   //   Float_t  diffoff = off - BeamOff;
   //   //
   //   //   if (!(TMath::Abs(z)>=35.)) continue;
   //   //
   //   // 0.70 cm/us = 0.70 x 10^(-1) mm/ns @ phys1802
   //   //y_b  = (off - BeamOff) * 20. * (fDriftVelocity / 100.);
   //   y_b  = off * 20. * (fDriftVelocity / 100.);
   //   //   z_by  = z;
   //   z_bx  = x;
   //   //
   //   //   if (fDetailDebug) fprintf(fp,"iHit  = %2d, id  = %3d, x = %5.1f, z = %5.1f, off  = %5.1f, y_b = %5.1f, q = %6.1f, diffoff  = %4.1f\n",iHit,id,x,z,off,y_b,q,diffoff);
   //   //
   //   //   if ((z_by==0.) || (y_b==0.)) continue;
   //   n++;
   //   //   Sz += z_by;
   //   //   Sy += y_b;
   //   //   Szz += z_by * z_by;
   //   //   Szy += z_by * y_b;
   //   //if ((!fLSM) || fDrawFig) fhBeamYHits->SetPoint(n-1,z_by,y_b);
   //   //if ((!fLSM) || fDrawFig) fhBeamYHits->SetPoint(n,z_by,y_b);
   //   if ((!fLSM) || fDrawFig) fhBeamYHits->SetPoint(n,z_bx,y_b);
   //   //if (fDetailDebug) fprintf(fp,"z_by = %5.1f, y_b = %6.1f\n",z_by,y_b);
   //   if (fDetailDebug) fprintf(fp,"z_by = %5.1f, y_b = %6.1f\n",z_bx,y_b);
   //}
   ////
   ////if (fLSM) {
   ////   if (fDebug) fprintf(fp,"Beam: Least Square Method selected.\n");
   ////   Double_t d   = 0;
   ////
   ////   d = n * Szz - Sz * Sz;
   ////   if (d!=0) {
   ////      a_by = (n * Szy - Sz * Sy) / d;
   ////      b_by = (Szz * Sy - Szy * Sz) / d;
   ////   } else {
   ////      a_by = b_by = 0;
   ////   }
   ////   if (fDebug) fprintf(fp,"a_by = %5.1f, b_by = %5.1f\n",a_by,b_by);
   ////
   ////   if (fDrawFig) {
   ////      if (a_by!=0.) {
   ////         fhBeamYRay->SetParameter(0,a_by);
   ////         fhBeamYRay->SetParameter(1,b_by);
   ////      }
   ////   }
   ////
   ////   if (TMath::Abs(a_by) != 0.) {
   ////      BeamIsGood = true;
   ////   } else {
   ////      if (fDebug) fprintf(fp,"Skip: |a_by| = 0.\n");
   ////      BeamIsGood = false;
   ////      continue;
   ////   }
   ////}
   ////y0 = a_by * z0 + b_by;
   //BeamIsGood = true;
   
   /*Recoil--------------------------------------------------------------------*/
   if (fDebug) fprintf(fp,"\n*Recoil :\n");

   for (Int_t iHit = 0; iHit != nHits; iHit++) {
      TCatPulseShape *hit = (TCatPulseShape*) (input)->At(iHit);

      if (fDrawFig) fHPadEvt->Fill(hit->GetX(),hit->GetZ(),hit->GetCharge());

      Double_t z_bx = 0.;
      Double_t y_b  = 0.;
      y_b  = hit->GetOffset() * 20. * (fDriftVelocity / 100.);
      z_bx  = hit->GetX();
      if ((!fLSM) || fDrawFig) fhBeamYHits->SetPoint(n,z_bx,y_b);
      n++;

      if (TMath::Abs(hit->GetX())<20.) continue;
      hits.push_back(hit);
   }

   Int_t nRecoilHits = hits.size();
   if (nRecoilHits==0) {
      if (fDebug) fprintf(fp,"Skip: No Recoil Candidate Hits\n");      
      //continue;
      return;
   }
   
   if (fDebug) fprintf(fp,"recoil %d hits:\n",nRecoilHits);

   TObjArray recoil_array;
   std::vector<TCatPulseShape*>::iterator it = hits.begin();
   std::vector<TCatPulseShape*>::iterator itend = hits.end();
   recoil_array.Clear();
   for (; it != itend; it++) {
      recoil_array.Add((*it));
   }

   // Derive Linear Equation of Recoil (x,z)
   if (fDebug) fprintf(fp,"// Derive Linear Equation of Recoil (x,z)\n");
   //nRecoilHits = recoil_array.GetEntriesFast();

   Bool_t RecoilIsGood = false;
   n   = 0;
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
      //continue;
      return;
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
         //continue;
         return;
      }

      Bool_t LSMIsBad = false;
      for (Int_t iHit=0; iHit < nRecoilHits; iHit++) {
         TCatPulseShape *hit = (TCatPulseShape*) recoil_array[iHit];
         Double_t x = hit->GetX();
         Double_t z = hit->GetZ();
         // Calcurate distance from point to line
         Double_t distance = TMath::Abs(a_rz * x - z + b_rz) / TMath::Sqrt(a_rz * a_rz + 1);
         if (fSaveHist) fhLSMXZDistance->Fill(distance);
         if (distance > 10.) LSMIsBad = true;
      }
#if 0
      if (LSMIsBad) {
         if (fDebug) fprintf(fp,"Skip: LSM XZ distance is not good\n"); 
         return;
      }
#endif
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
         //continue;
         return;
      }

      Bool_t LSMIsBad = false;
      for (Int_t iHit=0; iHit < nRecoilHits; iHit++) {
         TCatPulseShape *hit = (TCatPulseShape*) recoil_array[iHit];
         Double_t x = hit->GetX();
         Float_t  off = hit->GetOffset();
         Double_t y = off * 20. * (fDriftVelocity / 100.);
         // Calcurate distance from point to line
         Double_t distance = TMath::Abs(a_ry * x - y + b_ry) / TMath::Sqrt(a_ry * a_ry + 1);
         if (fSaveHist) fhLSMXYDistance->Fill(distance);
         if (distance > 10.) LSMIsBad = true;
      }
#if 0
      if (LSMIsBad) {
         if (fDebug) fprintf(fp,"Skip: LSM XY distance is not good\n"); 
         return;
      }
#endif
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
      //continue;
      return;
   }
   /*--------------------------------------------------------------------------*/
   
   Double_t max_length = 0;
   Double_t max_X = 0;
   Double_t max_X_off = 0;
   Bool_t IsPenetrating = false;
   Double_t BeamX = 0;
   for (Int_t iHit=0; iHit < nRecoilHits; iHit++) {
      TCatPulseShape *hit = (TCatPulseShape*) recoil_array[iHit];
      Double_t x = hit->GetX();
      Double_t z = hit->GetZ();
      Double_t q = hit->GetCharge();
      Float_t  off = hit->GetOffset();
      if (fDebug) fprintf(fp,"id = %3d, off = %6.1f, x = %5.1f, z = %5.1f, q = %5.1f\n",hit->GetID(),off,x,z,q);
      //if (fLSM) {
      //   BeamX = (z - b_bz) / a_bz;
      //} else {
      //   BeamX = (z - p1) / p0;
      //}
      Double_t lx = TMath::Sqrt((x - BeamX)*(x - BeamX));
      if (max_length < lx) {
         max_length = lx;
         max_X = x;
         max_X_off = off;
      }
      if (TMath::Abs(x)>=40.) IsPenetrating = true;
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
   std::vector<TCatPulseShape*> beam;
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
   
   //if (fDebug) fprintf(fp,"Results: cls[%d] size = %zd -> %zd\n",icls,cls[icls].size(),event.size());
   //if (fDetailDebug) printf("Results: cls[%d] size = %zd -> %zd\n",icls,cls[icls].size(),event.size());
   /*--------------------------------------------------------------------------*/
    
   //Bool_t IsPenetrating = false;
   //Bool_t BeamIsGood = false;
   //Bool_t IsGood = false;
   //Bool_t IsGoodBeamEvent = false;
   Bool_t IsGoodBeamEvent = true;
   //switch(fSelectCondition) {
   //case 0: // Recoil Range >= SetRecoilRange, and Recoil is penetrating reading pad
   //   if (fDebug) fprintf(fp,"case 0 was selected\n");
   //   if (fDetailDebug) printf("case 0 was selected\n");
   //   if ((BeamIsGood) && (recoil_range>=SetRecoilRange) && (IsPenetrating)) IsGood = true;
   //   break;
   //case 1: // Recoil Range >= SetRecoilRange, and Recoil is NOT penetrating reading pad
   //   if (fDebug) fprintf(fp,"case 1 was selected\n");
   //   if (fDetailDebug) printf("case 1 was selected\n");
   //   if ((BeamIsGood) && (recoil_range>=SetRecoilRange) && (!IsPenetrating)) IsGood = true;
   //   break;
   //case 2: // Recoil Range >= SetRecoilRange
   //   if (fDebug) fprintf(fp,"case 2 was selected\n");
   //   if (fDetailDebug) printf("case 2 was selected\n");
   //   if ((BeamIsGood) && (recoil_range>=SetRecoilRange) && (RecoilIsGood)) IsGood = true;
   //   break;
   //case 3: // Recoil Range >= 5.0 mm
   //   if (fDebug) fprintf(fp,"case 3 was selected\n");
   //   if (fDetailDebug) printf("case 3 was selected\n");
   //   if ((BeamIsGood) && (recoil_range>=5.)) IsGood = true;
   //   break;
   //case 4: // Good Beam Event
   //   if (fDebug) fprintf(fp,"case 3 was selected\n");
   //   if (fDetailDebug) printf("case 3 was selected\n");
   //   if (BeamIsGood) IsGood = true;
   //   IsGoodBeamEvent = true;
   //   break;
   //default: // Not Good Event
   //   if (fDebug) fprintf(fp,"Not Good Event, No Save\n");
   //   if (fDetailDebug) printf("Not Good Event, No Save\n");
   //   break;
   //}
   //if (!IsGood) continue;
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
   
   if (!((angle>=fAngle_lowth) && (angle<=fAngle_hith))) return; //continue;
   
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
         //if (fDebug) fprintf(fp,"Save event = beam + recoil, icls = %d, cls_i = %d\n",icls,cls_i);
         //if (fDetailDebug) printf("Save event = beam + recoil, icls = %d, cls_i = %d\n",icls,cls_i);
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

         //// Save only Recoil hits
         //for (Int_t iHit=0; iHit < nRecoilHits; iHit++) {
         //   TCatPulseShape *hit = (TCatPulseShape*) recoil_array[iHit];
         //   TCatPulseShape *out = (TCatPulseShape*) fOutputRecoil->ConstructedAt(fOutputRecoil->GetEntriesFast());
         //   hit->Copy(*out);
         //  
         //   Int_t num = 0;
         //   num = hit->GetNumSample();
         //   out->GetSample().resize(num);
         //   out->GetClock().resize(num);
         //   std::copy(hit->GetSample().begin(),hit->GetSample().end(),out->GetSample().begin());
         //   for (Int_t i=0; i != num; i++) {
         //      out->GetClock()[i] = (Double_t)i;
         //   }
         //   out->SetNumSample(num);
         //}
      //} else if ((fShowRecoil) || (fShowBeam)) {
      //   if (!fShowBeam) {
      //      // Save only recoil hits
      //      //if (fDebug) fprintf(fp,"Save recoil, icls = %d, cls_i = %d\n",icls,cls_i);
      //      //if (fDetailDebug) printf("Save recoil, icls = %d, cls_i = %d\n",icls,cls_i);
      //      for (Int_t iHit=0; iHit < nRecoilHits; iHit++) {
      //         TCatPulseShape *hit = (TCatPulseShape*) recoil_array[iHit];
      //         TCatPulseShape *out = (TCatPulseShape*) fOutputRecoil->ConstructedAt(fOutputRecoil->GetEntriesFast());
      //         hit->Copy(*out);
      //          
      //         Int_t num = 0;
      //         num = hit->GetNumSample();
      //         out->GetSample().resize(num);
      //         out->GetClock().resize(num);
      //         std::copy(hit->GetSample().begin(),hit->GetSample().end(),out->GetSample().begin());
      //         for (Int_t i=0; i != num; i++) {
      //            out->GetClock()[i] = (Double_t)i;
      //         }
      //         out->SetNumSample(num);
      //          
      //         if (fDrawPulse) {
      //            TObject *hitobj = out;
      //            fPulseDrawer->SetPulse(*hitobj);
      //            fPulseDrawer->SetEvent(fEvt,iHit);
      //            fPulseDrawer->SetGraphRange(0.,100.,-2.,60.);
      //            //fPulseDrawer->SetViewID(fSaveID);
      //            fPulseDrawer->SetTextNDC(Form("Evt = %5d, id = %3d, offset = %5.1f, cls = %2d",fEvt,out->GetID(),out->GetOffset(),cls_i),1);
      //            fPulseDrawer->SetTextNDC(Form("nHits = %3d, iHit = %3d, nSample = %3d, charge = %5.2f",nHits,iHit,num,out->GetCharge()),5,0.10,0.91);
      //            fPulseDrawer->SetTextNDC(GetName(),5,0.10,0.01);
      //            fPulseDrawer->Draw();
      //         }
      //      }
      //   } else if (!fShowRecoil) {
      //      // Save only beam hits
      //      //if (fDebug) fprintf(fp,"Save beam, icls = %d, cls_i = %d\n",icls,cls_i);
      //      //if (fDetailDebug) printf("Save beam, icls = %d, cls_i = %d\n",icls,cls_i);
      //      it    = beam.begin();
      //      itend = beam.end();
      //      for (; it != itend; it++) {
      //         TCatPulseShape *out = (TCatPulseShape*) fOutputBeam->ConstructedAt(fOutputBeam->GetEntriesFast());
      //         (*it)->Copy(*out);
      //          
      //         Int_t num = 0;
      //         num = (*it)->GetNumSample();
      //         out->GetSample().resize(num);
      //         out->GetClock().resize(num);
      //         std::copy((*it)->GetSample().begin(),(*it)->GetSample().end(),out->GetSample().begin());
      //         for (Int_t i=0; i != num; i++) {
      //            out->GetClock()[i] = (Double_t)i;
      //         }
      //         out->SetNumSample(num);
      //          
      //         if (fDrawPulse) {
      //            TObject *hitobj = out;
      //            fPulseDrawer->SetPulse(*hitobj);
      //            Int_t iHit = std::distance(beam.begin(),it);
      //            fPulseDrawer->SetEvent(fEvt,iHit);
      //            fPulseDrawer->SetGraphRange(0.,100.,-2.,60.);
      //            //fPulseDrawer->SetViewID(fSaveID);
      //            fPulseDrawer->SetTextNDC(Form("Evt = %5d, id = %3d, offset = %5.1f, cls = %2d",fEvt,out->GetID(),out->GetOffset(),cls_i),1);
      //            fPulseDrawer->SetTextNDC(Form("nHits = %3d, iHit = %3d, nSample = %3d, charge = %5.2f",nHits,iHit,num,out->GetCharge()),5,0.10,0.91);
      //            fPulseDrawer->SetTextNDC(GetName(),5,0.10,0.01);
      //            fPulseDrawer->Draw();
      //         }
      //      }
      //   }
      }
   //} else { // Good Beam Event
   //   //if (fDebug) fprintf(fp,"Save Good Beam Event, icls = %d, cls_i = %d\n",icls,cls_i);
   //   //if (fDetailDebug) printf("Save Good Beam Event, icls = %d, cls_i = %d\n",icls,cls_i);
   //   //it    = cls[icls].begin();
   //   //itend = cls[icls].end();
   //   it    = cls.begin();
   //   itend = cls.end();
   //   for (; it != itend; it++) {
   //      TCatPulseShape *out = (TCatPulseShape*) fOutput->ConstructedAt(fOutput->GetEntriesFast());
   //      (*it)->Copy(*out);
   //       
   //      Int_t num = 0;
   //      num = (*it)->GetNumSample();
   //      out->GetSample().resize(num);
   //      out->GetClock().resize(num);
   //      std::copy((*it)->GetSample().begin(),(*it)->GetSample().end(),out->GetSample().begin());
   //      for (Int_t i=0; i != num; i++) {
   //         out->GetClock()[i] = (Double_t)i;
   //      }
   //      out->SetNumSample(num);
   //   }
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
                  //t->DrawTextNDC(0.01,0.04,Form("Vertex: (x,z,y) = (%5.1f,%5.1f,%5.1f), theta_lab = %5.1f deg.",x_v,z_v,y_v,angle));
                  //t->DrawTextNDC(0.01,0.01,Form("Recoil %2d hits: range = %5.1f mm, Ex = %5.1f MeV",nRecoilHits,recoil_range,Ex));
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
               //t->DrawTextNDC(0.01,0.04,Form("Vertex: (x,z,y) = (%5.1f,%5.1f,%5.1f), theta_lab = %5.1f deg.",x_v,z_v,y_v,angle));
               //t->DrawTextNDC(0.01,0.01,Form("Recoil %2d hits: range = %5.1f mm, Ex = %5.1f MeV",nRecoilHits,recoil_range,Ex));
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
               //t->DrawTextNDC(0.01,0.04,Form("Vertex: (x,z,y) = (%5.1f,%5.1f,%5.1f), theta_lab = %5.1f deg.",x_v,z_v,y_v,angle));
               //t->DrawTextNDC(0.01,0.01,Form("Recoil %2d hits: range = %5.1f mm, Ex = %5.1f MeV",nRecoilHits,recoil_range,Ex));
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
            //t->DrawTextNDC(0.01,0.04,Form("Vertex: (x,z,y) = (%5.1f,%5.1f,%5.1f), theta_lab = %5.1f deg.",x_v,z_v,y_v,angle));
            //t->DrawTextNDC(0.01,0.01,Form("Recoil %2d hits: range = %5.1f mm, Ex = %5.1f MeV",nRecoilHits,recoil_range,Ex));
            fCanvas->Update();
            fCanvas->Modified();
            if (fWaitTime>0) usleep(fWaitTime*100000); // [0.1sec]
         }
      }
   }
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

TH2Poly* TCatSimpleTrackingByChargeDivision::DrawPad(Int_t &nPads, Double_t &padX, Double_t &padY) 
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
