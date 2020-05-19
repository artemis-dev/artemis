/**
 * @file   TNArray.cc
 * @brief
 *
 * @date   Created       : 2016-01-29 14:16:43 JST
 *         Last Modified : 2020-05-07 18:43:03 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#include "TNArray.h"
#include "TMatrixD.h"
#include "TVectorD.h"
#include "TDecompChol.h"

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
   std::vector<Int_t> idx(fNumVars);
   for (Int_t i = 0; i!=fNumVars; ++i) {
      if (x[i] < fVars[i].GetMin()) {
         idx[i] = 0;
      } else if (fVars[i].GetMax() < x[i]) {
         idx[i] = fVars[i].GetNumVals() - 1;
      } else {
         idx[i] = fVars[i].IndexI(x[i]);
      }
      baseIndex += idx[i] * fTotalNumVals[i];
   }
#if 0
   printf("baseIndex = %d\n",baseIndex);
#endif   
      
   Double_t reference = retval  = fValues[baseIndex];
   for (Int_t i = 0; i!=fNumVars; ++i) {
      Double_t diff = 0;
      Int_t nDiff = 0.;

      if (idx[i] < fVars[i].GetNumVals() - 1 && baseIndex + fTotalNumVals[i] < fTotalNumVals[0] * fVars[0].GetNumVals()) {
         diff += (fValues[baseIndex + fTotalNumVals[i]] - reference);
         nDiff++;
      }
      
      if (idx[i] < fVars[i].GetNumVals() - 2 && baseIndex + 2 * fTotalNumVals[i] < fTotalNumVals[0] * fVars[0].GetNumVals() ) {
         diff += (fValues[baseIndex + 2 * fTotalNumVals[i]] - fValues[baseIndex + fTotalNumVals[i]]);
         nDiff ++;
      }

      if (idx[i] > 1) {
         diff += (fValues[baseIndex] - fValues[baseIndex - fTotalNumVals[i]]);
         nDiff ++;
      }
         
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
   return retval;
}


void TNArray::CalcBilinearWeights(double dx, std::vector<double>& weight) {
   weight.resize(2);
   weight[0] = 1 - dx;
   weight[1] = dx;
}

void TNArray::CalcBicubicWeights(double dx, std::vector<double>& weight, double param) {
   weight.resize(4);
   double d = 1 + dx;
   weight[0] = param * (d * d * d - 5 * d * d + 8 * d - 4);
   d = dx;
   weight[1] = (param + 2) * d * d * d - (param + 3) * d * d + 1;
   d = 1 - dx;
   weight[2] = (param + 2) * d * d * d - (param + 3) * d * d + 1;
   d= 2 - dx;
   weight[3] = param * (d * d * d - 5 * d * d + 8 * d - 4);
}

double TNArray::WeightedSum(int ipar, double w, int idxbase,
                            const std::vector< std::vector<double> > &weight,
                            const std::vector< std::vector<int> > &indexes)
{
   double sum = 0.;
   for (int i = 0, n = weight[ipar].size(); i < n; ++i) {
      double this_w = w * weight[ipar][i];
      int idx = idxbase + indexes[ipar][i] * fTotalNumVals[ipar];
      if (ipar != 0) {
         sum += WeightedSum(ipar - 1,this_w,idx, weight, indexes);
      } else {
//         printf("ipar = %d, idx = %10d, w = %10.5f, wprod = %10.5f\n",ipar,idx,weight[ipar][i],w);
         
         sum += this_w * fValues[idx];
//         printf("ipar = %d, idx = %10d, w = %10.5f, wprod = %10.5f, value = %f\n",ipar,idx,weight[ipar][i],w,fValues[idx]);
         
      }
//      printf("ipar = %2d, ip = %2d, w = %10.5f, sum = %10.5f\n",ipar,i, weight[ipar][i], sum);
   }
   return sum;
}


Double_t TNArray::Eval2(Double_t *x)
{
//   Int_t baseIndex = 0;
//   Double_t retval = 0.;
   std::vector<Int_t> idx(fNumVars);
   std::vector<double> deriv(fNumVars);
   std::vector< std::vector< double > > weight(fNumVars);
   std::vector< std::vector< int > > indexes(fNumVars);
   
   for (Int_t i = 0; i!=fNumVars; ++i) {
      if (x[i] < fVars[i].GetMin()) {
         return TMath::QuietNaN();
      } else if (fVars[i].GetMax() < x[i]) {
         return TMath::QuietNaN();
      }
      idx[i] = fVars[i].IndexI(x[i]);
      deriv[i] = fVars[i].Derivative(x[i]);
//      printf("ipar = %d, idx = %d, deriv = %f\n",i,idx[i],deriv[i]);


      if (0 < idx[i] && idx[i] + 2 < fVars[i].GetNumVals()) {
         CalcBicubicWeights(deriv[i],weight[i]);
         indexes[i].push_back(idx[i] - 1);
         indexes[i].push_back(idx[i]);
         indexes[i].push_back(idx[i] + 1);
         indexes[i].push_back(idx[i] + 2);
      } else if (0 == idx[i] || idx[i] + 1 < fVars[i].GetNumVals()) {
         CalcBilinearWeights(deriv[i],weight[i]);
         indexes[i].push_back(idx[i]);
         indexes[i].push_back(idx[i] + 1);
      } else if (idx[i] + 1 == fVars[i].GetNumVals()) {
         weight[i].push_back(1.);
         indexes[i].push_back(idx[i]);
      }
   }

   return WeightedSum(fNumVars - 1, 1., 0, weight, indexes);
   
#if 0
   printf("baseIndex = %d\n",baseIndex);
   Double_t reference = retval  = fValues[baseIndex];
      
   for (Int_t i = 0; i!=fNumVars; ++i) {
      Double_t diff = 0;
      Int_t nval = 1;
      int idxdiff = 1;
      const int npt = 8;
      double node[npt];
      double node2[npt];
      double node3[npt];
      double node4[npt];
      double val[npt];

      node[0] = 0;
      node2[0] = 0;
      val[0] = 0.;
      
      

      while (nval < npt) {
         if (idx[i] + idxdiff < fVars[i].GetNumVals()) {
            node[nval] = idxdiff;
            node2[nval] = idxdiff * idxdiff;
            node3[nval] = idxdiff * idxdiff * idxdiff;
            node4[nval] = idxdiff * idxdiff * idxdiff * idxdiff;
            val[nval] = fValues[baseIndex + idxdiff * fTotalNumVals[i]] - reference;
            nval++;
            if (nval >= npt) break;
         }
         if (idx[i] - idxdiff > 0) {
            node[nval] = - idxdiff;
            node2[nval] = idxdiff * idxdiff;
            node3[nval] = idxdiff * idxdiff * idxdiff;
            node4[nval] = idxdiff * idxdiff * idxdiff * idxdiff;
            val[nval] = fValues[baseIndex - idxdiff * fTotalNumVals[i]] - reference;
            nval++;
         }
         idxdiff++;
      }
      TVectorD x; x.Use(npt,node);
      TVectorD x2; x2.Use(npt,node2);
      TVectorD x3; x3.Use(npt,node3);
      TVectorD x4; x4.Use(npt,node4);
      TVectorD y;  y.Use(npt,val);
      TMatrixD A(npt,5); // 4th order polynomial;
      TMatrixDColumn(A,0) = 1.0;
      TMatrixDColumn(A,1) = x;
      TMatrixDColumn(A,2) = x2;
      TMatrixDColumn(A,3) = x3;
      TMatrixDColumn(A,4) = x4;
      const TVectorD c_norm = NormalEqn(A,y);
      const double deriv = fVars[i].Derivative(x[i]);
      
      retval += c_norm[0] + deriv * (c_norm[1] + deriv * (c_norm[2] + deriv * (c_norm[3] + deriv * (c_norm[4]))));
      
#if 0
      for (int i = 0; i < npt; ++i) {
         printf("[%d] node = %5.1f, node2 = %5.1f, val = %10.5f\n",i,node[i],node2[i],val[i]);
      }
      A.Print();
      y.Print();
      c_norm.Print();
#endif
      
#if 0
      printf("diff[%d] = %f %f %f @ idx=%d\n",i,diff,fVars[i].Derivative(x[i]),fValues[baseIndex+fTotalNumVals[i]],baseIndex+fTotalNumVals[i]);
      printf("   min = %g\n",fVars[i].GetMin());
      printf("   x-min = %g step = %g\n",x[i]-fVars[i].GetMin(),fVars[i].GetStep());
      printf("   x-min = %g\n",((x[i]-fVars[i].GetMin())*(1./fVars[i].GetStep())));
      printf("   x-min = %d\n",(Int_t)((x[i]-fVars[i].GetMin())*(1./fVars[i].GetStep())));
      printf(" IndexI = %d\n",fVars[i].IndexI(x[i]));
      
#endif         
   }
   return retval;
#endif   
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
            printf(" %.20e / %.20f = %.20e => %.20e\n",(vars[iVar]-fVars[iVar].GetMin()),fVars[iVar].GetStep(),(vars[iVar]-fVars[iVar].GetMin())/fVars[iVar].GetStep(),
                   TMath::Floor((vars[iVar]-fVars[iVar].GetMin())/(fVars[iVar].GetStep())));
            printf("index = %d\n",fVars[iVar].IndexI(vars[iVar]));
            
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
