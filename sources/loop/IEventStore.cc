/**
 * @file   IEventStore.cc
 * @brief  iterface for event store
 *
 * @date   Created       : 2018-07-28 16:46:53 JST
 *         Last Modified : 2018-07-28 16:46:55 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#include "IEventStore.h"

using art::IEventStore;

ClassImp(IEventStore)

IEventStore::IEventStore()
{
}

IEventStore::~IEventStore()
{
}

IEventStore::IEventStore(const IEventStore& rhs)
{
}

IEventStore& IEventStore::operator=(const IEventStore& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}
