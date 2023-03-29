/**
 * @file   TCatTrackMinuitHelper.cc
 * @brief  helper of minuit for tracking
 *
 * @date   Created       : 2016-07-22 08:53:36 JST
 *         Last Modified : 2016-09-16 15:48:37 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#include "TCatTrackMinuitHelper.h"
#include <TArtSystemOfUnit.h>
#include <TCatReadoutPadArray.h>
#include <TRangeTableHelper.h>
#include <TProcessor.h>
#include <TVector3.h>
#include <TCatPulseShape.h>

using art::TCatTrackMinuitHelper;

ClassImp(TCatTrackMinuitHelper)

TCatTrackMinuitHelper::TCatTrackMinuitHelper()
{
   fRangeTableHelper = new TRangeTableHelper;
}

TCatTrackMinuitHelper::~TCatTrackMinuitHelper()
{
   if (fRangeTableHelper) {
      delete fRangeTableHelper;
   }
}

TCatTrackMinuitHelper::TCatTrackMinuitHelper(const TCatTrackMinuitHelper& rhs)
{
}

TCatTrackMinuitHelper& TCatTrackMinuitHelper::operator=(const TCatTrackMinuitHelper& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TCatTrackMinuitHelper::Init(TEventCollection *col)
{
   // reset array and I/O for the threading
   DeleteArrays();
   BuildArrays(5);
   mninit(5,6,7);
   fRangeTableHelper->Init(col);
   fRange2EnergyLoss = fRangeTableHelper->GetR2DE();
   fWorkFunction *= TArtSystemOfUnit::eV;
}

void TCatTrackMinuitHelper::Register(TProcessor *proc)
{
   fRangeTableHelper->Register(proc);
   proc->RegisterProcessorParameter("WorkFunction","value of work function (eV)",fWorkFunction,37.);
   proc->RegisterInputInfo("ReadoutPad","name of readout pad array",fNameReadoutPadArray,TString("readoutPad"),
                           &fReadoutPadArray,TCatReadoutPadArray::Class_Name());
   
}

void TCatTrackMinuitHelper::SetParameters(Double_t x0, Double_t y0, Double_t z0, Double_t vx, Double_t vy, Double_t vz, Double_t range)
{
   Int_t ier;
   mnparm(0,"X0", x0, 2.,0.,0.,ier);
   mnparm(1,"Y0", y0, 2.,0.,0.,ier);
   mnparm(2,"Z0", z0, 2.,0.,0.,ier);
   mnparm(3,"VX", vx, 1,0.,0.,ier);
   mnparm(4,"VY", vy, 1,0.,0.,ier);
   mnparm(5,"VZ", vz, 1,0.,0.,ier);
   mnparm(6,"R", range, 10,0.,0.,ier);
}

void TCatTrackMinuitHelper::GetParameters(Double_t &x0, Double_t &y0, Double_t &z0, Double_t &vx, Double_t &vy, Double_t &vz, Double_t &range)
{
   Int_t ierr = 0;
   TString pname;
   Double_t verr,vlow,vhigh;

   mnpout(0, pname,x0,verr,vlow,vhigh,ierr);
   mnpout(1, pname,y0,verr,vlow,vhigh,ierr);
   mnpout(2, pname,z0,verr,vlow,vhigh,ierr);
   mnpout(3, pname,vx,verr,vlow,vhigh,ierr);
   mnpout(4, pname,vy,verr,vlow,vhigh,ierr);
   mnpout(5, pname,vz,verr,vlow,vhigh,ierr);
   mnpout(6, pname,range,verr,vlow,vhigh,ierr);
}
Int_t TCatTrackMinuitHelper::Eval(Int_t npar, Double_t *grad, Double_t &fval, Double_t *par, Int_t flag)
{
   Double_t x0 = par[0];
   Double_t y0 = par[1];
   Double_t z0 = par[2];
   Double_t p1x = par[3];
   Double_t p1y = par[4];
   Double_t p1z = par[5];
   Double_t range = par[6];
   TVector3 v = range * TVector3(p1x,p1y,p1z);
   fval = CalcResidual(x0,y0,z0,v.X(),v.Y(),v.z());
   return 0;
}

void TCatTrackMinuitHelper::SetData(TObjArray *input)
{
   fHits.clear();
   const Int_t n = input->GetEntriesFast(); 
   for (Int_t i=0; i<n; ++i) {
      TCatPulseShape *pulse = dynamic_cast<TCatPulseShape*>(input->UncheckedAt(i)) ;
      if ( pulse == NULL) {
         printf (Form("input pulse should inherites from %s instead of %s\n",
                             TCatPulseShape::Class_Name(),input->UncheckedAt(i)->IsA()->GetName()));

         return;
      }
      fHits.push_back(pulse);
   }
}

Double_t TCatTrackMinuitHelper::CalcResidual(Double_t x0, Double_t y0, Double_t z0, Double_t vx, Double_t vy, Double_t vz)
{
   return 0.;
}
  
