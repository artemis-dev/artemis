/* $Id:$ */
/**
 * @file   TMappingProcessor.cc
 * @date   Created : Nov 22, 2013 17:22:19 JST
 *   Last Modified : 2018-07-24 16:51:10 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TMappingProcessor.h"

#include <TCategorizedData.h>
#include <TSegmentedData.h>
#include <TSystem.h>
#include <TMapTable.h>
#include <TConfigFile.h>

using art::TMappingProcessor;

art::TMappingProcessor::FormatType_t art::TMappingProcessor::fgFormatType = art::TMappingProcessor::RIDF;

TMappingProcessor::TMappingProcessor()
{
   StringVec_t input;
   input.push_back("segdata");
   RegisterInputCollection("InputCollection","Input collection should inherit from TSegmentedData",fInputColName,input);
   RegisterOutputCollection("CategorizedData","The name of output array for categorized data",
                            fNameCategorized,TString("catdata"));
   RegisterOptionalParameter("MapConfig","File for map configuration. Not mapped if the name is blank.",
                             fMapConfigName,TString("mapper.conf"));
   fCategorizedData = new TCategorizedData;
   fMapTable        = NULL;
   fSegmentedData   = NULL;
}
TMappingProcessor::~TMappingProcessor()
{
   if (fCategorizedData) delete fCategorizedData; 
   if (fMapTable) delete fMapTable;
}

void TMappingProcessor::Init(TEventCollection *col)
{
   // get object reference
   for (Int_t i=0; i!=fInputColName.size(); i++) {
      fSegmentedData = reinterpret_cast<TSegmentedData**>(col->GetObjectRef(fInputColName[i]));
      if (!fSegmentedData) {
         SetStateError(Form("No such input collection '%s' (TSegementedData)\n",fInputColName[i].Data()));
         return;
      }
      fSegmentedDataArray.push_back(fSegmentedData);
   }

   // do nothing if MapConfig is null 
   if (fMapConfigName.IsNull()) {
      Warning("Init","Name of MapConfig is null. Nothing can be done\n");
      return;
   }

   // check if map config exists
   if (!gSystem->FindFile(".",fMapConfigName)) {
      Error("Init","No such file : %s\n",fMapConfigName.Data());
      return;
   }
   
   //--------------------------------------------------
   // adding output collection
   //--------------------------------------------------
   col->Add(fNameCategorized,fCategorizedData,fOutputIsTransparent);
   //--------------------------------------------------
   // Create map table 
   //--------------------------------------------------
   fMapTable = new TMapTable;
   TConfigFile file(fMapConfigName);
   const Int_t nids = 5;
   while (1) {
      const TString& mapfilename = file.GetNextToken();
      const Int_t&   ndata       = file.GetNextToken().Atoi();
      if (!mapfilename.Length() || !ndata) {
         // no more map file is available 
         break;
      }
      TConfigFile mapfile(mapfilename,"#",", \t","#");
      while (1) {
         const TString& cidstr = mapfile.GetNextToken();
         const TString& didstr = mapfile.GetNextToken();
         if (!cidstr.Length() || !didstr.Length()) break;
         const Int_t& cid = cidstr.Atoi();
         const Int_t& did = didstr.Atoi();
         Int_t ids[nids];
         for (Int_t i = 0; i!=ndata; i++) {
            for (Int_t j=0; j!=nids; j++) {
               ids[j] = mapfile.GetNextToken().Atoi();
            }
            Int_t segid;
            if (fgFormatType == RIDF) {
               segid = (((ids[0]&0x3f)<<20) | ((ids[1]&0x3f)<<14) |((ids[2]&0x3f)<<8));
            } else if (fgFormatType == RDF) {
               segid = ids[2];
            }
            fMapTable->SetMap(segid,ids[3],ids[4],cid,did,i);
         }
      }
   }
   Info("Init","mapfile %s is loaded\n",fMapConfigName.Data());
}


void TMappingProcessor::Process()
{
   if (!fMapTable) {
      Error("Process","MapConfig is not loaded correctly\n");
      sleep(1);
      return;
   }
   fCategorizedData->Clear("C");
   Int_t nSegData = fSegmentedDataArray.size();
   for (Int_t iSegData = 0; iSegData != nSegData; iSegData++) {
      fSegmentedData = fSegmentedDataArray[iSegData];
      const Int_t nSeg = (*fSegmentedData)->GetEntriesFast();
      for (Int_t iSeg = 0; iSeg != nSeg; iSeg++) {
         TObjArray *seg = (TObjArray*)(*fSegmentedData)->UncheckedAt(iSeg);
         const Int_t nData = seg->GetEntriesFast();
         for (Int_t i=0; i!=nData; i++) {
            TRawDataObject *obj = (TRawDataObject*) seg->At(i);
            if (obj->GetCatID() != TRawDataObject::kInvalid) {
               // already mapped
               continue;
            }
            fMapTable->Map(obj);
            fCategorizedData->Add(obj);
         }
      }
   }
}
