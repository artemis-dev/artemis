/* $Id:$ */
/**
 * @file   TCatProcessor.h
 * @date   Created : Feb 19, 2012 10:19:49 JST
 *   Last Modified : Apr 27, 2012 19:01:42 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATPROCESSOR_H
#define TCATPROCESSOR_H

#include <TNamed.h>

class TCatProcessor  : public TNamed {

public:
   TCatProcessor();
   ~TCatProcessor();

   virtual void Init() { }
   virtual void BeginOfRun() { }
   virtual void EndOfRun() { }
   virtual void Process() { }
   virtual void Check() { }
   virtual void End() { }

   virtual void Clear(Option_t* /* opt */) { fIsFirst = kFALSE; }

//   virtual void SetParameters(TCatStringParameters prm);
//   virtual void UpdateParameters();

   Bool_t IsFirst() { return fIsFirst; }
   void SetFirst(Bool_t isFirst = kTRUE) { fIsFirst = isFirst; }
protected:
//   TCatStringParameters fParameters;
   Bool_t fIsFirst;

   ClassDef(TCatProcessor,1);
};
#endif // end of #ifdef TCATPROCESSOR_H
