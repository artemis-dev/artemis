/**
 * @file   TRDFEventStore.cc
 * @brief  RDF Event Store
 *
 * @date   Created       : 2014-03-30 10:56:06 JST
 *         Last Modified : Jun 22, 2014 17:26:29 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#include "TRDFEventStore.h"

#include <TSystem.h>
#include <TFileDataSource.h>
#include <TClonesArray.h>
#include <TModuleDecoderFactory.h>
#include <TSegmentInfo.h>
#include <TModuleType.h>
#include <TSegmentedData.h>
#include <TRunInfo.h>
#include <TEventHeader.h>
#include <TTimeStamp.h>
#include <map>

using art::TRDFEventStore;

ClassImp(TRDFEventStore)

const Int_t art::TRDFEventStore::kMaxBufSize = 1024*1024; // default size is 1 MB
const Int_t art::TRDFEventStore::kBlockSize = 0x4000; // 16384 bytes for 1 block


TRDFEventStore::TRDFEventStore()
: fIsOnline(kFALSE), fDataSource(NULL),fIsEOB(kTRUE), fSearchPath(".")
{
   StringVec_t dummy;
   RegisterProcessorParameter("InputFiles","The names of input files",fFileName,dummy);
   RegisterOutputCollection("SegmentedData","The name of output array for segmented data",
                            fNameSegmented,TString("segdata"),&fSegmentedData,"art::TSegmentedData");
   RegisterProcessorParameter("RunHeadersName","the name of output array for run headers, run header will be stored once",
                            fNameRunHeaders,TString("runheader"));
   RegisterOutputCollection("EventHeaderName","the name of event header",
                            fNameEventHeader,TString("eventheader"));
   RegisterInputInfo("SegmentInfoName","the name of segment information",fSegmentInfoName,
                     TString("seglist"),&fSegmentInfoArray,"TClonesArray","art::TSegmentInfo");
   RegisterInputInfo("ModuleListName","the name of module type list",fModuleListName,
                     TString("modlist"),&fModuleTypeArray,"TClonesArray","art::TModuleType");
   fBuffer = new Char_t[kMaxBufSize];
   fDecoderFactory = TModuleDecoderFactory::Instance()->CloneInstance();
   fDecoders = new TObjArray;
   fSegmentedData = new TSegmentedData;
   fRunHeaders = new TList;
   fEventHeader = new TEventHeader;
}

TRDFEventStore::~TRDFEventStore()
{
   delete [] fBuffer;
   delete fDecoderFactory;
   delete fDecoders;
   delete fSegmentedData;
   delete fEventHeader;
   delete fRunHeaders;
}

TRDFEventStore::TRDFEventStore(const TRDFEventStore& rhs)
{
}

TRDFEventStore& TRDFEventStore::operator=(const TRDFEventStore& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TRDFEventStore::Init(TEventCollection *col)
{
   Int_t n = fFileName.size();
   for (Int_t i=0; i!= n; i++) {
      printf("file = %s\n",fFileName[i].Data());
   }
   // check segment configuration
   Int_t nSeg = (*fSegmentInfoArray)->GetEntriesFast();
   for (Int_t iSeg = 0; nSeg != iSeg; iSeg++) {
      TSegmentInfo *segment = (TSegmentInfo*) (*fSegmentInfoArray)->At(iSeg);
      Int_t segid = (segment->GetSegID()) ;
      TModuleType *modtype = (TModuleType*) (*fModuleTypeArray)->FindObject(segment->GetModuleType());
      if (!modtype) {
         // error
         SetStateError(TString::Format("Unknown module '%s'",segment->GetModuleType().Data()));
         return;
      }
      TObject *dec;
      if ((dec = fDecoderFactory->Get(modtype->GetDecoderID()))) {
         fDecoders->AddAt(dec,segid);
      } else {
         SetStateError(TString::Format("Unknown decoder id = %d",modtype->GetDecoderID()));
         return;
      }
   }

   // prepare run header information
   fRunHeaders->SetName(fNameRunHeaders);
   col->AddInfo(fNameRunHeaders,fRunHeaders);
   col->Add(fNameEventHeader,fEventHeader,kFALSE);
}

void TRDFEventStore::Process()
{
   // clear segment data
   fSegmentedData->Clear("C");
   // loop over the events if exist
   while (!GetNextEvent()) {
   // get next block
      while (!GetNextBlock()) {
         if ((*fCondition)->IsSet(TLoop::kStopLoop)) {
            SetStopEvent();
            return;
         }
         // try to open data source if no block is available 
         if (!Open()) {
            // loop is end if no data source is available
            SetStopEvent();
            SetStopLoop();
            SetEndOfRun();
            return;
         }
      }
   }
}
Bool_t TRDFEventStore::Open() {
   if (fDataSource) {
      if (fIsOnline) {
         return kTRUE;
      }
      Error("Open","Data source is already prepared");
      return kFALSE;
   }

   if (fIsOnline) {
#if 0
      Info("Open","Online mode");
      Int_t shmid = kSHMID_BASE + 2 * fSHMID;
      Int_t semkey = kSEMKEY_BASE + 2 * fSHMID;
      Int_t size = kSHM_BUFF_SIZE + 4;
      fDataSource = new TSharedMemoryDataSource(shmid,semkey,size);
      
      if (!fDataSource->IsPrepared()) {
         Error("Open","Catnnot prepare shared memory data source with shmid = %d",fSHMID);
         return kFALSE;
      }
      TRunInfo *runinfo = new TRunInfo("Online","Online");
      runinfo->SetRunName("Online");
      runinfo->SetRunNumber(0);
      runinfo->SetStartTime(0);
      runinfo->SetStopTime(0);
      runinfo->SetHeader("");
      runinfo->SetEnder("");
      fRIDFData.fRunHeaders->Add(runinfo);
      fRIDFData.fEventHeader->SetRunName("Online");
      fRIDFData.fEventHeader->SetRunNumber(0);
#endif
      Error("Open","online mode is not implimented yet");
      return kFALSE;
   } else {
      TString filename;
      while (fFileName.size()) {
         filename = fFileName.front();
         fSourceName = fFileName.front();
         fFileName.erase(fFileName.begin());
         if (gSystem->FindFile(fSearchPath,fSourceName)) {
            break;
         }
         Error("Open","no such file '%s' in search path '%s'",filename.Data(),fSearchPath.Data());
      }
      if (filename == "") {
         // no file is available
         return kFALSE;
      }
      fDataSource = new TFileDataSource(filename);
      if (!fDataSource) {
         Error("Open","Cannot open file %s",filename.Data());
         return kFALSE;
      }

      // if data source is ready, read comment block
      std::map<TString,Int_t> month;
      month.insert(std::map<TString,Int_t>::value_type(TString("Jan"),1));
      month.insert(std::map<TString,Int_t>::value_type(TString("Feb"),2));
      month.insert(std::map<TString,Int_t>::value_type(TString("Mar"),3));
      month.insert(std::map<TString,Int_t>::value_type(TString("Apr"),4));
      month.insert(std::map<TString,Int_t>::value_type(TString("May"),5));
      month.insert(std::map<TString,Int_t>::value_type(TString("Jun"),6));
      month.insert(std::map<TString,Int_t>::value_type(TString("Jul"),7));
      month.insert(std::map<TString,Int_t>::value_type(TString("Aug"),8));
      month.insert(std::map<TString,Int_t>::value_type(TString("Sep"),9));
      month.insert(std::map<TString,Int_t>::value_type(TString("Oct"),10));
      month.insert(std::map<TString,Int_t>::value_type(TString("Nov"),11));
      month.insert(std::map<TString,Int_t>::value_type(TString("Dec"),12));
      {
         struct stat fsta;
         stat(filename,&fsta);
         char runnumber[9]; runnumber[8] = 0;
         char starttime[19]; starttime[18] =0;
         char stoptime[19]; stoptime[18] = 0;
         char stadate[11]; stadate[10] = 0;
         char stodate[11]; stodate[10] = 0;
         char header[81]; header[80] = 0;
         char ender[81]; ender[80] = 0;

         // read first comment block
         fDataSource->Seek(0,0);
         fDataSource->Read(fBuffer,kBlockSize);
         if (fBuffer[0] != 1) {
            SetStateError("Unexpected comment block\n");
            return kFALSE;
         }
         memcpy(runnumber,fBuffer+20,8);
         memcpy(starttime,fBuffer+30,18);
         memcpy(stadate,fBuffer+86,10);
         memcpy(header,fBuffer+100,80);
         fDataSource->Seek(fsta.st_size-kBlockSize,0);
         fDataSource->Read(fBuffer,kBlockSize);
         memcpy(stoptime,fBuffer+48,18);
         memcpy(stodate,fBuffer+86,10);
         memcpy(ender,fBuffer+100,80);
         if (strcmp(header,ender) == 0) memcpy(ender,fBuffer+180,80);
         fDataSource->Seek(kBlockSize,0);
         TString runName = "run";
         TString runNumber = &runnumber[4];
         TString startTime = starttime;
         TString stopTime = stoptime;
         TString staDate = stadate;
         TString stoDate = stodate;
         TString fHeader = header;
         TString fEnder = ender;
#if 0         
         printf("siez      : %d\n",fsta.st_size);
         printf("runnumber : %s\n",runnumber);
         printf("starttime : %s\n",starttime);
         printf("stoptime  : %s\n",stoptime);
         printf("stadate   : %s\n",stadate);
         printf("stodate   : %s\n",stodate);
#endif         
         TRunInfo *runinfo = new TRunInfo(runName+runNumber,runName+runNumber);         
         TTimeStamp start(2000+TString(staDate(8,2)).Atoi(),month[TString(staDate(4,3))],TString(staDate(1,2)).Atoi(),
                          TString(startTime(9,2)).Atoi(),TString(startTime(12,2)).Atoi(),TString(startTime(15,2)).Atoi(),
                          0,kFALSE);
         TTimeStamp stop(2000+TString(stoDate(8,2)).Atoi(),month[TString(stoDate(4,3))],TString(stoDate(1,2)).Atoi(),
                         TString(stopTime(9,2)).Atoi(),TString(stopTime(12,2)).Atoi(),TString(stopTime(15,2)).Atoi(),
                         0,kFALSE);
         if (start.GetSec() > stop.GetSec()) {
            stop.SetSec(stop.GetSec()+86400);
         }
         runinfo->SetRunName(runName.Data());
         runinfo->SetRunNumber(runNumber.Atoll());
         runinfo->SetStartTime(start.GetSec());
         runinfo->SetStopTime(stop.GetSec());
         runinfo->SetHeader(fHeader);
         runinfo->SetEnder(fEnder);
         runinfo->SetTotalSize(fsta.st_size);

         fRunHeaders->Add(runinfo);
         fEventHeader->SetRunName(runName.Data());
         fEventHeader->SetRunNumber(runNumber.Atoll());
      }
      
      return kTRUE;
   }

   // unexpected to reach here
   Error("Open","Unexpected error at %d",__LINE__);
   return kFALSE;
}

Bool_t TRDFEventStore::GetNextBlock()
{
   if (!fIsEOB) {
      Error("GetNextBlock","Unexpectedly called. Event block contains more data.");
      return kFALSE;
   }
   if (!fDataSource) {
      // data source is not ready
      return kFALSE;
   }
   if (fIsOnline) {
#if 0      
      Int_t nBlock = 0;
      fDataSource->Lock();
      fDataSource->Seek(kSHM_BUFF_SIZE,SEEK_SET);
      fDataSource->Read((char*)&nBlock,sizeof(Int_t));
      fDataSource->Seek(0,SEEK_SET);
      fDataSource->Unlock();
      if (fBlockNumber == nBlock) {
         gSystem->Sleep(1);
         return kFALSE;
      } else {
         fBlockNumber = nBlock;
      }
#endif
   }
   fDataSource->Lock();
   // check if block is availalbe
   if (!fDataSource->Read(fBuffer,kBlockSize)) {
      // no more data is ready in this data source
      fDataSource->Unlock();
      delete fDataSource;
      fDataSource = NULL;
      return kFALSE;
   }
   ((TRunInfo*)fRunHeaders->Last())->AddAnalyzedSize(kBlockSize);
   // block was read
   fIsEOB = kFALSE;
   fOffset = 0;
   // total block size includes the size of header
   fDataSource->Unlock();
   return kTRUE;
}

Bool_t TRDFEventStore::GetNextEvent()
{
      fDecoderFactory->Clear();
   unsigned short *bufp = (unsigned short*)fBuffer;
   Int_t offset = fOffset / sizeof(short);
   if (fIsEOB) return kFALSE;
   // parse data if available
   while (1) {
      if (offset == 0) {
         if (bufp[offset] != 0x0000) {
            // comment block
            fOffset = kBlockSize;
            // need to read new block
            fIsEOB = kTRUE;
            return kFALSE;
         }
         // special treatment for the first event in each block
         // then increment the pointer
         offset = 4;
      }

      // event block
      if ((bufp[offset] & 0xf000) == 0x8000 && (bufp[offset + 1]  == 0x0001)) {
         // check if data size is correct
         fEventHeader->IncrementEventNumber();
         ((TRunInfo*)fRunHeaders->Last())->IncrementEventNumber();
         Int_t eventsize = (bufp[offset] & 0x0fff);
         Int_t evtoffset = offset + 3;
//         printf("%x %x %x %x\n",bufp[offset],bufp[offset+1],bufp[offset+2],bufp[offset+3]);
         offset += eventsize;
         fOffset = offset * sizeof(short);
         if (!eventsize || fOffset > kBlockSize) {
            // return because of abnormal event
            Error("GetNextEvent","Abnormal event found at %d",__LINE__);
            fIsEOB = kTRUE;
            return kFALSE;
         }
         // analyze segment
         while (evtoffset < offset) {
//            printf("segid = %x %x %x\n",bufp[evtoffset+1],bufp[evtoffset]);
            Int_t size = bufp[evtoffset] * sizeof(short);
            UInt_t segid = bufp[evtoffset+1];
            TObjArray *seg = fSegmentedData->FindSegmentByID(segid);
            if (!seg) seg = fSegmentedData->NewSegment(segid);
            TModuleDecoder *decoder = (TModuleDecoder*) fDecoders->At(segid);
            if (decoder) {
               decoder->Decode((char*)&bufp[evtoffset+2],size,seg);
            } else {
               printf("No such decoder for segid = %d\n",segid);
            }
            evtoffset += bufp[evtoffset];
         }
//         if (!fRIDFData.fSegmentedData->GetEntriesFast()) {
//            Warning("GetNextEvent","No event is found in event block");
//            continue;
//         } 
         // else do analysis and return
         
         return kTRUE;
      } else if (bufp[offset] == 0xffff && bufp[offset + 1] == 0xffff) {
         // end of block
         fIsEOB = kTRUE;
         return kFALSE;
      } else if ((bufp[offset]&0xf000) == 0x8000 && bufp[offset + 1] == 0x7f00) {
         // event scaler
         fIsEOB = kTRUE;
         return kFALSE;
      }
   }
   Error("GetNextEvent","Something wrong happened");
   fIsEOB = kTRUE;
   return kFALSE;
}
