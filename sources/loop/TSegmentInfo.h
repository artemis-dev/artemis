/**
 * @file   TSegmentInfo.h
 * @brief  segment information
 *
 * @date   Created       : 2014-05-17 09:13:41 JST
 *         Last Modified : Jun 05, 2014 23:30:32 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_747AA7E2_ADDB_4034_8C55_830A9CC389B9
#define INCLUDE_GUARD_UUID_747AA7E2_ADDB_4034_8C55_830A9CC389B9

#include "TParameterObject.h"
#include <vector>

namespace art {
   class TSegmentInfo;
   class TModuleInfo;
}

class art::TSegmentInfo : public TParameterObject {
public:
   typedef enum { RIDF, RDF } FormatType_t;
   TSegmentInfo();
   virtual ~TSegmentInfo();

   TSegmentInfo(const TSegmentInfo& rhs);
   TSegmentInfo& operator=(const TSegmentInfo& rhs);


   virtual Bool_t LoadYAMLNode(const YAML::Node& /* node */);
   virtual Int_t GetNumModules() { return fModules.size(); }
   virtual TModuleInfo* GetModule(Int_t idx) { return fModules[idx]; }
   virtual Int_t GetID(Int_t idx = 0) { return fID[idx]; }
   virtual Int_t GetSegID() {
      if (fgFormatType == RDF) return fID[0];
      if (fgFormatType == RIDF) return (((fID[0]&0x3f)<<20) | ((fID[1]&0x3f)<<14) |((fID[2]&0x3f)<<8));
      return -1;
   }
   virtual void Print(Option_t *opt) const;
   virtual TString GetModuleType() { return fModuleType; }
   static void SetFormatType(FormatType_t type) { fgFormatType = type; }
protected:
   std::vector<Int_t> fID; // segment id
   TString fModuleType; // module type
   std::vector<TModuleInfo*> fModules; // module information
   static FormatType_t fgFormatType;

private:

   ClassDef(TSegmentInfo,1) // segment information
};

#endif // INCLUDE_GUARD_UUID_747AA7E2_ADDB_4034_8C55_830A9CC389B9
