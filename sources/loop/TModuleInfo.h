/**
 * @file   TModuleInfo.h
 * @brief  module information
 *
 * @date   Created       : 2014-05-17 09:24:38 JST
 *         Last Modified : Jun 01, 2014 21:54:37 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_6804FCBC_B769_4240_849F_67ECD5D0727C
#define INCLUDE_GUARD_UUID_6804FCBC_B769_4240_849F_67ECD5D0727C

#include "TParameterObject.h"
#include <TAxis.h>
#include <vector>
#include <TObjArray.h>
#include <TH1.h>


namespace art {
   class TModuleInfo;
   class TModuleType;
}

namespace YAML {
   class Node;
}

class art::TModuleInfo : public TParameterObject {
public:
   TModuleInfo();
   virtual ~TModuleInfo();

   TModuleInfo(const TModuleInfo& rhs);
   TModuleInfo& operator=(const TModuleInfo& rhs);

   virtual Int_t GetID() const { return fID; }
   virtual TString GetModuleType() const { return fType; }
   virtual void SetHist(Int_t idx, TH1 *h1) { fHists->AddAt(h1,idx); }
   virtual TH1* GetHist(Int_t idx) { return (TH1*) fHists->At(idx); }
   virtual Int_t GetNumValues() { return fHists->GetEntriesFast(); }
   
   virtual Bool_t LoadYAMLNode(const YAML::Node& /* node */);

   virtual void Print(Option_t *opt = "") const;
protected:
   Int_t fID; // module id
   TString fType; //-> module type
   std::vector<TAxis*> fRanges; // histograms not saved implicitly
   TObjArray  *fHists; //!
private:

   ClassDef(TModuleInfo,1) // module information
};

#endif // INCLUDE_GUARD_UUID_6804FCBC_B769_4240_849F_67ECD5D0727C
