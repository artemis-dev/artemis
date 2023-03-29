/**
 * @file   TCatRecoilEventGenerator.cc
 * @brief  recoil event generator with diffusion
 *
 * @date   Created       : 2015-12-21 14:36:14 JST
 *         Last Modified : 2020-05-21 14:15:13 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 * 2017-04-20 10:52:29 JST (ota) Parameter BeamRegion added
 *
 *    (C) 2015 Shinsuke OTA
 */

#include "TCatRecoilEventGenerator.h"
#include <TCatReadoutPadArray.h>
#include <TCatReadoutPad.h>
#include <TGraph.h>
#include <TRandomGraph.h>
#include <TH2Poly.h>
#include <TClonesArray.h>
#include <TArtParticle.h>
#include <TCatPulseShape.h>
#include <TCatTrackResultPulse.h>
#include <TArtSystemOfUnit.h>
#include <TEventHeader.h>
#include <TRunInfo.h>
#include <TTimeStamp.h>
#include <TVector3.h>
#include <TRangeTableHelper.h>
#include <TCatTpcTrack.h>
#include "TArtSystemOfUnit.h"
#include <TH2F.h>

using art::TCatRecoilEventGenerator;
using TArtSystemOfUnit::ns;
using TArtSystemOfUnit::us;
using TArtSystemOfUnit::cm;
using TArtSystemOfUnit::mm;
using TArtSystemOfUnit::eV;
using TArtSystemOfUnit::keV;

ClassImp(TCatRecoilEventGenerator)

TCatRecoilEventGenerator::TCatRecoilEventGenerator()
{

   FloatVec_t z;
   z.push_back(-50.);
   z.push_back(50.);
   
   RegisterInputCollection("Input","input collection (TClonesArray of TArtParticle)",
                           fInputName, TString("recoil"),
                           &fInput, TClonesArray::Class_Name(), TArtParticle::Class_Name());
   RegisterOutputCollection("Output","output collection (TClonesArray of TCatPulseShape)",
                            fOutputName, TString("mcpulse"),
                            &fOutput,TClonesArray::Class_Name(), TCatPulseShape::Class_Name());
   RegisterOutputCollection("OutputTrackResult","true track result to be compared with the tracking result",
                            fNameOutputTrackResult,TString("track_true"),
                            &fOutputTrackResult,TClonesArray::Class_Name(),TCatTpcTrack::Class_Name());
   RegisterOutputCollection("OutputTrackResultFolded","true track result with folding to be compared with the tracking result",
                            fNameOutputTrackResultFolded,TString("track_true_folded"),
                            &fOutputTrackResultFolded,TClonesArray::Class_Name(),TCatTpcTrack::Class_Name());

   RegisterOutputCollection("RunHeadersName","the name of output array for run headers, run header will be stored once",
                            fNameRunHeaders,TString("runheader"));
   RegisterOutputCollection("EventHeaderName","the name of event header",
                            fNameEventHeader,TString("eventheader"),&fEventHeader,TEventHeader::Class_Name());

   // @todo define the coordinate system
   RegisterProcessorParameter("Zrange","simulate range for z axis",fZRange,z);
   RegisterProcessorParameter("PhiRange","simulate range for phi",fPhiRange,DoubleVec_t(2,0.));

   RegisterProcessorParameter("BeamRegion","low gain region",fBeamRegion,
                              DoubleVec_t(2,0.));
   RegisterProcessorParameter("ChargeThreshold","threshold of charge for each pad (MeV)",fChargeThreshold,0.01);
   
   RegisterHelper(&fChargeResponseHelper);
   RegisterHelper(&fEventDisplayHelper);

   DoubleVec_t defSigmaCoeff(2);
   defSigmaCoeff[0] = 0;
   defSigmaCoeff[1] = 0.1;
   RegisterProcessorParameter("SigmaCoeff","coefficient to calculate sigma in TCatFunctionChargeResidual as sqrt(s0*s0+s1*s1*N*N)",
                              fSigmaCoeff,defSigmaCoeff);
   
}

TCatRecoilEventGenerator::~TCatRecoilEventGenerator()
{
}

TCatRecoilEventGenerator::TCatRecoilEventGenerator(const TCatRecoilEventGenerator& rhs)
{
}

TCatRecoilEventGenerator& TCatRecoilEventGenerator::operator=(const TCatRecoilEventGenerator& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}



void TCatRecoilEventGenerator::Init(TEventCollection *col)
{

   if (fPhiRange.size() != 2) {
      SetStateError(Form("PhiRange require 2 values while %d value(s) is given",(Int_t)fPhiRange.size()));
      return;
   }
   if (fSigmaCoeff.size() != 3) {
      SetStateError("SigmaCoeff should have three figures, 0th, 1st and 2nd order coefficient");
      return;
   }
   fSigmaCoeffTrue.resize(3);
   fSigmaCoeffTrue[0] = 0.;
   fSigmaCoeffTrue[1] = 1.;
   fSigmaCoeffTrue[2] = 0.;
   fSigmaCoeffTrueFolded = fSigmaCoeff;
   fSigmaCoeffTrueFolded[1] = sqrt(1 + fSigmaCoeffTrueFolded[2]*fSigmaCoeffTrueFolded[2]);
   fFunctionChargeResidual.SetResponseHelper(&fChargeResponseHelper);
   fFunctionChargeResidual.SetSigmaCoeff(fSigmaCoeffTrue);
   fFunctionChargeResidual.SetUseTotalCharge(false);
   fFunctionChargeResidual.SetRangeUnit(TArtSystemOfUnit::mm);
   const TCatReadoutPadArray *array = fChargeResponseHelper.GetReadoutPadArray();
   fHistPad = array->CreateHist("hPad","pad",-50.,50.,-50.,50.);
   fHistPadTime = array->CreateHist("hPadTime","pad",-50.,50.,-50.,50.);
   fRandomBrag = new TRandomGraph(fChargeResponseHelper.GetRangeTableHelper()->GetR2DE());
   
   fNumPads = fHistPad->GetNumberOfBins();
   fRunHeaders = new TList;
   fRunHeaders->SetName(fNameRunHeaders);

   if (fBeamRegion.size() != 2) {
      SetStateError("Unexpected setting for beam region");
      return;
   }

   fChargeThreshold /= fChargeResponseHelper.GetWorkFunction();

//   Info("Init","Diff Coeff = %f",fChargeResponseHelper.GetDiffusionCoefficient());

   col->AddInfo(fNameRunHeaders,fRunHeaders);


   if (fEventDisplayHelper.Displays()) {
      // prepare histogram if event display is ready 
      fHists.push_back(new TH2F("hChi2","Chi2 map",100.,0.,100,100,0.,1000.));
      TPad *pad = fEventDisplayHelper.GetMainPad();
      pad->cd();
      fHists[0]->Draw("colz");
   }
}

void TCatRecoilEventGenerator::BeginOfRun()
{
   fNumLoop = 0;
   TTimeStamp start;
   TRunInfo *runinfo = new TRunInfo("RecoilEventGenerator0","RecoilEventGenerator0");
   runinfo->SetRunName("RecoilEventGenerator");
   runinfo->SetRunNumber(0);
   runinfo->SetStartTime(start.GetSec());
   fRunHeaders->Add(runinfo);
   fEventHeader->SetRunName("RecoilEventGenerator");
   fEventHeader->SetRunNumber(0);
   
}

void TCatRecoilEventGenerator::PostLoop()
{
   TTimeStamp stop;
   TRunInfo *runinfo = (TRunInfo*) (fRunHeaders)->Last();
   runinfo->SetStopTime(stop.GetSec());
}

void TCatRecoilEventGenerator::Process()
{
//   const Double_t resolution = 0.1;
   const Double_t threshold = fChargeThreshold; // electrons
   fHistPad->ClearBinContents();
   fHistPadTime->ClearBinContents();
   fOutput->Clear("C");
   fOutputTrackResult->Clear("C");
   fOutputTrackResultFolded->Clear("C");
   if ((*fInput)->GetEntriesFast() == 0) {
      return;
   }

   // get result
   Double_t planeOffset = fChargeResponseHelper.GetReadoutPlaneOffset();
      TArtParticle *recoil = (TArtParticle*) (*fInput)->UncheckedAt(0);
   if (TMath::Abs(fPhiRange[0] - fPhiRange[1]) > TMath::Limits<Double_t>::Epsilon()) {
      Double_t phi = gRandom->Uniform(fPhiRange[0],fPhiRange[1]);
      recoil->SetPhi(phi);
   }
   Double_t z = TMath::Abs((fZRange[0] - fZRange[1])) < TMath::Limits<Float_t>::Epsilon() ? fZRange[0] : gRandom->Uniform(fZRange[0],fZRange[1]);
   TVector3 start(0.,0,z);
   TVector3 unit = recoil->Vect().Unit();
   Double_t tke = recoil->TKE();
   Double_t range = (fChargeResponseHelper.GetRangeTableHelper())->GetE2R()->Eval(tke);
   TVector3 end = start + range * unit;
//   unit.Print();
   // printf("mag = %f range = %f\n",range,fChargeResponseHelper.GetRangeTableHelper()->GetE2R()->Eval(recoil->TKE()));
   Int_t nElectrons = TMath::FloorNint(tke/fChargeResponseHelper.GetWorkFunction());
   // printf("tke = %f range = %f, nElectrons = %d\n",tke,range, nElectrons);
   const TCatReadoutPadArray *array = fChargeResponseHelper.GetReadoutPadArray();
   std::vector<Double_t> yvalue(array->GetEntries(),TMath::Limits<Double_t>::Max());
   // generate electrons 
   for (Int_t iEl = 0; iEl != nElectrons; iEl++) {
      Double_t mag  = range - fRandomBrag->Generate(0.,range);
      TVector3 point = start + mag * unit;
      Double_t y = point.Y();
      if (y < planeOffset) continue;
      Double_t sigma = fChargeResponseHelper.GetTransDiff(y-planeOffset);
      Double_t x, z;
      x = gRandom->Gaus(point.X(),sigma);
      if (fBeamRegion[0] < x && x < fBeamRegion[1]) continue;
      z = gRandom->Gaus(point.Z(),sigma);
      Int_t bin = fHistPad->Fill(x,z);
      if (bin >=0 && yvalue[bin-1]  > y) {
         yvalue[bin-1] = y;
      }
   }

   // printf("nContents = %f Integral = %f\n",fHistPad->GetEntries(),fHistPad->Integral());
   // printf("nContents = %f Integral = %f\n",hoge->GetEntries(),hoge->Integral());

   
   std::vector<TCatPulseShape*> hits;
   for (Int_t i = 0; i!=fNumPads; ++i) {
      Double_t fNumE = fHistPad->GetBinContent(i+1);
      Double_t y = yvalue[i];
      if (fNumE < threshold) continue;
//      if (y < 0) continue;
      TCatReadoutPad *pad = (TCatReadoutPad*) (fChargeResponseHelper.GetReadoutPadArray())->At(i);
      TCatPulseShape *pulse = (TCatPulseShape*) fOutput->ConstructedAt(fOutput->GetEntriesFast());
      
      pulse->SetID(i);
      pulse->SetCharge(fNumE * fChargeResponseHelper.GetWorkFunction());
      pulse->GetPos().SetXYZ(pad->X(),y,pad->Y());
      pulse->Validate();
      
      
      hits.push_back(pulse);
   }
   TCatTpcTrack *result = static_cast<TCatTpcTrack*>(fOutputTrackResult->ConstructedAt(fOutputTrackResult->GetEntriesFast()));
   TCatTpcTrack *resultfolded = static_cast<TCatTpcTrack*>(fOutputTrackResultFolded->ConstructedAt(fOutputTrackResultFolded->GetEntriesFast()));
   result->Init();
   resultfolded->Init();
   fFunctionChargeResidual.SetHits(hits);
   const Double_t dx = end.X()-start.X();
   const Double_t dy = end.Y()-start.Y();
   const Double_t dz = end.Z()-start.Z();
   const Double_t mag = TMath::Sqrt(dx*dx+dy*dy+dz*dz);
   const Double_t x[7] = {start.X(),start.Y(),start.Z(),
                          dx,dy,dz,mag};
//   end.Print();
//   start.Print();
   // create output
   result->EstimateFromHits(hits);
   result->SetResults(hits,start,end,&fChargeResponseHelper);
   fFunctionChargeResidual.SetSigmaCoeff(fSigmaCoeffTrue);
   result->SetChi2(fFunctionChargeResidual.DoEval(x));


   // display
   TH2F* chi2map = NULL;
   if (fEventDisplayHelper.Displays()
       && (chi2map = dynamic_cast<TH2F*>(fHists[0])) != NULL) {
      const Int_t n1 = 51;
      const Int_t n2 = 51;
      const Double_t s1 = 0.1;
      const Double_t s2 = 0.1;
      const Double_t min1 = x[2] - (n1-1)/2 * s1;
      const Double_t min2 = x[5] - (n2-1)/2 * s2; 
      const Double_t max1 = min1 + (n1-1) * s1;
      const Double_t max2 = min2 + (n2-1) * s2;
      chi2map->GetXaxis()->Set(n1,min1-s1/2.,max1+s1/2.);
      chi2map->GetYaxis()->Set(n2,min2-s2/2.,max2+s2/2.);
      
      
      for (Int_t i1 = 0; i1 < n1; i1++) {
         for (Int_t i2 = 0; i2 < n2; i2++) {
            Double_t z0 = min1 + i1 * s1;
            Double_t vz = min2 + i2 * s2;
            Double_t xnew[7] = { x[0], x[1], z0,
                                 x[3], x[4], vz,
                                 x[6] };
            chi2map->Fill(z0,vz,fFunctionChargeResidual.DoEval(xnew));
         }
      }
      fEventDisplayHelper.Process();
   }


   
   const Double_t &wf = fChargeResponseHelper.GetWorkFunction();
   for (Int_t i=0, n=fOutput->GetEntriesFast(); i < n; ++i) {
      TCatPulseShape *pulse = static_cast<TCatPulseShape*>(fOutput->At(i));
      const DoubleVec_t &sc = fSigmaCoeff;
      const Double_t c = pulse->GetCharge();
      const Double_t ne = c/wf;
      const Double_t sigma = TMath::Sqrt(sc[0]*sc[0] + sc[1]*sc[1]*ne + sc[2]*sc[2]*ne*ne);

      pulse->SetCharge(gRandom->Gaus(ne,sigma)*wf);
   }
   resultfolded->EstimateFromHits(hits);
   resultfolded->SetResults(hits,start,end,&fChargeResponseHelper);
   fFunctionChargeResidual.SetSigmaCoeff(fSigmaCoeffTrueFolded);
   resultfolded->SetChi2(fFunctionChargeResidual.DoEval(x));

}
