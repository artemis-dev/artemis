/**
 * @file   TEventInfoGenerator.h
 * @brief  add simulation information
 *
 * @date   Created       : 2018-05-23 22:40:37 JST
 *         Last Modified : 2018-05-24 10:12:22 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_1F5D2205_B414_4B44_ADDD_67FBDBB1AC10
#define INCLUDE_GUARD_UUID_1F5D2205_B414_4B44_ADDD_67FBDBB1AC10

#include "TProcessor.h"

namespace art {
   class TEventInfoGenerator;
   class TEventHeader;
}

class art::TEventInfoGenerator : public TProcessor {
public:
   TEventInfoGenerator();
   virtual ~TEventInfoGenerator();

   TEventInfoGenerator(const TEventInfoGenerator& rhs);
   TEventInfoGenerator& operator=(const TEventInfoGenerator& rhs);

   virtual void Init(TEventCollection *col);
   virtual void BeginOfRun();
   virtual void PostLoop();
   virtual void Process();

protected:

   TString fRunName;
   Int_t   fRunNumber;

   TString fRunHeaderName; // name of run header
   TList *fRunHeader; //!
   TString fEventHeaderName; // name of event header
   TEventHeader *fEventHeader; //!

private:

   ClassDef(TEventInfoGenerator,1) // add simulation information
};

#endif // INCLUDE_GUARD_UUID_1F5D2205_B414_4B44_ADDD_67FBDBB1AC10
