/**
 * @file   TNyokiGateProcessor.h
 * @brief  nyoki gate (only charge impremented)
 *
 * @date   Created       : 2014-09-26 10:39:47 JST
 *         Last Modified : 2018-02-04 11:57:51 JST (ota)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_252AAE7B_9D68_4901_A6FB_37417E71E260
#define INCLUDE_GUARD_UUID_252AAE7B_9D68_4901_A6FB_37417E71E260

#include <TProcessor.h>

namespace art {
   namespace sh04 {
      class TNyokiGateProcessor;
   }

   class TGateObject;
}

class TClonesArray;

class art::sh04::TNyokiGateProcessor : public TProcessor {
public:
   TNyokiGateProcessor();
   virtual ~TNyokiGateProcessor();

   void Init(TEventCollection *col);
   void Process();

protected:
   /* input */
   TString        fNyokiArrayName;
   TClonesArray **fNyokiArray; //!

   /* output */
   TGateObject   *fGate; //!

   /* gate setting */
   FloatVec_t    fChargeGate;

private:
   TNyokiGateProcessor(const TNyokiGateProcessor& rhs);
   TNyokiGateProcessor& operator=(const TNyokiGateProcessor& rhs);

   ClassDef(TNyokiGateProcessor,0) // nyoki gate
};

#endif // INCLUDE_GUARD_UUID_252AAE7B_9D68_4901_A6FB_37417E71E260
