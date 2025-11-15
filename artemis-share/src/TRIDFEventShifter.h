/**
 * @file   TRIDFEventShifter.h
 * @brief
 *
 * @date   Created:       2013-11-19 15:03:23
 *         Last Modified: 2014-09-03 16:20:13 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TRIDFEVENTSHIFTER_H
#define TRIDFEVENTSHIFTER_H

#include <TProcessor.h>
#include <TRIDFEventStore.h>

namespace art {
   class TRIDFEventShifter;
   class TSegmentedData;
   class TEventHeader;
}

class TList;

class art::TRIDFEventShifter : public TProcessor {
public:
   // Default constructor
   TRIDFEventShifter();
   virtual ~TRIDFEventShifter();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:

   TRIDFEventStore *fRIDFEventStore; //!child RIDF event stores
   Int_t            fNChildren;      // number of children

   struct TShiftInfo {
      TShiftInfo() : fName(""), fIdx(0) {}
      TString  fName;                // name of segment group (usually DAQ controller)
      IntVec_t fSegIDList;           // list of target segid of shift
      IntVec_t fEventNum;            // list of evt# at which the shift occurs 
      Int_t    fIdx;
   };

   TShiftInfo *fShiftInfo;           //!

   StringVec_t fFileName;            // input filename
   TString     fNameSegmented;       // name of output segdata
   TString     fNameRunHeaders;      // basename of output runheader
   TString     fNameEventHeader;     // basename of output eventheader
   StringVec_t fParameterDirList;    // list of parameter directory path

   TSegmentedData   *fSegmentedData; //!output segdata
   TSegmentedData ***fChildSegData;  //!references to segdata of children

   TString     fCurrentRunName;      // current run name
   Int_t       fCurrentRunNumber;    // current run number

   Int_t       fEventNumberOffset;   // offset of event number for current run

   TList         **fRunInfoList;     //!
   TEventHeader ***fEventHeader;     //!
   Int_t          *fEventNumber;     //!

private:
   void ProcessChild(Int_t);
   void ProceedChildrenToNewRun();
   void ComposeOutput();
   void LoadSegIDList();               // called in Init()
   void LoadShiftInfo(const TString&); // called in Process()

   void SetParameterForEachChild(const TString&, const TString&);
   void SetParameterForEachChild(const TString&, const StringVec_t&);
   void SetParameterForEachChildWithSuffix(const TString&, const TString&);

   // Copy constructor (prohibited)
   TRIDFEventShifter(const TRIDFEventShifter&);
   // Assignment operator (prohibited)
   TRIDFEventShifter& operator=(const TRIDFEventShifter&);

   ClassDef(TRIDFEventShifter,0) // processor for correction of event mismatch between modules
};

#endif // TRIDFEVENTSHIFTER_H
