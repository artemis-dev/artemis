/* $Id:$ */
/**
 * @file   TCatCategorizingProcessor.cc
 * @date   Created : May 13, 2012 19:13:45 JST
 *   Last Modified : May 21, 2012 11:24:20 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCategorizingProcessor.h"

#include <TArtMapper.h>


TCatCategorizingProcessor::TCatCategorizingProcessor()
{
   StringVec_t defInput;
   defInput.push_back("rawdata");
   
   RegisterInputCollection("InputCollection",
                           "rawdata object returned by TArtEventStore",
                           fInputColName,defInput);
   RegisterOutputCollection("OutputCollection","categorized rawdata object",
                            fOutputColName,TString("categorized"));
   RegisterProcessorParameter("OutputTransparency",
                              "Output is transparent if true",
                              fOutputIsTransparent,kFALSE);
   RegisterProcessorParameter("MapConfig","configuration file to map",
                              fMapFileName,TString("mapper.conf"));
}
TCatCategorizingProcessor::~TCatCategorizingProcessor()
{
   if (fMapper) delete fMapper;
   if (fRawCategrized) delete fRawCategrized;
}


void TCatCategorizingProcessor::Init(TCatEventCollection *col)
{
   fRawData = dynamic_cast<TArtRawEventObject*> 
      (col->Get(fInputColName.front()));
   fRawCategrized = new TCatRawDataCategorized;
   fRawCategrized->SetName(fOutputColName);
   col->Add(fRawCategrized,fOutputIsTransparent);

   // mapper config
   fMapper = TArtMapper::Instance();
   fMapper->LoadConfig(fMapFileName);
}

void TCatCategorizingProcessor::Process()
{

   // Since Actual mapping process is done during parsing RIDF format
   // by TArtParserRIDF for now, here is no mapping procedure
   // implemented. Just use mapped values.
   fRawCategrized->Clear("C");
   // if mapper configuration is not loaded, do nothing
   if (!fMapper->isLoaded()) return;

   // mapping to reference array
   Int_t nSeg = fRawData->GetNumSeg();
   for (Int_t iSeg = 0; iSeg != nSeg; iSeg++) {
      TArtRawSegmentObject *seg = fRawData->GetSegment(iSeg);
      Int_t nData = seg->GetNumData();
      for (Int_t iData = 0; iData < nData; iData++) {
         TArtRawDataObject *obj = (TArtRawDataObject*) seg->GetData(iData);
         const Int_t &cid = obj->GetCategoryID();
         const Int_t &did = obj->GetDetectorID();
         const Int_t &tid = obj->GetDatatypeID();
         if (cid == -9999) continue;
         TCatObjRefArray *cats = fRawCategrized->GetCategoryById(cid);
         if (!cats) {
            cats = fRawCategrized->AddCat();
            cats->SetID(cid);
         }
         TCatObjRefArray *dets = (TCatObjRefArray*) cats->Find(did);
         if (!dets) {
            dets = fRawCategrized->AddDet();
            dets->SetID(did);
            cats->Add(dets);
         }
         TCatObjRefArray *typs = (TCatObjRefArray*) dets->Find(tid);
         if (!typs) {
            typs = fRawCategrized->AddType();
            typs->SetID(tid);
            dets->Add(typs,tid-1);
         }
         typs->Add(obj);
      }
   }
}
