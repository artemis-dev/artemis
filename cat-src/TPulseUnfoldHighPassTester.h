/* $Id:$ */
/**
 * @file   TPulseUnfoldHighPassTester.h
 * @date   Created : Jan 31, 2014 13:31:47 JST
 *   Last Modified : 2015-05-19 17:16:07 JST by tokieda
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *  Added from share/src/cat/TPulseUnfoldHighPassProcessor.h
 *  
 *    Copyright (C)2015
 */
#ifndef TPULSEUNFOLDHIGHPASSTESTER_H
#define TPULSEUNFOLDHIGHPASSTESTER_H

#include <TProcessor.h>
#include <TClonesArray.h>
#include <TList.h>
#include <TRunInfo.h>

namespace art {
   class TPulseUnfoldHighPassTester;
}

class TString;

class art::TPulseUnfoldHighPassTester  : public TProcessor {

public:
   TPulseUnfoldHighPassTester();
   ~TPulseUnfoldHighPassTester();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   StringVec_t  fInputColName;

   TClonesArray **fInput; //!

   Float_t fTimeConstant; 
   
   Int_t        fEvt;
   Bool_t       fDebug;
   Bool_t       fDetailDebug;
   FILE         *fp; //!->
   TString      fLogFilePath;
   TString      fLogFileName;
   TString      fNameRunHeaders;
   TList        **fRunHeaders; //!
   TRunInfo     *fRunInfo;
   TString      fRunName;
   Int_t        fRunNumber;

   ClassDef(TPulseUnfoldHighPassTester,1);
};
#endif // end of #ifdef TPULSEUNFOLDHIGHPASSTESTER_H
