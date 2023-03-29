/* $Id:$ */
/**
 * @file   TCatBeamXProcessor.h
 * @date   Created : May 13, 2015 16:13:48 JST
 *   Last Modified : 2018-02-04 13:13:14 JST (ota)
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2015
 */
#ifndef TCATBEAMXPROCESSOR_H
#define TCATBEAMXPROCESSOR_H

#include <TProcessor.h>
#include <TSimpleData.h>
#include <TList.h>
#include <TRunInfo.h>

namespace art {
   class TCatBeamXProcessor;
}
class TClonesArray;
class art::TCatBeamXProcessor  : public TProcessor {

public:
   TCatBeamXProcessor();
   virtual ~TCatBeamXProcessor();

   TCatBeamXProcessor(const TCatBeamXProcessor& rhs);
   TCatBeamXProcessor& operator=(const TCatBeamXProcessor& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   
private:
   TClonesArray **fInput; //!
   StringVec_t  fInputColName;
   TSimpleData  *fOutputP0; //!
   TString      fOutputP0ColName;
   TSimpleData  *fOutputP1; //!
   TString      fOutputP1ColName;
   Bool_t       fDebug;
   Bool_t       fDetailDebug;
   Int_t        fEvt;
   FILE         *fp; //!->
   TString      fLogFilePath;
   TString      fLogFileName;

   TString      fNameRunHeaders;
   TList        **fRunHeaders; //!
   TRunInfo     *fRunInfo; //!
   TString      fRunName;
   Int_t        fRunNumber;

   ClassDef(TCatBeamXProcessor,1) 
};
#endif // end of #ifdef TCATBEAMXPROCESSOR_H
