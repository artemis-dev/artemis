/* $Id:$ */
/**
 * @file   TBinaryReactionGenerator.h
 * @date   Created : Aug 18, 2013 12:18:48 JST
 *   Last Modified : Aug 22, 2013 15:45:33 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TBINARYREACTIONGENERATOR_H
#define TBINARYREACTIONGENERATOR_H

#include <TProcessor.h>


// forward declaration
namespace art {
   class TBinaryReactionGenerator;
}
   class TCatTwoBodyKinematics;
class TH2F;
class TClonesArray;

class art::TBinaryReactionGenerator  : public TProcessor {

public:
   TBinaryReactionGenerator();
   virtual ~TBinaryReactionGenerator();

   virtual void Init(TEventCollection *col);
   virtual void Process();

private:
   Int_t fMaxLoop;
   IntVec_t fP1; // mass and atomic number for particle 1
   IntVec_t fP2; // mass and atomic number for particle 2
   IntVec_t fP3; // mass and atomic number for particle 3

   Float_t fKinMean;
   Float_t fExMean;
   Float_t fExWidth;
   Int_t   fAngMom;
   TString fAngDistFile;

   Int_t fNumLoop;

   TCatTwoBodyKinematics *fKinematics;
   TClonesArray *fOutputArray;
   TClonesArray *fMCTruthArray;
   TString fOutputColName;
   TString fMCTruthColName;
   FloatVec_t fExRange;
   FloatVec_t fAngRange;
   
   

   
   TH2F *fExAngDistribution;
};
#endif // end of #ifdef TBINARYREACTIONGENERATOR_H
