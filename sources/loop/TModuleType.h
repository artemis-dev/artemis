/**
 * @file   TModuleType.h
 * @brief  module type description
 *
 * @date   Created       : 2014-04-28 16:22:48 JST
 *         Last Modified : May 29, 2014 15:50:29 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_8CC8C935_5B35_418E_98FA_3ACB167CEC1D
#define INCLUDE_GUARD_UUID_8CC8C935_5B35_418E_98FA_3ACB167CEC1D

#include "TParameterObject.h"
#include <TAxis.h>
#include <TClonesArray.h>

namespace art {
   class TModuleType;
}

namespace YAML {
   class Node;
}

class art::TModuleType : public TParameterObject {
public:
   TModuleType();
   virtual ~TModuleType();

   TModuleType(const TModuleType& rhs);
   TModuleType& operator=(const TModuleType& rhs);

   virtual void SetValueRange(Int_t idx, const char *name, Int_t nbins, Double_t min, Double_t max);
   virtual Bool_t LoadYAMLNode(const YAML::Node& node);
   virtual const TAxis* GetRange(Int_t idx) const { return (TAxis*) fAxes[idx]; }
   virtual Int_t GetNch() const { return fNch; }
   virtual Int_t GetGeo() const { return fGeo; }
   virtual Int_t GetNumValues() const { return fAxes.GetEntriesFast(); }
   virtual Int_t GetDecoderID() { return fDecoderID; }

   virtual void SetGeo(Int_t geo) { fGeo = geo; }

   virtual void Print(Option_t *opt) const; // *MENU* 

protected:
   Int_t fDecoderID;  // module id 
   Int_t fNch; // the number of channel
   Int_t fGeo; // geographical address, virtual station number or other identification of indivisual module other than the segment id
   TClonesArray fAxes; // array of axes to indicate the ranges of values.
   

private:

   ClassDef(TModuleType,1) // module type description 
};

#endif // INCLUDE_GUARD_UUID_8CC8C935_5B35_418E_98FA_3ACB167CEC1D
