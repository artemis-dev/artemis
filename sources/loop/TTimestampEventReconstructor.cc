/**
 * @file   TTimestampEventReconstructor.cc
 * @brief  event reconstruction using timestamp
 *
 * @date   Created       : 2018-06-27 15:37:30 JST
 *         Last Modified : 2018-06-27 19:17:46 JST (ota)
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

using art::TTimestampEventReconstructor;

ClassImp(TTimestampEventReconstructor)

TTimestampEventReconstructor::TTimestampEventReconstructor()
{

   RegisterInputCollection("EventLists","set of event list: n * [file name, timestamp, event header, window start, window end,  list name]",
                           fInputLists,StringVec_t(0));

   RegisterProcessorParameter("OutputFileName","name of output file",fOutputFileName,TString(""));
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
      fSearchWindowStarts.push_back(fInputLists[i + 3].Atof());
      fSearchWindowEnds.push_back(fInputLists[i + 4].Atof());
      fOutputListNames.push_back(fInputLists[i + 5]);
   }

   fNumLists = fFileNames.size();

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
   gROOT->mkdir("eventReconstruction")->cd();
   for (Int_t i = 1, n = fNumLists; i < n; ++i) {
      TString name = TString::Format("%s_%s",fOutputEventLists[0]->GetName(),
                                     fOutputEventLists[i]->GetName());
      TH1F *hist = new TH1F(name,name,1000,fSearchWindowStarts[i],fSearchWindowEnds[i]);
      fTimestampHists.push_back(hist);
   }
   dirsaved->cd();

}

void TTimestampEventReconstructor::BeginOfRun()
{
}

void TTimestampEventReconstructor::Process()
{
   for (Int_t i = 0, n = fNumLists; i < n; ++i ) {
      TEventHeader *header = *fEventHeaders[i];
      TSimpleData *timestamp = *fTimestamps[i];
      fOutputEventLists[i]->AddEntry(header->GetEventNumber(),timestamp->GetValue());
   }
}

void TTimestampEventReconstructor::PostLoop()
{
   fOutputFile->Write(0,TFile::kOverwrite);
}
