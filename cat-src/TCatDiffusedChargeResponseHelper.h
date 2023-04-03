/**
 * @file   TCatDiffusedChargeResponseHelper.h
 * @brief  helper to calculate the pad response for diffused charge
 *
 * @date   Created       : 2016-09-12 10:48:37 JST
 *         Last Modified : 2020-05-27 14:59:38 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_2B53EFAB_DAA5_4A46_A4BA_80103B841B93
#define INCLUDE_GUARD_UUID_2B53EFAB_DAA5_4A46_A4BA_80103B841B93

#include "IProcessorHelper.h"
#include <TVector3.h>
#include <vector>
#include <TRangeTableHelper.h>
#include <TMath.h>
#include <TArtSystemOfUnit.h>
#include <TGraph.h>
#include <TCatReadoutPadArray.h>
#include <TArtTypes.h>
#include "TTpcStaticGasProperty.h"

using TArtSystemOfUnit::mm;
using TArtSystemOfUnit::cm;


namespace art {
   class TCatDiffusedChargeResponseHelper;
   class TCatReadoutPadArray;
   class TCatReadoutPad;
   class TNArray;
   class TCatPulseShape;
   class TRandomGraph;

}

class TGraph;
class TH2Poly;


class art::TCatDiffusedChargeResponseHelper : public IProcessorHelper {
public:
   TCatDiffusedChargeResponseHelper();
   virtual ~TCatDiffusedChargeResponseHelper();

   TCatDiffusedChargeResponseHelper(const TCatDiffusedChargeResponseHelper& rhs);
   TCatDiffusedChargeResponseHelper& operator=(const TCatDiffusedChargeResponseHelper& rhs);

   void Init(TEventCollection *col);
   void Register(TProcessor *proc);
   
   void GetResponse(Int_t id, Double_t &charge, Double_t &closest, Double_t &angle, Double_t &rangeToEnd, Double_t& trackLength) const;
   Double_t GetCharge(Int_t id) const {
      Double_t charge, dummy;
      GetResponse(id,charge,dummy,dummy,dummy,dummy);
      return charge;
   }

   
   void SetTrack(const TVector3& start, const TVector3& end);
   const TCatReadoutPadArray* GetReadoutPadArray() const { return fReadoutPadArray.Data(); }
   const TRangeTableHelper* GetRangeTableHelper() const { return fRangeTable.Data(); }
   Double_t GetReadoutPlaneOffset() const { return fReadoutPlaneOffset; }
   Double_t GetDefaultIntegrateStep() const { return fDefaultIntegrateStep; }
   const std::vector<Double_t>& GetActiveArea() const { return fActiveArea.Value(); }
   double GetWorkFunction() const { return fTpcGasProp->GetWorkFunction(); }

   void DisplaceTrack(TVector3& start, TVector3& end);
   
   Double_t GetLongDiff(double length /* mm */) const { return fTpcGasProp->GetLongDiff(length); }
   Double_t GetTransDiff(double length /* mm */) const { return fTpcGasProp->GetTransDiff(length); }
   Double_t GetDriftLength(double time /* mm */) const { return fTpcGasProp->GetDriftLength(time); }
   //
   void SetAdditionalRange(double range) {fAdditionalRange = range;}

   //  function to calculate active range inside active volume
   void EstimateActiveRange();

   // function to calculate number of electrons inside active volume using active range
   Int_t GetNumOfElectrons() const;

   // function to calculate energy from number of electrons
   Double_t GetEnergyFrom(Double_t nElectrons) const;

   // function to get readout pad
   const TCatReadoutPad* GetReadoutPad(Int_t i) const;

   // set the position of  generate enelection
   // returns false if point is outside of active area
   Bool_t GeneratedElectronAt(TVector3& point);

   // function to get number of readout pads
   Int_t GetNumPads() const { return fReadoutPadArray->GetEntries(); }
   

   // helper parameters
   void SetDefaultIntegrateStep(Double_t step)  { fDefaultIntegrateStep = step; }
   void SetReadoutPadArray(TCatReadoutPadArray **array) { fReadoutPadArray.SetData(array); }
   void SetResponseTable(TNArray** table) { fResponseTable.SetData(table); }
   void SetReadoutPlaneOffset(Double_t offset) { fReadoutPlaneOffset = offset; }
   void SetRangeTableHelper(TRangeTableHelper **rangeTable) { fRangeTable.SetData(rangeTable); }
   void SetDisplacementTableX(TNArray** table) { fDisplacementTableX = table; }
   void SetDisplacementTableZ(TNArray** table) { fDisplacementTableZ = table; }
   void SetActiveArea(const std::vector<Double_t>& area) { fActiveArea = area; }

   

   virtual void Copy(TCatDiffusedChargeResponseHelper& object) const;

#if 0
   static void Test(const TVector3& start, const TVector3& end, Double_t step = 0.1);
   static void Test(Int_t id, const TVector3& start, const TVector3& end, Double_t& charge, Double_t &mccharge);
   static void TestConvergenceFor(Int_t id, TVector3 start, TVector3 end, Double_t dc = 0.04);
#endif
protected:
   TVector3 fTrackStart; //! track start point
   TVector3 fTrackEnd;  //! track end point
   TVector3 fDirection; //! direction with magnitude
   TVector3 fUnitDirection; //! unit direction
   Double_t fRange; //! range
   Int_t    fNumStep; //! calculated number of step
   Double_t fStep; //! calculated step

   std::vector<TVector2> fPadCenter; //!
   std::vector<TVector2> fPadVertex; //!


   InputInfo<TTpcStaticGasProperty> fTpcGasProp;
   InputInfo<TNArray> fResponseTable;
   InputInfo<TCatReadoutPadArray> fReadoutPadArray;
   
   Parameter<Double_t> fDefaultIntegrateStep; //! default integrate step
   Parameter<Double_t> fReadoutPlaneOffset;
   Parameter<Int_t> fUseDisplacement3D;
   Parameter<DoubleVec_t> fActiveArea;
   InputInfo<TRangeTableHelper> fRangeTable;

   TGraph *fR2DE; //!


   TRandomGraph *fRandomBrag; //!

   TString fDisplacementTableXName; // name of displacement table for X
   TString fDisplacementTableZName; // name of displacement table for Z
   TNArray **fDisplacementTableX; //!
   TNArray **fDisplacementTableZ; //!

   Double_t fAdditionalRange; // range to be added in evaluation of charge
    

private:
   Double_t fShorterRange; //! used for charge calculation inside
   Double_t fLongerRange; //! used for charge calculation inside

   ClassDef(TCatDiffusedChargeResponseHelper,1) // helper to calculate the pad response for diffused charge
};


inline Int_t art::TCatDiffusedChargeResponseHelper::GetNumOfElectrons() const
{
   return TMath::FloorNint(fTpcGasProp->GetNumElectrion((fRangeTable)->R2E(fLongerRange) - (fRangeTable)->R2E(fShorterRange)));
}

inline Double_t art::TCatDiffusedChargeResponseHelper::GetEnergyFrom(Double_t nEl) const
{
   return fTpcGasProp->GetEnergy(nEl);
}

inline const art::TCatReadoutPad* art::TCatDiffusedChargeResponseHelper::GetReadoutPad(Int_t i) const
{
   return (TCatReadoutPad*)((fReadoutPadArray)->At(i));
}



#endif // INCLUDE_GUARD_UUID_2B53EFAB_DAA5_4A46_A4BA_80103B841B93
