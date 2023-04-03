/**
 * @file   TUpdateParticleMomentumProcessor.h
 * @brief  update particle momentum using track information
 *
 * @date   Created       : 2017-09-26 19:32:53 JST
 *         Last Modified : 2018-02-24 20:13:27 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2017 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_24920B10_B93C_4120_9AE9_518C66A665B4
#define INCLUDE_GUARD_UUID_24920B10_B93C_4120_9AE9_518C66A665B4

#include "TProcessor.h"

namespace art {
   class TUpdateParticleMomentumProcessor;
   class TRangeTableHelper;
}
class TArtParticle;

class TClonesArray;

class art::TUpdateParticleMomentumProcessor : public TProcessor {
public:
   TUpdateParticleMomentumProcessor();
   virtual ~TUpdateParticleMomentumProcessor();

   TUpdateParticleMomentumProcessor(const TUpdateParticleMomentumProcessor& rhs);
   TUpdateParticleMomentumProcessor& operator=(const TUpdateParticleMomentumProcessor& rhs);

   void Init(TEventCollection *col);
   void Process();

protected:
   TString fNameParticle; // name of input partcile name
   TString fNameTrack; // name of input particle
   TString fNameRangeTable; // name of range table

   TClonesArray **fParticle; //! pointer to input particle
   TClonesArray **fTrack; //! pointer to clones array of track (use first track)
   TRangeTableHelper **fRangeTableHelper; //! range table helper

   Int_t fIgnoreRange; // ignore range of input track (only angular information is used)

private:

   ClassDef(TUpdateParticleMomentumProcessor,1) // update particle momentum using track information
};

#endif // INCLUDE_GUARD_UUID_24920B10_B93C_4120_9AE9_518C66A665B4
