/**
 * @file   TSegmentCheckProcessor.h
 * @brief  segment checking processor
 *
 * @date   Created       : 2014-05-18 14:13:24 JST
 *         Last Modified : Jun 01, 2014 18:43:03 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_2FDF93A9_EBEC_4A9F_9FA5_C3F9E7C657F7
#define INCLUDE_GUARD_UUID_2FDF93A9_EBEC_4A9F_9FA5_C3F9E7C657F7

#include "TProcessor.h"
#include <vector>

namespace art {
   class TSegmentCheckProcessor;
   class TSegmentInfo;
   class TSegmentedData;
   class TModuleInfo;
}

class TClonesArray;
class TFolder;
class TDirectory;

class art::TSegmentCheckProcessor : public TProcessor {
public:
   TSegmentCheckProcessor();
   virtual ~TSegmentCheckProcessor();

   TSegmentCheckProcessor(const TSegmentCheckProcessor& rhs);
   TSegmentCheckProcessor& operator=(const TSegmentCheckProcessor& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   TFolder *fFolder; //!
   TDirectory *fHistDir; //!
   TSegmentInfo *fSegmentInfo; //!
   TString fSegmentListName;
   TString fModuleListName;
   TClonesArray *fSegmentList; //!
   TClonesArray *fModuleList; //!

   TSegmentedData **fSegmentedData;//!
   TString fSegmentedDataName;
   std::vector<Int_t> fIgnore; //! list of ignored segment
   std::map<Int_t, std::vector<TModuleInfo*> > fSegments; //!

private:

   ClassDef(TSegmentCheckProcessor,1) // segment checking processor
};

#endif // INCLUDE_GUARD_UUID_2FDF93A9_EBEC_4A9F_9FA5_C3F9E7C657F7
