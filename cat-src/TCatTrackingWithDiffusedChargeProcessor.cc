/*
 * @file   TCatTrackingWithDiffusedChargeProcessor.cc
 * @brief  tracking with diffused charge
 *
 * @date   Created       : 2016-07-24 08:49:05 JST
 *         Last Modified : 2021-04-13 11:58:08 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#include "TCatTrackingWithDiffusedChargeProcessor.h"
#include <TCatTrackDiffusedMinuitHelper.h>
#include <TCatPulseShape.h>
#include <TCatTrackResult.h>
#include <TCatTrackResultPulse.h>
#include <TClonesArray.h>
#include <TMath.h>
#include <Math/GSLSimAnMinimizer.h>
#include <Math/GSLMinimizer.h>
#include <Math/GSLNLSMinimizer.h>
#include <Math/GeneticMinimizer.h>
#include <Minuit2/Minuit2Minimizer.h>
#include <Minuit2/MnPrint.h>
#include <TCatReadoutPadArray.h>
#include <TCatReadoutPad.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TSystem.h>
#include <TRangeTableHelper.h>
#include <TF1.h>
#include <TH2F.h>
#include <algorithm>
#include <TStopwatch.h>
#include <TDirectory.h>
#include <TCatTpcTrack.h>
#include <TRandom.h>
using art::TCatTrackingWithDiffusedChargeProcessor;

ClassImp(TCatTrackingWithDiffusedChargeProcessor)


TCatTrackingWithDiffusedChargeProcessor::TCatTrackingWithDiffusedChargeProcessor()
: fNameInput(""),fInput(NULL), fNameOutput(""), fOutput(NULL)
{
   DoubleVec_t defSigmaCoeff(3);
   defSigmaCoeff[0] = 0;
   defSigmaCoeff[1] = 1.;
   defSigmaCoeff[2] = 0.;

   RegisterInputCollection("InputCollection","input array name", fNameInput, TString("atpulse"),
                           &fInput,TClonesArray::Class_Name(),TCatTpcTrack::Class_Name());
   RegisterOutputCollection("OutputCollection","tracking result collection", fNameOutput, TString("track"),
                            &fOutput,TClonesArray::Class_Name(),TCatTpcTrack::Class_Name());
   RegisterOutputCollection("NameCpuTime","Cpu Time", fNameCpuTime, TString("cputime"),
                            "D",&fCpuTime);
   RegisterProcessorParameter("IndependentVariable","In order to calculate the initial parameter, this variable is used as a independent variable",
                              fIndependentVariable,(Int_t)kAxisX);
   RegisterProcessorParameter("FixedOrigin","fixed origin parameter of the independent axis (as an assumption)",fFixedOrigin,0.);
   RegisterProcessorParameter("DoMeshSearch","flag to do mesh search (default: 1 to do, 0)",fDoMeshSearch,kFALSE);
   RegisterProcessorParameter("SigmaCoeff","coefficient to calculate sigma in TCatFunctionChargeResidual as sqrt(s0*s0+s1*s1*N*N)",
                              fSigmaCoeff,defSigmaCoeff);
   {
      Int_t dim[] = {20,20,20};
      IntVec_t defaultMeshDim(dim,dim+sizeof(dim)/sizeof(Int_t));
      RegisterProcessorParameter("MeshDim","dimension of mesh search. [range, offset, slope]",fMeshDim,defaultMeshDim);
   }
   {
      Double_t step[] = {10.,1.,0.1};
      DoubleVec_t defaultMeshStep(step,step+sizeof(step)/sizeof(Double_t));
      RegisterProcessorParameter("MeshStep","step of mesh search. secondary search will be performed with 1/10 step. [range, offset, slope]",
                                 fMeshStep,defaultMeshStep);
   }
   RegisterOptionalParameter("InitialRange","set initial range if non zero (default 0), fix range in minuit minimization  if positive ",fInitialRange,0.);

   RegisterOptionalParameter("MinuitMinType",
                             "type of minimizer (simplex (default) or migrad) case insensitive",
                             fMinuitMinTypeName,TString("simplex"));
   RegisterOptionalParameter("Offset","fixed offset for rest of independent axis (other than Y direction) X0 for idependent Z, Z0 for independent X",fOffset,TMath::QuietNaN());
   Register(fResTrack("ResTrack","tracks for resolution evaluation","resTrack"));
   Register(fDoPrepResTrack("DoPrepResTrack","flag to prepare tracks",kFALSE));
   
   RegisterHelper(&fEventDisplayHelper);
   RegisterHelper(&fChargeResponseHelper);



   // initialization
   fEMinuitMinType = ROOT::Minuit2::kSimplex;
}

TCatTrackingWithDiffusedChargeProcessor::~TCatTrackingWithDiffusedChargeProcessor()
{

}

TCatTrackingWithDiffusedChargeProcessor::TCatTrackingWithDiffusedChargeProcessor(const TCatTrackingWithDiffusedChargeProcessor& rhs)
{
}

TCatTrackingWithDiffusedChargeProcessor& TCatTrackingWithDiffusedChargeProcessor::operator=(const TCatTrackingWithDiffusedChargeProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

TCanvas *canvas;

void TCatTrackingWithDiffusedChargeProcessor::Init(TEventCollection *col)
{
   // check and assign minuit type
   if (fMinuitMinTypeName.EqualTo("simplex",TString::kIgnoreCase)) {
      fEMinuitMinType = ROOT::Minuit2::kSimplex;
   } else if (fMinuitMinTypeName.EqualTo("migrad",TString::kIgnoreCase)) {
      fEMinuitMinType = ROOT::Minuit2::kMigrad;
   } else if (fMinuitMinTypeName.EqualTo("minimize",TString::kIgnoreCase)) {
      fEMinuitMinType = ROOT::Minuit2::kCombined;
      
   } else {
      // unknown
      SetStateError(TString::Format("Unknown minimization type: %s",fMinuitMinTypeName.Data()));
      return;
   }

   
   if (fSigmaCoeff.size() != 3) {
      SetStateError("SigmaCoeff should have three figures, 0th, 1st and 2nd order coefficient");
      return;
   }
   fFunctionChargeResidual.SetResponseHelper(&fChargeResponseHelper);
   fFunctionChargeResidual.SetSigmaCoeff(fSigmaCoeff);   


   Int_t nTotal = fMeshDim[0] * fMeshDim[1] * fMeshDim[2];
   if (fDoMeshSearch || 1) {
      fChargeResponses.resize(nTotal);
      fFunctions.resize(nTotal);
      for (Int_t i=0; i<nTotal; i++) {
         fChargeResponseHelper.Copy(fChargeResponses[i]);
         fFunctions[i].SetResponseHelper(&fChargeResponses[i]);
         fFunctions[i].SetSigmaCoeff(fSigmaCoeff);
         fFunctions[i].SetUseTotalCharge(false);
      }
      
   }
   
   canvas = new TCanvas;
   if (fEventDisplayHelper.Displays()) {
      // prepare histogram if event display is ready 
      fHists.push_back(new TH2F("hChi2","Chi2 map",100.,0.,100,100,0.,1000.));
      TPad *pad = fEventDisplayHelper.GetMainPad();
      pad->cd();
      fHists[0]->Draw("colz");
   }
}

void TCatTrackingWithDiffusedChargeProcessor::Process()
{
   if (fVerboseLevel > 2) {
      Info("Process","======================================================================");
      Info("Process","                             NEW EVENT");
      Info("Process","======================================================================");
   }

   fOutput->Clear("C");
   fResTrack->Clear("C");
   
   for (Int_t iEntry = 0, nEntry = (*fInput)->GetEntriesFast(); iEntry < nEntry; ++iEntry) {
      TCatTpcTrack *input_track = static_cast<TCatTpcTrack*>((*fInput)->UncheckedAt(iEntry));
      TCatTpcTrack *output = static_cast<TCatTpcTrack*>(fOutput->ConstructedAt(fOutput->GetEntriesFast()));
      input_track->Copy(*output);
      ReconstructTrack(output);
      if (!fDoPrepResTrack) return;
      const int nhits = output->GetHits().size();
      
      for (int ihits = 0; ihits < nhits; ++ihits) {
         
         output = static_cast<TCatTpcTrack*>(fResTrack->ConstructedAt(fResTrack->GetEntriesFast()));
         input_track->Copy(*output);
         TCatTrackResultPulse *pulse = output->GetHits()[ihits];
         output->GetHits().erase(output->GetHits().begin() + ihits);
//         printf("ihits = %d, nhits = %d\n",ihits,output->GetHits().size());
         ReconstructTrack(output);
         fChargeResponseHelper.SetTrack(output->GetStartPoint(),output->GetEndPoint());
         double charge, closest, angle, rangeToEnd, trackLength;
         fChargeResponseHelper.GetResponse(pulse->GetID(),charge,closest,angle,rangeToEnd,trackLength);
         output->SetCharge(pulse->GetCharge());
         output->SetEnergyDeposit(charge);
         output->SetID(pulse->GetID());
         output->SetAuxID(input_track->GetID());
         output->Validate();
      }
   }
}


void TCatTrackingWithDiffusedChargeProcessor::ReconstructTrack(TCatTpcTrack* output)
{
   if (fDoMeshSearch) {
      ReconstructTrackMesh(output);
   } else {
      ReconstructTrackMinuit(output);
   }
}

void TCatTrackingWithDiffusedChargeProcessor::ReconstructTrackMinuit(TCatTpcTrack* input_track)
{
   TCatTpcTrack *output = input_track;
   
   const Int_t vIdx = fIndependentVariable;
   Double_t px[3], va[3];
   TVector3 start = input_track->GetStartPoint();
   TVector3 end   = input_track->GetEndPoint();
   TVector3 direction = input_track->GetDirection();
   std::vector<TCatPulseShape*> hits;
   for (std::size_t i = 0, n = input_track->GetHits().size(); i < n; ++i) {
      hits.push_back(input_track->GetHits()[i]);
   }
   // initialize track parameters
   if (TMath::Abs(fFixedOrigin - start[vIdx]) > TMath::Abs(fFixedOrigin - end[vIdx])) {
      TVector3 tmp = start;
      start = end;
      end = tmp;
      direction = end - start;
   } 
   start = end - (end[vIdx] - fFixedOrigin) / direction[vIdx] * direction;

   for (Int_t i=0; i<3; ++i) {
      px[i] = start[i];
      va[i] = direction[i]/TMath::Abs(direction[vIdx]);
   }

   Double_t range = TMath::Abs(fInitialRange);
   if (range < TMath::Limits<Double_t>::Epsilon()) {
      range = -(end-start).Mag();
   }
//   Double_t rangeUnit = 10*TArtSystemOfUnit::m;
   Double_t rangeUnit = TArtSystemOfUnit::mm;
   fFunctionChargeResidual.SetHits(hits);
   fFunctionChargeResidual.SetRangeUnit(rangeUnit);
   ROOT::Minuit2::Minuit2Minimizer min(fEMinuitMinType);
   fFunctionChargeResidual.SetUseTotalCharge(false);

   const TCatReadoutPadArray *padArray = fChargeResponseHelper.GetReadoutPadArray();
   double side = static_cast<TCatReadoutPad*>((padArray)->UncheckedAt(0))->Side();

   double pend = input_track->GetOriginalEndPoint()[fIndependentVariable];
   double pstart = input_track->GetOriginalStartPoint()[fIndependentVariable];
   double dx0 = TMath::Abs(pend - pstart);
   double mean = (pend + pstart) * 0.5;
   double dx = TMath::Abs(mean - input_track->GetStartPoint()[fIndependentVariable]);
   double evx = TMath::Abs(side / 3. / dx0);
   double ex = TMath::Abs(dx * evx);
   
#if 0   
   Info("ReconstructTrack","dx0 = %f evx = %f ex = %f, x0 = %f(%f), y0 = %f(%f), z0 = %f(%f), range = %f",dx0,evx,ex,
        px[0],va[0],px[1],va[1],px[2],va[2], range);
#endif   
   min.SetFunction(fFunctionChargeResidual);
   min.SetVariable(0,"x0",px[0],ex);
   min.SetVariable(1,"y0",px[1],1);
   min.SetVariable(2,"z0",px[2],ex);
   min.SetVariable(3,"vx",va[0],evx);
   min.SetVariable(4,"vy",va[1],0.01);
   min.SetVariable(5,"vz",va[2],evx);
   min.SetVariable(6,"R",range/rangeUnit,range/rangeUnit*0.5);

      // find proper range
      fFunctionChargeResidual.SetUseTotalCharge(true);
      fFunctionChargeResidual.SetVerboseLevel(0);
      min.SetFunction(fFunctionChargeResidual);
      min.FixVariable(fIndependentVariable);
      min.FixVariable(fIndependentVariable+3);
//      min.FixVariable(1);
      min.FixVariable(4);
      if (fInitialRange > 0) {
         min.FixVariable(6);
      }
      min.SetPrintLevel(0);
      if (!TMath::IsNaN(fOffset)) {
         Int_t idx = fIndependentVariable == 0 ? 2 : 0;
         min.SetVariableValue(idx,fOffset);
         min.FixVariable(idx);
      }
      fFunctionChargeResidual.SetUseTotalCharge(false);
      Int_t savedLevel = gErrorIgnoreLevel;
      if (fVerboseLevel == 0 ) gErrorIgnoreLevel = 1001;
      Info("ReconstructTrack","Minimize with minuit");
      min.Minimize();
      gErrorIgnoreLevel = savedLevel;
         


   TVector3 startPoint(min.X()[0],min.X()[1],min.X()[2]);
   TVector3 endPoint = min.X()[6] * rangeUnit * TVector3(min.X()[3],min.X()[4],min.X()[5]).Unit() + startPoint;
   
   output->Validate();
   output->Init();
   output->SetResults(std::vector<TCatPulseShape*>(),startPoint,endPoint,&fChargeResponseHelper);
   output->SetChi2(fFunctionChargeResidual.DoEval(min.X()));
   output->SetStatus(min.Status());
   output->SetEDM(min.Edm());

}
   
void TCatTrackingWithDiffusedChargeProcessor::ReconstructTrackMesh(TCatTpcTrack* input_track)
{
   TCatTpcTrack *output = input_track;
   const Int_t vIdx = fIndependentVariable;
   Double_t px[3], va[3];
   TVector3 start = input_track->GetStartPoint();
   TVector3 end   = input_track->GetEndPoint();
   TVector3 direction = input_track->GetDirection();
   std::vector<TCatPulseShape*> hits;
   for (std::size_t i = 0, n = input_track->GetHits().size(); i < n; ++i) {
      hits.push_back(input_track->GetHits()[i]);
   }
   // initialize track parameters
   if (TMath::Abs(fFixedOrigin - start[vIdx]) > TMath::Abs(fFixedOrigin - end[vIdx])) {
      TVector3 tmp = start;
      start = end;
      end = tmp;
      direction = end - start;
   } 
   start = end - (end[vIdx] - fFixedOrigin) / direction[vIdx] * direction;

   for (Int_t i=0; i<3; ++i) {
      px[i] = start[i];
      va[i] = direction[i]/TMath::Abs(direction[vIdx]);
   }

   Double_t range = TMath::Abs(fInitialRange);
   if (range < TMath::Limits<Double_t>::Epsilon()) {
      range = -(end-start).Mag();
   }
//   Double_t rangeUnit = 10*TArtSystemOfUnit::m;
   Double_t rangeUnit = TArtSystemOfUnit::mm;
   fFunctionChargeResidual.SetHits(hits);
   fFunctionChargeResidual.SetRangeUnit(rangeUnit);
   ROOT::Minuit2::Minuit2Minimizer min(fEMinuitMinType);
   fFunctionChargeResidual.SetUseTotalCharge(false);

   const TCatReadoutPadArray *padArray = fChargeResponseHelper.GetReadoutPadArray();
   double side = static_cast<TCatReadoutPad*>((padArray)->UncheckedAt(0))->Side();

   double pend = input_track->GetOriginalEndPoint()[fIndependentVariable];
   double pstart = input_track->GetOriginalStartPoint()[fIndependentVariable];
   double dx0 = TMath::Abs(pend - pstart);
   double mean = (pend + pstart) * 0.5;
   double dx = TMath::Abs(mean - input_track->GetStartPoint()[fIndependentVariable]);
   double evx = TMath::Abs(side / 3. / dx0);
   double ex = TMath::Abs(dx * evx);
   
#if 0   
   Info("ReconstructTrack","dx0 = %f evx = %f ex = %f, x0 = %f(%f), y0 = %f(%f), z0 = %f(%f), range = %f",dx0,evx,ex,
        px[0],va[0],px[1],va[1],px[2],va[2], range);
#endif   
   min.SetFunction(fFunctionChargeResidual);
   min.SetVariable(0,"x0",px[0],ex);
   min.SetVariable(1,"y0",px[1],1);
   min.SetVariable(2,"z0",px[2],ex);
   min.SetVariable(3,"vx",va[0],evx);
   min.SetVariable(4,"vy",va[1],0.01);
   min.SetVariable(5,"vz",va[2],evx);
   min.SetVariable(6,"R",range/rangeUnit,range/rangeUnit*0.5);


   // mesh search
   const Int_t varIdx = (vIdx == 0) ? 2 : 0;
   const Int_t nR = fMeshDim[0];
   const Int_t n0 = fMeshDim[1];
   const Int_t nV = fMeshDim[2];
   const Int_t numMeshTotal = nR * n0 * nV;
   Double_t sR = fMeshStep[0];
   Double_t s0 = fMeshStep[1];
   Double_t sV = fMeshStep[2];
   Double_t min_chi2 = TMath::Limits<Double_t>::Max();
   //    Double_t xorg[] = { px[0], px[1], px[2], va[0], va[1], va[2], range};
      
   Double_t xorg[] = { min.X()[0],min.X()[1],min.X()[2],
                       min.X()[3],min.X()[4],min.X()[5],min.X()[6] };
   Double_t mag = TMath::Sqrt(xorg[3]*xorg[3] + xorg[4]*xorg[4] + xorg[5]*xorg[5]);
   xorg[3] /= mag;
   xorg[4] /= mag;
   xorg[5] /= mag;
   std::vector<Double_t> chi2val(numMeshTotal);
   std::vector<Double_t> val0(numMeshTotal);
   std::vector<Double_t> valV(numMeshTotal);
   std::vector<Double_t> valR(numMeshTotal);
   if (fEventDisplayHelper.Displays()) {
      fHists[0]->GetXaxis()->Set(n0,xorg[varIdx]+s0*(0-TMath::Floor(n0/2.)),xorg[varIdx]+s0*(TMath::Floor(n0/2.)));
      fHists[0]->GetYaxis()->Set(nV,xorg[varIdx+3]+sV*(0-TMath::Floor(nV/2.)),xorg[varIdx+3]+sV*(TMath::Floor(nV/2.)));
   }


   for (Int_t iR = 0; iR < nR; ++iR) {
      Double_t xr = TMath::Tan(gRandom->Uniform(-TMath::Pi()/2.,TMath::Pi()/2.));
      xr = (iR - TMath::Floor(nR/2.));
      for (Int_t i0 = 0; i0 < n0; ++i0) {
         Double_t x0 = TMath::Tan(gRandom->Uniform(-TMath::Pi()/2.,TMath::Pi()/2.));
         x0 = i0 - TMath::Floor(n0/2.);
         for (Int_t iV = 0; iV < nV; ++iV) {
            Double_t xv = TMath::Tan(gRandom->Uniform(-TMath::Pi()/2.,TMath::Pi()/2.));
            xv = iV - TMath::Floor(nV/2.);

            Double_t xnew[] = {xorg[0],xorg[1],xorg[2],
                               xorg[3],xorg[4],xorg[5],xorg[6]};
            xnew[6]    += xr * sR;
            xnew[varIdx] += x0 * s0;
            xnew[varIdx+3] += xv * sV;
            xnew[6]/=rangeUnit;
               
            Int_t idx = (iR *  n0 + i0) * nV + iV;
            fFunctions[idx].SetHits(hits);
            fFunctions[idx].SetRangeUnit(rangeUnit);
            Double_t chi2 = fFunctions[idx].DoEval(xnew);
            chi2val[idx] = chi2;
            val0[idx] = xnew[varIdx];
            valV[idx] = xnew[varIdx + 3];
            valR[idx] = xnew[6];
//               printf("chi2 = %10.5f, R = %f, z0 = %f, vz = %f\n",chi2,valR[idx],val0[idx],valV[idx]);
            if (fEventDisplayHelper.Displays()) {
               fHists[0]->SetBinContent(i0+1,iV+1,chi2);
            }
         }
      }
   }
   std::vector<Double_t>::iterator it = std::min_element(chi2val.begin(),chi2val.end());
   Int_t idx = std::distance(chi2val.begin(),it);
   min_chi2 = *it;
   px[varIdx] = val0[idx];
   va[varIdx] = valV[idx];
   range    = valR[idx];
   Double_t xnew[] = {xorg[0],xorg[1],xorg[2],
                      xorg[3],xorg[4],xorg[5],xorg[6]};
   xnew[varIdx+3] = va[varIdx];
   min.SetFunction(fFunctionChargeResidual);
   min.SetVariableValue(varIdx,px[varIdx]);
   min.SetVariableValue(3,xnew[3]);
   min.SetVariableValue(4,xnew[4]);
   min.SetVariableValue(5,xnew[5]);
   min.SetVariableValue(6,range);
   if (fVerboseLevel>0) {
      printf("*** chi2 = %10.5f, R = %f, z0 = %f (%f), vz = %f (%f) mag = %f\n",min_chi2,range,px[varIdx],xorg[varIdx],va[varIdx],xorg[varIdx+3],mag);
   }
   if (fEventDisplayHelper.Displays()) {
      fEventDisplayHelper.GetMainPad()->cd();
      fHists[0]->SetXTitle("offset");
      fHists[0]->SetYTitle("slope");
      fHists[0]->Draw("colz");
//      canvas->Update();
//      canvas->Modified();
      fEventDisplayHelper.Process();
   }
         
//   min.PrintResults();

   TVector3 startPoint(min.X()[0],min.X()[1],min.X()[2]);
   TVector3 endPoint = min.X()[6] * rangeUnit * TVector3(min.X()[3],min.X()[4],min.X()[5]).Unit() + startPoint;
   output->Validate();
   output->Init();
   output->SetResults(hits,startPoint,endPoint,&fChargeResponseHelper);
   output->SetChi2(fFunctionChargeResidual.DoEval(min.X()));
   output->SetStatus(min.Status());
   output->SetEDM(min.Edm());
}
   


