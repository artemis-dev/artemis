/**
 * @file   TTrackProjectionProcessor.h
 * @brief  project track to another plane
 *
 * @date   Created       : 2014-12-08 19:17:53 JST
 *         Last Modified : 2014-12-08 19:45:36 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_576178E3_D205_4D69_918C_4DADE13056B9
#define INCLUDE_GUARD_UUID_576178E3_D205_4D69_918C_4DADE13056B9

#include "TProcessor.h"

namespace art {
   namespace sh04 {
      class TTrackProjectionProcessor;
   }
}

class TClonesArray;

class art::sh04::TTrackProjectionProcessor : public TProcessor {
public:
   TTrackProjectionProcessor();
   virtual ~TTrackProjectionProcessor();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   /* input */
   TString        fInputColName;
   TClonesArray **fTrackIn;      //!

   /* output */
   TString        fOutputColName;
   TClonesArray  *fTrackOut;     //!

   /* parameters */
   Float_t        fZ;
   Float_t        fCenterX; // X pos of the center of rotation
   Float_t        fTheta;

   Double_t       fTan;

private:
   ClassDef(TTrackProjectionProcessor,1) // project track to another plane
};

#endif // INCLUDE_GUARD_UUID_576178E3_D205_4D69_918C_4DADE13056B9
