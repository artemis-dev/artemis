/**
 * @file   TTimestampEventReconstructor.cc
 * @brief  event reconstruction using timestamp
 *
 * @date   Created       : 2018-06-27 15:37:30 JST
 *         Last Modified : 2018-07-03 01:27:46 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#include "TTimestampEventReconstructor.h"
#include <TSimpleData.h>
#include <TEventHeader.h>
#include <TDirectory.h>
#include <TFile.h>
#include <TTimestampEventList.h>
#include <TH1F.h>
#include <TROOT.h>
#include <TMath.h>

using art::TTimestampEventReconstructor;

ClassImp(TTimestampEventReconstructor)

TTimestampEventReconstructor::TTimestampEventReconstructor()
{

   RegisterInputCollection("EventLists","set of event list: n * [file name, timestamp, event header, window offset, window half width,  list name]",
                           fInputLists,StringVec_t(0));

   RegisterProcessorParameter("OutputFileName","name of output file",fOutputFileName,TString(""));

   RegisterOptionalParameter("MaxAllCombination","make N x N combination if non-zeo",fMaxAllCombination,0);
   RegisterOptionalParameter("CombinationThreshold","threshold for combination to verify the good combination",fCombinationThreshold,0.9);
}

TTimestampEventReconstructor::~TTimestampEventReconstructor()
{
   for (Int_t i = 0, n = fOutputEventLists.size(); i < n; ++i) {
      delete fOutputEventLists[i];
   }
   fOutputEventLists.clear();

   for (Int_t i = 0, n = fTimestampHists.size(); i < n; ++i) {
      delete fTimestampHists[i];
   }
   fTimestampHists.clear();
}

TTimestampEventReconstructor::TTimestampEventReconstructor(const TTimestampEventReconstructor& rhs)
{
}

TTimestampEventReconstructor& TTimestampEventReconstructor::operator=(const TTimestampEventReconstructor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}


void TTimestampEventReconstructor::Init(TEventCollection *col)
{
   const Int_t kTokens = 6;
   
   if (fInputLists.size() == 0) {
      SetStateError("No list of events is set");
      return;
   }

   if ((fInputLists.size() % kTokens)) {
      SetStateError(TString::Format("The number of components in set of event list should be multiple of %d",kTokens));
      return;
   }

   for (Int_t i = 0, n = fInputLists.size(); i < n; i += kTokens) {
      fFileNames.push_back(fInputLists[i]);
      fTimestampNames.push_back(fInputLists[i + 1]);
      fEventHeaderNames.push_back(fInputLists[i + 2]);
      Double_t offset = fInputLists[i + 3].Atof();
      Double_t width = fInputLists[i + 4].Atof();
      fOutputListNames.push_back(fInputLists[i + 5]);
      fSearchWindowStarts.push_back(offset - width);
      fSearchWindowEnds.push_back(offset + width);
   }

   fNumLists = fFileNames.size();
   fHeaderQueue.resize(fNumLists);
   fTimestampQueue.resize(fNumLists);

   //////////////////////////////////////////////////////////////////////
   // set output file
   //////////////////////////////////////////////////////////////////////
   TDirectory *dirsaved = gDirectory;
   fOutputFile = TFile::Open(fOutputFileName,"recreate");
   if (!fOutputFile) {
      SetStateError(TString::Format("Cannot open file: '%s'",fOutputFileName.Data()));
      return;
   }
   for (Int_t i = 0, n = fNumLists; i < n; ++i) {
      TTimestampEventList *list = new TTimestampEventList;
      list->SetName(fOutputListNames[i]);
      list->SetDirectory(fOutputFile);
      list->SetFileName(fFileNames[i]);
      fOutputEventLists.push_back(list);
   }
   dirsaved->cd();

   
   //////////////////////////////////////////////////////////////////////
   // check and get timestamps
   //////////////////////////////////////////////////////////////////////
   std::vector<TString> badlist;
   for (Int_t i = 0, n = fNumLists; i < n; ++i) {
      TSimpleData **timestamp = reinterpret_cast<TSimpleData**>(col->GetObjectRef(fTimestampNames[i]));
      if (!timestamp) {
         badlist.push_back(fTimestampNames[i]);
         continue;
      } else {
         fTimestamps.push_back(timestamp);
      }
   }
   if (badlist.size() > 0) {
      TString message = "Timestamps are missing: ";
      for (Int_t i = 0, n = badlist.size(); i < n; ++i) {
         message += badlist[i];
         message += ', ';
      }
      message[message.Length()-1] = '\0';
      SetStateError(message);
   }
   //////////////////////////////////////////////////////////////////////
   // check and get event headers

   //////////////////////////////////////////////////////////////////////
   badlist.clear();
   for (Int_t i = 0, n = fNumLists; i < n; ++i) {
      TEventHeader **eventHeader = reinterpret_cast<TEventHeader**>(col->GetObjectRef(fEventHeaderNames[i]));
      if (!eventHeader) {
         badlist.push_back(fEventHeaderNames[i]);
         continue;
      } else {
         fEventHeaders.push_back(eventHeader);
      }
   }
   if (badlist.size() > 0) {
      TString message = "Event headers are missing: ";
      for (Int_t i = 0, n = badlist.size(); i < n; ++i) {
         message += badlist[i];
         message += ', ';
      }
      message[message.Length()-1] = '\0';
      SetStateError(message);
   }

   //////////////////////////////////////////////////////////////////////
   // create histogram
   //////////////////////////////////////////////////////////////////////
   fOutputFile->mkdir("eventReconstruction")->cd();
   for (Int_t i = 0, n = fNumLists; i < n; ++i) {
      TString name = TString::Format("%s_%s",fOutputEventLists[0]->GetName(),
                                     fOutputEventLists[i]->GetName());
      TString nameAll = TString::Format("%s_%s_all",fOutputEventLists[0]->GetName(),
                                     fOutputEventLists[i]->GetName());
      TH1F *hist = new TH1F(name,name,1000000,fSearchWindowStarts[i],fSearchWindowEnds[i]);
      TH1F *histAll = new TH1F(nameAll,nameAll,1000000,fSearchWindowStarts[i],fSearchWindowEnds[i]);
      fTimestampHists.push_back(hist);
      fTimestampHistsAll.push_back(histAll);
   }
   dirsaved->cd();

}

void TTimestampEventReconstructor::BeginOfRun()
{
}

void TTimestampEventReconstructor::Process()
{
   Int_t foundTimestamps = 1;
   for (Int_t i = 0, n = fNumLists; i < n; ++i ) {
      TEventHeader *header = *fEventHeaders[i];
      TSimpleData *timestamp = *fTimestamps[i];
      fHeaderQueue[i].push_back(header->GetEventNumber());
      fTimestampQueue[i].push_back(timestamp->GetValue());
//      while (fTimestampQueue[i].front() > timestamp->GetValue()) {
//         Pop(i);
//      }
   }

   if (fMaxAllCombination) {
      if (fMaxAllCombination > fTimestampQueue[0].size()) return;
      // make all the combination
      for (Int_t iList = 0; iList < fNumLists; ++iList) {
         for (Int_t i = 0, ni = fTimestampQueue[iList].size(); i < ni; ++i) {
            for (Int_t j = 0, nj = fTimestampQueue[iList].size(); j < nj; ++j) {
               Double_t t0 = fTimestampQueue[0][i];
               Double_t t1 = fTimestampQueue[iList][j];
               fTimestampHistsAll[iList]->Fill(t1-t0);
            }
         }
      }
      Bool_t found = kTRUE;
      for (Int_t iList = 0; iList < fNumLists; ++iList) {
         Int_t iBin = fTimestampHistsAll[iList]->GetMaximumBin();
         Double_t tMean = fTimestampHistsAll[iList]->GetBinCenter(iBin);
         Double_t count = fTimestampHistsAll[iList]->GetBinContent(iBin);
         if (count < fMaxAllCombination * fCombinationThreshold) {
            found = kFALSE;
            break;
         }
         fSearchWindowStarts[iList] = tMean - fTimestampHistsAll[iList]->GetBinWidth(iBin) * 5.;
         fSearchWindowEnds[iList] = tMean + fTimestampHistsAll[iList]->GetBinWidth(iBin) * 5.;
         Info("Process","Timestamp matched [%d]: search window set from %f to %f",iList,fSearchWindowStarts[iList],fSearchWindowEnds[iList]);
      }
      if (!found) {
         Warning("Process","Timestamp mismatched, please check the timestamp histogram and change the definition");
         SetStopEvent();
         SetStopLoop();
         SetEndOfRun();
         return;
      } else {
         for (Int_t iList = 0; iList < fNumLists; ++iList) {
            Double_t binW = fTimestampHistsAll[iList]->GetBinWidth(1);
            Int_t nBins = 10* TMath::Floor((fSearchWindowEnds[iList] - fSearchWindowStarts[iList])/binW);
            fTimestampHistsAll[iList]->Reset();
            fTimestampHists[iList]->Reset();
            fTimestampHistsAll[iList]->SetBins(nBins,fSearchWindowStarts[iList],fSearchWindowEnds[iList]);
            fTimestampHists[iList]->SetBins(nBins,fSearchWindowStarts[iList],fSearchWindowEnds[iList]);
         }
         fMaxAllCombination = 0;
      }
   }

   
   while (fTimestampQueue[0].size()) {
      Double_t timestampOrigin = fTimestampQueue[0].front();
      for (Int_t i = 1; i < fNumLists; ++i) {
         while (fTimestampQueue[i].size()) {
            Double_t timestamp = fTimestampQueue[i].front();
            if (timestamp - timestampOrigin < fSearchWindowStarts[i]) {
               // pop timestamp because it becomes old
#if 0               
               Info("Process","Dropped [%d][%d] %20.10f, %20.10f < %20.10f",i,timestamp, fHeaderQueue[i].front(),timestamp-timestampOrigin,fSearchWindowStarts[i]);
#endif               
               Pop(i);
               fTimestampHistsAll[i]->Fill(timestamp-timestampOrigin);
               if (fTimestampQueue[i].size() == 0) {
                  // no more loop is available
                  return;
               }
            } else if (timestamp - timestampOrigin > fSearchWindowEnds[i]) {
               // pop timestamp reference because it becomes old
#if 0               
               Info("Process","Dropped [%d] %20.10f",0,timestampOrigin);
#endif               
               Pop(0);
               fTimestampHistsAll[0]->Fill(0);
               if (fTimestampQueue[0].size() == 0) {
                  // no more loop is available
                  return;
               }
               // get new timestamp origin
               timestampOrigin = fTimestampQueue[0].front();
            } else {
               // timestamp is within the search window
               foundTimestamps++;
               break;
            }
         }
      }

      if (foundTimestamps != fNumLists) return;
      Double_t timeref = fTimestampQueue[0].front();
#if 0      
      Info("Process","Match found");
#endif      
      for (Int_t i = 0, n = fNumLists; i < n; ++i ) {
         Double_t timestamp = fTimestampQueue[i].front();
         Long64_t entry = fHeaderQueue[i].front();
#if 0         
         Info("Process","timestamp = %20.10f entry = %lld",timestamp,entry);
#endif         
         Pop(i);
         fOutputEventLists[i]->AddEntry(entry,timestamp);
         fTimestampHists[i]->Fill(timestamp-timeref);
         fTimestampHistsAll[i]->Fill(timestamp-timeref);
      }
      foundTimestamps = 1;
   }
}

void TTimestampEventReconstructor::PostLoop()
{
   fOutputFile->Write(0,TFile::kOverwrite);

   for (Int_t i = 0; i < fNumLists; ++i) {
      Info("PostLoop","Remains [%d] : %d",i,fHeaderQueue[i].size());
   }
}
