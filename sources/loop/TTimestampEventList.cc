/**
 * @file   TTimestampEventList.cc
 * @brief  container for timestamp and entry
 *
 * @date   Created       : 2018-06-27 15:00:39 JST
 *         Last Modified : 2018-06-27 17:25:15 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#include "TTimestampEventList.h"
#include <TEventList.h>
#include <TDirectory.h>

using art::TTimestampEventList;

ClassImp(TTimestampEventList)

TTimestampEventList::TTimestampEventList()
: fDirectory(NULL)
{
}

TTimestampEventList::~TTimestampEventList()
{
}

TTimestampEventList::TTimestampEventList(const TTimestampEventList& rhs)
{
}

TTimestampEventList& TTimestampEventList::operator=(const TTimestampEventList& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}


void TTimestampEventList::AddEntry(Long64_t entry, Double_t timestamp)
{
   fEntries.push_back(entry);
   fTimestamps.push_back(timestamp);
   fNumEntries = fEntries.size();
}

TEventList* TTimestampEventList::GetEventList() const
{
   TEventList *list = new TEventList;
   for (Long64_t i = 0; i < fNumEntries; ++i) {
      list->Enter(fEntries[i]);
   }
   return list;
}

void TTimestampEventList::SetDirectory(TDirectory *dir)
{
   if (fDirectory == dir) return;
   if (fDirectory) fDirectory->Remove(this);
   fDirectory = dir;
   if (fDirectory) fDirectory->Append(this);
}

void TTimestampEventList::DirectoryAudoAdd(TDirectory *dir)
{
   SetDirectory(dir);
}
