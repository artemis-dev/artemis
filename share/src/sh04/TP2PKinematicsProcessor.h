/**
 * @file   TP2PKinematicsProcessor.h
 * @brief
 *
 * @date   Created:       2013-12-11 14:38:49
 *         Last Modified: 2014-10-28 09:48:52 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TP2PKINEMATICSPROCESSOR_H
#define TP2PKINEMATICSPROCESSOR_H

#include <TProcessor.h>

namespace art {
   namespace sh04 {
      class TP2PKinematicsProcessor;
      class TP2PKinematicsData;
   }
}

class TLorentzVector;
class TVector3;
class TClonesArray;

class art::sh04::TP2PKinematicsProcessor : public TProcessor {
public:
   // Default constructor
   TP2PKinematicsProcessor();
   virtual ~TP2PKinematicsProcessor();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   /* inputs */
   TString          fRecoilLName;
   TLorentzVector **fRecoilL;             //!
   TString          fRecoilRName;
   TLorentzVector **fRecoilR;             //!
   TString          fF6PPACColName;
   TClonesArray   **fF6PPAC;              //!
   Int_t            fF6PPACDetID;

   /* output */
   TString             fOutputColName;
   TP2PKinematicsData *fKinematics; //!

   /* parameters */
   TString  fParameterName;
   Float_t  fBrho;
   Int_t    fA;
   Int_t    fZ;
   Double_t fM; // mass of beam nuclei
   Double_t fQvalue;
   Float_t  fF6XDelta;
   Bool_t   fIsInverseKinematics;
   Int_t    fResidualFrame;

private:
   Bool_t LoadParameterFile();

   // Copy constructor (prohibited)
   TP2PKinematicsProcessor(const TP2PKinematicsProcessor&);
   // Assignment operator (prohibited)
   TP2PKinematicsProcessor& operator=(const TP2PKinematicsProcessor&);

   ClassDef(TP2PKinematicsProcessor,0) // processor for SHARAQ04 (p,2p) kinematics calculation
};

#endif // TP2PKINEMATICSPROCESSOR_H
