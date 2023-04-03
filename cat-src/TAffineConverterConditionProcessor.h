/**
 * @file   TAffineConverterConditionProcessor.h
 * @brief  time dependent affine converter
 *
 * @date   Created       : 2016-11-15 17:53:02 JST
 *         Last Modified : 2019-09-09 18:03:18 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_DFA1F8B7_7079_4736_BFC3_9CBF551D9808
#define INCLUDE_GUARD_UUID_DFA1F8B7_7079_4736_BFC3_9CBF551D9808

#include "TProcessor.h"

namespace art {
   class TAffineConverterConditionProcessor;
   class TEventHeader;
}
class TClonesArray;

class art::TAffineConverterConditionProcessor : public TProcessor {
public:
   TAffineConverterConditionProcessor();
   virtual ~TAffineConverterConditionProcessor();

   TAffineConverterConditionProcessor(const TAffineConverterConditionProcessor& rhs);
   TAffineConverterConditionProcessor& operator=(const TAffineConverterConditionProcessor& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   virtual Bool_t LoadFile(const TString& file, TClonesArray* converterArray);

private:
   TEventHeader **fEventHeader; //!
   TString        fNameEventHeader;
   TList *fConverterArrayList; //!
   StringVec_t fParameterList;

   Double_t fNextTimestamp;
   Int_t fNextIdx;

   Parameter<Int_t> fTypeOfTimestamp;
   
   
   std::vector<std::vector<Double_t> > fTimestamp;
   std::vector<std::vector<Double_t> > fA0;
   std::vector<std::vector<Double_t> > fA1;

   ClassDef(TAffineConverterConditionProcessor,1) // time dependent affine converter
};

#endif // INCLUDE_GUARD_UUID_DFA1F8B7_7079_4736_BFC3_9CBF551D9808
