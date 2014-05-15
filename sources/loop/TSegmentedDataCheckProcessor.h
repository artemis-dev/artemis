/**
 * @file   TSegmentedDataCheckProcessor.h
 * @brief  Checker processor for TSegmentedData
 *
 * @date   Created       : 2014-04-27 14:09:31 JST
 *         Last Modified : Apr 27, 2014 14:10:02 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_04AC7DEE_D7C3_4394_9F1F_F37694345E63
#define INCLUDE_GUARD_UUID_04AC7DEE_D7C3_4394_9F1F_F37694345E63

#include "TProcessor.h"

namespace art {
   class TSegmentedDataCheckProcessor;
}

class art::TSegmentedDataCheckProcessor : public TProcessor {
public:
   TSegmentedDataCheckProcessor();
   virtual ~TSegmentedDataCheckProcessor();

   TSegmentedDataCheckProcessor(const TSegmentedDataCheckProcessor& rhs);
   TSegmentedDataCheckProcessor& operator=(const TSegmentedDataCheckProcessor& rhs);

   void Init(TEventHeader *col);
   void Process();

protected:

private:

   ClassDef(TSegmentedDataCheckProcessor,1) // Checker processor for TSegmentedData
};

#endif // INCLUDE_GUARD_UUID_04AC7DEE_D7C3_4394_9F1F_F37694345E63
