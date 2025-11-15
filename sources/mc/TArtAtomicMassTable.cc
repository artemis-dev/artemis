/* $Id:$ */
/**
 * @file   TArtAtomicMassTable.cc
 * @date   Created : Aug 04, 2011 19:04:38 JST
 *   Last Modified : 2023-10-15 13:24:24 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2011
 */
#include "TArtAtomicMassTable.h"

#include "TArtSystemOfUnit.h"
using namespace TArtSystemOfUnit;
#include <TEnv.h>
#include <TGeoManager.h>
#include <TGeoElement.h>
#include <fstream>
#include "TObjString.h"
using namespace std;

ClassImp(TArtAtomicMassTable);

const TString TArtAtomicMassTable::kEnvName  = "Art.MassTable";
const Double_t TArtAtomicMassTable::kAtomicMassUnit = 931.494061;
const Double_t TArtAtomicMassTable::kElectronMass = 0.510999;


TArtAtomicMassTable *gAtomicMassTable = new TArtAtomicMassTable;
TArtAtomicMassTable::TArtAtomicMassTable()
   : fIsCreated(kFALSE)
{
   // assumu mass.mas03 from http://www.nndc.bnl.gov/masses/mass.mas03
//   printf("%s\n",filepath.Data());
   Build();
}

TArtAtomicMassTable::~TArtAtomicMassTable()
{
}

void TArtAtomicMassTable::Build()
{
   if (!gGeoManager) new TGeoManager;
   for (Int_t iz = 0; iz < kNumZ; ++iz) {
      for (Int_t ia = 0; ia < kNumA; ++ia) {
         if (TGeoElementRN *element = gGeoManager->GetElementTable()->GetElementRN(ia,iz)) {
            fMass[iz][ia] = kAtomicMassUnit * ia + element->MassEx();
//            printf("(%d,%d) = %f\n",iz,ia,fMass[iz][ia]);
            fIsEvaluated[iz][ia] = kTRUE;
            if (TString(element->GetName()).Contains('-')) {
               fIsotopeName[iz][ia] = Form("%d%s",ia,((TObjString*)((TObjArray*)TString(element->GetName()).Tokenize("-"))->At(1))->GetString().Data());
            } else {
               fIsotopeName[iz][ia] = Form("%c",element->GetName()[0]);
            }
         } else {
            fIsEvaluated[iz][ia] = kFALSE;
         }
      }
   }
}



void TArtAtomicMassTable::SetMassTable(const char *filename, Int_t firstLine)
{
   fstream fin;
   char line[1024];
   Int_t ain,zin;
   Int_t    massA;
   Double_t dA;
   char elm[4];
   elm[3] = '\0';

   const Double_t amu = 931.494 * MeV;

   for (Int_t iz = 0; iz < kNumZ; iz++) {
      for (Int_t ia = 0; ia < kNumA; ia++) {
         fIsEvaluated[iz][ia] = kFALSE;
      }
   }
   if (TString(filename).IsNull()) return;
   fTableName = filename;
   fin.open(filename);
   if (!fin) {
      Error("SetMassTable","File not found : %s",filename);
      return;
   }
   for (Int_t i=0; i<firstLine-1; i++) {
      fin.getline(line,1024);
   }
   while (1) {
      fin.getline(line,1024);
      if (fin.eof()) break;

      sscanf(line,
             "%*1c%*3d%*5d%5d%5d%*1c%3c%*4c%*1c%*13c%*11c%*11c%*9c%*1c"
             "%*2c%*11c%*9c%*1c%3d%*1c%12lf"
             ,&zin,&ain,elm,&massA,&dA);
//      printf("%s\n",line);
//      printf("%d %d %f\n",zin,ain,(massA+dA*1.E-6)*amu);
      fIsEvaluated[zin][ain] = kTRUE;
      fMass[zin][ain] = (massA+dA*1.E-6)*amu;
      TString name = TString(elm).Strip();
      fIsotopeName[zin][ain]= Form("%d%s",ain,name.Strip(TString::kBoth).Data());
   }

   fIsCreated = kTRUE;
//   Info("SetMassTable","Mass table generated from %s",filename);
   return;
}

Double_t TArtAtomicMassTable::GetAtomicMass(Int_t z, Int_t a)
{
   return fIsEvaluated[z][a] ? fMass[z][a] : fMass[1][1] * z + fMass[0][1] * (a - z) ;
}

Double_t TArtAtomicMassTable::GetNucleusMass(Int_t z, Int_t a)
{
   Double_t electronMass = 0.511 * MeV;
   if (!fIsEvaluated[z][a]) return fMass[1][1] * z + fMass[0][1] * (a - z) - z * electronMass;
   return fMass[z][a] - z * electronMass;
}

const char* TArtAtomicMassTable::GetIsotopeName(Int_t z, Int_t a) const
{
   if (!fIsEvaluated[z][a]) return "";
   return fIsotopeName[z][a].Data();
}
