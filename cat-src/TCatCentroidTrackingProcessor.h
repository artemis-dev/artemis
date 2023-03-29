/**
 * @file   TCatCentroidTrackingProcessor.h
 * @brief
 *
 * @date   Created       : 2016-04-19 12:16:22 JST
 *         Last Modified : 2016-07-21 16:48:49 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_2F50F2FF_9484_4E3D_96B6_924139857EE4
#define INCLUDE_GUARD_UUID_2F50F2FF_9484_4E3D_96B6_924139857EE4

#include "TProcessor.h"

namespace art {
   class TCatCentroidTrackingProcessor;
   class TCatReadoutPadArray;
   class TSimpleData;
   class TEventHeader;
   class TEventDisplayProcessorHelper;
   class TRangeTableHelper;
}


class TVector3;
class TClonesArray;
class TH2Poly;
class TH2F;
class TLine;
class TGraph;

class art::TCatCentroidTrackingProcessor : public TProcessor {
public:
   TCatCentroidTrackingProcessor();
   virtual ~TCatCentroidTrackingProcessor();

   TCatCentroidTrackingProcessor(const TCatCentroidTrackingProcessor& rhs);
   TCatCentroidTrackingProcessor& operator=(const TCatCentroidTrackingProcessor& rhs);

   virtual void Process();
   virtual void Init(TEventCollection *col);

protected:

private:
   TString fInputNameGuide; // name of input array of selected hits
   TClonesArray **fInputGuide; //! array of input hit

   TString fInputNameAll; // name of input array of all the hits
   TClonesArray **fInputAll; // input array of all the hits
   
   TString fOutputName; // name of output array of calculated array
   TClonesArray *fOutput; //! output array of 

   TString fOutputNameTrack; // name of output array of calculated array
   TClonesArray *fOutputTrack; //! output array of 
   
   TString fOutputNameEndPoint; // name of output momentum
   TVector3 *fEndPoint; //! output 

   TString fOutputNameStartPoint; // name of output momentum
   TVector3 *fStartPoint; //! output

   TString fOutputNameFirstPadPos; // name of first
   TVector3 *fFirstPadPos; //! output

   TString fOutputNameLastPadPos; // name of output momentum
   TVector3 *fLastPadPos; //! output

   TString fInfoNameReadoutPadArray; // name of readout
   TCatReadoutPadArray **fReadoutPadArray; //!

   TString fOutputNameDEDX;
   TClonesArray *fOutputDEDX;

   // track starting point from outside
   TString fNameBeamMomentum;
   TString fNameBeamPosition;
   TClonesArray **fBeamMomentum; //!
   TClonesArray **fBeamPosition; //!

   TString fNameRangeMeasured;
   Double_t *fRangeMeasured;

   TString fNameTKECalc;
   Double_t *fTKECalc;

   TString fNameTKEMeasured;
   Double_t *fTKEMeasured;

   TString fNameEx; // name of excitation energy
   TSimpleData *fEx; //!
   TString fNameThetaCM; // name of thetaCM
   TSimpleData *fThetaCM; //!

   TString       fNameEventHeader;
   TEventHeader **fEventHeader;

   
   TH2Poly *fHistXZ; //! histogram for the hit pattern in xz
   TH2F    *fHistXY; //! histogram for the hit pattern in xy
   TLine   *fTrackXZ; //!
   TLine   *fTrackXY; //!


   ////////////////////////////////////////////////////////////////////////
   // utility

   TString fRangeTableFile;
   TString fRangeTableFormat;
   TGraph *fRangeTable;
   TGraph *fRange2EnergyTable;
   
   TEventDisplayProcessorHelper *fEventDisplayHelper;
   TRangeTableHelper *fRangeTableHelper;

   ClassDef(TCatCentroidTrackingProcessor,1) //
};

#endif // INCLUDE_GUARD_UUID_2F50F2FF_9484_4E3D_96B6_924139857EE4
