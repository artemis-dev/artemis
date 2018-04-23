/**
 * @file   TNArray.cc
 * @brief
 *
 * @date   Created       : 2016-01-29 14:16:43 JST
 *         Last Modified : 2018-04-23 23:13:31 JST (ota)
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
   for (Int_t i = 0; i!=fNumVars; ++i) {
      Int_t idx;
      if (x[i] < fVars[i].GetMin()) {
         idx = 0;
      } else if (fVars[i].GetMax() < x[i]) {
         idx = fVars[i].IndexI(fVars[i].GetMax() - TMath::Limits<Double_t>::Epsilon()) - 1.;
      } else {
         idx = fVars[i].IndexI(x[i]);
      }
      baseIndex += idx * fTotalNumVals[i];
   }
#if 0
   printf("baseIndex = %d\n",baseIndex);
#endif   
#if 0
   // calculate value and distance
   std::vector<Double_t> values, distances;
   const Int_t nVertex = TMath::Power(2,fNumVars);   
#if 0   
   std::vector<Int_t> used(fNumVars,0.);
   const Int_t nVertex = TMath::Power(2,fNumVars);
   Int_t idxdiff = 0;
   idxdiff = 0;
   idxdiff = fTotalNumVals[0];
   idxdiff = fTotalNumVals[1];
   idxdiff = fTotalNumVals[2];
   idxdiff = fTotalNumVals[0] + fTotalNumVals[1];
   idxdiff = fTotalNumVals[1] + fTotalNumVals[2];
   idxdiff = fTotalNumVals[2] + fTotalNumVals[0];
   idxdiff = fTotalNumVals[0] + fTotalNumVals[1] + fTotalNumVals[2];
#endif
   // dimension
   Double_t value = 0;
   Double_t weight = 0;
   Double_t totaldiff = 0.;
   for (Int_t i = 0; i < nVertex; ++i) {
      std::vector<Int_t> use(fNumVars,0);
      Int_t idxdiff = 0;
      Double_t dist = 0.;
      for (Int_t j = 0; j < fNumVars; ++j) {
         if (( i & (1 << j))) {
            idxdiff += fTotalNumVals[j];
            use[j] = 1;
         } 
      }
      if (baseIndex + idxdiff >= fTotalNumVals[0] * fVars[0].GetNumVals()) {
//         printf("x[0] = %f, x[1] = %f, x[2] = %f, baseIndex = %d, i = %d, idxdiff = %d\n",x[0],x[1],x[2],baseIndex, i,idxdiff);
         continue;
      }
#if 0      
      for (Int_t j = 0; j < fNumVars; ++j) {
         Double_t der = (use[j] ? 1 - fVars[j].Derivative(x[j]) : fVars[j].Derivative(x[j]));
         dist += der * der;
      }
//      dist = TMath::Sqrt(dist);
//      printf("dist = %f\n",dist);
      if (dist < TMath::Limits<Double_t>::Epsilon()) {
         value = fValues[baseIndex + idxdiff];
         weight = 1;
         break;
      }
      value += fValues[baseIndex + idxdiff] / dist;
      weight += 1./dist;
      values.push_back(fValues[baseIndex + idxdiff]);
      distances.push_back(TMath::Sqrt(dist));
#endif
      Double_t multideriv = 1.;
      for (Int_t j = 0; j < fNumVars; ++j) {
         if (!use[j]) continue;
         multideriv *=  fVars[j].Derivative(x[j]);
      }
      totaldiff += (fValues[baseIndex + idxdiff] - fValues[baseIndex]) * multideriv;
   }
   value /= weight;
   value = fValues[baseIndex] + totaldiff;

   return value;

   printf("%f \n",value);
#endif
      
   

   Double_t reference = retval  = fValues[baseIndex];
   for (Int_t i = 0; i!=fNumVars; ++i) {
       if (baseIndex + fTotalNumVals[i] < fTotalNumVals[0] * fVars[0].GetNumVals()) {
          Double_t diff = (fValues[baseIndex + fTotalNumVals[i]] - reference) ;
          Int_t nDiff = 1;
#if 1          
          if (baseIndex - fTotalNumVals[i] > 0) {
             diff += (fValues[baseIndex] - fValues[baseIndex - fTotalNumVals[i]]);
             nDiff ++;
          }
          if (baseIndex + 2* fTotalNumVals[i] < fTotalNumVals[0] * fVars[0].GetNumVals()) {
             diff += (fValues[baseIndex + 2 * fTotalNumVals[i]] - fValues[baseIndex + fTotalNumVals[i]]);
             nDiff ++;
          }
#endif          
          diff *= fVars[i].Derivative(x[i]) / nDiff;
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
         printf("index = %d used for entry %d\n",index,iEntry);
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
