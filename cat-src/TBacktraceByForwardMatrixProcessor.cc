// @file TBacktraceProcessor.h
// @parameters
// track in track at the detector
// a out 
// y out
// b out
// d out

#include "TBacktraceByForwardMatrixProcessor.h"

ClassImp(art::TBacktraceByForwardMatrixProcessor)

using art::TBacktraceByForwardMatrixProcessor;



TBacktraceByForwardMatrixProcessor::TBacktraceByForwardMatrixProcessor()
{
  Register(fInput("InputTrack","input track at the detector","track"));
  Register(fOutput("Output","output a at the target","output"));
  

}

TBacktraceByForwardMatrixProcessor::~TBacktraceByForwardMatrixProcessor()
{
}


void TBacktraceByForwardMatrixProcessor::Init(TEventCollection* col ) {
  
}

  // fElements[destination][x order][a order][y order][b order][d order]

void TBacktraceByForwardMatrixProcessor::Process() {

  fOutput.Data()->clear();
  // fOutput->clear();
  if (fInput->GetEntriesFast() == 0) return;
  fOutput->push_back(((TTrack*)fInput->UncheckedAt(0))->GetA());
#if 0
  // solve (a,y,b,d) at the exit of the target from (x,a,y,b) at detector
  // The combination of 6 digits indicate the optical transfer matrix
  // The most significant digit indicates the destination, for instance, 0 project to x
  // 0 ~ 4 are label for (x,a,y,b,d) 
  // The next digit from the most significant digit indicate the order of the x and the rest are similar according to the order.
  // 001000 means (x|a), 001001 means (x|ad), 100001 means (a,d) and so on.
  
  // The first order term will be used to solve the inverse matrix.
  // The higher order term will be used to correct the first order solutions.

  const double z = 0. // focus plaane

  // double y = (fTrack->G / fElements[2][2][0]
  double delta = fTrack->GetX() / fElements[0][0][0][0][0][1];
  double a = (fTrack->GetA() - delta * fElements[1][0][0][0][0][1]) / fElements[1][0][1][0][0][0];
  double b = fTrack->GetY() / fElements[2][0][0][0][1][0];
#endif  
}