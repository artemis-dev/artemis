/* $Id:$ */
/**
 * @file   TRCNPRunInfo.h
 * @date   Created : Nov 19, 2013 17:19:35 JST
 *   Last Modified : Jun 22, 2014 17:11:49 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TRCNPRUNINFO_H
#define TRCNPRUNINFO_H

#include <TNamed.h>

namespace art {
   class TRCNPRunInfo;
}

class art::TRCNPRunInfo  : public TNamed {

public:
   // default constructure
   TRCNPRunInfo();
   // constructor w/ setting name and title
   TRCNPRunInfo(const char *name,const char *title);
   // destructor
   virtual ~TRCNPRunInfo();

   // set run name
   virtual void SetRunName(const char* runname) { fRunName = runname; }
   // set run number
   virtual void SetRunNumber(Int_t runnumber) { fRunNumber = runnumber; }
   // set start time
   virtual void SetStartTime(ULong64_t time) { fStartTime = time; }
	 //set block number
   virtual void SetBlockNumber(double blk) { fBlockNumber= blk; }
   // set stop time
   virtual void SetStopTime(ULong64_t time) { fStopTime = time; }
   // set header comment
   virtual void SetHeader(const char* comment) { fHeader = comment; }
   // set ender comment
   virtual void SetEnder(const char* comment) { fEnder = comment; }
   // set total size
   virtual void SetTotalSize(ULong64_t size) { fTotalSize = size; }
   // set total size
   virtual void AddAnalyzedSize(ULong64_t size) { fAnalyzedSize += size; }
   
   // get run name
   virtual TString GetRunName() { return fRunName; }
   // get run number
   virtual Int_t GetRunNumber() { return fRunNumber; }
   // get start time
   virtual Long64_t GetStartTime() { return fStartTime; }
   // get stop time
   virtual Long64_t GetStopTime() { return fStopTime; }
   // get header
   virtual TString GetHeader() { return fHeader; }
   // get ender
   virtual TString GetEnder() { return fEnder; }
   // increment event number
   virtual void IncrementEventNumber() { fEventNumber++; }

   // print
   virtual void Print(Option_t *opt = "") const;
   
protected:
   TString  fRunName;
   Int_t   fRunNumber;
   Long64_t fStartTime; 
   Long64_t fStopTime; 
   TString  fHeader;
   TString  fEnder;
   Long_t   fEventNumber;
   Long_t   fBlockNumber;
   ULong64_t fTotalSize;
   ULong64_t fAnalyzedSize;


   ClassDef(TRCNPRunInfo,1);
};
#endif // end of #ifdef TRUNINFO_H
