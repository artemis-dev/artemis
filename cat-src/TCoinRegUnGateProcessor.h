/* $Id:$ */
/**
 * @file   TCoinRegUnGateProcessor.h
 * @date   Created : 2016-01-29 16:51:49 JST
 *   Last Modified : 2016-01-29 17:36:49 JST by tokieda
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *    Originated from TCoinRegGateProcessor.h
 *  
 *    Copyright (C)2016
 */
#ifndef TCONTREGUNGATEPROCESSOR_H
#define TCONTREGUNGATEPROCESSOR_H

#include <TProcessor.h>

namespace art {
   class TCoinRegUnGateProcessor;
   class TCoinRegData;
}

class art::TCoinRegUnGateProcessor : public TProcessor {
public:
   // Default constructor
   TCoinRegUnGateProcessor();
   virtual ~TCoinRegUnGateProcessor();

   virtual void Init(TEventCollection*);
   virtual void Process();

protected:
   TString        fInputColName;
   IntVec_t       fUnGatedCoinID;
   TCoinRegData **fRegHit;

private:
   // Copy constructor (prohibited)
   TCoinRegUnGateProcessor(const TCoinRegUnGateProcessor&);
   // Assignment operator (prohibited)
   TCoinRegUnGateProcessor& operator=(const TCoinRegUnGateProcessor&);

   ClassDef(TCoinRegUnGateProcessor,0)
};

#endif // TCONTREGUNGATEPROCESSOR_H
