/**
 * @file   TP2PKinematicsData.h
 * @brief
 *
 * @date   Created:       2013-12-11 11:02:32
 *         Last Modified: 2014-01-08 16:53:43
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TP2PKINEMATICSDATA_H
#define TP2PKINEMATICSDATA_H

#include "TDataObject.h"
#include <TLorentzVector.h> // need to generate Streamer

namespace art {
   namespace sh04 {
      class TP2PKinematicsData;
   }
}

class art::sh04::TP2PKinematicsData : public TDataObject {
public:
   // Default constructor
   TP2PKinematicsData();
   virtual ~TP2PKinematicsData();
   // Copy constructor
   TP2PKinematicsData(const TP2PKinematicsData& rhs);
   // Assignment operator
   TP2PKinematicsData& operator=(const TP2PKinematicsData& rhs);

   void Copy(TObject &obj) const;
   void Clear(Option_t *opt = "");

   Double_t GetSp() const { return fSp; }
   void SetSp(Double_t val) { fSp = val; }

   Double_t GetEx() const { return fEx; }
   void SetEx(Double_t val) { fEx = val; }

   const TLorentzVector* GetRecoilL() const { return fRecoilL; }

   const TLorentzVector* GetRecoilR() const { return fRecoilR; }
   TLorentzVector* GetIncident() { return &fIncident; }
   const TLorentzVector* GetIncident() const { return &fIncident; }
   TLorentzVector* GetResidual() { return &fResidual; }
   const TLorentzVector* GetResidual() const { return &fResidual; }

   void SetRecoilL(const TLorentzVector* p) { fRecoilL = p; }
   void SetRecoilR(const TLorentzVector* p) { fRecoilR = p; }

   // use TDataObject::fQualityBit (0--2) as flags for correction
   //enum ECorrectionBit {kReactionPoint=0,kIncidentAngle,kMagnetBending};

protected:
   const TLorentzVector *fRecoilL; //-> 4-vector of left recoil proton. not owned by this
   const TLorentzVector *fRecoilR; //-> 4-vector of right recoil proton. not owned by this
   TLorentzVector fIncident; // 4-vector of incident particle.
   TLorentzVector fResidual; // 4-vector of residual nuclei.

   Double_t fSp; // Proton separation energy
   Double_t fEx; // Excitation energy

private:

   ClassDef(TP2PKinematicsData,1) // SHARAQ04 p2p kinematics data container

};

#endif // TP2PKINEMATICSDATA_H
