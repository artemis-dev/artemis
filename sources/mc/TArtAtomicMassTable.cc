/* $Id:$ */
/**
 * @file   TArtAtomicMassTable.cc
 * @date   Created : Aug 04, 2011 19:04:38 JST
 *   Last Modified : Aug 17, 2011 17:45:53 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2011
 */
#include "TArtAtomicMassTable.h"

#include "TArtSystemOfUnit.h"
using namespace TArtSystemOfUnit;

#include <fstream>
using namespace std;

ClassImp(TArtAtomicMassTable);

TArtAtomicMassTable::TArtAtomicMassTable()
{
}

TArtAtomicMassTable::~TArtAtomicMassTable()
{
}

void TArtAtomicMassTable::SetMassTable(const char *filename, Int_t firstLine)
{
   fstream fin;
   char line[1024];
   Int_t ain,zin;
   Int_t    massA;
   Double_t dA;
   Double_t mass;
   const Double_t amu = 931.494 * MeV;

   for (Int_t iz = 0; iz < kNumZ; iz++) {
      for (Int_t ia = 0; ia < kNumA; ia++) {
         fIsEvaluated[iz][ia] = kFALSE;
      }
   }

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
             "%*1c%*3d%*5d%5d%5d%*1c%*3c%*4c%*1c%*13c%*11c%*11c%*9c%*1c"
             "%*2c%*11c%*9c%*1c%3d%*1c%12lf"
             ,&zin,&ain,&massA,&dA);
//      printf("%s\n",line);
//      printf("%d %d %f\n",zin,ain,(massA+dA*1.E-6)*amu);
      fIsEvaluated[zin][ain] = kTRUE;
      fMass[zin][ain] = (massA+dA*1.E-6)*amu;
   }
   Info("SetMassTable","Mass table generated from %s",filename);
   return;
}

Double_t TArtAtomicMassTable::GetAtomicMass(Int_t z, Int_t a)
{
   return fMass[z][a];
}

Double_t TArtAtomicMassTable::GetNucleusMass(Int_t z, Int_t a)
{
   Double_t electronMass = 0.511 * MeV;
   if (!fIsEvaluated[z][a]) return -1.;
   return fMass[z][a] - z * electronMass;
}
