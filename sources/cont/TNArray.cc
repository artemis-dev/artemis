/**
 * @file   TNArray.cc
 * @brief
 *
 * @date   Created       : 2016-01-29 14:16:43 JST
 *         Last Modified : 2016-10-24 20:16:12 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#include "TNArray.h"

using art::TNArray;

ClassImp(TNArray)

TNArray::TNArray()
{
   fValues = NULL;
}
TNArray::TNArray(const char* name, const char* title, const char* varlist, Int_t bufsize)
   : TNtupleD(name,title,varlist,bufsize), fNumVars(0)
{
   fValues = NULL;
}

TNArray::~TNArray()
{
   if (fValues) {
      delete [] fValues;
      fValues = NULL;
   }
}

TNArray::TNArray(const TNArray& rhs)
{
}

TNArray& TNArray::operator=(const TNArray& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}


void TNArray::Add(const char* name, Double_t min, Double_t max, Int_t num)
{
   fVars.push_back(Variable(name,(max-min)/(num-1),min,max,num));
   fNumVars++;
}

Double_t TNArray::Eval(Double_t *x)
{
   Int_t baseIndex = 0;
   Double_t retval = 0.;
   std::vector<Double_t> deltaEnergy(fNumVars,0.);
   for (Int_t i = 0; i!=fNumVars; ++i) {
      if (!fVars[i].CheckBounce(x[i])) {
         return TMath::QuietNaN();
      }
      baseIndex += fVars[i].IndexI(x[i]) * fTotalNumVals[i];
   }
#if 0
   printf("baseIndex = %d\n",baseIndex);
#endif   
   Double_t reference = retval  = fValues[baseIndex];
   for (Int_t i = 0; i!=fNumVars; ++i) {
      if (baseIndex + fTotalNumVals[i] < fTotalNumVals[0] * fVars[0].GetNumVals()) {
         Double_t diff = (fValues[baseIndex + fTotalNumVals[i]] - reference) * fVars[i].Derivative(x[i]);
         retval += diff;
#if 0
         printf("diff[%d] = %f %f %f @ idx=%d\n",i,diff,fVars[i].Derivative(x[i]),fValues[baseIndex+fTotalNumVals[i]],baseIndex+fTotalNumVals[i]);
         printf("   min = %g\n",fVars[i].GetMin());
         printf("   x-min = %g step = %g\n",x[i]-fVars[i].GetMin(),fVars[i].GetStep());
         printf("   x-min = %g\n",((x[i]-fVars[i].GetMin())*(1./fVars[i].GetStep())));
         printf("   x-min = %d\n",(Int_t)((x[i]-fVars[i].GetMin())*(1./fVars[i].GetStep())));
         printf(" IndexI = %d\n",fVars[i].IndexI(x[i]));

#endif         
      }
   }
   return retval;
}

void TNArray::Load()
{
   Int_t nEntries = GetEntries();
   Int_t modulo = nEntries/1000;
   // nVars is the number of index variables (except for the array value)
   Int_t nVars = GetNvar() - 1;
   Double_t *vars = GetArgs();
   Int_t index = 0;
   std::vector<Int_t> used(nEntries,0);
   if (fNumVars != nVars) {
      Error("Load","fNumVars (%d) differes from # of vars in tree (%d)",fNumVars,nVars);
      return;
   }
   fTotalNumVals.resize(fNumVars,1);
   for (Int_t i = 0; i!=fNumVars; ++i) {
      for (Int_t j = i+1; j != fNumVars; ++j) {
         fTotalNumVals[i] *= fVars[j].GetNumVals();
      }
   }
   if (nEntries != fTotalNumVals[0] * fVars[0].GetNumVals()) {
      Error("Load","Inconsistent numver of values: nEntries = %d, total = %d",nEntries, fTotalNumVals[0] * fVars[0].GetNumVals());
      return;
   }

   Info("Load","Loading from ntuple (%.5f Mbytes)... please wait for a while",fTotalNumVals[0] * fVars[0].GetNumVals() * sizeof(Double_t) / 1024. / 1024.);

   fValues = new Double_t[fTotalNumVals[0] * fVars[0].GetNumVals()];

   for (Int_t iEntry = 0; iEntry!= nEntries; ++iEntry) {
      GetEntry(iEntry);
      index = 0;
      for (Int_t iVar = 0; iVar != nVars; ++iVar) {
         index += fVars[iVar].IndexI(vars[iVar]) * fTotalNumVals[iVar];
      }
      if (used[index]) {
         printf("index = %d used\n",index);
         for (Int_t iVar = 0; iVar != nVars; ++iVar) {
            printf("iVar = %d vars[%d] = %.10g IndexI = %d\n",iVar,iVar,vars[iVar],fVars[iVar].IndexI(vars[iVar]));
            printf(" %.20e / %.20f = %.20e => %.20e\n",vars[iVar],fVars[iVar].GetStep(),vars[iVar]/fVars[iVar].GetStep(),
                   TMath::Floor(((Float_t)vars[iVar])/((Float_t)fVars[iVar].GetStep())));
         }
         return;
      }
      used[index] = 1;
      fValues[index] = vars[nVars];
      if (!(iEntry % modulo)) {
         char  progress[12];
         Int_t ii;
         for (ii = 0; ii < TMath::Floor(iEntry*10/nEntries); ii++) {
            progress[ii] = '=';
         }
         progress[ii] = '>';
         progress[ii+1] = '\0';
         fprintf(stderr,"\r[%-11s] %15d/%d",progress,iEntry,nEntries);
         fflush(stderr);
      }
   }
   fprintf(stderr,"\r"); fflush(stderr);
   Info("Load","loaded %d/%d",nEntries,nEntries);

   
}

TNArray::Variable::Variable(const char* name, Double_t step, Double_t min, Double_t max, Int_t num)
   : TNamed(name,name), fStep(step), fMin(min), fMax(max), fNumVals(num)
{
}
