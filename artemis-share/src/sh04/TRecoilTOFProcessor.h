/**
 * @file   TRecoilTOFProcessor.h
 * @brief
 *
 * @date   Created:       2013-12-12 15:15:29
 *         Last Modified: 2014-04-22 09:44:56 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TRECOILTOFPROCESSOR_H
#define TRECOILTOFPROCESSOR_H

#include <TProcessor.h>

namespace art {
   namespace sh04 {
      class TRecoilTOFProcessor;
   }
}

class TClonesArray;

class art::sh04::TRecoilTOFProcessor : public TProcessor {
public:
   // Default constructor
   TRecoilTOFProcessor();
   virtual ~TRecoilTOFProcessor();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   /* input */
   TString        fF3PlasticColName;
   Int_t          fF3PlasticDetID;
   TClonesArray **fF3Plastic; //!
   TString        fFH9PlasticColName;
   Int_t          fFH9PlasticDetID;
   TClonesArray **fFH9Plastic; //!
   TString        fS0DPlasticColName;
   Int_t          fS0DPlasticDetID;
   TClonesArray **fS0DPlastic; //!
   TString        fTPlaColName;
   Int_t          fTPlaLDetID;
   Int_t          fTPlaRDetID;
   TClonesArray **fTPlastic; //!

   Float_t        fShift;

   /* output */
   TString      fOutputLName;
   TString      fOutputRName;
   TString      fOutputS0DName;
   TClonesArray *fTOFL;
   TClonesArray *fTOFR;
   TClonesArray *fTOFS0D;

   /* parameters */
   Float_t  fBeamZ;
   TString  fBeamName;
   Double_t fPrinciple;
   Double_t fTOffsetUS;
   Double_t fTOffsetL;
   Double_t fTOffsetR;

private:
   // Copy constructor (prohibited)
   TRecoilTOFProcessor(const TRecoilTOFProcessor&);
   // Assignment operator (prohibited)
   TRecoilTOFProcessor& operator=(const TRecoilTOFProcessor&);

   ClassDef(TRecoilTOFProcessor,0) // processor for TOF of SHARAQ04 recoil particle
};

#endif // TRECOILTOFPROCESSOR_H
