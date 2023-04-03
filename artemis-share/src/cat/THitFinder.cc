/**
 * @file   THitFinder.cc
 * @brief  hit finder for tracking detector
 *
 * @date   Created       : 2014-03-08 09:32:26 JST
 *         Last Modified : May 27, 2014 19:25:42 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#include "THitFinder.h"
#include <TCatPulseShape.h>
#include <TClonesArray.h>
#include <TH3F.h>
#include <TH2F.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TTrackerHit3D.h>
#include <TCatReadoutPadArray.h>
#include <TGraph.h>
#include <TH2Poly.h>
#include <TF1.h>
#include <TCatReadoutPad.h>
using art::THitFinder;

ClassImp(THitFinder)
TCanvas *fCanvas ;
THitFinder::THitFinder()
{
   RegisterInputCollection("InputCollection","pulse data (TCatPulseShape)",
                           fInputColName, TString("atpulse"), &fInput,"TClonesArray","art::TCatPulseShape");
   RegisterOutputCollection("OutputCollection","output pulse data (TCatPulseShape)",
                            fOutputColName,TString("athit"),&fOutput,"TClonesArray","art::TCatPulseShape");
   RegisterProcessorParameter("AcceptableOffsetDifference","offset difference to judge if two pulss are in same group",
                              fAcceptableOffsetDifference,(Float_t)50.);
   RegisterInputInfo("ReadoutPad","Name of Readout Pad",fReadoutPadName,
                     TString("readoutPad"),&fPads,"art::TCatReadoutPadArray","");

   hHough = new TH3F("hHough","Hough3D", 45,0.,180.,30,0.,360,100,0.,100.);
   hHough2D = new TH2F("hHough2D","Hough2D", 60,0.,360.,50,0.,100.);
   hHough2DXZ = new TH2F("hHough2DXZ","Hough2DXZ", 60,0.,360.,50,0.,100.);
}

THitFinder::~THitFinder()
{
}

THitFinder::THitFinder(const THitFinder& rhs)
{
   MayNotUse("THitFinder");
}

THitFinder& THitFinder::operator=(const THitFinder& rhs)
{
   MayNotUse("operator=");
   if (this != &rhs) {

   }
   return *this;
}

void THitFinder::Init(TEventCollection *col)
{
#if 0
   TEventObject *obj = NULL;
   if (!(obj = col->Get(fInputColName.front()))) {
      Info("Init","No such input %s",fInputColName.front().Data());
      return;
   }
   fInput = (TClonesArray**) obj->GetObjectRef();
   Info("Init","%p\n",fInput);
   fOutput = new TClonesArray("art::TCatPulseShape");
   col->Add(fOutputColName,fOutput,fOutputIsTransparent);
#endif
   fHitCandidates = new TClonesArray("art::TTrackerHit3D");
      
   printf("fHitCandates = %p\n",fHitCandidates);
   col->Add("hits",fHitCandidates,fOutputIsTransparent);

   if (!fPads) {
      Warning("Init","What's happened");
      fPads = (TCatReadoutPadArray*) col->GetInfo(fReadoutPadName);
   } else {
      Info("Init","pad array found at %p",fPads);
   }
   fCanvas = new TCanvas;
}

void THitFinder::Process()
{
   static TH2Poly *hPad = fPads->CreateHist("hPad","hPad",-50.,50.,-50.,50.);
   fOutput->Clear("C");
   fHitCandidates->Clear("C");
   TClonesArray *input = *fInput;
   Int_t nHits = input->GetEntriesFast();
    // input should be sorted by offset time
   std::vector<TCatPulseShape*> hits;
   std::list<TCatPulseShape*> hitlist;
   TCatPulseShape::SetSortType(TCatPulseShape::kTiming);
   TCatPulseShape::SetSortOrder(TCatPulseShape::kASC);
   
   (input)->Sort();
   (input)->Compress();
   TCatPulseShape *currentPulse = NULL;
   Float_t refOffset = 0;
   for (Int_t iHit = 0; iHit != nHits; iHit++) {
      TCatPulseShape *hit = (TCatPulseShape*) (input)->At(iHit);
      if (!hit->IsValid()) break;
      hits.push_back(hit);
   }
   nHits = hits.size();
   
   for (Int_t iP = 0; iP != nHits; iP++) {
      TCatPulseShape *pulse = hits[iP];
      if (pulse->GetID() > 399) continue;

      for (Int_t iP2 = iP+1; iP2 != nHits; iP2++) {
         TCatPulseShape *pulse2 = hits[iP2];
         TCatReadoutPad *pad = (TCatReadoutPad*) fPads->At(pulse->GetID());

         if ((TMath::Abs(pulse2->GetOffset() - pulse->GetOffset()) < fAcceptableOffsetDifference) &&
             pad->IsNeighbor(pulse2->GetID())) {

            TTrackerHit3D *hit = (TTrackerHit3D*) fHitCandidates->ConstructedAt(fHitCandidates->GetEntriesFast());
            hit->SetPulses(pulse,pulse2);
            hit->SetPosition((pulse->GetX()+pulse2->GetX())/2.,
                             (pulse->GetOffset()+pulse2->GetOffset())/2. * 0.02 * 10,
                             (pulse->GetZ()+pulse2->GetZ())/2.);
            hit->SetCharge(pulse->GetCharge() + pulse2->GetCharge());
            hit->SetOffsetDiff(pulse->GetOffset() - pulse2->GetOffset());
         }
      }
   }
   return;
   hHough->Reset();
   hHough2D->Reset();
   hHough2DXZ->Reset();
   Double_t ct[90], st[90];
   Double_t cp[30], sp[30];
   for (Int_t it = 0; it != 90; it++) {
      Double_t t = (it)*4./180.*TMath::Pi();
      ct[it] = cos(t);
      st[it] = sin(t);
   }
   for (Int_t ip = 0; ip != 30; ip++) {
      Double_t p = (ip + 0.5) * 3./180.*TMath::Pi();
      cp[ip] = cos(p);
      sp[ip] = sin(p);
   }
   nHits = fHitCandidates->GetEntriesFast();
   for (Int_t iHit = 0; iHit != nHits; iHit++) {
//      TCatPulseShape *hit = (TCatPulseShape*)fHitCandidates->At(iHit);
      TTrackerHit3D *hit = (TTrackerHit3D*)fHitCandidates->At(iHit);
//      if (!hit->IsValid()) continue;
      Double_t x = hit->X();
      Double_t y = hit->Y();
      Double_t z = hit->Z();
      for (Int_t it = 0; it != 90; it++) {
         for (Int_t ip = 0; ip != 30; ip++) {
            Double_t r = x*ct[it]*cp[ip] + y*st[it]*cp[ip] + z*sp[ip];
            hHough->Fill((it+0.5)*4.,(ip+0.5)*3.,r);
         }
         Double_t r2d = y*ct[it] + z*st[it];
         hHough2D->Fill((it)*4.,r2d);
         r2d = x*ct[it] + z*st[it];
         hHough2DXZ->Fill((it)*4.,r2d);
      }
   }
//   printf("Draw\n");
   fCanvas->Clear();
//   fCanvas->Divide(2,2);
//   fCanvas->cd(1);
//   hHough2D->Draw("colz");
//      hHough->Draw("");
//   fCanvas->cd(2);
//   hHough2DXZ->Draw("colz");
   fCanvas->Update();
   fCanvas->Modified();
//   usleep(1000000);
   Int_t ibin = hHough2D->GetMaximumBin();
   Int_t ibinx, ibiny,ibinz;
   hHough2D->GetBinXYZ(ibin,ibinx,ibiny,ibinz);
   Double_t t = hHough2D->GetXaxis()->GetBinLowEdge(ibinx)/180.*TMath::Pi();
   Double_t r = hHough2D->GetYaxis()->GetBinLowEdge(ibiny);
   ibin  = hHough2DXZ->GetMaximumBin();
   hHough2DXZ->GetBinXYZ(ibin,ibinx,ibiny,ibinz);
   Double_t txz = hHough2DXZ->GetXaxis()->GetBinLowEdge(ibinx)/180.*TMath::Pi();
   Double_t rxz = hHough2DXZ->GetYaxis()->GetBinLowEdge(ibiny);


//   printf("r = %f, t = %f, g = %d, x = %d, y = %d\n",r,t,ibin,ibinx,ibiny);
   std::vector<Double_t> yy,zz;
   TObjArray candidates;
   for (Int_t iHit = 0; iHit!=nHits; iHit++) {
      TTrackerHit3D *hit = (TTrackerHit3D*)fHitCandidates->At(iHit);
      Double_t x = hit->X();
      Double_t y = hit->Y();
      Double_t z = hit->Z();
//      printf(" val = %f\n",y*TMath::Cos(t)+z*TMath::Sin(t));
      if (TMath::Abs(r - (y*TMath::Cos(t)+z*TMath::Sin(t))) > 15 ||
         TMath::Abs(rxz - (x*TMath::Cos(txz)+z*TMath::Sin(txz))) > 15) {
         hit->SetQualityBit(TDataObject::kInvalid);
      } else {
         hit->ResetQualityBit(TDataObject::kInvalid);
         yy.push_back(y);
         zz.push_back(z);
         if (hit->GetPulse1()->GetAuxID() == 0) {
            hit->GetPulse1()->SetAuxID(1);
            candidates.Add(hit->GetPulse1());
         }
         if (hit->GetPulse2()->GetAuxID() == 0) {
            hit->GetPulse2()->SetAuxID(1);
            candidates.Add(hit->GetPulse2());
         }
      }
   }
   TVector3 direction(cos(txz),0,sin(txz));
   TCatPulseShape::SetSortType(TCatPulseShape::kID);
   TCatPulseShape::SetSortOrder(TCatPulseShape::kASC);
   candidates.Sort();
   TCatPulseShape::SetSortType(TCatPulseShape::kCharge);
   TCatPulseShape::SetSortOrder(TCatPulseShape::kDESC);
   Int_t nCand = candidates.GetEntriesFast();
   std::vector<Double_t> hitsX;
   std::vector<Double_t> hitsZ;
//   printf("nCand = %d\n",nCand);
   fHitCandidates->Clear("C");
   for(Int_t i=0; i< nCand; i++) {
//      printf("i = %d\n",i);
      TObjArray row;
      TCatPulseShape *pulse = (TCatPulseShape*) candidates[i];
      if (!pulse) continue;
      candidates.Remove(pulse);
      Int_t id1 = pulse->GetID();
      TCatReadoutPad *pad = (TCatReadoutPad*) fPads->At(id1);
      row.Add(pulse);
      for (Int_t j=i+1; j< nCand; j++) {
         TCatPulseShape *pulse2 = (TCatPulseShape*) candidates[j];
         if (!pulse2) continue;
         Int_t id2 = pulse2->GetID();
         if (!pad->IsNeighbor(id2)) continue;
         if ((id1%20) != (id2%20)) {
//            printf("i = %d id1 = %d, id2 = %d\n",i,id1,id2);
            continue;
         }
         row.Add(pulse2);
         candidates.Remove(pulse2);
      }
      if (row.GetEntriesFast() < 2) continue;
      row.Sort();
      TCatPulseShape *p1 = (TCatPulseShape*) row[0];
      TCatPulseShape *p2 = (TCatPulseShape*) row[1];
      Double_t c1 = p1->GetCharge();
      Double_t c2 = p2->GetCharge();
      id1 = p1->GetID();
      Int_t id2 = p2->GetID();
      TCatReadoutPad *pad1 = (TCatReadoutPad*)fPads->At(id1);
      TCatReadoutPad *pad2 = (TCatReadoutPad*)fPads->At(id2);
      if (!pad1->IsNeighbor(id2)) {
         continue;
         printf("id2 %d is not neighbor of id1 %d\n",id2,id1);
         for (Int_t i=0; i!=row.GetEntriesFast(); i++) {
            TCatPulseShape *p = (TCatPulseShape*) row[i];
            printf(" %d %3d %f\n",i,p->GetID(),p->GetCharge());
         }
      }
      switch (row.GetEntriesFast()) {
      case 2:
         break;
      case 3:
         c1 += ((TCatPulseShape*)row[2])->GetCharge();
         break;
      case 4:
         // to be impremented
         // pad1->IsNeighbor((TCatPulseShape*)row[2])->GetID())
         break;
      default:
         break;
      }
      Double_t x,z;
      pad1->CalculatePosition(direction,pad2,c1,c2,x,z);
      TTrackerHit3D *hit = (TTrackerHit3D*) fHitCandidates->ConstructedAt(fHitCandidates->GetEntriesFast());
      if (p1->GetID() < p2->GetID()) {
         hit->SetPulses(p1,p2);
      } else {
         hit->SetPulses(p2,p1);
      }
      hit->SetPosition(x,TMath::Min(p1->GetOffset(),p2->GetOffset())*0.02*10.,z);
      hit->SetCharge(c1+c2);
      hit->SetOffsetDiff(p1->GetOffset()-p2->GetOffset());
//      printf("(x,z) = (%f, %f)\n",x,z);
      hitsX.push_back(x);
      hitsZ.push_back(z);
   }
   if (hitsX.size()==0) return;
//   printf("drawing\n");
   TGraph graph(hitsX.size(),&hitsX[0],&hitsZ[0]);
//   fCanvas->cd(4);
//   hPad->Draw();
//   graph.Draw("P*");
//   graph.Fit("pol1","Q");
#if 0   
   fCanvas->cd(3);
   if (sin(t) < 0.001) {
      TGraph graph(yy.size(),&zz[0],&yy[0]);
      graph.Fit("pol1","Q");
      graph.Draw("AP*");
      TF1 fun("fun","pol1",-50.,50.);
      fun.SetLineColor(kBlue);
      fun.SetParameters( r/cos(t),-sin(t)/cos(t));
      fun.Draw("same");
      fCanvas->Update();
      fCanvas->Modified();
      usleep(1000000);
   } else {
      TGraph graph(yy.size(),&yy[0],&zz[0]);
      graph.Fit("pol1","Q");
      graph.Draw("AP*");
      TF1 fun("fun","pol1",-50.,50.);
      fun.SetLineColor(kBlue);
      fun.SetParameters( r/sin(t),-cos(t)/sin(t));
      fun.Draw("same");
      fCanvas->Update();
      fCanvas->Modified();
      usleep(1000000);
   }
#endif
      fCanvas->Update();
      fCanvas->Modified();
//      usleep(1000000);
//   fHitCandidates->Sort();
//   fHitCandidates->Compress();
   return;
#if 0
   for (Int_t iHit = 0; iHit != nHits; iHit++) {
      TCatPulseShape *hit = hits[iHit];
      if (!hit || hit->GetCharge() < 100) continue;
      if (hit->GetID() < 340 && hit->GetID() > 59) continue;
      hitlist.push_back(hit);
      for (Int_t iHit2 = 0; iHit2 != nHits; iHit2++) {
         TCatPulseShape *hit2 = hits[iHit2];
         if (!hit2 || hit2->GetCharge() < 0) continue;
         if ((TMath::Abs(hit2->GetOffset() - hit->GetOffset()) < fAcceptableOffsetDifference) &&
             ((TMath::Abs(hit2->GetID() - hit->GetID()) == 10) ||
              (TMath::Abs(hit2->GetID() - hit->GetID()) == 20))){
            if (hit->GetCharge() + hit2->GetCharge() < 20) continue;
            hitlist.push_back(hit2);
         } 
      }
      if (hitlist.size() > 2) {
         // require more than 3 hits
         while (hit != hitlist.front() && hitlist.size()) {
            hitlist.pop_front();
         }
         candidates.merge(hitlist);
         candidates.sort();
         candidates.unique();
      }
      hitlist.clear();
   }

   // saving the samples
   nHits = candidates.size();
   // printf("n candidates = %d\n",nHits);
   std::list<TCatPulseShape*>::const_iterator it = candidates.begin();
   std::list<TCatPulseShape*>::const_iterator itend = candidates.end();
   fOutput->Clear("C");
   for (; it != itend; it++) {
      TCatPulseShape *out = (TCatPulseShape*) fOutput->ConstructedAt(fOutput->GetEntriesFast());
      (*it)->Copy(*out);
      Int_t num = (*it)->GetNumSample();
      out->GetSample().resize(num);
      out->GetClock().resize(num);
      std::copy((*it)->GetSample().begin(),(*it)->GetSample().end(),out->GetSample().begin());
      for (int i = 0; i!= num; i++) {
         out->GetClock()[i] = (double)i;
      }
      out->SetNumSample(num);
   }
#endif
}
