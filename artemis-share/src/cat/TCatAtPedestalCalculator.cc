/* $Id:$ */
/**
 * @file   TCatAtPedestalCalculator.cc
 * @date   Created : Sep 19, 2013 09:19:02 JST
 *   Last Modified : Oct 18, 2013 20:15:40 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TCatAtPedestalCalculator.h"

#include <TSegmentedData.h>
#include <TRawDataFadc.h>
#include <TROOT.h>

#include <TDirectory.h>
#include <TH2F.h>

ClassImp(art::TCatAtPedestalCalculator)

art::TCatAtPedestalCalculator::TCatAtPedestalCalculator()
   : fSegments(NULL)
{
   StringVec_t defInput; defInput.push_back("segdata");
   IntVec_t defSegID;
   defSegID.push_back(0);
   defSegID.push_back(0);
   defSegID.push_back(0);
   RegisterInputCollection("InputCollection","rawdata object returned by TArtEventStore",
                           fInputColName,defInput);
   RegisterProcessorParameter("SegID","IDs in SegID (device, fp, detector) ",fSegID,defSegID);
   RegisterProcessorParameter("MaxGeo","Maximum geographic address (assumed to use V1740)\n",fMaxGeo,7);
   RegisterProcessorParameter("OutputRootFile","Name of output root file (default root/pedestal.root)",
                              fOutputRootFile,TString("root/pedestal.root"));
   RegisterProcessorParameter("OutputTextFile","Name of output text file (default output/pedestal.txt)",
                              fOutputTextFile,TString("output/pedestal.txt"));
   
   
}
art::TCatAtPedestalCalculator::~TCatAtPedestalCalculator()
{
}


void art::TCatAtPedestalCalculator::Init(art::TEventCollection *col)
{
   if (col->Get(fInputColName.front())) {
      fSegments = (TSegmentedData**)(col->Get(fInputColName.front())->GetObjectRef());
   } else {
      printf("Initialization failed with input collection %s\n",fInputColName.front().Data());
      return;
   }
}

void art::TCatAtPedestalCalculator::Process()
{
   Int_t segid = ((fSegID[0]&0x3f)<<20);
   segid |= ((fSegID[1]&0x3f)<<14);
   segid |= ((fSegID[2]&0x3f)<<8);

      
   TSegmentedData *segs = (*fSegments);
   TObjArray *seg = segs->FindSegmentByID(segid);
   if (!seg) {
      printf("no such segment (dev,fp,det) = (%d,%d,%d)\n",fSegID[0],fSegID[1],fSegID[2]);
      return;
   }
//   UInt_t id = seg->GetUniqueID(); // unused
   const Int_t &ndata = seg->GetEntriesFast();
   for (Int_t idata = 0; idata != ndata; idata++) {
      TRawDataFadc *data = (TRawDataFadc*) seg->At(idata);
      const Int_t &nsample = data->GetNumSample();
      for (Int_t is = 0; is != nsample; is++) {
         fHistPedestal->Fill(data->GetGeo()*64+data->GetCh(),(*data)[is]);
      }
   }

}

void art::TCatAtPedestalCalculator::BeginOfRun()
{
   if (!gDirectory) gDirectory = gROOT;
   TObject* obj = gDirectory->Get(GetName());
   if (obj) {
      if (!obj->InheritsFrom("TDirectory")) {
         printf("%s is not a directory",GetName());
         return;
      }
      obj->Delete();
   } 
      
   TDirectory *saved = gDirectory;
   saved->mkdir(GetName())->cd();
   fHistPedestal = new TH2F("hPedestal","Pedestals",fMaxGeo*64,-0.5,(fMaxGeo*64)-0.5,4000,0.,4000.);
   saved->cd();
}


void art::TCatAtPedestalCalculator::EndOfRun()
{
}
