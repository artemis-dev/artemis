/**
 * @file   TPPACParameter.h
 * @brief  ppac parameter holder for one ppac
 *
 * @date   Created       : 2014-06-08 16:34:06 JST
 *         Last Modified : Jun 15, 2014 12:19:16 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_200AA9DE_9502_45C1_8AF4_32295F6160AD
#define INCLUDE_GUARD_UUID_200AA9DE_9502_45C1_8AF4_32295F6160AD

#include "TParameterObject.h"
namespace art {
   class TPPACParameter;
}

namespace YAML {
   class Node;
}

class art::TPPACParameter : public TParameterObject {
public:
   TPPACParameter();
   virtual ~TPPACParameter();

   TPPACParameter(const TPPACParameter& rhs);
   TPPACParameter& operator=(const TPPACParameter& rhs);

   Float_t GetNs2mm(Int_t idx) { return fNs2mm[idx]; }
   Float_t GetInsideOffset(Int_t idx) { return fInsideOffset[idx]; }
   Float_t GetOutsideOffset(Int_t idx) { return fOutsideOffset[idx]; }
   Bool_t GetExchangeXY() { return fExchangeXY; }
   Bool_t GetReflection() { return fReflection; }
   Float_t GetGeometryOffset(Int_t idx) { return fGeometyOffset[idx]; }

   Bool_t IsInsideTXSum(Double_t val) { return (fTXSumLimit[0] < val && val <fTXSumLimit[1]); }
   Bool_t IsInsideTYSum(Double_t val) { return (fTYSumLimit[0] < val && val <fTYSumLimit[1]); }
   
protected:
   FloatVec_t fNs2mm; // ns2mm (input should not be devided by 2)
   FloatVec_t fInsideOffset; // delayline offset parameter (dT = dT + fInlideOffset)
   FloatVec_t fOutsideOffset; // outside line offset parameter (dT = dT - fOutsideOffset)
   Bool_t  fExchangeXY; // X-Y exchange flag
   Bool_t  fReflection; // X reflection flag
   FloatVec_t  fGeometyOffset; // geometry offset ( X = X - fGeometryOffset)
   FloatVec_t  fTXSumLimit; // lmitation on TXsum
   FloatVec_t  fTYSumLimit; // lmitation on TYsum

// to be exported to TParameterObject
public:
//   Bool_t LoadYAMLNode(const YAML::Node& node);
protected:
   
   
   
private:

   ClassDef(TPPACParameter,1) // ppac parameter holder
};

#endif // INCLUDE_GUARD_UUID_200AA9DE_9502_45C1_8AF4_32295F6160AD
