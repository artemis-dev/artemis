/**
 * @file   IEventStore.h
 * @brief  interface for event store
 *
 * @date   Created       : 2018-07-28 14:18:02 JST
 *         Last Modified : 2018-07-30 13:21:39 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_5FF932D8_B189_4B06_871A_ECF1B9134498
#define INCLUDE_GUARD_UUID_5FF932D8_B189_4B06_871A_ECF1B9134498

#include <Rtypes.h>

namespace art {
   class IEventStore;
}

class art::IEventStore {
public:
   IEventStore();
   virtual ~IEventStore();

   IEventStore(const IEventStore& rhs);
   IEventStore& operator=(const IEventStore& rhs);

   virtual Int_t GetRunNumber() const { return 0; }

   virtual const char* GetRunName() const { return ""; }

protected:

private:

   ClassDef(IEventStore,1) // interface for event store
};

#endif // INCLUDE_GUARD_UUID_5FF932D8_B189_4B06_871A_ECF1B9134498
