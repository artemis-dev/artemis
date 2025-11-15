/**
 * @file   TPPACData.cc
 * @brief
 *
 * @date   Created:       2013-10-29 15:53:41
 *         Last Modified: 2014-06-19 17:28:32 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TPPACData.h"

#include "TConverterBase.h"

using art::TPPACData;

ClassImp(art::TPPACData)

Int_t TPPACData::fgOrderSign = -1;
TPPACData::ESortOrder TPPACData::fgSortOrder = kASC;
TPPACData::ESortType TPPACData::fgSortType = kID;

// Default constructor
TPPACData::TPPACData()
   : fTX1(kInvalidD), fTX2(kInvalidD), fTY1(kInvalidD), fTY2(kInvalidD),
     fTAnode(kInvalidD),
     fQX1(kInvalidD), fQX2(kInvalidD), fQY1(kInvalidD), fQY2(kInvalidD),
     fQAnode(kInvalidD),
     fX(kInvalidD), fY(kInvalidD),
     fTXDiff(kInvalidD), fTYDiff(kInvalidD), fTXSum(kInvalidD), fTYSum(kInvalidD),
     fQXSum(kInvalidD), fQYSum(kInvalidD), fZ(kInvalidD),
     fXConv(NULL), fYConv(NULL)
{
   TDataObject::SetID(kInvalidI);
}

TPPACData::~TPPACData() {}

// Copy constructor
TPPACData::TPPACData(const TPPACData& rhs)
   : TDataObject(rhs),
     fTX1(rhs.fTX1), fTX2(rhs.fTX2), fTY1(rhs.fTY1), fTY2(rhs.fTY2),
     fTAnode(rhs.fTAnode),
     fQX1(rhs.fQX1), fQX2(rhs.fQX2), fQY1(rhs.fQY1), fQY2(rhs.fQY2),
     fQAnode(rhs.fQAnode),
     fX(rhs.fX), fY(rhs.fY),
     fTXDiff(rhs.fTXDiff), fTYDiff(rhs.fTYDiff),
     fTXSum(rhs.fTXSum), fTYSum(rhs.fTYSum),
     fQXSum(rhs.fQXSum), fQYSum(rhs.fQYSum), fZ(kInvalidD),
     fXConv(rhs.fXConv), fYConv(rhs.fYConv)
{}

// Assignment operator
TPPACData& TPPACData::operator=(const TPPACData& rhs)
{
   if (this != &rhs) {
      ((TPPACData&)rhs).Copy(*this);
   }
   return *this;
}

void TPPACData::Copy(TObject& dest) const
{
   TDataObject::Copy(dest);
   TPPACData &cobj = *(TPPACData*)&dest;
   for (Int_t i = 0; i!=kNRAW; ++i) {
      cobj.SetT(this->GetT(i),i);
      cobj.SetQ(this->GetQ(i),i);
   }
   cobj.fX      = this->GetX();
   cobj.fY      = this->GetY();
   cobj.fZ      = this->GetZ();
   cobj.fTXDiff = this->GetTXDiff();
   cobj.fTYDiff = this->GetTYDiff();
   cobj.fTXSum  = this->GetTXSum();
   cobj.fTYSum  = this->GetTYSum();
   cobj.fQXSum  = this->GetQXSum();
   cobj.fQYSum  = this->GetQYSum();
   cobj.fXConv  = fXConv;
   cobj.fYConv  = fYConv;
}

void TPPACData::Clear(Option_t *opt)
{
   TDataObject::Clear(opt);
   TDataObject::SetID(kInvalidI);
   for (Int_t i = 0; i!=kNRAW; ++i) {
      this->SetT(kInvalidD,i);
      this->SetQ(kInvalidD,i);
   }
   fX      = kInvalidD;
   fY      = kInvalidD;
   fZ      = kInvalidD;
   fTXDiff = kInvalidD;
   fTYDiff = kInvalidD;
   fTXSum  = kInvalidD;
   fTYSum  = kInvalidD;
   fQXSum  = kInvalidD;
   fQYSum  = kInvalidD;
   fXConv  = NULL;
   fYConv  = NULL;
}

void TPPACData::Update()
{
   using art::IsValid;
   if ( IsValid(fTX1) && IsValid(fTX2) ) {
      fTXDiff = fTX1 - fTX2;
      if (IsValid(fTAnode)) fTXSum  = fTX1 + fTX2 - fTAnode * 2;
      if (fXConv) fX = fXConv->Convert(fTXDiff);
   }

   if ( IsValid(fTY1) && IsValid(fTY2) ) {
      fTYDiff = fTY1 - fTY2;
      if (IsValid(fTAnode)) fTYSum  = fTY1 + fTY2 - fTAnode * 2;
      if (fYConv) fY = fYConv->Convert(fTYDiff);
   }

   if ( IsValid(fQX1) && IsValid(fQX2) ) fQXSum = TMath::Sqrt(fQX1 * fQX2);
   if ( IsValid(fQY1) && IsValid(fQY2) ) fQYSum = TMath::Sqrt(fQY1 * fQY2);
}

Int_t TPPACData::Compare(const TObject* obj) const
{
   using art::IsValid;
   TPPACData *data = (TPPACData*) obj;
   if(!obj) {
      return -1;
   } else {
      if (fgSortType == kID) {
         if (!IsValid(data->GetID())) {
            if (!IsValid(this->GetID())) return 0;
            return -1;
         } else {
            if (!IsValid(this->GetID())) return 1;
         }
         return TObject::Compare(obj);
      } else if (fgSortType == kTiming) {
         if (!data->IsValid() && !this->IsValid()) return 0;
         if (!data->IsValid() && this->IsValid()) return 1;
         if (data->IsValid() && !this->IsValid()) return -1;
         if (data->GetTAnode() < this->GetTAnode()) {
            return fgOrderSign;
         } else if (data->GetTAnode() > this->GetTAnode()) {
            return -fgOrderSign;
         } else {
            return 0;
         }
      }
   }
   return 0;
}
