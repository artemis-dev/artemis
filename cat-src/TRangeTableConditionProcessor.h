/**
 * @file   TRangeTableConditionProcessor.h
 * @brief  range table condition
 *
 * @date   Created       : 2016-09-18 10:38:55 JST
 *         Last Modified : 2016-09-18 23:42:34 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_520BCBCC_5471_4380_AFDD_6A0D871CABAA
#define INCLUDE_GUARD_UUID_520BCBCC_5471_4380_AFDD_6A0D871CABAA

#include "TProcessor.h"
#include <TRangeTableHelper.h>
namespace art {
   class TRangeTableConditionProcessor;
}

class art::TRangeTableConditionProcessor : public TProcessor {
public:
   TRangeTableConditionProcessor();
   virtual ~TRangeTableConditionProcessor();

   TRangeTableConditionProcessor(const TRangeTableConditionProcessor& rhs);
   TRangeTableConditionProcessor& operator=(const TRangeTableConditionProcessor& rhs);

protected:

private:
   TRangeTableHelper fHelper;

   ClassDef(TRangeTableConditionProcessor,1) // range table condition
};

#endif // INCLUDE_GUARD_UUID_520BCBCC_5471_4380_AFDD_6A0D871CABAA
