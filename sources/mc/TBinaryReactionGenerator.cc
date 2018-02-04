/* $Id:$ */
/**
 * @file   TBinaryReactionGenerator.cc
 * @date   Created : Aug 18, 2013 12:18:37 JST
 *   Last Modified : 2016-10-27 18:49:45 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TBinaryReactionGenerator.h"

#include <TCatTwoBodyKinematics.h>
#include <TH2F.h>
#include <TClonesArray.h>
#include <TF1.h>
#include <TArtSystemOfUnit.h>

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

   printf("A1 = %d, Z1 = %d\n",fP1[0],fP1[1]);
   printf("A2 = %d, Z2 = %d\n",fP2[0],fP2[1]);
   printf("A3 = %d, Z3 = %d\n",fP3[0],fP3[1]);
   printf("ex  = [%f,%f]\n",fExRange[0],fExRange[1]);
   printf("ang = [%f,%f]\n",fAngRange[0],fAngRange[1]);
   printf("L   = %d\n",fAngMom);
         

   
   fKinematics = new TCatTwoBodyKinematics(fP1[0],fP1[1],fP2[0],fP2[1],fP3[0],fP3[1]);
   fKinematics->SetIncidentEnergy(fKinMean);
   fOutputArray = new TClonesArray("TArtParticle");
   fMCTruthArray = new TClonesArray("TArtParticle");

   // prepare the distribution
   fExAngDistribution = new TH2F("hseed","hseed",nEx,fExRange[0],fExRange[1],nAng,fAngRange[0],fAngRange[1]);
//   fExAngDistribution->SetDirectory(NULL);

   TF1 *fExFun = NULL;
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
      Double_t  R = 1.2*(TMath::Power(fP1[0],1./3.) + TMath::Power(fP2[0],1./3.)); 
      for (Int_t iEx = 0; iEx!=nEx; iEx++) {
         Double_t ampl;
         Double_t ex = stepEx * iEx + fExRange[0];
         fKinematics->SetExcitationEnergy(ex);
         for (Int_t iAng = 0; iAng!=nAng; iAng++) {
            Double_t theta = fAngRange[0] + stepAng * (iAng+0.5);
            fKinematics->SetTheta(theta*deg);
            ampl = fun->Eval(fKinematics->GetDq()/197.*R);
            ampl *= ampl;
            if (fAngMom != -2) {
               ampl *= stepAng*deg*sin(theta*deg);
            }
            if (fExFun) ampl*=fExFun->Eval(ex);
            fExAngDistribution->Fill(ex,theta,ampl);
         }
      }
   }
   
   col->Add(fOutputColName,fOutputArray,fOutputIsTransparent);
   col->Add(fMCTruthColName,fMCTruthArray,fOutputIsTransparent);
}

void art::TBinaryReactionGenerator::Process()
{
   Double_t ex = 0;
   Double_t theta = 0;
   fExAngDistribution->GetRandom2(ex,theta);
   fKinematics->SetExcitationEnergy(ex);
   fKinematics->SetTheta(theta*deg);
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
   fNumLoop++;
   if (fMaxLoop<=fNumLoop) {
      SetStopLoop();
      SetEndOfRun();
   }
}


