/**
 * @file   TSeparateOutputProcessor.h
 * @brief  separate output into named outputs
 *
 * @date   Created       : 2014-06-17 01:57:53 JST
 *         Last Modified : Jun 17, 2014 02:17:57 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_969C155E_B55F_4CC1_8262_8559ACD014ED
#define INCLUDE_GUARD_UUID_969C155E_B55F_4CC1_8262_8559ACD014ED

#include "TProcessor.h"
#include <vector>

namespace art {
   class TSeparateOutputProcessor;
}

class TClonesArray;

class art::TSeparateOutputProcessor : public TProcessor {
public:
   TSeparateOutputProcessor();
   virtual ~TSeparateOutputProcessor();

   TSeparateOutputProcessor(const TSeparateOutputProcessor& rhs);
   TSeparateOutputProcessor& operator=(const TSeparateOutputProcessor& rhs);

   virtual void Init(TEventCollection* col);
   virtual void Process();
   
protected:
   TString fInputColName;
   StringVec_t fOutputColNames;

   TClonesArray **fInput; //! input
   std::vector<TClonesArray*> fOutput; //! outputs
private:

   ClassDef(TSeparateOutputProcessor,1) // separate output into named outputs
};

#endif // INCLUDE_GUARD_UUID_969C155E_B55F_4CC1_8262_8559ACD014ED
