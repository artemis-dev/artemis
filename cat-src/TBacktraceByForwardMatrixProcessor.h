// @file TBacktraceProcessor.h
// @parameters
// track in track at the detector
// a out 
// y out
// b out
// d out

#ifndef INCLUDE_GUARD_TBacktraceByForwardMatrixProcessor_h
#define INCLUDE_GUARD_TBacktraceByForwardMatrixProcessor_h


#include "TProcessor.h"
#include "TTrack.h"

namespace art {
   class TBacktraceByForwardMatrixProcessor;
}

class art::TBacktraceByForwardMatrixProcessor : public TProcessor {
  public:
  TBacktraceByForwardMatrixProcessor();
  virtual ~TBacktraceByForwardMatrixProcessor();


  virtual void Init(TEventCollection* col );
  virtual void Process();

  private:
  
  InputData<TClonesArray,TTrack> fInput;
  OutputVec<double> fOutput;

  ClassDef(TBacktraceByForwardMatrixProcessor,1)
};

#endif // INCLUDE_GUARD_TBacktraceByForwardMatrixProcessor_h