/* $Id:$ */
/**
 * @file   TEventHeader.h
 * @date   Created : Nov 20, 2013 13:20:07 JST
 *   Last Modified : Nov 26, 2013 13:46:51 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TEVENTHEADER_H
#define TEVENTHEADER_H

#include <TObject.h>
#include <TString.h>

namespace art {
   class TEventHeader;
}

class art::TEventHeader  : public TObject {

public:
   TEventHeader();
   virtual ~TEventHeader();

   // get run name
   virtual TString GetRunName() { return fRunName; }
   // get run number
   virtual Int_t   GetRunNumber() { return fRunNumber; }
   // get timestamp
   virtual Long64_t GetTimestamp() { return fTimestamp; }
   // get event number in one run
   virtual Long64_t GetEventNumber() { return fEventNumber; }
   // get event number in total 
   virtual Long64_t GetEventNumberTotal() { return fEventNumberTotal; }
   // increment event number
   virtual void IncrementEventNumber() {
      fEventNumber++;
      fEventNumberTotal++;
   }
   // set run name
   virtual void SetRunName(const char *name) { fRunName = name; }
   // set run number
   virtual void SetRunNumber(Int_t num) { fRunNumber = num; }
   // set timestamp
   virtual void SetTimestamp(Long64_t time) { fTimestamp = time; }
   // set event number
   virtual void SetEventNumber(Int_t num = 0) { fEventNumber = num; }
   // clear
   virtual void SetEventNumberTotal (Int_t num = 0) { fEventNumberTotal = num; }
protected:
   TString fRunName;  // run name string
   Int_t fRunNumber;  // run number 
   Long64_t fTimestamp; // timestamp
   Long64_t fEventNumber; // event number in one run
   Long64_t fEventNumberTotal; // total event number in a loop
   
   ClassDef(TEventHeader,1); // Header for each event

};
#endif // end of #ifdef TEVENTHEADER_H
