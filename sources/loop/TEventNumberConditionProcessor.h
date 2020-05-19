/* @file TEventNumberConditionProcessor.h
 * @brief processor for event number alignment
 * This processor is to align event number or event list index for multiple input (ridf).
 * This processor can be placed after one event store and before a certain skip processor.
 * Following event stores can refer the event number stored in the event index prepared by this processor in UserInfo.
 *
 * @date Create        : 2019-06-09 12:55:19 JST
 *       Last Modified : 2019-05-15 11:05:47 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */

#ifndef INCLUDE_GUARD_UUID_6AA37CCC_B187_4E8B_9741_BA48C4BC3999
#define INCLUDE_GUARD_UUID_6AA37CCC_B187_4E8B_9741_BA48C4BC3999

#include "TProcessor.h"

namespace art {
   class TEventNumberConditionProcessor;
   class TSimpleDataLong;
}


class art::TEventNumberConditionProcessor : public TProcessor {
public:
   TEventNumberConditionProcessor();
   virtual ~TEventNumberConditionProcessor();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   OutputInfo<TSimpleDataLong> fOutput;
   

   ClassDef(TEventNumberConditionProcessor,1)
}; // end of TEventNumberConditionProcessor

#endif // #ifndef INCLUDE_GUARD_UUID_6AA37CCC_B187_4E8B_9741_BA48C4BC3999}
