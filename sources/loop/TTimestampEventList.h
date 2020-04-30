/**
 * @file   TTimestampEventList.h
 * @brief  event list with timestamp and corresponding file
 *
 * @date   Created       : 2018-06-27 11:49:43 JST
 *         Last Modified : 2018-06-27 17:25:07 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_3B9722C4_D401_4392_99CB_01F8CA5B31D3
#define INCLUDE_GUARD_UUID_3B9722C4_D401_4392_99CB_01F8CA5B31D3

#include "TNamed.h"

namespace art {
   class TTimestampEventList;
}

class TEventList;

class art::TTimestampEventList : public TNamed {
public:
   TTimestampEventList();
   virtual ~TTimestampEventList();

   TTimestampEventList(const TTimestampEventList& rhs);
   TTimestampEventList& operator=(const TTimestampEventList& rhs);

   virtual Long64_t GetEntry(Long64_t index) const { return (index < fNumEntries) ? fEntries[index] : -1; }
   virtual Double_t GetTimestamp(Long64_t index) const { return (index < fNumEntries) ? fTimestamps[index] : -1; }
   virtual Long64_t GetN() const { return fNumEntries; };
   virtual const char* GetFileName() const { return fFileName.Data(); }
   virtual TEventList* GetEventList() const;

   virtual void AddEntry(Long64_t entry, Double_t timestamp);
   virtual void SetFileName(const char* filename) { fFileName = filename; }
   
   virtual void DirectoryAudoAdd(TDirectory *dir);
   virtual void SetDirectory(TDirectory *dir);
   virtual TDirectory* GetDirection() const { return fDirectory; };

protected:
   Long64_t fNumEntries; // size of 
   std::vector<Long64_t> fEntries; // entry
   std::vector<Double_t> fTimestamps; // timestamp

   TString fFileName; // name of corresponding file

   TDirectory* fDirectory; //! Pointer to directory holding this event list

private:

   ClassDef(TTimestampEventList,1) // event list with timestamp and corresponding file
};

#endif // INCLUDE_GUARD_UUID_3B9722C4_D401_4392_99CB_01F8CA5B31D3
