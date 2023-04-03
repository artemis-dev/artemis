/**
 * @file   TNyokiTimingProcessor.h
 * @brief  nyoki timing
 *
 * @date   Created       : 2014-11-16 13:29:00 JST
 *         Last Modified : 2018-02-04 12:05:07 JST (ota)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_CA8ED36C_C519_4242_8E05_FC44593AEE89
#define INCLUDE_GUARD_UUID_CA8ED36C_C519_4242_8E05_FC44593AEE89

#include "TProcessor.h"

namespace art {
   namespace sh04 {
      class TNyokiTimingProcessor;
   }
}

class TClonesArray;

class art::sh04::TNyokiTimingProcessor : public TProcessor {
public:
   TNyokiTimingProcessor();
   virtual ~TNyokiTimingProcessor();

   void Init(TEventCollection *col);
   void Process();

protected:
   /* input */
   TString        fInputColName;
   TClonesArray **fInputCol;//!

   /* output */
   TString        fOutputColName;
   TClonesArray  *fOutputCol; //!

private:
   TNyokiTimingProcessor(const TNyokiTimingProcessor&);
   TNyokiTimingProcessor& operator=(const TNyokiTimingProcessor&);

   ClassDef(TNyokiTimingProcessor,0) // nyoki timing
};

#endif // INCLUDE_GUARD_UUID_CA8ED36C_C519_4242_8E05_FC44593AEE89
