/**
 * @file   TReactionVertexProcessor.h
 * @brief  calculate reaction vertex in SHARAQ04
 *
 * @date   Created       : 2014-03-25 17:01:49 JST
 *         Last Modified : 2018-02-04 12:04:26 JST (ota)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_6667A81A_D3C9_4612_858A_E7DDF12CAB5D
#define INCLUDE_GUARD_UUID_6667A81A_D3C9_4612_858A_E7DDF12CAB5D

#include "TProcessor.h"

namespace art {
   namespace sh04 {
   class TReactionVertexProcessor;
   }
}

class TClonesArray;

class art::sh04::TReactionVertexProcessor : public TProcessor {
public:
   TReactionVertexProcessor();
   virtual ~TReactionVertexProcessor();

   void Init(TEventCollection *col);
   void Process();

protected:
   /* input */
   TString        fTrackLName;
   TClonesArray **fTrackL; //!
   TString        fTrackRName;
   TClonesArray **fTrackR; //!
   TString        fS0TrackName;
   TClonesArray **fS0Track; //!

   /* parameter */
   TString  fGeometryName;
   Double_t fDistance[2];
   Double_t fAngle[2];

   /* output */
   TString       fOutputColName;
   TClonesArray *fVertex; //!

private:
   TReactionVertexProcessor(const TReactionVertexProcessor&);
   TReactionVertexProcessor& operator=(const TReactionVertexProcessor&);

   ClassDef(TReactionVertexProcessor,0) // calculate reaction vertex in SHARAQ04
};

#endif // INCLUDE_GUARD_UUID_6667A81A_D3C9_4612_858A_E7DDF12CAB5D
