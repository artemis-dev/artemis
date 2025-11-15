/**
 * @file   TPPACProcessorQTC.h
 */

#ifndef TPPACPROCESSORQTC_H
#define TPPACPROCESSORQTC_H

#include <TProcessor.h>

#include <vector>

namespace art {
   class TPPACProcessorQTC;
   class TCategorizedData;
   class TConverterBase;
   class TPPACParameter;
}

class TClonesArray;

class art::TPPACProcessorQTC : public TProcessor {

public:
   // Default constructor
   TPPACProcessorQTC();
   virtual ~TPPACProcessorQTC();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   StringVec_t		  fInputColName;
   TString		  fOutputColName;
   TCategorizedData	**fCategorizedData; //!
   TClonesArray		 *fPPACOut;

   Int_t fCatID; // default: 1 (ppac in anapaw)

   static const Int_t DETID_ORIGIN = 0;

   TString fTimingConverterArrayName;
   TString fChargeConverterArrayName;
   TString fXConverterArrayName;
   TString fYConverterArrayName;

   std::vector<TConverterBase*> *fTimingConverterArray; //!
   std::vector<TConverterBase*> *fChargeConverterArray; //!
   std::vector<TConverterBase*> *fXConverterArray;      //!
   std::vector<TConverterBase*> *fYConverterArray;      //!


   // using ppac parameter set
   TString fParameterName; // name of parameter set 
   TClonesArray **fParameterSet; //! parameter set
   StringVec_t fListOfParameterNames; // list of parameter name for each ppac
   std::vector<TPPACParameter*> fPPACParameter; //! list of parmaeter for each ppac
   // output separately
   StringVec_t fListOfPPACNames; // list of ppac name for each ppac
   std::vector<TClonesArray*> fPPACArray; //! list of ppacs
   Bool_t fDoSeparatePPACs; // flag if separte the output of PPACs
   Bool_t fHasEachChConverter; // flag if the converter for each channel is prepared
   Bool_t fDoInsideOffset; // flag to calibrate inside offset
   Bool_t fDoOutsideOffset; // flag to calibrate outside offset
   Bool_t fDoGeometryOffset; // flag to calibrate geometry offset
   

private:
   // Copy constructor (prohibited)
   TPPACProcessorQTC(const TPPACProcessorQTC& rhs);
   // Assignment operator (prohibited)
   TPPACProcessorQTC& operator=(const TPPACProcessorQTC& rhs);

   ClassDef(TPPACProcessorQTC,0) // processor for PPAC analysis
};

#endif // TPPACPROCESSORQTC_H
