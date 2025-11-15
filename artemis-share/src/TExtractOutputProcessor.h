/**
 * @file   TExtractOutputProcessor.h
 * @brief  extract output with single ID from a branch
 *
 * @date   Created       : 2015-05-19 11:53:03 JST
 *         Last Modified : 2015-05-19 12:08:36 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_B1CB8462_012E_4C2B_8C14_D25D2ED610E7
#define INCLUDE_GUARD_UUID_B1CB8462_012E_4C2B_8C14_D25D2ED610E7

#include "TSeparateOutputProcessor.h"

namespace art {
   class TExtractOutputProcessor;
}

class art::TExtractOutputProcessor : public TSeparateOutputProcessor {
public:
   TExtractOutputProcessor();
   virtual ~TExtractOutputProcessor();

   virtual void Init(TEventCollection* col);

protected:
   Int_t   fInputID;
   TString fOutputColName;

private:
   TExtractOutputProcessor(const TExtractOutputProcessor&);
   TExtractOutputProcessor& operator=(const TExtractOutputProcessor&);

   ClassDef(TExtractOutputProcessor,1) // extract output with single ID from a branch
};

#endif // INCLUDE_GUARD_UUID_B1CB8462_012E_4C2B_8C14_D25D2ED610E7
