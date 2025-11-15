/**
 * @file   TCatAtClusteringProcessor.cc
 * @brief  clustering for tracking detector
 *
 * @date   Created       : 2015-12-18 11:35:15 JST
 *         Last Modified : 2016-08-31 21:16:27 JST by tokieda
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *
 *    
 */

#include "TCatAtClusteringProcessor.h"
#include <TCatPulseShape.h>
#include <TClonesArray.h>
#include <TEventHeader.h>
#include <TROOT.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <stdio.h>
#include <TMath.h>
#include <string>
#include <sstream>
#include <unistd.h>

using art::TCatAtClusteringProcessor;

ClassImp(TCatAtClusteringProcessor)

TCatAtClusteringProcessor::TCatAtClusteringProcessor()
{
  StringVec_t def;
  def.push_back("atevt");
  RegisterInputCollection("InputCollection","pulse data (TCatPulseShape)",
			  fInputColName, def);
  RegisterOutputCollection("OutputCollection","output pulse data (TCatPulseShape)",
			   fOutputColName,TString("atcls"));
  RegisterOutputCollection("OutputBeamCollection","output pulse data (TCatPulseShape)",
			   fOutputBeamColName,TString("atevtselbeam"));
  RegisterOutputCollection("OutputExcludeEvtCollection","output pulse data (TCatPulseShape)",
			   fOutputExcludeEvtColName,TString("atexcludeevt2"));
  RegisterProcessorParameter("LogFilePath","Path of Log File",fLogFilePath,TString("../log"));
  RegisterProcessorParameter("LogFileName","Name of Log File",fLogFileName,TString("clusterproc.log"));
  RegisterProcessorParameter("DebugMode","Flag For Debug",fDebug,false);
  RegisterProcessorParameter("DetailDebugMode","Flag For Detail Debug",fDetailDebug,false);
  RegisterProcessorParameter("HistSaveFlag","Flag For Histgram Save",fSaveHist,false);
  RegisterProcessorParameter("SavePulse","flag to save pulse",fSavePulse,kFALSE);
  RegisterProcessorParameter("DrawPulseFlag","Flag of Draw Pulse",fDrawPulse,false);
  RegisterProcessorParameter("WaitTime","Waiting time (ms)",fWaitTime,200000);
  RegisterProcessorParameter("SaveFigFlag","Flag of Save Figure",fSaveFig,false);
  RegisterProcessorParameter("FigFilePath","Path of Figure File",fFigFilePath,TString("./"));
  RegisterProcessorParameter("AddFigFilePath","Add Path of Figure File",fAddFigFilePath,TString("./"));
  RegisterProcessorParameter("SaveEvtNum","Number of Save Event",fSaveEvt,-1);
  RegisterProcessorParameter("CanvasX","X Position of Canvas",fCanvasX,(Int_t)0);
  RegisterProcessorParameter("CanvasY","Y Position of Canvas",fCanvasY,(Int_t)0);
  RegisterProcessorParameter("CanvasW","Width of Canvas",fCanvasW,(Int_t)1000);
  RegisterProcessorParameter("CanvasH","Hight of Canvas",fCanvasH,(Int_t)1050);
  RegisterProcessorParameter("SetCutHits","Set Hits to cut",fCutHits,(Int_t)3.);
  RegisterOptionalParameter("UseAllCluster","Use all the  clusters without choosing maximum length cluster when true (default false)",
                            fUseAllCluster,false);
  RegisterProcessorParameter("MinNHits","Min # of Hits",fMinNHits,(Int_t)0);
  RegisterProcessorParameter("ExcludeEvtSaveMode","Flag For Saving Excluding Events",fSaveExcludeEvt,false);
  RegisterInputInfo("RunHeadersName","run headers",fNameRunHeaders,TString("runheader"),&fRunHeaders,"TList","TList");
  RegisterInputCollection("EventHeader","name of event header (default : eventheader)",fNameEventHeader,TString("eventheader"),
                          &fEventHeader,art::TEventHeader::Class_Name());
}

TCatAtClusteringProcessor::~TCatAtClusteringProcessor()
{
  delete fOutput;
  delete fOutputBeam;
  if (fSaveExcludeEvt) delete fExcludeEvtOutput;
  if (fSaveHist) {
     delete fhNumOfCluster;
     delete fhClusterSize;
     delete fhClusterQ;
  }
  if (fDrawPulse) if (fPulseDrawer) delete fPulseDrawer;
}

TCatAtClusteringProcessor::TCatAtClusteringProcessor(const TCatAtClusteringProcessor& rhs)
{
  MayNotUse("TCatAtClusteringProcessor");
}

TCatAtClusteringProcessor& TCatAtClusteringProcessor::operator=(const TCatAtClusteringProcessor& rhs)
{
  MayNotUse("operator=");
  if (this != &rhs) {

  }
  return *this;
}

void TCatAtClusteringProcessor::Init(TEventCollection *col)
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
  col->Add(fOutputBeamColName,fOutputBeam,fOutputIsTransparent);
  if (fSaveExcludeEvt) {
    fExcludeEvtOutput = new TClonesArray("art::TCatPulseShape");
    col->Add(fOutputExcludeEvtColName,fExcludeEvtOutput,fOutputIsTransparent);
  }

  if (fSaveHist) {
     fhNumOfCluster = new TH1F("hNumOfCluster","hNumOfCluster",10,0.5,10.5);
     fhClusterSize = new TH1F("hClusterSize","hClusterSize",30,0.5,30.5);
     fhClusterQ = new TH1F("hClusterQ","hClusterQ",100,0.,1e+4);
  }

  // Prepare Figure and Histgram
  if (fDrawPulse) {
     fFigFile = Form("%s/%s/%s",fFigFilePath.Data(),fOutputColName.Data(),fAddFigFilePath.Data());
     fPulseDrawer = new TPulseDrawer(GetName(),GetName(),fCanvasX,fCanvasY,fCanvasW,fCanvasH);
     fPulseDrawer->SetOptions(fWaitTime,fSaveEvt,fSaveFig,fFigFile);
     fPulseDrawer->SetRunHeader(fRunHeaders);
  }

  gStyle->SetOptStat(111111);
  gStyle->SetStatX(0.900);
  gStyle->SetStatY(0.980);
  gStyle->SetStatH(0.159);
  gStyle->SetStatW(0.150);
  gStyle->SetTitleYOffset(1.5);
}

void TCatAtClusteringProcessor::Process()
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
   if (fSaveExcludeEvt) fExcludeEvtOutput->Clear("C");
   TClonesArray *input = *fInput;
   Int_t nHits = (input)->GetEntriesFast();
   Int_t ncls = 0;

   TCatPulseShape::SetSortType(TCatPulseShape::kTiming);
   TCatPulseShape::SetSortOrder(TCatPulseShape::kASC);
   (input)->Sort();
   (input)->Compress();
  
   if (fDebug) if (nHits) fprintf(fp,"-----------\n*Run: %s%04d, Evt = %d, nHits = %3d\n",fRunName.Data(),fRunNumber,fEvt,nHits);
   if (fDetailDebug) if (nHits) printf("-----------\n[cluster]   Evt = %6d, nHits = %3d\n",fEvt,nHits);

   /*1st-----------------------------------------------------------------------*/

   std::vector<std::vector<TCatPulseShape*> > cls;
   std::vector<TCatPulseShape*> icls;
   Int_t nCls = 0;
   for (Int_t iHit1=0; iHit1 < nHits; iHit1++) {
      TCatPulseShape *hit1 = (TCatPulseShape*) (*input)[iHit1];
      if (!(hit1->IsValid())) continue;
      if (!hit1) {
         if (fDetailDebug) fprintf(fp,"Skip: No Object iHit1 = %d\n",iHit1);
         if (fDetailDebug) printf("Skip: No Object iHit1 = %d\n",iHit1);
         continue;
      }
      Int_t id1 = hit1->GetID();
      Double_t x1 = hit1->GetX();
      Double_t z1 = hit1->GetZ();
      Float_t off1 = hit1->GetOffset();
      if (fDetailDebug) fprintf(fp,"iHit1 = %d, id1 = %3d, x1 = %4.1f, z1 = %4.1f, off1 = %6.1f\n",iHit1,id1,x1,z1,off1);
      if (fDetailDebug) printf("iHit1 = %d, id1 = %3d, x1 = %4.1f, z1 = %4.1f, off1 = %6.1f\n",iHit1,id1,x1,z1,off1);

      if (iHit1 != nHits-1) { // iHit1 != last hit
         Int_t iHit2 = iHit1 + 1;
         TCatPulseShape *hit2 = (TCatPulseShape*) (*input)[iHit2];
         if (!(hit2->IsValid())) continue;
         if (!hit2) {
            if (fDetailDebug) fprintf(fp,"Skip: No Object iHit2 = %d\n",iHit2);
            if (fDetailDebug) printf("Skip: No Object iHit2 = %d\n",iHit2);
            continue;
         }
         Int_t id2 = hit2->GetID();
         Double_t x2 = hit2->GetX();
         Double_t z2 = hit2->GetZ();
         Float_t off2 = hit2->GetOffset();
         if (fDetailDebug) fprintf(fp,"iHit2 = %d, id2 = %3d, x2 = %4.1f, z2 = %4.1f, off2 = %6.1f\n",iHit2,id2,x2,z2,off2);
         if (fDetailDebug) printf("iHit1 = %d, iHit2 = %d, id2 = %3d, x2 = %4.1f, z2 = %4.1f, off2 = %6.1f\n",iHit1,iHit2,id2,x2,z2,off2);

         Double_t distance = TMath::Sqrt((x1-x2)*(x1-x2)+(z1-z2)*(z1-z2));
         Float_t  diffoff  = off1 - off2;
         if (fDetailDebug) fprintf(fp,"distance = %6.1f, diffoff = %6.1f\n",distance,diffoff);
         if (fDetailDebug) printf("distance = %6.1f, diffoff = %6.1f\n",distance,diffoff);

         if ((TMath::Abs(distance) < 13.) && (TMath::Abs(diffoff) <= 50.)) {
            icls.push_back(hit1);
            icls.push_back(hit2);
         } else {
            icls.erase(unique(icls.begin(),icls.end()),icls.end());
            Int_t nicls = icls.size();
            if (nicls==0) icls.push_back(hit1);
            cls.push_back(icls);
            icls.clear();
            nCls++;
         }
      } else { // iHit = last hit
         icls.push_back(hit1);
         icls.erase(unique(icls.begin(),icls.end()),icls.end());
         cls.push_back(icls);
      }
   }
   if (nHits) ncls = nCls + 1;

   if (ncls && fDebug) {
      if (fDebug) fprintf(fp,"\n*First Results");
      if (fDetailDebug) printf("\n*First Results");
      fprintf(fp,"\nncls = %d: \n",ncls);
      if (fDetailDebug) printf("\nncls = %d: \n",ncls);
      for (Int_t i=0; i<ncls; i++) {
         Int_t nicls = cls[i].size();
         fprintf(fp,"\n***i = %d: \n",i);
         fprintf(fp,"nicls = %d: \n",nicls);
         if (fDetailDebug) printf("\n***i = %d:\n",i);
         if (fDetailDebug) printf("nicls = %d: \n",nicls);
         std::vector<TCatPulseShape*>::iterator iclsit = cls[i].begin();
         std::vector<TCatPulseShape*>::iterator iclsitend = cls[i].end();
         for (; iclsit != iclsitend; iclsit++) {
            Int_t id = (*iclsit)->GetID();
            Float_t off = (*iclsit)->GetOffset();
            fprintf(fp,"id = %3d, off = %4.0f\n",id,off);
            if (fDetailDebug) printf("id = %3d, off = %4.0f\n",id,off);
         }
      }
   }
   /*2nd-----------------------------------------------------------------------*/
  
   if (nHits && fDebug) fprintf(fp,"\n*2nd");
   if (nHits && fDetailDebug) printf("\n*2nd");
   Bool_t fAddCls[ncls];
   for (Int_t i=0; i<ncls; i++) {
      std::sort(cls[i].begin(),cls[i].end());
      cls[i].erase(unique(cls[i].begin(),cls[i].end()),cls[i].end());
      if (fDebug) fprintf(fp,"\n*** i = %3d, cls[%d] = %3zd\n",i,i,cls[i].size());
      if (fDetailDebug) printf("\n*** i = %3d, cls[%d] = %3zd\n",i,i,cls[i].size());
      Bool_t fAdd = false;
      for (Int_t j=0; j<ncls; j++) {
         fAddCls[j] = false;
      }
      std::vector<TCatPulseShape*>::iterator it1 = cls[i].begin();
      std::vector<TCatPulseShape*>::iterator it1end = cls[i].end();
      Int_t nicls = cls[i].size();
      if (fDebug) {
         fprintf(fp,"cls[%d] %3d hits : \n",i,nicls);
         for (; it1 != it1end; it1++) {
            Int_t id1 = (*it1)->GetID();
            Float_t off1 = (*it1)->GetOffset();
            fprintf(fp,"id1 = %3d, off1 = %4.0f\n",id1,off1);
         }
      }
      if (fDetailDebug) {
         printf("cls[%d] %3d hits : \n",i,nicls);
         for (; it1 != it1end; it1++) {
            Int_t id1 = (*it1)->GetID();
            Float_t off1 = (*it1)->GetOffset();
            printf("id1 = %3d, off1 = %4.0f\n",id1,off1);
         }
      }
      for (Int_t j=0; j<ncls; j++) {
         std::sort(cls[j].begin(),cls[j].end());
         cls[j].erase(unique(cls[j].begin(),cls[j].end()),cls[j].end());
         icls.clear();
         if (i<j) {
            if (fDebug) fprintf(fp,"(i,j) = (%d,%d):\n",i,j);
            it1 = cls[i].begin();
            it1end = cls[i].end();
            for (; it1 != it1end; it1++) {
               Int_t id1 = (*it1)->GetID();
               Double_t x1 = (*it1)->GetX();
               Double_t z1 = (*it1)->GetZ();
               Float_t off1 = (*it1)->GetOffset();
               if (fDetailDebug) fprintf(fp,"id1 = %3d, x1 = %4.1f, z1 = %4.1f, off1 = %6.1f\n",id1,x1,z1,off1);
               if (fDetailDebug) printf("id1 = %3d, x1 = %4.1f, z1 = %4.1f, off1 = %6.1f\n",id1,x1,z1,off1);
               std::vector<TCatPulseShape*>::iterator it2 = cls[j].begin();
               std::vector<TCatPulseShape*>::iterator it2end = cls[j].end();
               for (; it2 != it2end; it2++) {
                  Int_t id2 = (*it2)->GetID();
                  Double_t x2 = (*it2)->GetX();
                  Double_t z2 = (*it2)->GetZ();
                  Float_t off2 = (*it2)->GetOffset();
                  if (fDetailDebug) fprintf(fp,"id2 = %3d, x2 = %4.1f, z2 = %4.1f, off2 = %6.1f\n",id2,x2,z2,off2);
                  if (fDetailDebug) printf("id2 = %3d, x2 = %4.1f, z2 = %4.1f, off2 = %6.1f\n",id2,x2,z2,off2);
                 
                  Double_t distance = TMath::Sqrt((x1-x2)*(x1-x2)+(z1-z2)*(z1-z2));
                  Float_t  diffoff  = off1 - off2;
                  if (fDetailDebug) fprintf(fp,"distance = %6.1f, diffoff = %6.1f\n",distance,diffoff);
                  if (fDetailDebug) printf("distance = %6.1f, diffoff = %6.1f\n",distance,diffoff);
                 
                  if ((TMath::Abs(distance) < 13.) && (TMath::Abs(diffoff) <= 50.)) {
                     fAdd = true;
                     fAddCls[j] = true;
                     break;
                  }
               }
               if (fAddCls[j]) break;
            }
         } else if (i>=j) {
            if (fDebug) fprintf(fp,"Skip (i,j) = (%d,%d) Loop.\n",i,j);
            if (fDetailDebug) printf("Skip (i,j) = (%d,%d) Loop.\n",i,j);
         }
         if (fDebug) {
            if ((!fAddCls[j]) && (i<j)) fprintf(fp,"cls[%d]: failed to add to cls[%d]\n",i,j);
         }
         if (fDetailDebug) {
            if ((!fAddCls[j]) && (i<j)) printf("cls[%d]: failed to add to cls[%d]\n",i,j);
         }
      }
      for (Int_t j=0; j<ncls; j++) {
         if (fAddCls[j]) {
            if (fDebug) fprintf(fp,"before: cls[%d].size = %zd, cls[%d].size = %zd\n",i,cls[i].size(),j,cls[j].size());
            if (fDetailDebug) printf("before: cls[%d].size = %zd, cls[%d].size = %zd\n",i,cls[i].size(),j,cls[j].size());
            cls[j].insert(cls[j].begin(),cls[i].begin(),cls[i].end());
            if (fDebug) fprintf(fp,"after: cls[%d].size = %zd, cls[%d].size = %zd\n",i,cls[i].size(),j,cls[j].size());
            if (fDetailDebug) printf("after: cls[%d].size = %zd, cls[%d].size = %zd\n",i,cls[i].size(),j,cls[j].size());
            cls[i].resize(cls[i].size());
            if (fDebug) fprintf(fp,"cls[%d]: add to cls[%d]\n",i,j);
            if (fDetailDebug) printf("cls[%d]: add to cls[%d]\n",i,j);
         }
      }
      if (fAdd) {
         cls[i].clear();
         if (fDebug) fprintf(fp,"cls[%d] cleared\n",i);
         if (fDetailDebug) printf("cls[%d] cleared\n",i);
      }
   }

   nCls = 0;
   for (Int_t i=0; i<ncls; i++) {
      if (cls[i].size()>0) {
         nCls++;
         Int_t nicls = cls[i].size();
         if (fSaveHist) fhClusterSize->Fill(nicls);
         if (fDebug) fprintf(fp,"cls[%d] size : %d\n",i,nicls);
         if (nicls<fMinNHits) {
            if (fDebug) fprintf(fp,"Skip: cluster size < %d hits\n",fMinNHits);
            cls[i].clear();
         }
      }
   }
   if (fSaveHist) fhNumOfCluster->Fill(nCls);

   /*--------------------------------------------------------------------------*/

   // Calcurate max length between  and beam
   Int_t max_Hits = 0;
   Int_t max_cls = -1;
   Double_t max_length = 0;
   Double_t max_X = 0;
   Double_t max_X_off = 0;
   Bool_t IsPenetrating = false;
   for (Int_t icls=0; icls<nCls; icls++) {
      Double_t max_cls_length = 0;
      Double_t max_cls_X = 0;
      Double_t max_cls_X_off = 0;
      Double_t BeamX = 0;
      Bool_t cls_IsPenetrating = false;
      std::vector<TCatPulseShape*>::iterator it = cls[icls].begin();
      std::vector<TCatPulseShape*>::iterator itend = cls[icls].end();
      Int_t nicls = cls[icls].size();
      if (fDebug) fprintf(fp,"[%d] %d hits remained\n",icls,nicls);
      if (nicls<=fCutHits) {
         if (fDebug) fprintf(fp,"Skip:  Hits <= %d\n",fCutHits);
         continue;
      }
      for (; it != itend; it++) {
         Double_t x = (*it)->GetX();
         Double_t z = (*it)->GetZ();
         Float_t off = (*it)->GetOffset();
         Double_t q = (*it)->GetCharge();
         if (fDebug) fprintf(fp,"id = %3d, off = %6.1f, x = %5.1f, z = %5.1f, q = %5.1f\n",(*it)->GetID(),off,x,z,q);
         Double_t lx = TMath::Sqrt((x - BeamX)*(x - BeamX));
         if (max_cls_length < lx) {
            max_cls_length = lx;
            max_cls_X = x;
            max_cls_X_off = off;
         }
         if (TMath::Abs(x)>=40.) cls_IsPenetrating = true;
      }
      if (fDetailDebug) fprintf(fp,"max_cls_length = %5.1f\n",max_cls_length);    
      if (fDetailDebug) printf("max_cls_length = %5.1f\n",max_cls_length);

      if (max_length < max_cls_length) {
         max_cls = icls;
         max_length = max_cls_length;
         max_X = max_cls_X;
         max_X_off = max_cls_X_off;
         IsPenetrating = cls_IsPenetrating;
      } else if (max_length == max_cls_length) {
         if (max_Hits < nicls) {
            max_cls = icls;
            max_X = max_cls_X;
            max_X_off = max_cls_X_off;
            max_Hits = nicls;
            IsPenetrating = cls_IsPenetrating;
         }
      }
   }

   if (fDebug) fprintf(fp,"max_cls = %d, max_length = %5.1f, max_X = %5.1f\n",max_cls,max_length,max_X);
   if (fDetailDebug) printf("max_cls = %d, max_length = %5.1f, max_X = %5.1f\n",max_cls,max_length,max_X);

   if (max_cls==-1.) {
      if (fDebug) fprintf(fp,"Skip: no match clusters.\n");
      return;
   }

   TObjArray cls_array;
   Double_t Qsum = 0;
   if (!fUseAllCluster) {
      // default
      // use the cluster with the maximum length
      std::vector<TCatPulseShape*>::iterator it    = cls[max_cls].begin();
      std::vector<TCatPulseShape*>::iterator itend = cls[max_cls].end();
      for (; it != itend; it++) {
         Double_t q = (*it)->GetCharge();
         Qsum += q;
         cls_array.Add((*it));
      }
      // Sum of Charge in cluster(s)
      if (fSaveHist) fhClusterQ->Fill(Qsum);
   } else {
      // optional
      // use all the clusters
      for (Int_t icls=0; icls<nCls; icls++) {
         std::vector<TCatPulseShape*>::iterator it    = cls[icls].begin();
         std::vector<TCatPulseShape*>::iterator itend = cls[icls].end();
         for (; it != itend; it++) {
            Double_t q = (*it)->GetCharge();
            Qsum += q;
            cls_array.Add((*it));
         }
         // Sum of Charge in cluster(s)
         if (fSaveHist) fhClusterQ->Fill(Qsum);
      }
   }
   /*Final Results-------------------------------------------------------------*/

   if (nHits && fDebug) fprintf(fp,"\n*Final Results\n");
   if (nHits && fDetailDebug) printf("\n*Final Results\n");
   Int_t nResultHits = cls_array.GetEntriesFast();

   /*Save----------------------------------------------------------------------*/

   for (Int_t iHit=0; iHit<nResultHits; iHit++) {
      TCatPulseShape *hit = (TCatPulseShape*) cls_array[iHit];
      TCatPulseShape *out = (TCatPulseShape*) fOutput->ConstructedAt(fOutput->GetEntriesFast());
      hit->Copy(*out);

      if (fSavePulse) {
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

      if (fDrawPulse) {
         TObject *hitobj = out;
         fPulseDrawer->SetPulse(*hitobj);
         fPulseDrawer->SetEvent(fEvt,iHit);
         fPulseDrawer->SetGraphRange(0.,100.,-10.,150.);
         fPulseDrawer->SetTextNDC(Form("Evt = %5d, id = %3d, offset = %5.1f",fEvt,out->GetID(),out->GetOffset()),1);
         fPulseDrawer->SetTextNDC(Form("nHits = %3d, iHit = %3d, nSample = %3d, charge = %5.2f",nHits,iHit,hit->GetNumSample(),out->GetCharge()),5,0.10,0.91);
         fPulseDrawer->SetTextNDC(GetName(),5,0.10,0.01);
         fPulseDrawer->Draw();
      }
   }
}
