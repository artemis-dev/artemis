/**
 * @file   TGetEventStore.cc
 * @brief  GET Event Store
 *
 * @date   Created       : 2017-12-21 00:29:39 JST
 *         Last Modified : 2022-10-12 19:31:53 JST (ota)
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
#include "TObjString.h"

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

const Int_t TGetEventStore::kFPNIDs[4] = {11, 22, 45, 56};
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
   RegisterOptionalParameter("StartEventNum","start point number of event",
                             fStartEventNum,0);
   RegisterProcessorParameter("MaxEventNum","maximum number of event (no limit if 0)",fMaxEventNum,0);
   RegisterProcessorParameter("RequireHitBit","require hit bit if 1 other wise all the data passed is filled",fRequireHitBit,1);
   RegisterOptionalParameter("SubtractFPN","flag for subtraction of FPN",fSubtractFPN,kFALSE);
   RegisterOptionalParameter("ValidBucket","range of valid time bucket",fValidBucket,IntVec_t(2,0));
   Register(fIsReducedCobo("IsReducedCobo","flag to deal with reduced cobo (default : false)",false));
   
#if USE_MPI
   fUseMPI = kFALSE;
#endif   
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
   if (fValidBucket.size() < 2) {
      SetStateError(TString::Format("ValidBucket has %d size instead of 2",fValidBucket.size()));
      return;
   }

   if (fValidBucket[0] > fValidBucket[1]) {
      SetStateError(TString::Format("ValidBucket start is larger than end"));
      return;
   }

   if (fValidBucket[0] < 0 || fValidBucket[1] >= 512) {
      SetStateError(TString::Format("ValidBucket [%d,%d] out of range [0, 512]",
                                    fValidBucket[0],fValidBucket[1]));
      return;
   }
   
   if (fValidBucket[0] == fValidBucket[1]){
      fValidBucket[0] = 0;
      fValidBucket[1] = 512;
   }
   if (fVerboseLevel > 0) {
      Info("Init","Valid bucket [%d %d]",fValidBucket[0], fValidBucket[1]);
   }

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

   // FPN ID 
   for (Int_t i = 0; i < 68; ++i) {
      if (i < 17) {
         fFPNID[i] = 0;
      } else if (i < 34) {
         fFPNID[i] = 1;
      } else if (i < 51) {
         fFPNID[i] = 2; 
      } else {
         fFPNID[i] = 3;
      }
   }

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
   fEventHeader->SetEventNumber(fStartEventNum);
   fEventHeader->SetEventNumberTotal(fStartEventNum);
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


   if (fIsReducedCobo) {
      ProcessReducedCobo();
   } else {
      ProcessFullCobo();
   }
}

void TGetEventStore::ProcessFullCobo() {
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
#ifdef USE_MPI
	 if (fUseMPI) {
	   Info("Process","All data are analyzed in rank %d\n",fRankID);
	 }
#endif
         return;
      } else {
         break;
      }
   } while (1);

   fEventHeader->IncrementEventNumber();
   ((TRunInfo*)fRunHeaders->Last())->IncrementEventNumber();
   for (Int_t iAsAd = 0; iAsAd < 4; ++iAsAd) {
      GETBasicFrame *asad = coboFrame->GetFrame(iAsAd);
      fEventHeader->SetTimestamp(asad->GetEventTime());
      ProcessAsAd(asad);
   }
}

void TGetEventStore::ProcessReducedCobo() {
   GETBasicFrame *asad = NULL;
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
      asad = fGetDecoder->GetBasicFrame(fEventHeader->GetEventNumber());
      if (asad == NULL) {
         // data file with number suffix in same run will be automatically open by GETDecoder
         // if there is no frame returned, finish loop
         SetStopEvent();
         SetStopLoop();
         SetEndOfRun();
#ifdef USE_MPI
      if (fUseMPI) {
         Info("Process","All data are analyzed in rank %d\n",fRankID);
      }      
#endif
         return;
      } else {
         break;
      }
   } while (1);

   fEventHeader->IncrementEventNumber();
   fEventHeader->SetTimestamp(asad->GetEventTime());
   ((TRunInfo*)fRunHeaders->Last())->IncrementEventNumber();
   ProcessAsAd(asad);
}

void TGetEventStore::ProcessAsAd(GETBasicFrame *asad) {
   Int_t coboId = asad->GetCoboID();
   Int_t iAsAd = asad->GetAsadID();
   Int_t segid = TSegmentInfo::SegID::Build(63,asad->GetCoboID(),asad->GetAsadID(),0);;
   // printf("cobo = %d, asad = %d\n",coboId,iAsAd);
   
   TObjArray *seg = fSegmentedData->FindSegmentByID(segid);
   if (!seg) seg = fSegmentedData->NewSegment(segid);
   for (Int_t iAGET = 0; iAGET < 4; ++iAGET) {
      bitset<72> hits = asad->GetHitPat(iAGET);
      // calcurate FPN
      if (fSubtractFPN) {
         for (Int_t i = 0; i < 4; ++i) {
            fFPN[i] = asad->GetSample(iAGET,kFPNIDs[i]);
            if (fFPN[i] == NULL) continue;
            Int_t ref = fFPN[i][fValidBucket[0]];
            for (Int_t ip = 0; ip < 512; ++ip) {
               fFPN[i][ip] -= ref;
            }
         }
      }
      

      for (Int_t iCh = 0; iCh < 68; iCh++) {
         //            printf("cobo %d asad = %d aget = %d ch = %d [%d]\n",asad->GetCoboID(),asad->GetAsadID(),iAGET,67-iCh,hits[67-iCh] == 1);
         if ( !fRequireHitBit || hits[67-iCh] ) {
            //               printf("hits in cobo = %d, asad = %d, aget = %d, ch = %d\n",asad->GetCoboID(),asad->GetAsadID(),iAGET,iCh);
            Int_t* adc = asad->GetSample(iAGET,iCh);
            // printf("ich = %d, adc = %p, adc = %d\n",iCh, adc, adc ? adc [1] : -1);
            
            if (!adc || adc[0] == 0) continue;
            Int_t offset = asad->GetReadOffset() + fValidBucket[0];
            Int_t timestamp = asad->GetEventTime();
            Int_t pattern = 0;
            TRawDataFadc *data = (TRawDataFadc*) fData->ConstructedAt(fData->GetEntriesFast());
            data->Clear("C");
            //               data->SetSegInfo(seg->GetUniqueID(),iAGET+4*(iAsAd+4*coboId),iCh);
            data->SetSegInfo(seg->GetUniqueID(),iAGET,iCh);
            data->SetUniqueID(iAGET+4*(iAsAd+4*coboId));
            data->SetFadcInfo(timestamp,offset,pattern);
            Int_t *fpn = fFPN[fFPNID[iCh]];
            if (kFPNIDs[fFPNID[iCh]] != iCh && fSubtractFPN && fpn != NULL) {
               for (Int_t i=fValidBucket[0]; i<= fValidBucket[1]; ++i) {
                  data->Add(adc[i]-fpn[i]);
                  //                     data->Add(fpn[i]);
               }
            } else {
               for (Int_t i=fValidBucket[0]; i<= fValidBucket[1]; ++i) {
                  data->Add(adc[i]);
               }
            }

            seg->Add(data);
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
   printf("%ld\n",runNumber.Atoll());
   
   runInfo->SetStartTime(startTime.GetSec());
   runInfo->SetTotalSize(size);
   fEventHeader->SetRunName(TString(runName.Data()));
   fEventHeader->SetRunNumber(runNumber.Atoll());
   return runInfo;
}

Int_t art::TGetEventStore::GetRunNumber() const
{
   return fEventHeader->GetRunNumber();
}

const char* art::TGetEventStore::GetRunName() const
{
   static char runname[128];
   strncpy(runname,fEventHeader->GetRunName(),4);
   return runname;
}

