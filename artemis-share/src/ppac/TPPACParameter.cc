/**
 * @file   TPPACParameter.cc
 * @brief  ppac parameter object
 *
 * @date   Created       : 2014-06-10 00:30:09 JST
 *         Last Modified : Jun 15, 2014 12:19:14 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#include "TPPACParameter.h"


using art::TPPACParameter;

ClassImp(TPPACParameter)

TPPACParameter::TPPACParameter()
{
   FloatVec_t defval;
   defval.resize(2,1.);
   RegisterParameter("ns2mm","convert ns to mm (kumagai parameter)",fNs2mm,defval,2);
   defval.clear(); defval.resize(2,0.);
   RegisterParameter("delayoffset","delayline offset parameter",fInsideOffset,defval,2);
   RegisterParameter("linecalib","line calibration parameter",fOutsideOffset,defval,2);
   defval.clear(); defval.resize(3,0);
   RegisterParameter("geometry","geometry offset to be added",fGeometyOffset,defval,3);
   RegisterParameter("exchange","exchange X and Y if flag is non 0",fExchangeXY,kFALSE,1);
   RegisterParameter("reflectx","reflection in X direction before exchange",fReflection,kFALSE,1);
   defval.clear(); defval.push_back(0.); defval.push_back(100.);
   RegisterParameter("TXSumLimit","validation gate for TXSum",fTXSumLimit,defval,2);
   RegisterParameter("TYSumLimit","validation gate for TYSum",fTYSumLimit,defval,2);
}

TPPACParameter::~TPPACParameter()
{
}

TPPACParameter::TPPACParameter(const TPPACParameter& rhs)
{
}

TPPACParameter& TPPACParameter::operator=(const TPPACParameter& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}


