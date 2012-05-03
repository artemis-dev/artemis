/* $Id:$ */
/**
 * @file   TCatProcessor.h
 * @date   Created : Feb 19, 2012 10:19:49 JST
 *   Last Modified : May 03, 2012 20:04:51 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATPROCESSOR_H
#define TCATPROCESSOR_H

#include <TNamed.h>

#include <iostream>

class TCatProcessor  : public TNamed {

public:
   TCatProcessor(std::ostream *out = &std::cout);
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
private:
   Bool_t   fIsFirst;
   ostream *fOut;

   ClassDef(TCatProcessor,1);
};
#endif // end of #ifdef TCATPROCESSOR_H
