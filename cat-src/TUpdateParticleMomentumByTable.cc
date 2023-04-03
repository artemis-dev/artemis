/**
 * @file   TUpdateParticleMomentumByTable.cc
 * @brief  adapt energy loss to particle
 *
 * @date   Created       : 2018-08-08 10:59:53 JST
 *         Last Modified : 2018-08-09 12:17:22 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#include "TUpdateParticleMomentumByTable.h"
#include <TClonesArray.h>
#include <TGraph.h>
#include <TF1.h>
#include <TArtParticle.h>
#include <TArtMathUtil.h>
#include <TArtAtomicMassTable.h>

using art::TUpdateParticleMomentumByTable;

ClassImp(TUpdateParticleMomentumByTable)

TUpdateParticleMomentumByTable::TUpdateParticleMomentumByTable()
{
   RegisterInputCollection("InputName","name of input particles to be updated",fInputName,TString("beam"),
                           &fInput, TClonesArray::Class_Name(), TArtParticle::Class_Name());
   RegisterProcessorParameter("ConverterSet",
                              "set of converter : [ zint, aint, filename, order ], file should contain pairs of incident and exit energy",
                              fConverterSet,StringVec_t());
}

TUpdateParticleMomentumByTable::~TUpdateParticleMomentumByTable()
{
}

TUpdateParticleMomentumByTable::TUpdateParticleMomentumByTable(const TUpdateParticleMomentumByTable& rhs)
{
}

TUpdateParticleMomentumByTable& TUpdateParticleMomentumByTable::operator=(const TUpdateParticleMomentumByTable& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}


void TUpdateParticleMomentumByTable::Init(TEventCollection *col)
{
   Int_t numElem = (Int_t) fConverterSet.size();
   Int_t numSet = numElem / kNumElementInConverterSet;
   if ( 0 != (numElem % kNumElementInConverterSet) ) {
      SetStateError(TString::Format("# of element in ConverterSet should be multiple of %d while %d elements given",
                                    kNumElementInConverterSet, numElem));
      return;
   }

   for (Int_t i = 0; i < numSet; ++i) {
      TString *aSet = &fConverterSet[kNumElementInConverterSet * i];
      Int_t zint = aSet[0].Atoi();
      Int_t aint = aSet[1].Atoi();
      Int_t order = aSet[3].Atoi();
      TGraph graph(aSet[2]);
      TString func = Form("pol%d",order);
      graph.Fit(func,"Q");
      TF1 *fun = graph.GetFunction(func);
      std::vector<Double_t> coeff(order + 1);
      for (Int_t io = 0; io <= order; ++io) {
         coeff[io] = fun->GetParameter(io);
      }
      fCoeff[std::pair<int,int>(zint,aint)] = coeff;
   }
                    
}

void TUpdateParticleMomentumByTable::Process()
{
   TClonesArray *input = (*fInput);
   const Int_t nHits = input->GetEntriesFast();
   if (!nHits) return;

   for (Int_t iHit = 0; iHit < nHits; ++iHit) {
      TArtParticle *particle = static_cast<TArtParticle*>(input->UncheckedAt(iHit));
      Int_t zint = particle->AtomicNumber();
      Int_t aint = particle->MassNumber();
      Double_t mass = particle->M();
      Double_t massAmu = (mass / (TArtAtomicMassTable::kAtomicMassUnit - TArtAtomicMassTable::kElectronMass)) ;
      Double_t tkeAmu = particle->TKE() / massAmu;
      CoeffMap_t::iterator it = fCoeff.find(std::pair<int,int>(zint,aint));
      if (it == fCoeff.end()) continue;
      std::vector<Double_t> &coeff = it->second;
      tkeAmu = TArtMathUtil::Polynomial(tkeAmu, &(coeff[0]), coeff.size());
      particle->SetKineticEnergy(tkeAmu * massAmu,mass);
   }
}
