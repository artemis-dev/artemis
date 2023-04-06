/**
 * @file   TRecoilProcessor.h
 * @brief
 *
 * @date   Created:       2013-12-09 16:47:39
 *         Last Modified: 2015-04-12 16:58:01 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TRECOILPROCESSOR_H
#define TRECOILPROCESSOR_H

#include <TProcessor.h>

namespace art {
   namespace sh04 {
      class TRecoilProcessor;
   }
   class ITiming;
}

class TClonesArray;
class TVector3;
class TLorentzVector;
class TSpline3;

class art::sh04::TRecoilProcessor : public TProcessor {
public:
   // Default constructor
   TRecoilProcessor();
   virtual ~TRecoilProcessor();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   /* input */
   TString	  fTrackColName;
   TClonesArray **fTrack;         //!
   TString        fTOFColName;
   TClonesArray **fTOF;           //!
   TString        fOutputColName;
   TString        fS0TrackName;
   TClonesArray **fS0Track;    //!
   TClonesArray **fVertex;
   TString        fVertexName;

   /* output */
   TLorentzVector *fRecoilOut;     //!

   /* parameters */
   Int_t   fSide;
   TString fParameterName;
   Float_t fMWDCDistance;
   Float_t fMWDCAngle;
   Float_t fMWDCYOffset;
   Float_t fStopDetDistance;
   Float_t fStopDetAngle;
   Float_t fBeamZ;         // for carbon analysis
   Float_t fAngleShift;    // magnitude of angle correction (rad)

   TString                fTableFile;
   std::vector<Double_t>  fThetas;  // array of theta for TOF->E conversion parameter table
   std::vector<TSpline3*> fSplines;

   /* temp variables */
   TVector3 *fP;             //! momentum of recoil particle 
   Double_t  fM;             //! mass of recoil particle
   TVector3 *fStopDetNormal; //! normal vector of stop timing detector

   /* switches */
   Bool_t fUseReactionPointCorrection;
   Bool_t fUseIncidentAngleCorrection;
   Bool_t fUsePolarizedMagnetCorrection;
   Bool_t fUseTable;

private:
   void CorrectionWithS0Track();
   void CorrectionWithVertex();
   Double_t CalcMomentum(const ITiming *tof);
   Double_t CalcMomentumWithTable(const ITiming *tof, Double_t theta, Double_t phi);

   Bool_t LoadTable(const TString &filename);

   // Copy constructor (prohibited)
   TRecoilProcessor(const TRecoilProcessor&);
   // Assignment operator (prohibited)
   TRecoilProcessor& operator=(const TRecoilProcessor&);

   ClassDef(TRecoilProcessor,0) // processor for SHARAQ04 recoil particles
};

#endif // TRECOILPROCESSOR_H
