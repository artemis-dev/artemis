/**
 * @file   TAttTreeProj.cc
 * @brief  Attribute to fill the tree projection
 *
 * @date   Created       : 2014-03-03 23:30:16 JST
 *         Last Modified : 2020-04-30 11:26:26 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#include "TAttTreeProj.h"
#include <TH3.h>
#include <TH2.h>
#include <TH1.h>
#include <TTreeFormulaManager.h>
#include <TFormLeafInfo.h>
#include <TAxisTreeProj.h>
#include <algorithm>

using art::TAttTreeProj;

ClassImp(TAttTreeProj)
namespace {
   template <typename T>
   struct delete_ptr {
      void operator() (T * p) {
         delete p;
      }
   };
}

TAttTreeProj::TAttTreeProj()
   : fH(NULL),fAxisAsync(kFALSE), fNeedSync(kTRUE)
{
}
TAttTreeProj::TAttTreeProj(TH1* h, const char *cut)
   :  TAttCut(cut), fH(h), fAxisAsync(kFALSE), fNeedSync(kTRUE)
{
}

TAttTreeProj::~TAttTreeProj()
{
   // Deletion of TTreeFormulaManager is the sole responsability of the TTreeFormulas
   // only TAxisTreeProj is deleted
   std::for_each(fAxes.begin(),fAxes.end(),delete_ptr<TAxisTreeProj>());
}

TAttTreeProj::TAttTreeProj(const TAttTreeProj& rhs)
{
}

TAttTreeProj& TAttTreeProj::operator=(const TAttTreeProj& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TAttTreeProj::Copy(TAttTreeProj &newatt) const
{
   newatt.fAxisAsync = fAxisAsync;
   newatt.fNeedSync  = kTRUE;
   TAttCut::Copy(newatt);
   Int_t n = fAxes.size();
   newatt.fAxes.clear();
   for (Int_t i=0; i!=n; i++) {
      newatt.fAxes.push_back((TAxisTreeProj*)fAxes[i]->Clone());
   }
   
}


void TAttTreeProj::FillTo(TH1* hist)
{
   TTreeFormulaManager *man = fManagers[0];
   TAxisTreeProj *xaxis = fAxes[0];
   const Int_t &nData = man->GetNdata();
   for (Int_t iData=0; iData!=nData; iData++) {
      if (!xaxis->EvalSelection(iData)) continue;
      hist->Fill(xaxis->EvalVariable(iData),xaxis->EvalSelection(iData));
   }
}

void TAttTreeProj::FillTo(TH2* hist)
{
   if (!fAxisAsync) {
      TAxisTreeProj *xaxis = fAxes[0];
      TAxisTreeProj *yaxis = fAxes[1];
      Int_t nData = TMath::Min(xaxis->GetNdata(),yaxis->GetNdata());
      for (Int_t iData=0; iData!=nData; iData++) {
         if (!xaxis->EvalSelection(iData)) continue;
         if (!yaxis->EvalSelection(iData)) continue;
         hist->Fill(xaxis->EvalVariable(iData),
                    yaxis->EvalVariable(iData),xaxis->EvalSelection(iData));
      }
   } else {
      // asynchronous fill
      TTreeFormulaManager *xman = fManagers[0];
      TTreeFormulaManager *yman = fManagers[1];
      TAxisTreeProj *xaxis = fAxes[0];
      TAxisTreeProj *yaxis = fAxes[1];
      const Int_t &nDataX = xman->GetNdata();
      const Int_t &nDataY = yman->GetNdata();
      for (Int_t iDataX = 0; iDataX != nDataX; iDataX++) {
         if (!xaxis->EvalSelection(iDataX)) continue;
         for (Int_t iDataY = 0; iDataY != nDataY; iDataY++) {
            if (!yaxis->EvalSelection(iDataY)) continue;
            hist->Fill(xaxis->EvalVariable(iDataX),
                       yaxis->EvalVariable(iDataY),xaxis->EvalSelection(iDataX));
         }
      }
   }
}


void TAttTreeProj::FillTo(TH3* hist)
{
   if (!fAxisAsync) {
      TAxisTreeProj *xaxis = fAxes[0];
      TAxisTreeProj *yaxis = fAxes[1];
      TAxisTreeProj *zaxis = fAxes[2];
      Int_t nData = TMath::Min(xaxis->GetNdata(),yaxis->GetNdata());
      nData = TMath::Min(nData,zaxis->GetNdata());
      for (Int_t iData=0; iData!=nData; iData++) {
         if (!xaxis->EvalSelection(iData)) continue;
         if (!yaxis->EvalSelection(iData)) continue;
         if (!zaxis->EvalSelection(iData)) continue;
         hist->Fill(xaxis->EvalVariable(iData),
                    yaxis->EvalVariable(iData),
                    zaxis->EvalVariable(iData),xaxis->EvalSelection(iData));
      }
   } else {
      // asynchronous fill
      TTreeFormulaManager *xman = fManagers[0];
      TTreeFormulaManager *yman = fManagers[1];
      TTreeFormulaManager *zman = fManagers[2];
      TAxisTreeProj *xaxis = fAxes[0];
      TAxisTreeProj *yaxis = fAxes[1];
      TAxisTreeProj *zaxis = fAxes[2];
      const Int_t &nDataX = xman->GetNdata();
      const Int_t &nDataY = yman->GetNdata();
      const Int_t &nDataZ = zman->GetNdata();
      for (Int_t iDataX = 0; iDataX != nDataX; iDataX++) {
         if (!xaxis->EvalSelection(iDataX)) continue;
         for (Int_t iDataY = 0; iDataY != nDataY; iDataY++) {
            if (!yaxis->EvalSelection(iDataY)) continue;
            for (Int_t iDataZ = 0; iDataZ != nDataZ; iDataZ++) {
               if (!zaxis->EvalSelection(iDataZ)) continue;
               hist->Fill(xaxis->EvalVariable(iDataX),
                          yaxis->EvalVariable(iDataY),
                          zaxis->EvalVariable(iDataZ),xaxis->EvalSelection(iDataX));
            }
         }
      }
   }
}


void TAttTreeProj::SetAxisDef(UInt_t i, TAxisTreeProj* a) {
   TAxis *axis = NULL;
   switch(i) {
   case 0:
      // x-axis
      axis = fH->GetXaxis();
      break;
   case 1:
      axis = fH->GetYaxis();
      break;
   case 2:
      axis = fH->GetZaxis();
      break;
   default:
      break;
   }
   if (!axis) return;
   if (fAxes.size() <= i) fAxes.resize(i+1);
   fAxes[i] = a;
   axis->Set(a->GetNbins(),a->GetXmin(),a->GetXmax());
   axis->SetTitle(a->GetTitle());
   
}


void TAttTreeProj::Sync()
{
   if (!fNeedSync) return;
   fH->Rebuild();
   Int_t nDim = fH->GetDimension();
   TTreeFormulaManager *man;
   if (!fAxisAsync) man = new TTreeFormulaManager;
   
   for (Int_t i = 0; i!=nDim; i++) {
      if (fAxisAsync) man = new TTreeFormulaManager;
      TAxisTreeProj *axis = fAxes[i];
      if (!axis->GetVariableFormula()) {
         printf("TAisTreeProj (%s) is not initialized correctly\n",axis->GetTitle());
         return;
      }
      man->Add(axis->GetVariableFormula());
      
      TAxis *histaxis = NULL;
      switch (i) {
      case 0:
         histaxis = fH->GetXaxis();
         break;
      case 1:
         histaxis = fH->GetYaxis();
         break;
      case 2:
         histaxis = fH->GetZaxis();
      }
      histaxis->SetTitle(axis->GetVariableFormula()->GetTitle());
      histaxis->SetTitleOffset(1.5);
      if (axis->GetSelectionFormula()) man->Add(axis->GetSelectionFormula());
      if (fAxisAsync) {
         man->Sync();
         fManagers.push_back(man);
      }
   }
   if (!fAxisAsync) {
      man->Sync();
      fManagers.push_back(man);
      
   }
   fNeedSync = kFALSE;
}
