/* $Id:$ */
/**
 * @file   TCoinRegMappingProcessor.h
 * @date   Created : Oct 31, 2013 06:31:02 JST
 *   Last Modified : 2014-06-19 17:01:04 JST (kawase)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *
 *    Copyright (C)2013
 */
#ifndef TCOINREGMAPPINGPROCESSOR_H
#define TCOINREGMAPPINGPROCESSOR_H

#include <TProcessor.h>

namespace art {
   class TCoinRegMappingProcessor;
   class TCategorizedData;
   class TCoinRegData;
}

class art::TCoinRegMappingProcessor  : public art::TProcessor {

public:
   TCoinRegMappingProcessor();
   virtual ~TCoinRegMappingProcessor();

   virtual void Process();

protected:
   TString            fInputColName;
   TString            fOutputColName;
   TCategorizedData **fCategorized; //!
   TCoinRegData      *fRegHit;
   Int_t              fCatID;
   Int_t              fDetID;
   Int_t             fMask;

   ClassDef(TCoinRegMappingProcessor,1)
};
#endif // end of #ifdef TCOINREGMAPPINGPROCESSOR_H
