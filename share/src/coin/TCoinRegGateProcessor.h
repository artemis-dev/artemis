/**
 * @file   TCoinRegGateProcessor.h
 * @brief
 *
 * @date   Created:       2014-01-15 13:01:07
 *         Last Modified: 2014-06-18 17:29:31 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro All rights reserved
 */

#ifndef TCOINREGGATEPROCESSOR_H
#define TCOINREGGATEPROCESSOR_H

#include <TProcessor.h>

namespace art {
   class TCoinRegGateProcessor;
   class TCoinRegData;
}

class art::TCoinRegGateProcessor : public TProcessor {
public:
   // Default constructor
   TCoinRegGateProcessor();
   virtual ~TCoinRegGateProcessor();

   virtual void Init(TEventCollection*);
   virtual void Process();

protected:
   TString        fInputColName;
   IntVec_t       fCoinID;
   TCoinRegData **fRegHit;

private:
   // Copy constructor (prohibited)
   TCoinRegGateProcessor(const TCoinRegGateProcessor&);
   // Assignment operator (prohibited)
   TCoinRegGateProcessor& operator=(const TCoinRegGateProcessor&);

   ClassDef(TCoinRegGateProcessor,0)
};

#endif // TCOINREGGATEPROCESSOR_H
