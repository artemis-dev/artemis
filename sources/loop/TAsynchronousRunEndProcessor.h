/**
 * @file   TAsynchronousRunEndProcessor.h
 * @brief  stop loop asynchronously by checking run status of each event store
 *
 * @date   Created       : 2018-06-29 18:15:33 JST
 *         Last Modified : 2018-06-29 18:29:03 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_89B66250_087D_4CC9_A5BE_1F3AF138A2FE
#define INCLUDE_GUARD_UUID_89B66250_087D_4CC9_A5BE_1F3AF138A2FE

#include "TProcessor.h"

namespace art {
   class TAsynchronousRunEndProcessor;
}

class art::TAsynchronousRunEndProcessor : public TProcessor {
public:
   TAsynchronousRunEndProcessor();
   virtual ~TAsynchronousRunEndProcessor();

   TAsynchronousRunEndProcessor(const TAsynchronousRunEndProcessor& rhs);
   TAsynchronousRunEndProcessor& operator=(const TAsynchronousRunEndProcessor& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   StringVec_t fRunStatusNames; // name of run status
   std::vector<TConditionBit*> fRunStatus; //! set of run status

private:

   ClassDef(TAsynchronousRunEndProcessor,1) // stop loop asynchronously by checking run status of each event store
};

#endif // INCLUDE_GUARD_UUID_89B66250_087D_4CC9_A5BE_1F3AF138A2FE
