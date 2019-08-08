/* $Id:$ */
/**
 * @file   TBinaryReactionGenerator.cc
 * @date   Created : Aug 18, 2013 12:18:37 JST
 *   Last Modified : 2019-08-08 11:16:49 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TBinaryReactionGenerator.h"

#include "TCatTwoBodyKinematics.h"
#include "TH2F.h"
#include "TClonesArray.h"
#include "TF1.h"
#include "TArtSystemOfUnit.h"
#include "TRandom.h"
#include "TSpline.h"
#include "TGraph.h"

using TArtSystemOfUnit::deg;


art::TBinaryReactionGenerator::TBinaryReactionGenerator()
{
   IntVec_t p;
   p.push_back(1);
   p.push_back(1);
   StringVec_t exParamDefault;
   exParamDefault.push_back(TString("0"));
   exParamDefault.push_back(TString("15"));
   StringVec_t angParamDefault;
   angParamDefault.push_back(TString("0"));
   angParamDefault.push_back(TString("0"));
   FloatVec_t exRangeDefault;
   exRangeDefault.push_back(0.);
   exRangeDefault.push_back(30.);
   FloatVec_t angRangeDefault;
   angRangeDefault.push_back(0.);
   angRangeDefault.push_back(10.);
   fAngSpline = NULL;
   
   
   RegisterOutputCollection("OutputCollection","output name of particle array",fOutputColName,TString("recoil"));
   RegisterOutputCollection("MCTruthCollection","output name of MC truth",fMCTruthColName,TString("mctruth"));
   RegisterProcessorParameter("MaxLoop","the maximum number of loop",fMaxLoop,(Int_t)1E6);
   RegisterProcessorParameter("Particle1","mass and atomic number for particle1",fP1,p);
   RegisterProcessorParameter("Particle2","mass and atomic number for particle2",fP2,p);
   RegisterProcessorParameter("Particle3","mass and atomic number for particle3",fP3,p);
   RegisterProcessorParameter("KinMean","mean kinetic energy per nucleon",fKinMean,(float)100.);
   RegisterProcessorParameter("ExRange","the range of excitation energy",fExRange,exRangeDefault);
   RegisterProcessorParameter("ExMean","mean of excitation energy",fExMean,(float)15.);
   RegisterProcessorParameter("ExWidth","width of excitation energy : default is delta function",fExWidth,(float)0.); 
   RegisterProcessorParameter("AngRange","the range of angular distribution",fAngRange,angRangeDefault);
   RegisterProcessorParameter("AngMom","angular momentum for bessel function. If -1 (default), the isotopic distribution is assumed.",fAngMom,(int)-1);
   RegisterProcessorParameter("AngDistFile","file name of the angular distribution. The format of content is '%f %f'. ",fAngDistFile,TString(""));
   RegisterOptionalParameter("DoRandomizePhi","Flag to randomize phi direction (uniform)",fDoRandomizePhi,1);
   Register(fRunName("RunName","run name","sim"));
   Register(fRunNumber("RunNumber","run number",0));
}
art::TBinaryReactionGenerator::~TBinaryReactionGenerator()
{
   if (fOutputArray) delete fOutputArray;
   if (fMCTruthArray) delete fMCTruthArray;
}

void art::TBinaryReactionGenerator::Init(TEventCollection *col)
{
   fNumLoop = 0;
   const Int_t nEx = 100;
   const Int_t nAng = 100;
   Float_t stepEx = (fExRange[1]-fExRange[0])/nEx;
   Float_t stepAng = (fAngRange[1]-fAngRange[0])/nAng;

   Info("Init","A1 = %d, Z1 = %d",fP1[0],fP1[1]);
   Info("Init","A2 = %d, Z2 = %d",fP2[0],fP2[1]);
   Info("Init","A3 = %d, Z3 = %d",fP3[0],fP3[1]);
   Info("Init","ex  = [%f,%f]",fExRange[0],fExRange[1]);
   Info("Init","ang = [%f,%f]",fAngRange[0],fAngRange[1]);
   Info("Init","L   = %d",fAngMom);
         

   
   fKinematics = new TCatTwoBodyKinematics(fP1[0],fP1[1],fP2[0],fP2[1],fP3[0],fP3[1]);
   fKinematics->SetIncidentEnergy(fKinMean);
   fOutputArray = new TClonesArray("TArtParticle");
   fMCTruthArray = new TClonesArray("TArtParticle");

   // prepare the distribution
   fExAngDistribution = new TH2F("hseed","hseed",nEx,fExRange[0],fExRange[1],nAng,fAngRange[0],fAngRange[1]);
//   fExAngDistribution->SetDirectory(NULL);

   fExFun = NULL;
   if (fExWidth > 0) {
      fExFun = new TF1("fExFun",
                       "[1]*[1]*x*x/(TMath::Power((x*x-[0]*[0]),2)+[1]*[1]*x*x)",
                       fExRange[0],fExRange[1]);
      fExFun->SetParameters(fExMean,fExWidth);
   } 
   // angular distribution function
   if (fAngDistFile.IsNull()) {
      TF1 *fun = NULL;
      switch (fAngMom) {
      case -2:
      case -1: // flat distribution in cos angle
         fun = new TF1("funL0","1",0.,100);
         break;
      case 0:
         fun = new TF1("funL0","sin(x)/x",0.,100);
         break;
      case 1:
         fun = new TF1("funL1","sin(x)/x/x - cos(x)/x",0.,20.);
         break;
      case 2:
         fun = new TF1("funL2","(3./x/x/x - 1./x)*sin(x) - 3./x/x*cos(x)",0.,90);
         break;
      default :
         break;
      }
      if (fun ==NULL) {
         printf("angular distribution should be indicated\n");
         return;
      }
      fAngFun = fun;
      Double_t  R = 1.2*(TMath::Power(fP1[0],1./3.) + TMath::Power(fP2[0],1./3.));
      fMaxAmpl = TMath::Limits<Double_t>::Min();
      for (Int_t iEx = 0; iEx!=nEx; iEx++) {
         Double_t ampl;
         Double_t ex = stepEx * iEx + fExRange[0];
         fKinematics->SetExcitationEnergy(ex);
         for (Int_t iAng = 0; iAng!=nAng; iAng++) {
            Double_t theta = fAngRange[0] + stepAng * (iAng+0.5);
            fKinematics->SetTheta(theta*deg);
            ampl = fun->Eval(fKinematics->GetDq()/197.*R);
            ampl *= ampl;
            fMaxAmpl = std::max(fMaxAmpl,ampl * sin(theta*deg));
            
            if (fAngMom != -2) {
               ampl *= stepAng*deg*sin(theta*deg);
            }
            if (fExFun) ampl*=fExFun->Eval(ex);
            fExAngDistribution->Fill(ex,theta,ampl);
         }
      }
      fMaxAmpl *= 1.2;
   } else {
      Info("Init","Read angular distribution file '%s'",fAngDistFile.Data());
      TGraph graph(fAngDistFile);
      if (graph.GetN() == 0) {
         SetStateError(Form("Invalid data points in %s",fAngDistFile.Data()));
         return;
      }
      fMaxAmpl = TMath::Limits<Double_t>::Min();
      for (Int_t i = 0, n = graph.GetN(); i < n; ++i) {
         Double_t angle = graph.GetX()[i];
         angle *= deg;
         graph.GetX()[i] = angle;
         graph.GetY()[i] *= TMath::Sin(angle);
         if (fAngRange[0] * deg < angle && angle < fAngRange[1] * deg) {
            fMaxAmpl = std::max(fMaxAmpl, graph.GetY()[i]);
         }
      }
//      graph.Print();
      
      fMaxAmpl *= 1.2;
      fAngSpline = new TSpline3(fAngDistFile,graph.GetX(),graph.GetY(),graph.GetN());
      fAngSpline->Draw();

      Double_t xsec = 0.;
      Double_t step = 0.05 * deg;
      const Int_t nStep = (fAngRange[1] - fAngRange[0]) * deg / step;
      for (Int_t i = 0; i < nStep; ++i) {
         Double_t theta = fAngRange[0] * deg + step * (i+0.5);
         xsec += fAngSpline->Eval(theta) * step * 2 * TMath::Pi();
#if 0         
         Info("Init","x = %f deg, ds/do = %.3g mb/sr, step = %g, sin = %g, ds = %.3g mb, xsec = %.3g mb",
              theta*TMath::RadToDeg(), fAngSpline->Eval(theta)/TMath::Sin(theta),step,TMath::Sin(theta),
              fAngSpline->Eval(theta) * step * 2 * TMath::Pi(), xsec);
#endif         
      }
      
      Info("Init","fMaxAmpl = %f, Cross section = %f",fMaxAmpl, xsec);
   }
   
   col->Add(fOutputColName,fOutputArray,fOutputIsTransparent);
   col->Add(fMCTruthColName,fMCTruthArray,fOutputIsTransparent);
}

void art::TBinaryReactionGenerator::Process()
{
   Double_t ex = 0;
   Double_t theta = 0;
   fOutputArray->Clear("C");
   fMCTruthArray->Clear("C");
#if 1
   while (1) {
      if (fExFun) {
         ex = fExFun->GetRandom();
      } else if (fExRange[1] - fExRange[0] > TMath::Limits<Double_t>::Epsilon()) {
         ex = gRandom->Uniform(fExRange[0],fExRange[1]);
      } else {
         ex = fExMean;
      }
      theta = gRandom->Uniform(fAngRange[0],fAngRange[1]) * deg;
      if (fAngSpline) {
         if (fAngSpline->Eval(theta) > gRandom->Uniform(0., fMaxAmpl)) {
            fKinematics->SetExcitationEnergy(ex);
            fKinematics->SetTheta(theta);
            break;
         }
      } else if (fAngMom == -2) {
         theta = gRandom->Uniform(fAngRange[0],fAngRange[1]) * deg;
         fKinematics->SetExcitationEnergy(ex);
         fKinematics->SetTheta(theta);
         break;
      } else {
         fKinematics->SetExcitationEnergy(ex);
         fKinematics->SetTheta(theta);
         Double_t  R = 1.2*(TMath::Power(fP1[0],1./3.) + TMath::Power(fP2[0],1./3.)); 
         Double_t ampl = fAngFun->Eval(fKinematics->GetDq() / 197. * R);
         ampl = ampl * ampl * TMath::Sin(theta);
         if (ampl > gRandom->Uniform(0.,fMaxAmpl)) break;
      }
      
   }
#else
   fExAngDistribution->GetRandom2(ex,theta);
   fKinematics->SetExcitationEnergy(ex);
   fKinematics->SetTheta(theta*deg);
#endif   
   const TArtParticle *p3 = fKinematics->GetParticle(3);
   const TArtParticle *p2 = fKinematics->GetParticle(2,kTRUE);
   const TArtParticle *p0 = fKinematics->GetParticle(0,kTRUE);
   const TArtParticle *p1 = fKinematics->GetParticle(1,kTRUE);
//   printf("ex = %f, theta = %f\n",ex,theta);
//   p3->Print();
   TArtParticle *out3 = (TArtParticle*)fOutputArray->ConstructedAt(0);
   TArtParticle *out0 = (TArtParticle*)fMCTruthArray->ConstructedAt(0);
   TArtParticle *out1 = (TArtParticle*)fMCTruthArray->ConstructedAt(1);
   TArtParticle *out2 = (TArtParticle*)fMCTruthArray->ConstructedAt(2);
   out3->SetXYZT(p3->X(),p3->Y(),p3->Z(),p3->T());
   *out0 = *p0;
   *out1 = *p1;
   *out2 = *p2;
   if (fDoRandomizePhi) {
      double phi = gRandom->Uniform(0.,2*TMath::Pi());
      out3->RotateZ(phi);
      out2->RotateZ(TMath::Pi() + phi);
   }
   fNumLoop++;
   if (fMaxLoop<=fNumLoop) {
      SetStopLoop();
      SetEndOfRun();
   }
}


