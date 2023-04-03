/**
 * @file   TCatTrackDiffusedMinuit.cc
 * @brief  implementation of tracking taking diffusion into account
 *
 * @date   Created       : 2015-08-26 10:56:30 JST
 *         Last Modified : 2016-11-11 23:36:16 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 Shinsuke OTA
 */

#include "TCatTrackDiffusedMinuit.h"

#include <TVector3.h>
#include <TMath.h>
#include <TObjArray.h>
#include <TGraph.h>
#include <TCatReadoutPad.h>
#include <TCatReadoutPadArray.h>
#include <TCatPulseShape.h>
#include <TH2.h>
#include <TFile.h>
#include <TList.h>
#include <TKey.h>
#include <TCatTrackResultPulse.h>
#include <TClonesArray.h>
#include <TObjString.h>
#include <TNArray.h>


using art::TCatTrackDiffusedMinuit;

ClassImp(TCatTrackDiffusedMinuit)

TCatTrackDiffusedMinuit::TCatTrackDiffusedMinuit()
: TCatTrackMinuit()
{
   fFile = NULL;
   fResponseTable = NULL;
}

TCatTrackDiffusedMinuit::~TCatTrackDiffusedMinuit()
{
   Int_t n = fHist.size();
   printf("deleting %d histograms\n",n);
   if (fFile) {
      for (Int_t i = 0; i!=n; i++) {
         fHist[i]->Delete();
      }
      fHist.clear();
      printf("cleared\n");
      fFile->Close();
   }
   printf("closed\n");
}

TCatTrackDiffusedMinuit::TCatTrackDiffusedMinuit(const TCatTrackDiffusedMinuit& rhs)
{
}

TCatTrackDiffusedMinuit& TCatTrackDiffusedMinuit::operator=(const TCatTrackDiffusedMinuit& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

Double_t TCatTrackDiffusedMinuit::CalcResidual(Double_t y0, Double_t z0, Double_t vx, Double_t vy, Double_t vz, TClonesArray *output)
{
   static TGraph gr;
   // Info("CalcResidual","in");
   // initialize
   Double_t fcn = 0.;
   Double_t fcnE = 0.;
   Double_t fcnY = 0.;
//   vy *= vx;
//   vz *= vx;
   // create utility vectors,
   Double_t x0 = (z0 - fP0) / fP1;
   TVector3 va(vx,vy,vz); // direction
   TVector3 va_unit = va.Unit();
   TVector3 a1(x0,y0,z0);
   TVector3 a2 = a1 + va;
   Int_t    nHits = fHits->GetEntriesFast();   
   // uprintf("// va\n");
   //va.Print();

   // loop all the hit pulses
   TVector3 points1[3];
   TVector3 points2[3];
   TVector3 b1, b2;
   TVector3 c1, c2;
   std::vector<TVector3*> vertex1;
   std::vector<TVector3*> vertex2;
   Double_t yEst = TMath::QuietNaN();
   Double_t yEst2 = TMath::QuietNaN(); // y estimated from the line on the center of the pad. The line is parallel to the track.
   Double_t tracklength = TMath::QuietNaN();
   Double_t totalCharge = 0.;
   for (Int_t iHit = 0; iHit != nHits; ++iHit)
   {
      TCatPulseShape *pulse = (TCatPulseShape*) fHits->UncheckedAt(iHit);
      totalCharge += pulse->GetCharge();
   }
   totalCharge /= fWorkFunction;
   
   for (Int_t iHit = 0; iHit != nHits; ++iHit)
   {
      vertex1.clear();
      vertex2.clear();
      TCatPulseShape *pulse = (TCatPulseShape*) fHits->UncheckedAt(iHit);
      TCatReadoutPad *pad = (TCatReadoutPad*) fPadArray->At(pulse->GetID());
      if (output) {
         printf("*** hit  ID = %d ***\n",pulse->GetID());
      }
      if (!pad) {
         Error("Eval","pad %p for pad id = %d\n",pad,pulse->GetID());
         return 1e10; // large value. is this ok?
      }
      Int_t nP = (pad->GetNumPoints() -1);
      Double_t xc = pad->X();
      Double_t zc = pad->Y();
      if (TMath::Abs(xc) < 19.) continue;
      const Double_t *x = pad->VertexX();
      const Double_t *y = pad->VertexY();

      // temporarlly use of vectors to calculate distance and ranges
      // calculate nearest point using vector
      // consider in X-Y plane instead of X-Z plane.
      Float_t angle = 0.;
      Float_t angle_org = angle;
      Double_t distance = 0;
      Double_t range = 0;
      
      {
         TVector2 pc2(xc,zc);
         TVector2 pa1(a1.X(),a1.Z());
         TVector2 pva_unit(va_unit.X(),va_unit.Z());
         TVector2 pa2(a2.X(),a2.Z());
         TVector2 pv1(x[0],y[0]);
         TVector2 pc1 = pa1 + ((pc2 - pa1) * (pva_unit)) * pva_unit;
         range = (pa2 - pc1) * pva_unit;
         if (output && 0){
            printf("va angle = %f\n",pva_unit.Phi()/TMath::Pi()*180);
            printf("(pa2 - pa1).Phi() = %f\n",(pa2-pa1).Phi()/TMath::Pi()*180);
            (pa2-pa1).Print();
            printf("(pc1 - pc2).Phi() = %f\n",(pc1-pc2).Phi()/TMath::Pi()*180);
            (pc1-pc2).Print();
            printf("(pv1 - pc2).Phi() = %f\n",(pv1-pc2).Phi()/TMath::Pi()*180);
            (pv1-pc2).Print();
            printf("pa2 - pc1\n");
            (pa2-pc1).Print();
            
            pv1.Print();
            pc1.Print();
            pc2.Print();
         }
         
         // transform line to hough space in X-Z plane
         // set y to zero in order to calculate in X-Z plane
         distance = (pc2-pc1).Mod();
         // a.Cross(b) returns a x b
         Double_t sign = (pc1-pc2)^(pa2-pa1);
         // reference angle 
         Double_t tau = TMath::ATan2(y[0] - pc2.Y(), x[0] - pc2.X());
         // vector to the nearest point from the pad center
         Double_t vxc = pc1.X() - pc2.X();
         Double_t vzc = pc1.Y() - pc2.Y();
         angle = (pc1-pc2).DeltaPhi(pv1 - pc2);
         if (output) printf("angle = %f\n",angle/TMath::Pi()*180.);

         if (sign < 0) {
#if 0            
            // angle after mirror transform
            Double_t vxm = vxc * TMath::Cos(2 * tau) + vzc * TMath::Sin(2 * tau) ;
            Double_t vzm = vxc * TMath::Sin(2 * tau) - vzc * TMath::Cos(2 * tau) ;
            angle = TMath::ATan2(vzm,vxm);
            
         } else {
            angle = TMath::ATan2(vzc,vxc);
#endif
            angle *= -1;
         }
         // angle += 30./180. *TMath::Pi() - tau;
         
         angle_org = angle;
         
         if (angle < 0.) angle += 2 * TMath::Pi();
         angle = angle - TMath::Floor(angle / (120./180.*TMath::Pi())) * 120./180.*TMath::Pi();
      }
      // threefold symmetry 
      // here the distance and angle are prepared
      
      // calculate energy by looking up the table

      // calculate the ratio
      c1 =  100.*va + TVector3(xc,y0 + vy * (distance *TMath::Cos(angle)+xc - x0) / vx,zc);
      c2 = -100.*va + TVector3(xc,y0 + vy * (distance *TMath::Cos(angle)+xc - x0) / vx,zc);

      // loop all the vertex of triangle
      gr.Set(nP);
      for (Int_t i=0; i!=nP; ++i) {
         gr.SetPoint(i,x[i],y[i]);
         b1.SetXYZ(x[i],0,y[i]);
         b2.SetXYZ(x[i+1],0,y[i+1]);
         if (FindVertex(a1,a2,b1,b2,points1[i])) {
            vertex1.push_back(&points1[i]);
         }
         if (FindVertex(c1,c2,b1,b2,points2[i])) {
            vertex2.push_back(&points2[i]);
         }
         // if track pass this pad, calculate y position
      }
      
      if (vertex1.size() > 2) {
         UniqueVector3(vertex1);
      }

      // remove same vertexes
      if (vertex2.size() > 2) {
         UniqueVector3(vertex2);
      }
      if (vertex2.size() != 2) {
         Error("CalcResidual", "something is wrong: vertex2.size() = %u",vertex2.size());
         Error("CalcResidual", "parm (%f, %f, %f, %f, %f)",y0,z0,vx,vy,vz);
         
         va.Print();
         c1.Print();
         c2.Print();
         b1.Print();
         b2.Print();
         return TMath::Limits<Double_t>::Max();
      }
      
      switch (vertex1.size()) {
      case 0:
         break;
      case 1:
         if (gr.IsInside(a1.X(),a1.Z())) {
            // starting point is inside this pad
            yEst = TMath::Min(vertex1[0]->Y(),a1.Y());
            tracklength = (*vertex1[0] - a1).Mag();
         } else if (gr.IsInside(a2.X(),a2.Z())) {
            // end point is inside this pad
            yEst = TMath::Min(vertex1[0]->Y(),a2.Y());
            tracklength = (*vertex1[0] - a2).Mag();
         }
         break;
      case 2:
         yEst = TMath::Min(vertex1[0]->Y(),vertex1[1]->Y());
         tracklength = (*vertex1[0] - *vertex1[1]).Mag();
         break;
      }

      yEst2 = TMath::Min(vertex2[0]->Y(), vertex2[1]->Y());
                                 

      Double_t len1 = (*vertex2[0] - *vertex2[1]).Mag();
      vertex2[0]->SetY(0);
      vertex2[1]->SetY(0);
      Double_t len2 = (*vertex2[0] - *vertex2[1]).Mag();
      Double_t ratio = len1/len2;
      
      // calculate likelihood and add
      Double_t yExp = pulse->GetOffset() * fDriftVelocity;
      Double_t nExp = pulse->GetCharge() / fWorkFunction;
      // assume a1 is the starting point of track measured
      Double_t driftLength = yExp - a1.Y() - fFieldCageOffset;
      // collection of attachment
      //nExp /= TMath::Exp(- fAttachment * driftLength / TArtSystemOfUnit::cm);
      // constant position resolution for drift length
      Double_t sigma = 1.;
      Double_t nEst = TMath::Limits<Double_t>::Min();
      Double_t closest = TMath::Limits<Double_t>::Max();;
#if 1
#if 0
      Double_t indexes[] = { angle/TMath::Pi()*180, distance, driftLength, -range };
      nEst = fResponseTable->Eval(indexes) * ratio;
#else
      
      {
         TVector3 &trackEnd = a2;
         TVector3 &trackStart = a1;
         const Double_t kDefaultIntegrateStep = 1.;
         const Double_t deg = TMath::Pi()/180.;
         // center of pad
         TVector2 c2(pad->X(),pad->Y());
         // one vertex of pad
         TVector2 v1(pad->VertexX()[0],pad->VertexY()[0]);
         // total range of trajectory
         Double_t mag = (trackEnd-trackStart).Mag();
         // unit vector for trajectory
         TVector3 unit = (trackEnd-trackStart).Unit();
         Int_t nStep = TMath::CeilNint(mag/kDefaultIntegrateStep);
         Double_t step = mag / nStep;
         
         Double_t charge = 0;
         Double_t start = 0;
         Double_t length = 0;
         if (nStep < 0 || nStep > 100000) {
            Info("CalcResidual","nStep = %d for mag = %f",nStep,mag);
            return (TMath::Limits<Double_t>::Max());
         }
         for (Int_t i = 0; i != nStep; ++i) {
            TVector3 rmid = trackStart + (i + 0.5) * step * unit;
            // position of energy-loss segment to be calculated
            TVector2 bmid(rmid.X(),rmid.Z());
            TVector2 c2bmid = bmid-c2;
            Double_t distance = c2bmid.Mod();

            if (distance > 10.) continue;
            if (distance < closest) {
               closest = distance;
            }
            angle = TVector2::Phi_0_2pi(c2bmid.Phi() - (v1 - c2).Phi())/deg;
            angle -= TMath::Floor(angle/120) * 120;
            if (angle > 60.) angle = TMath::Abs(120 - angle);
            Double_t drift = rmid.Y();
            Double_t x[] = {angle, distance, drift };
            Double_t weight =fResponseTable->Eval(x);
            if (TMath::IsNaN(weight)) {
               charge = TMath::QuietNaN();
            } else {
               Double_t dedx = (fRange2EnergyLoss->Eval(mag - i*step) +
                                fRange2EnergyLoss->Eval(mag - (i +1)*step)) * 0.5;
               charge += step * dedx * weight;
               length += step * weight;
               if (start == 0 && weight > 0) {
                  start = (rmid-trackStart).Mag();
               }
            }
#if 0
            if (weight > 0.001) {
               printf("x = %f z = %f\n",bmid.X(),bmid.Y());
               printf("i = %d / n = %d, angle = %f, distance = %f, drift = %f\n",i,nStep,angle,distance,drift);
               printf("range = %f, dE = %f, step = %f, weight = %f\n",mag - (i + 0.5)*step,fRange2EnergyLoss->Eval(mag - (i + 0.5)*step), step, fResponseTable->Eval(x));
               printf("%f\n",step * fRange2EnergyLoss->Eval(mag - (i + 0.5)*step) * fResponseTable->Eval(x));
            }
#endif            
         }
         nEst = charge/fWorkFunction;
#if 0
         printf("total range = %f\n",mag);
         printf("effective range from this pad = %f, length on this pad = %f (tracklength = %f)\n",mag-start,length,tracklength);
         printf("charge = %f, nEst = %f\n",charge,nEst);

#endif         
      }
#endif      
#else
      Double_t distanceIndex = distance / fStepDistance;
      Double_t driftIndex = (driftLength - fOriginDrift) / fStepDrift;
      Double_t rangeIndex = (-range - fOriginRange) / fStepRange;
      Double_t angleIndex = angle / fStepAngle;

      driftIndex += 1; // add one because of starting with 0 mm
      rangeIndex += 1;

      
      Int_t it     = TMath::FloorNint(angle / fStepAngle);
      Int_t ir     = TMath::FloorNint(distance / fStepDistance);
      Int_t id     = TMath::FloorNint(driftIndex);
      Int_t iRange = TMath::FloorNint(rangeIndex);

      // check range
      if (it >= fNumAngle || it < 0) {
         Warning("CalcResidual","it = %d exceeds the range (fNumAngle = %d)\n",it,fNumAngle);
         it = fNumAngle - 1;
      }
      if (ir >= fNumDistance || ir < 0) {
         Warning("CalcResidual","it = %d exceeds the range (fNumAngle = %d)\n",it,fNumDistance);
         ir = fNumDistance - 1;
      }
      if (id >= fMaxBinDrift || id < 0) {
         Warning("CalcResidual","it = %d exceeds the range (fMaxBinDrift = %d)\n",it,fMaxBinDrift);
         id = fMaxBinDrift;;
      }
      if (iRange >= fMaxBinRange || iRange < 0) {
         Warning("CalcResidual","it = %d exceeds the range (fMaxBinDrift = %d)\n",it,fMaxBinDrift);
         iRange = fMaxBinRange;
      }
         
      // a1.Y() is beam position, 50 mm offset relative to this origin

      if (output) {
         printf("it     = %d, angle    = %f fNumAngle = %d\n",it,angle/TMath::Pi()*180, fNumAngle);
         printf("ir     = %d, distance = %f fNumDistance = %d\n",ir,distance,fNumDistance);
         printf("id     = %d, drift    = %f min = %d, max = %d\n",id,driftLength,fMinBinDrift,fMaxBinDrift);
         printf("iRange = %d, range    = %f min = %d, max = %d\n",iRange,range,fMinBinRange,fMaxBinRange);
      }
      if ( 0 <= it && it < fNumAngle &&
           0 <= ir && ir < fNumDistance &&
           fMinBinDrift <= id && id <= fMaxBinDrift &&
           fMinBinRange <= iRange && iRange <= fMaxBinRange ) {
         // correction w/ angle, distance, range and drift length
         // calculate derivative and then difference
         Double_t e0 = fHist[it * fNumDistance + ir]->GetBinContent(id,iRange) * ratio;

         Double_t e1 = fHist[(it+1) * fNumDistance + ir]->GetBinContent(id,iRange) * ratio;
         Double_t dEdTheta = (e1 - e0) * (angleIndex - it);

         Double_t e2 = fHist[it * fNumDistance + ir + 1]->GetBinContent(id,iRange) * ratio;
         Double_t dEdDist = (e2 - e0) * (distanceIndex - ir);

         Double_t e3 = fHist[it * fNumDistance + ir]->GetBinContent(id+1,iRange) * ratio;
         Double_t dEdDrift = (e3 - e0) * (driftIndex - id);

         Double_t e4 = fHist[it * fNumDistance + ir]->GetBinContent(id,iRange+1) * ratio;
         Double_t dEdRange = (e4 - e0) * (rangeIndex - iRange);
         nEst = e0 + dEdTheta + dEdDist + dEdDrift + dEdRange;

         if (output) {
            printf("phi      = %7.2f\n",va_unit.Phi()/TMath::Pi()*180);
            printf("ratio    = %7.2f\n",ratio);
            printf("e0       = %7.2f\n",e0);

            printf("dEdTheta = %7.2f\n",dEdTheta);
            printf("dEdDist  = %7.2f\n",dEdDist );
            printf("dEdDrift = %7.2f\n",dEdDrift);
            printf("dEdRange = %7.2f\n",dEdRange);
            printf("nEst     = %7.2f\n",nEst);
         }
         
         
         if (nEst <  TMath::Limits<Double_t>::Epsilon()) {
            nEst = TMath::Limits<Double_t>::Min();
         }
      }
      if (output && 0) {
         printf("Pad ID = %d\n",pad->GetID());
         printf("nExp = %f nEst = %f c = %f wf = %f\n",nExp,nEst,pulse->GetCharge(),fWorkFunction);
         printf("yExp = %f yEst = %f yEst2 = %f\n",yExp,yEst, yEst2);
         printf("dist = %f (%d), angle = %f (%d), id = %d, irange = %d\n",distance,ir,angle/TMath::Pi()*180,it,id,iRange);
         printf("range = %f, origin = %f, step = %f, irange = %d\n",range,fOriginRange, fStepRange,iRange);
      }
#endif      
//      fcn -= nExp * TMath::Log(nEst) - nEst - nExp * TMath::Log(nExp) + nExp;


      fcnE += (nExp - nEst) * (nExp-nEst) / (0.1 * nExp) / (0.1 * nExp);
//      fcnE += (nExp - nEst) * (nExp-nEst)  / nExp;
      fcnE += closest*closest / (100) * (nExp/totalCharge);
      if (!TMath::IsNaN(yEst)) {
//         fcn +=(yExp - yEst) * (yExp - yEst) * 0.5 / sigma / sigma;
         fcnY +=(yExp - yEst) * (yExp - yEst) / sigma / sigma;
      } else {
//         fcn +=(yExp - yEst2) * (yExp - yEst2) * 0.5 / sigma / sigma;
         fcnY +=(yExp - yEst2) * (yExp - yEst2)  / sigma / sigma;
      }         

      if (output) { 
         // create output
//         if (output->GetEntriesFast() == 0) {
            // fill track start
//            TCatTrackResultPulse *reshit = (TCatTrackResultPulse*) output->ConstructedAt(output->GetEntriesFast());
//            reshit->AddVertex(TVector3(x0,y0,z0));
//         }
         TCatTrackResultPulse *reshit = (TCatTrackResultPulse*) output->ConstructedAt(output->GetEntriesFast());
         pulse->Copy(*reshit);
         reshit->SetTrackLength(tracklength);
         reshit->SetCharge(nExp * fWorkFunction);
         reshit->SetEnergyDeposit(nEst * fWorkFunction);
         reshit->SetCharge(nExp);
         reshit->SetEnergyDeposit(nEst);
         reshit->SetDistance(closest);
         reshit->SetAngle(angle);
         // printf("nExp = %f nEst = %f fcn = %f\n",nExp,nEst);
         reshit->SetDEDX(nEst*fWorkFunction/tracklength);
      }
      // printf("nExp = %f nEst = %f\n",nExp,nEst);
      
   }
   //  fcn = TMath::Floor((fcnE + fcnY)*10)/10.;
   fcn = fcnE + fcnY;
   // Info("CalcResidual","fcn = %f\n",fcn);
   if (output) Info("CalcResidual","nOutput = %d\n",output->GetEntries());
   if (fcn == TMath::Infinity() || fcn == TMath::QuietNaN() ) {
      fcn = TMath::Limits<Double_t>::Max();
//      fcn = TMath::QuietNaN();
   }
#if 1
   if (output) printf("CalcResidual: chi2 = %g (fcnE = %g, fcnY = %g), y0 = %f, z0 = %f, vx = %f, vy = %f, vz = %f\n",fcn,fcnE,fcnY,y0,z0,vx,vy,vz);
//    printf("CalcResidual: chi2 = %g, y0 = %f, z0 = %f, vx = %f, vy = %f, vz = %f\n",fcn,y0,z0,vx,vy,vz);
//   printf("fcn = %g\n",fcn);
#endif   
   return fcn;
}

Bool_t TCatTrackDiffusedMinuit::LoadResponseTable(const char* filename)
{
   TDirectory *saved = gDirectory;
   fFile = TFile::Open(filename);
   saved->cd();
   TList *list = fFile->GetListOfKeys();
   Int_t n = list->GetEntries();
   TObject *obj = NULL;
   for (Int_t i=0; i!=n; i++) {
      const char* name = list->At(i)->GetName();
      obj = fFile->Get(name);
      if (obj->InheritsFrom(TNArray::Class())) {
         break;
      }
   }
   fResponseTable = (TNArray*) obj;
   if (fResponseTable) {
      fResponseTable->Load();
   }
   return kTRUE;
}
   
Bool_t TCatTrackDiffusedMinuit::RegisterHistogram(const char* filename)
{
   TDirectory* saved = gDirectory;
   fFile = TFile::Open(filename);
   saved->cd();
   if (!fFile) {
      Error("RegisterHistgram", "Cannot open file '%s'", filename);
      return kFALSE;
   }

   TList *list = fFile->GetListOfKeys();
   Int_t nDir = list->GetEntries();
   fNumAngle = nDir;

   // calculate angle step and distance step
   TDirectory *dir1 = dynamic_cast<TDirectory*>(((TKey*)list->At(0))->ReadObj());
   TDirectory *dir2 = dynamic_cast<TDirectory*>(((TKey*)list->At(1))->ReadObj());
   TList *histlist = dir1->GetListOfKeys();
   TList *histlist2 = dir2->GetListOfKeys();
   TH2* h1 = dynamic_cast<TH2*>(((TKey*)histlist->At(0))->ReadObj());
   TH2* h2 = dynamic_cast<TH2*>(((TKey*)histlist2->At(1))->ReadObj());
   Double_t angle1 = ((TObjString*)TString(h1->GetTitle()).Tokenize(" ")->At(0))->GetString().Atof();
   Double_t angle2 = ((TObjString*)TString(h2->GetTitle()).Tokenize(" ")->At(0))->GetString().Atof();
   Double_t distance1 = ((TObjString*)TString(h1->GetTitle()).Tokenize(" ")->At(1))->GetString().Atof();
   Double_t distance2 = ((TObjString*)TString(h2->GetTitle()).Tokenize(" ")->At(1))->GetString().Atof();
   fStepAngle = (angle2 - angle1)/180. * TMath::Pi();
   fStepDistance = distance2 - distance1;
   // printf("********** fStepAngle    = %f\n",fStepAngle);
   // printf("********** fStepDistance = %f\n",fStepDistance);
   fStepDrift = h1->GetXaxis()->GetBinWidth(1);
   fStepRange = h1->GetYaxis()->GetBinWidth(1);
   fOriginDrift = h1->GetXaxis()->GetBinCenter(1);
   fOriginRange = h1->GetYaxis()->GetBinCenter(1);
   fMinBinRange = 1;
   fMinBinDrift = 1;
   fMaxBinRange = h1->GetYaxis()->GetNbins();
   fMaxBinDrift = h1->GetXaxis()->GetNbins();
   
   for (Int_t i=0; i!=nDir; i++) {
      TDirectory *dir = dynamic_cast<TDirectory*>(((TKey*)list->At(i))->ReadObj());
      TList *histlist = dir->GetListOfKeys();
      Int_t nHist = histlist->GetEntries();
      fNumDistance = nHist;
      for (Int_t j=0; j!=nHist; j++) {
         TH2* h = dynamic_cast<TH2*>(((TKey*)histlist->At(j))->ReadObj());
         h->SetName(Form("%s_%s",dir->GetName(),h->GetName()));
         fHist.push_back(h);
      }
   }
   return kTRUE;
}
