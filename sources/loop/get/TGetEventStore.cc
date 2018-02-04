/**
 * @file   TGetEventStore.cc
 * @brief  GET Event Store
 *
 * @date   Created       : 2017-12-21 00:29:39 JST
 *         Last Modified : 2017-12-28 04:11:56 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2017 Shinsuke OTA
 */

#include "TGetEventStore.h"
#include <TSegmentInfo.h> // use segid
#include <TSegmentedData.h>
#include <TEventHeader.h>
#include <TRunInfo.h>
#include <TRawDataFadc.h>
#include <TTimeStamp.h>
#if defined (__CINT__)
#define _SYS_TYPES_H_
#endif
#include <sys/stat.h>
#include <TFile.h>


// GET
#include <GETDecoder.hh>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_MPI_H
#include <mpi.h>
#endif


using art::TGetEventStore;

ClassImp(TGetEventStore)

TGetEventStore::TGetEventStore()
{
   StringVec_t dummy;
   RegisterProcessorParameter("InputFiles","The names of input files",fFileName,dummy);
   RegisterOutputCollection("SegmentedData","The name of output array for segmented data",
                            fNameSegmented,TString("segdata"),&fSegmentedData,"art::TSegmentedData");
   RegisterOutputCollection("EventHeaderName","the name of event header",
                            fNameEventHeader,TString("eventheader"),&fEventHeader,TEventHeader::Class_Name());
   RegisterProcessorParameter("RunHeadersName","the name of output array for run headers, run header will be stored once",
                            fNameRunHeaders,TString("runheader"));
   RegisterProcessorParameter("MaxEventNum","maximum number of event (no limit if 0)",fMaxEventNum,0);

   fUseMPI = kFALSE;
   fGetDecoder = new GETDecoder;
   fRunHeaders = new TList;
   fData = new TClonesArray(TRawDataFadc::Class_Name());
   fDataID = 0;
}

TGetEventStore::~TGetEventStore()
{
   delete fGetDecoder;
   delete fRunHeaders;
   delete fData;
}

TGetEventStore::TGetEventStore(const TGetEventStore& rhs)
{
}

TGetEventStore& TGetEventStore::operator=(const TGetEventStore& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TGetEventStore::Init(TEventCollection *col) 
{
   for (StringVec_t::iterator it = fFileName.begin(), itend = fFileName.end(); it < itend; ++it) {
      TString filelist = gSystem->GetFromPipe(Form("ls -tr %s*",(*it).Data()));
      TObjArray *files = (filelist.Tokenize("\n"));
      if (!files) {
         Error("Init",Form("No such files %s",(*it).Data()));
         SetStateError("Init");
         return;
      } 

      for (Int_t i = 0, n = files->GetEntries(); i < n; ++i) {
         TString file = static_cast<TObjString*>(files->At(i))->GetName();
         Info("Init","Adding file %s",file.Data());
         fGetDecoder->AddData(file.Data());
      }
   }
   fGetDecoder->SetData(fDataID);
   fRunHeaders->Add(GetRunInfo());
   // prepare run header information
   fRunHeaders->SetName(fNameRunHeaders);

#if USE_MPI
   MPI_Initialized(&fUseMPI);

   if (fUseMPI) {
      MPI_Comm_size(MPI_COMM_WORLD, &fNPE);
      MPI_Comm_rank(MPI_COMM_WORLD, &fRankID);
   } else {
      fNPE = 1;
      fRankID = 0;
   }
#endif

   col->AddInfo(fNameRunHeaders,fRunHeaders);
   col->Add(fNameEventHeader,fEventHeader,kFALSE);
}

void TGetEventStore::Process()
{
   fSegmentedData->Clear("C");
   fData->Clear("C");

   // stop after exceeding maximum number of event
   if (fMaxEventNum > 0 && fEventHeader->GetEventNumberTotal() > fMaxEventNum) {
      SetStopEvent();
      SetStopLoop();
      SetEndOfRun();
      return;
   }
   GETCoboFrame *coboFrame = NULL;
   // get next event
   do {
#ifdef USE_MPI
      if (fUseMPI) {
         if (((fEventHeader->GetEventNumberTotal()) % fNPE) != fRankID) {
            fEventHeader->IncrementEventNumber();
            ((TRunInfo*)fRunHeaders->Last())->IncrementEventNumber();
            continue;
         }
      }
#endif
      coboFrame = fGetDecoder->GetCoboFrame(fEventHeader->GetEventNumber());
      if (coboFrame == NULL) {
         // data file with number suffix in same run will be automatically open by GETDecoder
         // if there is no frame returned, finish loop
         SetStopEvent();
         SetStopLoop();
         SetEndOfRun();
         Info("Process","All data are analyzed in rank %d\n",fRankID);
         return;
      } else {
         break;
      }
   } while (1);

   fEventHeader->IncrementEventNumber();
   ((TRunInfo*)fRunHeaders->Last())->IncrementEventNumber();
   for (Int_t iAsAd = 0; iAsAd < 4; ++iAsAd) {
      GETBasicFrame *asad = coboFrame->GetFrame(iAsAd);
      Int_t coboId = asad->GetCoboID();
      Int_t segid = TSegmentInfo::SegID::Build(63,asad->GetCoboID(),asad->GetAsadID(),0);;
      TObjArray *seg = fSegmentedData->FindSegmentByID(segid);
      if (!seg) seg = fSegmentedData->NewSegment(segid);
      for (Int_t iAGET = 0; iAGET < 4; ++iAGET) {
         bitset<72> hits = asad->GetHitPat(iAGET);
         for (Int_t iCh = 0; iCh < 68; iCh++) {
            if ( hits[67-iCh] ) {
//               printf("hits in cobo = %d, asad = %d, aget = %d, ch = %d\n",asad->GetCoboID(),asad->GetAsadID(),iAGET,iCh);
               Int_t* adc = asad->GetSample(iAGET,iCh);
               Int_t offset = asad->GetReadOffset();
               Int_t timestamp = asad->GetEventTime();
               Int_t len = 512;
               Int_t pattern = 0;
               TRawDataFadc *data = (TRawDataFadc*) fData->ConstructedAt(fData->GetEntriesFast());
               data->Clear("C");
               data->SetSegInfo(seg->GetUniqueID(),iAGET+4*(iAsAd+4*coboID),iCh);
               data->SetFadcInfo(timestamp,offset,pattern);
               for (Int_t i=0; i< len; ++i) {
                  data->Add(adc[i]);
               }
               seg->Add(data);
            }
         }
      }
   }
}


art::TRunInfo* TGetEventStore::GetRunInfo()
{
   TString filename = fGetDecoder->GetDataName(fDataID);
   struct stat fsta;
   FILE *fp = NULL;
   int fd;
   fd = open (filename.Data(),O_RDONLY);
   if (!fd) {
      SetStateError(Form("Cannot open file: %s",filename.Data()));
      return NULL;
   }
   fp = fopen (filename.Data(),"rb");
   if (!fp) {
      SetStateError(Form("Cannot open file: %s",filename.Data()));
      return NULL;
   }
   if (fstat(fd,&fsta) == -1) {
      SetStateError(Form("Cannot get status : %s",filename.Data()));
      fclose(fp);
      return NULL;
   }
   
   Double_t size = fsta.st_size/1024./1024.; //  MB
   
   fclose(fp);

   Int_t ret = stat(filename.Data(),&fsta);
   perror("");
   printf("%d %s\n",ret,filename.Data());
   filename = gSystem->BaseName(filename);
   TString runName = filename(0,3);
   TString runNumber = filename(4,4);
   TString day = filename(13,2);
   TString mon = filename(16,2);
   TString year = filename(19,2);
   TString hour = filename(22,2);
   TString min = filename(25,2);
   TString seg = filename(28,2);
   TTimeStamp startTime(2000+year.Atoi(),mon.Atoi(),day.Atoi(),
                        hour.Atoi(),min.Atoi(),seg.Atoi(),0,kFALSE);
   TRunInfo *runInfo = new TRunInfo(runName+runNumber,runName+runNumber);
   runInfo->SetRunName(runName.Data());
   runInfo->SetRunNumber(runNumber.Atoll());
   runInfo->SetStartTime(startTime.GetSec());
   runInfo->SetTotalSize(size);
   fEventHeader->SetRunName(runName.Data());
   fEventHeader->SetRunNumber(runNumber.Atoll());
   return runInfo;
}
