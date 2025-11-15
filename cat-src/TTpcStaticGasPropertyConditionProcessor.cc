/* @file TTpcStaticGasPropertyConditionProcessor.cc
 * @brief prepare condition of static gas property of tpc
 *
 * @date Create        : 2020-04-23 16:04:06 JST
 *       Last Modified : 2020-05-21 18:22:29 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */


#include "TTpcStaticGasPropertyConditionProcessor.h"
#include "TArtSystemOfUnit.h"

ClassImp(art::TTpcStaticGasPropertyConditionProcessor)

using art::TTpcStaticGasPropertyConditionProcessor;
using TArtSystemOfUnit::mm;
using TArtSystemOfUnit::us;
using TArtSystemOfUnit::ns;
using TArtSystemOfUnit::cm;
using TArtSystemOfUnit::eV;


TTpcStaticGasPropertyConditionProcessor::TTpcStaticGasPropertyConditionProcessor()
{
   Register(fProperty("PropertyName","name of tpc static gas property","tpcGasProperty"));
   Register(fDriftVelocity("DriftVelocity","drift velocity in cm/us",1.));
   Register(fTransDiffCoeff("TransDiffCoeff","transverse diffusion coefficient (cm^{1/2})",0.04));
   Register(fLongDiffCoeff("LongDiffCoeff","longitudinal diffusion coefficient (cm^{1/2})",0.02));
   Register(fConstTransDiff("ConstTransDiff","constant transverse diffusion (mm)",0.4));
   Register(fConstLongDiff("ConstLongDiff","constant longitudinal diffusion (mm)",0.2));
   Register(fAttachCoeff("AttachCoeff","attachment coefficient (1/cm)",0.));
   Register(fWorkFunction("WorkFunction","WorkFunction (eV)",37.));
}

TTpcStaticGasPropertyConditionProcessor::~TTpcStaticGasPropertyConditionProcessor()
{
}

void TTpcStaticGasPropertyConditionProcessor::Init(TEventCollection* col)
{
   TTpcStaticGasProperty *prop = new TTpcStaticGasProperty;
   prop->SetDriftVelocity(fDriftVelocity * cm / us);
   prop->SetTransDiffCoeff(fTransDiffCoeff * TMath::Sqrt(cm));
   prop->SetLongDiffCoeff(fLongDiffCoeff * TMath::Sqrt(cm));
   prop->SetConstTransDiff(fConstTransDiff);
   prop->SetConstLongDiff(fConstLongDiff);
   prop->SetAttachCoeff(fAttachCoeff / cm);
   prop->SetWorkFunction(fWorkFunction * eV);
   fProperty.SetData(prop);
   prop->SetName(fProperty.Value());
   
   col->AddInfo(prop->GetName(),prop,fOutputIsTransparent);
}

