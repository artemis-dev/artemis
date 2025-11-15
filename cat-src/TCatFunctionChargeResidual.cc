/**
 * @file   TCatFunctionChargeResidual.cc
 * @brief  chi2 function
 *
 * @date   Created       : 2016-09-18 07:33:26 JST
 *         Last Modified : 2020-09-03 14:38:33 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#include "TCatFunctionChargeResidual.h"
#include <TCatDiffusedChargeResponseHelper.h>
#include <TVector3.h>
#include <TCatPulseShape.h>
#include <TRangeTableHelper.h>
#include <numeric>
#include <TMath.h>

using art::TCatFunctionChargeResidual;

// ClassImp(TCatFunctionChargeResidual)

TCatFunctionChargeResidual::TCatFunctionChargeResidual()
{
   fVerboseLevel = 0;
}

TCatFunctionChargeResidual::~TCatFunctionChargeResidual()
{
}

TCatFunctionChargeResidual::TCatFunctionChargeResidual(const TCatFunctionChargeResidual& rhs)
{
}

TCatFunctionChargeResidual& TCatFunctionChargeResidual::operator=(const TCatFunctionChargeResidual& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}


double TCatFunctionChargeResidual::DoEval(const double *x) const
{
   const int n = fHits.size();
   TVector3 start(x[0],x[1],x[2]);
   TVector3 end;
   if (fUseRange) {
      end = x[6] * fRangeUnit * TVector3(x[3],x[4],x[5]).Unit() + start;
   } else {
      end.SetXYZ(x[3],x[4],x[5]);
//      end = TVector3(x[3],x[4],x[5]) + start;
   }
    
//   printf("x[6] = %f , fRangeUnit = %f",x[6],fRangeUnit);
   fResponseHelper->SetTrack(start,end);
//   start.Print();
//   end.Print();
   if (fUseTotalCharge) {
      Double_t closeToStart = TMath::Limits<Double_t>::Max();
      Double_t closeToEnd   = TMath::Limits<Double_t>::Min();
      Double_t sum = 0;
      Double_t range = (end-start).Mag();
      for (Int_t i = 0; i < n; i++) {
         double charge, closest, angle, rangeToEnd, trackLength;
         fResponseHelper->GetResponse(fHits[i]->GetID(),charge,closest,angle,rangeToEnd, trackLength);
         sum += fHits[i]->GetCharge();
         if (closeToStart > rangeToEnd) {
            closeToStart = rangeToEnd;
         }
         if (closeToEnd < rangeToEnd) {
            closeToEnd = rangeToEnd;
         }
      }
      Double_t estimation = 0;
      const Double_t step = 0.1;
      const Int_t nStep = (closeToEnd - closeToStart)/step;
      for (Int_t i = 0.; i < nStep; ++i) {
         Double_t pos = closeToStart + step * i;
         estimation += fResponseHelper->GetRangeTableHelper()->R2DE(pos);
      }
      estimation *= step;
//      printf("charge = %10.5f estimation %10.5f, closeToStart= %f, closeToEnd = %f\n",sum,estimation,closeToStart,closeToEnd);
      return TMath::Power(sum - estimation,2);
   }
   
   std::vector<double> diff(n);
   if (TMath::IsNaN(start.Mag()) || TMath::IsNaN(end.Mag())) {
//      printf("unexpected\n");
      return 0;
   }

   double qsum = 0.;
   for (Int_t i = 0; i < n; i++) {
      qsum += fHits[i]->GetCharge() / 37e-6;
   }
   

   double diff_sum = 0;
   
   for (Int_t i = 0; i < n; i++) {
      double charge, closest, angle, rangeToEnd, trackLength;
      if (fHits[i]->GetCharge() <= 0 ) continue;
      
      fResponseHelper->GetResponse(fHits[i]->GetID(),charge,closest,angle,rangeToEnd, trackLength);
      double qcalc = TMath::Abs(charge / 37.e-6);
      double qmeas = TMath::Abs(fHits[i]->GetCharge() / 37.e-6);
#if 0      
      double arg = - 2.404  *(closest - 5);
#else
      double hitY = fHits[i]->Y();
      double arg = - (1.6) * (closest - (4.665 + 0.01028 * hitY));
#endif         
      Double_t diff_distance = -2 * (arg - TMath::Log(1+TMath::Exp(arg)));
      
//      printf("%d: id = %d, distance = %f, charge = %f\n",i,fHits[i]->GetID(), closest, qcalc);

//      printf("[%d] closest = %f, hitY = %f, diff_distance = %f\n",i,closest, hitY, diff_distance);
      
      
      
      // log liklihood
//      Double_t s2 = TMath::Abs(fSigmaCoeff[0]*fSigmaCoeff[0]+fSigmaCoeff[1]*fSigmaCoeff[1]*qmeas + fSigmaCoeff[2]*fSigmaCoeff[2]*qmeas*qmeas);
      Double_t s2 = TMath::Abs(fSigmaCoeff[0]*fSigmaCoeff[0]+fSigmaCoeff[1]*fSigmaCoeff[1]*qcalc + fSigmaCoeff[2]*fSigmaCoeff[2]*qcalc*qcalc);
      if (0) { 
         diff[i] = - (TMath::Log(qcalc) * qmeas - qmeas * TMath::Log(qmeas) + qmeas - qcalc);
      } else if (0) {
         Double_t s2 = TMath::Abs(fSigmaCoeff[0]*fSigmaCoeff[0]+fSigmaCoeff[1]*fSigmaCoeff[1]*qcalc + fSigmaCoeff[2]*fSigmaCoeff[2]*qcalc*qcalc);
         Double_t w = 1 - TMath::Sqrt(qcalc / s2);
         Double_t r = qcalc * ( 1 - w);
         w = TMath::Max(TMath::Max(-1., -r/4),w);
         r = qcalc * ( 1 - w );
         
         diff[i] = - (TMath::Log(r) + (qmeas-1) * TMath::Log(r + w * qmeas) - qmeas * TMath::Log(qmeas) + qmeas - (r + w * qmeas));
      } else if (qcalc < 1.) {
        diff[i] = (qmeas * qmeas)/ s2;
//         printf("qcalc = %f, qmeas = %f diff[%d] = %f\n",qcalc,qmeas,i,diff[i]);
      } else {
         diff[i] = ((qcalc - qmeas) * (qcalc - qmeas))/s2;
      }
//      diff[i] += TMath::Power(diff_distance,2);
      diff[i] += diff_distance;
      
//      diff[i] += TMath::Power(closest/10.,4)/10.;
#if 0      
      if (fVerboseLevel > 2) {
         printf("id = %03d meas = %10.1f calc = %10.1f diff = %10.5f closest = %10.5f\n",fHits[i]->GetID(),qmeas,qcalc,diff[i],closest);
      }
#endif      
   }

   double chi2 = std::accumulate(diff.begin(),diff.end(),0.);
//   printf("chi2 = %f\n",chi2);
#if 0
      printf("%d\n",fVerboseLevel);
      printf("[DoEval] chi2 = %10.5f, range : x0 = %f, y0 = %f, z0 = %f, vx = %f, vy = %f, vz = %f,range = %f\n",chi2, x[0],x[1],x[2],x[3],x[4],x[5],x[6]);
#endif   
   return chi2;
}

double TCatFunctionChargeResidual::GetCharge(int id) const {
   return fResponseHelper->GetCharge(id);
}

ROOT::Math::IBaseFunctionMultiDim* TCatFunctionChargeResidual::Clone() const {
   TCatFunctionChargeResidual *clone = new TCatFunctionChargeResidual;
   clone->SetHits(fHits);
   clone->SetResponseHelper(fResponseHelper);
   clone->SetRangeUnit(GetRangeUnit());
   clone->SetUseTotalCharge(fUseTotalCharge);
   clone->SetUseRange(fUseRange);
   clone->SetSigmaCoeff(fSigmaCoeff);
   clone->SetVerboseLevel(0);
   return clone;
}
