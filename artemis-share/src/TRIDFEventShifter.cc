/**
 * @file   TRIDFEventShifter.cc
 * @brief
 *
 * @date   Created:       2013-11-19 15:42:52
 *         Last Modified: 2014-09-10 13:23:23 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TRIDFEventShifter.h"

#include <fstream>

#include <TRegexp.h>

#include <TSegmentedData.h>
#include <TRunInfo.h>
#include <TConfigFile.h>
#include <TEventHeader.h>

using art::TRIDFEventShifter;

ClassImp(art::TRIDFEventShifter)

// Default constructor
TRIDFEventShifter::TRIDFEventShifter()
   : fRIDFEventStore(NULL), fNChildren(0), fShiftInfo(NULL),
     fSegmentedData(NULL), fChildSegData(NULL), fCurrentRunName(""), fCurrentRunNumber(0),
     fRunInfoList(NULL), fEventHeader(NULL), fEventNumber(NULL) {
   StringVec_t dummy;
   RegisterInputCollection("InputFiles","The names of input files",fFileName,dummy);
   RegisterOutputCollection("SegmentedData","The name of output array for segmented data",
			    fNameSegmented,TString("segdata"));
   RegisterProcessorParameter("ParameterDirList","List of directories including segment.dat",
			      fParameterDirList,dummy);
   RegisterOutputCollection("RunHeadersName","the basename of output array for run headers, run header will be stored once",
                            fNameRunHeaders,TString("runheader"));
   RegisterOutputCollection("EventHeaderName","the basename of event header",
                            fNameEventHeader,TString("eventheader"));
}

TRIDFEventShifter::~TRIDFEventShifter()
{
   delete [] fRIDFEventStore;
   delete [] fShiftInfo;
   delete    fSegmentedData;
   delete [] fChildSegData;
   delete [] fEventNumber;
   delete [] fRunInfoList;
   delete [] fEventHeader;
}

void TRIDFEventShifter::Init(TEventCollection *col)
{
   if (fParameterDirList.empty()) {
      Error("Init","ParameterDirList is empty.");
      return;
   }

   fNChildren = fParameterDirList.size() + 1;
   fRIDFEventStore = new TRIDFEventStore[fNChildren];
   fShiftInfo      = new TShiftInfo[fNChildren];
   fSegmentedData  = new TSegmentedData;
   fChildSegData   = new TSegmentedData**[fNChildren];
   fEventHeader    = new TEventHeader**[fNChildren];
   fEventNumber    = new Int_t[fNChildren];
   fRunInfoList    = new TList*[fNChildren];

   LoadSegIDList();

   SetParameterForEachChildWithSuffix("SegmentedData",fNameSegmented);
   SetParameterForEachChild("InputFiles",fFileName);
   SetParameterForEachChildWithSuffix("RunHeadersName",fNameRunHeaders);
   SetParameterForEachChildWithSuffix("EventHeaderName",fNameEventHeader);
   SetParameterForEachChild("OutputTransparency","1");

   for (Int_t iChild = 0; iChild != fNChildren; ++iChild) {
      fEventNumber[iChild] = -1;
      Info("Init","initialize fRIDFEventStore[%d] for %s ...",
	   iChild,iChild ? fShiftInfo[iChild].fName.Data() : "mother");
      fRIDFEventStore[iChild].InitProc(col);

      const TString &suffix = iChild ? TString::Format("_%s", fShiftInfo[iChild].fName.Data()) : "0";
      fChildSegData[iChild] = reinterpret_cast<TSegmentedData**>(col->GetObjectRef(fNameSegmented + suffix));
      fEventHeader[iChild]  = reinterpret_cast<TEventHeader**>(col->GetObjectRef(fNameEventHeader + suffix));
      fRunInfoList[iChild]  = static_cast<TList*>(col->GetInfo(fNameRunHeaders + suffix));
   }

   col->Add(fNameSegmented,fSegmentedData,fOutputIsTransparent);
}

void TRIDFEventShifter::Process()
{
   fSegmentedData->Clear("C");

   for (Int_t iChild = 0; iChild!= fNChildren; ++iChild) {
      ProcessChild(iChild);
   }

   {  // track the change of run
      TRunInfo *const runInfo   = static_cast<TRunInfo*>(fRunInfoList[0]->Last());
      const TString  &runName   = runInfo->GetRunName();
      const Int_t     runNumber = runInfo->GetRunNumber();

      if(fCurrentRunNumber != runNumber || fCurrentRunName.CompareTo(runName)) {
	 fCurrentRunName   = runName;
	 fCurrentRunNumber = runNumber;
	 const TString &newRunName = TString::Format("%s%04d",runName.Data(),runNumber);
	 Info("Process","New Run: %s",newRunName.Data());
	 ProceedChildrenToNewRun();
	 LoadShiftInfo(newRunName);
      }
   }

   // compose output segdata from children's ones
   ComposeOutput();
}

void TRIDFEventShifter::ProcessChild(Int_t iChild)
{
   if (iChild) {
      Int_t &idx = fShiftInfo[iChild].fIdx;
      const Int_t n = fShiftInfo[iChild].fEventNum.size();

      if (idx != n && fEventNumber[iChild] - fEventNumberOffset == fShiftInfo[iChild].fEventNum[idx] - idx) {
	 Info("ProcessChild","%s: skip event number = %d (total event number = %d)",
	      fShiftInfo[iChild].fName.Data(),fShiftInfo[iChild].fEventNum[idx], fEventNumber[iChild]);
	 ++idx;
	 return;
      }
   }

   fRIDFEventStore[iChild].Process();
   fEventNumber[iChild] = (*fEventHeader[iChild])->GetEventNumber();
}

void TRIDFEventShifter::ProceedChildrenToNewRun()
{
   for (Int_t iChild = 0; iChild != fNChildren; ++iChild) {
      while (1) {
	 TRunInfo *const runInfo   = static_cast<TRunInfo*>(fRunInfoList[iChild]->Last());
	 const TString  &runName   = runInfo->GetRunName();
	 const Int_t     runNumber = runInfo->GetRunNumber();

	 if (runNumber == fCurrentRunNumber && !runName.CompareTo(fCurrentRunName) ) break;
	 fRIDFEventStore[iChild].Process();
      }
   }
}

void TRIDFEventShifter::ComposeOutput()
{
   // copy segdata from 0th child as a base for output
   fSegmentedData->TObjArray::operator=(**fChildSegData[0]);

   for(Int_t iChild = 1; iChild != fNChildren; ++iChild) {
      const IntVec_t &segIDList = fShiftInfo[iChild].fSegIDList;

      const Int_t nSegID = segIDList.size();
      for(Int_t iSegID = 0; iSegID != nSegID; ++iSegID) {
	 const UInt_t segID = segIDList.at(iSegID);
	 TObjArray *childSeg = (*fChildSegData[iChild])->FindSegmentByID(segID);

	 const Int_t nSegment = fSegmentedData->GetEntriesFast();
	 for (Int_t iSegment = 0; iSegment != nSegment; ++iSegment) {
	    if (fSegmentedData->At(iSegment)->GetUniqueID() == segID) {
	       (*fSegmentedData)[iSegment] = childSeg;

	       // when 0th child has segment and i-th child doesn't,
	       // there is an empty slot to be compressed
	       if (!childSeg) fSegmentedData->Compress();

	       childSeg = NULL;
	       break;
	    }
	 }

	 // when 0th child doesn't have segment,
	 // childSeg has to be added.
	 if (childSeg) fSegmentedData->Add(childSeg);
      }
   }
}

void TRIDFEventShifter::SetParameterForEachChild(const TString &key, const TString &value)
{
   const StringVec_t values(1,value);
   SetParameterForEachChild(key,values);
}

void TRIDFEventShifter::SetParameterForEachChild(const TString &key, const StringVec_t &values)
{
   for (Int_t iChild = 0; iChild != fNChildren; ++iChild) {
      TParameterStrings *prm = new TParameterStrings;
      prm->Add(key,values);
      fRIDFEventStore[iChild].SetParameters(prm); // move prm
   }
}

void TRIDFEventShifter::SetParameterForEachChildWithSuffix(const TString &key, const TString &value)
{
   for (Int_t iChild = 0; iChild != fNChildren; ++iChild) {
      TParameterStrings *prm = new TParameterStrings;
      const TString &suffix =
	 iChild ? TString::Format("_%s", fShiftInfo[iChild].fName.Data()) : "0";
      const StringVec_t values(1,value + suffix);
      prm->Add(key,values);
      fRIDFEventStore[iChild].SetParameters(prm); // move prm
   }
}

void TRIDFEventShifter::LoadSegIDList()
{
   Info("LoadSegIDList","loading segid list for event shift...");

   for(Int_t iChild = 1; iChild != fNChildren; ++iChild) {
      const TString &parameterDir = fParameterDirList[iChild-1];

      TString &segGroupName = fShiftInfo[iChild].fName;
      segGroupName = parameterDir(parameterDir.Index(TRegexp("[^/]+[a-zA-Z0-9_]$")),
				  parameterDir.Length());

      IntVec_t &segIDList = fShiftInfo[iChild].fSegIDList;
      segIDList.clear();

      const TString& fileName = TString::Format("%s/segment.dat",parameterDir.Data());
      TConfigFile segFile(fileName,"#",", \t","#");

      const Int_t devMask  = 0x3f;
      const Int_t devShift = 20;
      const Int_t fpMask   = 0x3f;
      const Int_t fpShift  = 14;
      const Int_t detMask  = 0x3f;
      const Int_t detShift = 8;

      while (1) {
	 const TString& devstr = segFile.GetNextToken();
	 const TString& fpstr  = segFile.GetNextToken();
	 const TString& detstr = segFile.GetNextToken();
	 if (devstr.IsNull() || fpstr.IsNull() || detstr.IsNull()) break;
	 const Int_t dev = devstr.Atoi() & devMask;
	 const Int_t fp  = fpstr.Atoi()  & fpMask;
	 const Int_t det = detstr.Atoi() & detMask;

	 const Int_t segID = (dev << devShift) | (fp << fpShift) | (det << detShift);

	 printf(" %s: dev = %d, fp = %d, det = %d\n",segGroupName.Data(),dev,fp,det);
	 segIDList.push_back(segID);
      }
   }
}

void TRIDFEventShifter::LoadShiftInfo(const TString &runName)
{
   fEventNumberOffset = (*fEventHeader[0])->GetEventNumber();

   for (Int_t iChild = 1; iChild!= fNChildren; ++iChild) {
      IntVec_t &eventNum = fShiftInfo[iChild].fEventNum;
      eventNum.clear();
      fShiftInfo[iChild].fIdx = 0;

      const TString& fileName =
	 TString::Format("%s/%s.dat",fParameterDirList[iChild-1].Data(),runName.Data());

      std::ifstream ifs(fileName.Data());
      if (ifs.fail()) {
	 Info("LoadShiftInfo","No shift info found for %s",
	      fShiftInfo[iChild].fName.Data());
	 continue;
      }

      std::string str;
      Int_t e,d;
      while (ifs && std::getline(ifs,str,'\n')) {
	 if (2 != std::sscanf(str.c_str(), "%d %d %*s", &e, &d)) {
	    continue;
	 }

	 if (d < 1) {
	    Error("LoadShiftInfo","2nd column in eventshift data must be > 0.\n");
	    continue;
	 }

	 for (Int_t j = 0; j != d; ++j) {
	    const Int_t input = e + j;
	    if(eventNum.size() && input <= eventNum.back()) {
	       Error("LoadShiftInfo","event number in eventshift data must monotonically increase. abort.");
	       return;
	    }
	    eventNum.push_back(input);
	 }
      }
   }
}
