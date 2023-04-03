/**
 * @file   TResidualPIDProcessor.h
 * @brief  processor for residual PID
 *
 * @date   Created       : 2014-12-25 16:31:13 JST
 *         Last Modified : 2018-02-04 12:06:05 JST (ota)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_A6A760B4_076F_4E49_972B_8135E88F0BAB
#define INCLUDE_GUARD_UUID_A6A760B4_076F_4E49_972B_8135E88F0BAB

#include "TProcessor.h"

namespace art {
   namespace sh04 {
      class TResidualPIDProcessor;
      class TParticleIdentifier;
   }
   class TGenericData;
}

class TClonesArray;

class art::sh04::TResidualPIDProcessor : public TProcessor {
public:
   TResidualPIDProcessor();
   virtual ~TResidualPIDProcessor();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   /* input */
   TString        fChargeColName;
   TClonesArray **fChargeArray; //!
   TString        fTOFColName;
   TClonesArray **fTOFArray; //!
   TString        fMWDCColName;
   TClonesArray **fMWDCArray; //!

   /* output */ 
   TParticleIdentifier *fPID; //!
   TString              fPIDName;
   TGenericData        *fFlightLength; //!
   TString              fFlightLengthName;

   /* parameter */
   Float_t fP1; // parameter 1
   Float_t fP2; // parameter 2
   Float_t fBrho; // magnetic rigidity of the beam
   Float_t fDispersion; // dispersion at the focal plane
   TString fBeamParameterName;
   
private:
   TResidualPIDProcessor(const TResidualPIDProcessor& rhs);
   TResidualPIDProcessor& operator=(const TResidualPIDProcessor& rhs);

   ClassDef(TResidualPIDProcessor,1) // processor for residual PID
};

#endif // INCLUDE_GUARD_UUID_A6A760B4_076F_4E49_972B_8135E88F0BAB
