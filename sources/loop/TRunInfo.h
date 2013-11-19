/* $Id:$ */
/**
 * @file   TRunInfo.h
 * @date   Created : Nov 19, 2013 17:19:35 JST
 *   Last Modified : Nov 19, 2013 17:53:08 JST
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

   // print
   virtual void Print(Option_t *opt = "") const;
   
protected:
   TString  fRunName;
   Int_t   fRunNumber;
   Long64_t fStartTime; 
   Long64_t fStopTime; 
   TString  fHeader;
   TString  fEnder;


   ClassDef(TRunInfo,1);
};
#endif // end of #ifdef TRUNINFO_H
