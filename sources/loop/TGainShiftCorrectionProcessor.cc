/**
 * @file   TGainShiftCorrectionProcessor.cc
 * @brief  generate gain shift correction table and correct by using table
 *
 * @date   Created       : 2018-07-25 18:21:47 JST
 *         Last Modified : 2018-07-30 14:10:41 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#include "TGainShiftCorrectionProcessor.h"
#include <TClonesArray.h>
#include <ICharge.h>
#include <ITiming.h>
#include <TRunInfo.h>
#include <TH2F.h>
#include <TMath.h>
#include <TFile.h>
#include <TDataObject.h>
#include <TEventHeader.h>
#include <TArtemisUtil.h>
#include <TSystem.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_MPI_H
#include <mpi.h>
#endif


using art::TGainShiftCorrectionProcessor;

ClassImp(TGainShiftCorrectionProcessor)


namespace {
   const TString defaultInputName = "atpulse";
   const TString defaultDirName = "prm/cat/gainshift";
   const TString defaultFileNamePrefix = "gainshift";
   const TString defaultRunHeaderName = "runheader";
   const TString defaultEventHeaderName = "eventheader";
   const Int_t defaultNumDet = 416;
   const Int_t defaultDoCreate = 0;
   const TString tableFileNameFormat = "%s_%04d.%04d.dat";
   const TString histFileNameFormat = "%s_%04d.hist.root";
   const Int_t defaultDataType = static_cast<Int_t>(art::TGainShiftCorrectionProcessor::ECharge);
   const Int_t defaultEventNumberModules = 10000;
}

TGainShiftCorrectionProcessor::TGainShiftCorrectionProcessor()
{
   RegisterInputCollection("Input","name of input collection",fInputName,defaultInputName);
   RegisterInputCollection("EventHeaderName","name of input event header",fEventHeaderName,defaultEventHeaderName,
                           &fEventHeader,TEventHeader::Class_Name());

   RegisterProcessorParameter("DoCreate","create correction table without correction",fDoCreate,defaultDoCreate);
   RegisterProcessorParameter("DataType","data type: 0 charge, 1 timing", fDataType,defaultDataType);
   RegisterInputInfo("RunHeaderName","name of run header",fRunHeaderName,defaultRunHeaderName,
                     &fRunHeader,TList::Class_Name());

   RegisterOptionalParameter("DirName","name of directory without run number where the table is stored",
                             fDirName, defaultDirName);
   RegisterOptionalParameter("FileNamePrefix",TString::Format("prefix of file name"),
                             fFileNamePrefix,defaultFileNamePrefix);
   RegisterOptionalParameter("NumDet","Number of detectors",fNumDet,defaultNumDet);
   RegisterOptionalParameter("AxisY","configuration of Y axis [nbins, minY, maxY]",fAxisY,DoubleVec_t(0));
   RegisterOptionalParameter("EventNumberModulus","modulus to calculate the number of bin in X-axis",fEventNumberModulus,defaultEventNumberModules);
}

TGainShiftCorrectionProcessor::~TGainShiftCorrectionProcessor()
{
}

TGainShiftCorrectionProcessor::TGainShiftCorrectionProcessor(const TGainShiftCorrectionProcessor& rhs)
{
}

TGainShiftCorrectionProcessor& TGainShiftCorrectionProcessor::operator=(const TGainShiftCorrectionProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}


void TGainShiftCorrectionProcessor::Init(TEventCollection *col)
{
   const Int_t eventNumberModulo = 10000.;

   fInput = reinterpret_cast<TClonesArray**>(col->GetObjectRef(fInputName));
   if (!fInput) {
      SetStateError(TString::Format("No such input or input is not clones array : %s",fInputName.Data()));
      return;
   }

   if ( 0 == (*fInput)->GetClass()->InheritsFrom(TDataObject::Class()) ) {
      SetStateError("Input class does not inherit from TDataObject");
      return;
   }

   switch (fDataType) {
   case ETiming:
      if ( 0 == (*fInput)->GetClass()->InheritsFrom(ITiming::Class()) ) {
         SetStateError("Input class does not inherit from ITiming while it is required");
         return;
      }
      break;
   case ECharge:
      if ( 0 == (*fInput)->GetClass()->InheritsFrom(ICharge::Class()) ) {
         SetStateError("Input class does not inherit from ITiming while it is required");
         return;
      }
      break;
   default:
      SetStateError(TString::Format("Unknown data type is indicated : %d\n",fDataType));
      return;
   }
   
   
   if (!fDoCreate) return;

   if ( 3 != fAxisY.size() ) {
      SetStateError(TString::Format("axis configuration size is not 3 : size = %zu",fAxisY.size()));
      return;
   }
   
   if (fAxisY[1] > fAxisY[2]) {
      Util::Swap(fAxisY[1],fAxisY[2]);
   }
   
   // get run information
   if ((*fRunHeader)->GetEntries() == 0) {
      SetStateError("run header does not exist");
      return;
   }
   TRunInfo *runinfo = static_cast<TRunInfo*>((*fRunHeader)->At(0));
   Int_t runNumber = runinfo->GetRunNumber();
   Long64_t eventNumber = runinfo->GetEventNumber();
   Int_t nBinsX = TMath::Floor(eventNumber/eventNumberModulo);
   
   if (eventNumber == 0) {
      SetStateError("Please use TTreeEventStore to find the number of events");
      return;
   }
   
   // create histogram if requested
   TString filenamefmt = fDirName + "/" + histFileNameFormat;
   fHistFileName = TString::Format(filenamefmt,fFileNamePrefix.Data(),runNumber);
   TString filename = fHistFileName;
   
   filenamefmt = fDirName + "/" + tableFileNameFormat;
   fTableFileName = TString::Format(filenamefmt,fFileNamePrefix.Data(),runNumber);
   
#ifdef USE_MPI
   int myrank, npe,useMPI;
   MPI_Initialized(&useMPI);
   if (useMPI) {
      MPI_Comm_size(MPI_COMM_WORLD, &npe);
      MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
      filename.Append(TString::Format("%d",myrank));
   }
#endif
   
   Util::PrepareDirectoryFor(filename);
   fFile = TFile::Open(filename,"recreate");
   if (!fFile) {
      SetStateError(TString::Format("Cannot create file : %s",filename.Data()));
      return;
   }

   fHists.resize(fNumDet);
   for (Int_t i = 0; i < fNumDet; ++i) {
      TString name = TString::Format("h%03d",i);
      fHists[i] = new TH2F(name,name,nBinsX,0.,eventNumber,TMath::FloorNint(fAxisY[0]+0.5),fAxisY[1],fAxisY[2]);
   }
}

void TGainShiftCorrectionProcessor::Process()
{
   if ((*fInput)->GetEntries() == 0) return;
   for (Int_t iHit = 0, nHits = (*fInput)->GetEntries(); iHit < nHits; ++iHit) {
      Double_t val = 0.;
      TDataObject *obj = static_cast<TDataObject*>((*fInput)->UncheckedAt(iHit));
      Int_t id = obj->GetID();
      if (id < 0 || fNumDet <= id) {
         continue;
      }
      
      if (fDataType == static_cast<Int_t>(ETiming)) {
         ITiming *tObj = dynamic_cast<ITiming*>(obj);
         val = tObj->GetTiming();
      } else if (fDataType == static_cast<Int_t>(ECharge)) {
         ICharge *cObj = dynamic_cast<ICharge*>(obj);
         val = cObj->GetCharge();
      }

      if (fDoCreate) {
         fHists[id]->Fill((*fEventHeader)->GetEventNumber(),val);
      }
   }
}

void TGainShiftCorrectionProcessor::PostLoop()
{
   fFile->Write();
}

void TGainShiftCorrectionProcessor::EndOfRun()
{
   if (fDoCreate) {
      Util::MPIFileMerger(fHistFileName.Data());
   }
}

