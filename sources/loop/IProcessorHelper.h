/**
 * @file   IProcessorHelper.h
 * @brief  interface class of the helper for processors
 *
 * @date   Created       : 2016-07-21 23:00:51 JST
 *         Last Modified : 2018-02-25 23:51:18 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_C4FB4BEB_330D_43B1_B9BF_1CC7665038CB
#define INCLUDE_GUARD_UUID_C4FB4BEB_330D_43B1_B9BF_1CC7665038CB

#include  <Rtypes.h>
#include <TString.h>
#include "TAttParameter.h"

namespace art {
   class IProcessorHelper;
   class TEventCollection;
   class TProcessor;
}

class art::IProcessorHelper : public TAttParameter {
public:
   IProcessorHelper();
   virtual ~IProcessorHelper();

   IProcessorHelper(const IProcessorHelper& rhs);
   IProcessorHelper& operator=(const IProcessorHelper& rhs);

   // called in TProcessor::InitProc
   virtual void Init(TEventCollection *col) = 0;

   // called in TProcessor::Clear
   virtual void Clear(Option_t *) {; }

   // call TProcessor::RegisterXXX
   virtual void RegisterProcessor(TProcessor *proc);

   virtual TProcessor* GetParent() {return fParent;}

   virtual void SetStateError(TString msg);

protected:
   virtual void Register(TProcessor *proc) = 0;

   TProcessor *fParent; //!

private:

   ClassDef(IProcessorHelper,1) // interface class of the helper for processors
};

#endif // INCLUDE_GUARD_UUID_C4FB4BEB_330D_43B1_B9BF_1CC7665038CB
