/* $Id:$ */
/**
 * @file   TRunInfo.h
 * @date   Created : Nov 19, 2013 17:19:35 JST
 *   Last Modified : 2016-08-17 14:52:48 JST (kawase)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TRUNINFO_H
#define TRUNINFO_H

#include <TNamed.h>

namespace art {
   class TRunInfo;
}

class art::TRunInfo  : public TNamed {

public:
   // default constructure
   TRunInfo();
   // constructor w/ setting name and title
   TRunInfo(const char *name,const char *title);
   // destructor
   virtual ~TRunInfo();

   // set run name
   virtual void SetRunName(const char* runname) { fRunName = runname; }
   // set run number
   virtual void SetRunNumber(Int_t runnumber) { fRunNumber = runnumber; }
   // set start time
   virtual void SetStartTime(ULong64_t time) { fStartTime = time; }
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
   virtual TString GetRunName() const { return fRunName; }
   // get run number
   virtual Int_t GetRunNumber() const { return fRunNumber; }
   // get start time
   virtual Long64_t GetStartTime() const { return fStartTime; }
   // get stop time
   virtual Long64_t GetStopTime() const { return fStopTime; }
   // get header
   virtual TString GetHeader() const { return fHeader; }
   // get ender
   virtual TString GetEnder() const { return fEnder; }
   // increment event number
   virtual void IncrementEventNumber() { fEventNumber++; }
   // get eventnumber
   virtual Long_t GetEventNumber() { return fEventNumber; }

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
   ULong64_t fTotalSize;
   ULong64_t fAnalyzedSize;


   ClassDef(TRunInfo,1);
};
#endif // end of #ifdef TRUNINFO_H
