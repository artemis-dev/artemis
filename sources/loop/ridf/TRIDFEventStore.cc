/* $Id:$ */
/**
 * @file   TRIDFEventStore.cc
 * @date   Created : Jul 12, 2013 17:12:35 JST
 *   Last Modified : Nov 30, 2013 01:00:18 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TRIDFEventStore.h"
#include <TSegmentedData.h>
#include <TDataSource.h>
#include <TFileDataSource.h>
#include <TSharedMemoryDataSource.h>
#include <TRawDataObject.h>
#include <TLoop.h>
#include <TModuleDecoderFactory.h>
#include <TModuleDecoder.h>
#include <TRunInfo.h>
#include <TTimeStamp.h>
#include <TEventHeader.h>
#include <TSystem.h>

#include <map>

art::TRIDFEventStore::TRIDFEventStore()
   : fMaxEventNum(0),fEventNum(0),fEventNumTotal(0), fMaxBufSize(kMaxBufSize),
     fSHMID(0), fBlockNumber(0)
{
   StringVec_t dummy;
   RegisterInputCollection("InputFiles","The names of input files",fFileName,dummy);
   RegisterOutputCollection("SegmentedData","The name of output array for segmented data",
                            fNameSegmented,TString("segdata"));
   RegisterOutputCollection("RunHeadersName","the name of output array for run headers, run header will be stored once",
                            fNameRunHeaders,TString("runheader"));
   RegisterOutputCollection("EventHeaderName","the name of event header",
                            fNameEventHeader,TString("eventheader"));
   RegisterProcessorParameter("SHMID","Shared memory id (default : 0)",fSHMID,0);
   

   fRIDFData.fSegmentedData = new TSegmentedData;
   fRIDFData.fRunHeaders = new TList;
   fRIDFData.fEventHeader = new TEventHeader;
   fRIDFData.fVerboseLevel = &fVerboseLevel;
   fIsOnline = kFALSE;
   fIsEOB = kTRUE;
   fBuffer = new Char_t[fMaxBufSize];
   fSearchPath = ".";


   // initialize 
   for (Int_t i=0; i!=64; i++) {
      fClassDecoder[i] = NULL;
   }
   // register class decoders
   // Class 3 : event header  w/o timestamp
   fClassDecoder[3] = ClassDecoder03;
   // Class 4 : event segment
   fClassDecoder[4] = ClassDecoder04;
   // Class 5 : comment block
   fClassDecoder[5] = ClassDecoder05;
   // Class 6 : event header w/ timestamp
   fClassDecoder[6] = ClassDecoder06;
   // Class 8 : 
   fClassDecoder[8] = ClassDecoderSkip;
   // Class 9 : 
   fClassDecoder[9] = ClassDecoderSkip;
   // Class 11 : Scaler
   fClassDecoder[11] = ClassDecoderSkip;
   // Class 11 : Scaler
   fClassDecoder[12] = ClassDecoderSkip;
   // Class 11 : Scaler
   fClassDecoder[13] = ClassDecoderSkip;

}
art::TRIDFEventStore::~TRIDFEventStore()
{
   if (fRIDFData.fSegmentedData) delete fRIDFData.fSegmentedData;
   if (fRIDFData.fRunHeaders) delete fRIDFData.fRunHeaders;
   if (fRIDFData.fEventHeader) delete fRIDFData.fEventHeader;
   if (fBuffer) delete [] fBuffer;
   if (fDataSource) delete fDataSource;
}


void art::TRIDFEventStore::Init(TEventCollection *col)
{
   Int_t n = fFileName.size();
   // online mode if no input file exists
   if (!n) fIsOnline = kTRUE;
   fDataSource = NULL;

   for (Int_t i=0; i!=n;i++) {
      printf("file = %s\n",fFileName[i].Data());
   }

   col->Add(fNameSegmented,fRIDFData.fSegmentedData,fOutputIsTransparent);
   col->Add(fNameEventHeader,fRIDFData.fEventHeader,kFALSE);
   col->AddInfo(fNameRunHeaders,fRIDFData.fRunHeaders,kFALSE);
   fRIDFData.fRunHeaders->SetName(fNameRunHeaders);

}


void art::TRIDFEventStore::Process()
{
   // try to prepare data source
   fRIDFData.fSegmentedData->Clear("C");
   // try to get next event
   while (!GetNextEvent()) {
      // try to get next block if no event is available
      while (!GetNextBlock()) {
         // check stop condition
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


//----------------------------------------
// Class : any (to be skip)
// dummy decoder to skip the data for debug
void art::TRIDFEventStore::ClassDecoderSkip(Char_t *buf, Int_t& offset, struct RIDFData* ridfdata)
{
   RIDFHeader header;
   memcpy(&header,buf+offset,sizeof(header));
   offset += header.Size();
}


//----------------------------------------
// Class : unknown
// unknown class decoder to skip the data
void art::TRIDFEventStore::ClassDecoderUnknown(Char_t *buf, Int_t& offset, struct RIDFData* ridfdata)
{
   RIDFHeader header;
   memcpy(&header,buf+offset,sizeof(header));
   printf("Unkown Class ID = %d, size = %d\n",header.ClassID(),header.Size());
   ClassDecoderSkip(buf,offset,ridfdata);
}


//----------------------------------------
// Class : 03
// decode the event header
void art::TRIDFEventStore::ClassDecoder03(Char_t *buf, Int_t& offset, struct RIDFData* ridfdata)
{
   RIDFHeader header;
   memcpy(&header,buf+offset,sizeof(header));
   Int_t last = offset + header.Size();
   offset += sizeof(header);
   offset += sizeof(int);
   ridfdata->fEventHeader->IncrementEventNumber();
   ((TRunInfo*)ridfdata->fRunHeaders->Last())->IncrementEventNumber();
   while (offset < last) {
      memcpy(&header,buf+offset,sizeof(header));
      if (header.ClassID() == 4) {
         ClassDecoder04(buf,offset,ridfdata);
      } else {
         printf("offset = %d, last = %d\n",offset,last);
         ClassDecoderUnknown(buf,offset,ridfdata);
      }
   }
}

//----------------------------------------
// Class : 04
// decode the segment
void art::TRIDFEventStore::ClassDecoder04(Char_t *buf, Int_t& offset, struct RIDFData* ridfdata)
{
   RIDFHeader header;
   SegID segid;
   Int_t index = offset;
   Int_t size;
   memcpy(&header,buf+offset,sizeof(header));
   // offset should be incremented by the size written in header 
   offset += header.Size();
   // local valude o
   index += sizeof(header);
   // read segment id
   memcpy(&segid,buf+index,sizeof(segid));
   index += sizeof(segid);
   // calculate segment size
   size = header.Size() - sizeof(header) - sizeof(segid);
   // Segid for map consists of rev, device, fp, and detector.
   // Module id should be removed. (now Get return the moduleid removed segment id).
   UInt_t mapsegid = segid.Get();
   TObjArray *seg = ridfdata->fSegmentedData->FindSegmentByID(mapsegid);
   if (!seg) seg = ridfdata->fSegmentedData->NewSegment(mapsegid);
   TModuleDecoder *decoder = TModuleDecoderFactory::Instance()->Get(segid.Module());
   if (decoder) {
      decoder->Decode(&buf[index],size,seg);
   } else {
      printf("No such decoder for Module %d\n",segid.Module());
   }
}

//----------------------------------------
// Class : 05
// decode the comment header
void art::TRIDFEventStore::ClassDecoder05(Char_t *buf, Int_t& offset, struct RIDFData* ridfdata)
{
   RIDFHeader header;
   Int_t local = offset;
   memcpy(&header,buf+offset,sizeof(header));
   offset += header.Size();
   local += sizeof(RIDFHeader) + sizeof(int);
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
   if ((*(int*)(buf+local)) == 1) {
      RIDFCommentRunInfo info;
      local+=sizeof(int);
      memcpy(&info,buf+local,sizeof(RIDFCommentRunInfo));
      if (*(ridfdata->fVerboseLevel) > 0) {
         info.Print();
      }
      TString runName = info.fRunName;
      TString runNumber = info.fRunNumber;
      TString startTime = info.fStartTime;
      TString stopTime = info.fStopTime;
      TString date = info.fDate;
      TString fHeader = info.fHeader;
      TString fEnder = info.fEnder;
      TRunInfo *runinfo = new TRunInfo(runName+runNumber,runName+runNumber);
      TTimeStamp start(2000+TString(date(7,2)).Atoi(),month[TString(date(3,3))],TString(date(0,2)).Atoi(),
                       TString(startTime(9,2)).Atoi(),TString(startTime(12,2)).Atoi(),TString(startTime(15,2)).Atoi(),
                       0,kFALSE);
      TTimeStamp stop(2000+TString(date(7,2)).Atoi(),month[TString(date(3,3))],TString(date(0,2)).Atoi(),
                      TString(stopTime(8,2)).Atoi(),TString(stopTime(11,2)).Atoi(),TString(stopTime(14,2)).Atoi(),
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
      ridfdata->fRunHeaders->Add(runinfo);
      ridfdata->fEventHeader->SetRunName(runName.Data());
      ridfdata->fEventHeader->SetRunNumber(runNumber.Atoll());
   }
}

//----------------------------------------
// Class : 06
// decode the time stamp event header
void art::TRIDFEventStore::ClassDecoder06(Char_t *buf, Int_t& offset, struct RIDFData* ridfdata)
{
   RIDFHeader header;
   memcpy(&header,buf+offset,sizeof(header));
   Int_t last = offset + header.Size();
   ridfdata->fEventHeader->IncrementEventNumber();
   ((TRunInfo*)ridfdata->fRunHeaders->Last())->IncrementEventNumber();
   offset += sizeof(header);
   offset += sizeof(int);
   ridfdata->fEventHeader->SetTimestamp(*(Long64_t*)(buf+offset));
   // ignore timestamp for now
   offset += sizeof(ULong64_t);
   while (offset < last) {
      memcpy(&header,buf+offset,sizeof(header));
      if (header.ClassID() == 4) {
         ClassDecoder04(buf,offset,ridfdata);
      } else {
         printf("offset = %d, last = %d\n",offset,last);
         ClassDecoderUnknown(buf,offset,ridfdata);
      }
   }
}

//----------------------------------------
// Open
Bool_t art::TRIDFEventStore::Open()
{
   if (fDataSource) {
      if (fIsOnline) {
         return kTRUE;
      }
      Error("Open","Data source is already prepared");
      return kFALSE;
   }

   if (fIsOnline) {
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
      return kTRUE;
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
      return kTRUE;
   }

   // unexpected to reach here
   Error("Open","Unexpected error at %d",__LINE__);
   return kFALSE;
}

//----------------------------------------
// GetNextBlock to get next block
Bool_t art::TRIDFEventStore::GetNextBlock()
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
   }
   fDataSource->Lock();
   if (!fDataSource->Read((char*)&fHeader,sizeof(fHeader))) {
      // no more data is ready in this data source
      fDataSource->Unlock();
      delete fDataSource;
      fDataSource = NULL;
      return kFALSE;
   }
   // check the header validity
   if (fHeader.Layer() != 0 ||
       (fHeader.ClassID() != 0 &&
        fHeader.ClassID() != 1 &&
        fHeader.ClassID() != 2 )
      ) {
      // invalid event block
      Error("Process","Invalid event block with class id = %d",fHeader.ClassID());
      fDataSource->Unlock();
      // erase invalid data source
      delete fDataSource;
      return kFALSE;
   }
   // check if block is availalbe
   fBlockSize   = fHeader.Size() - sizeof(fHeader);
   if (fDataSource->Read(fBuffer,fBlockSize)) {
      // block was read
      fIsEOB = kFALSE;
      fOffset = 0;
      // total block size includes the size of header
      fDataSource->Unlock();
      return kTRUE;
   } else {
      // unexpectedly block was not available
      fDataSource->Unlock();
      Error("Process","Empty block");
      fOffset = fBlockSize = 0;
      fHeader = 0LL;
      return kFALSE;
   }
}


Bool_t art::TRIDFEventStore::GetNextEvent()
{
   if (fIsEOB) return kFALSE;
   // parse data if available
   while (1) {
      TModuleDecoderFactory::Instance()->Clear();
      memcpy(&fHeader,fBuffer+fOffset,sizeof(fHeader));
      if (fClassDecoder[fHeader.ClassID()]) {
         fClassDecoder[fHeader.ClassID()](fBuffer,fOffset,&fRIDFData);
      } else {
         printf("Class ID = %d\n",fHeader.ClassID());
         ClassDecoderUnknown(fBuffer,fOffset,&fRIDFData);
      }
      if (fOffset >= fBlockSize)  {
         fIsEOB = kTRUE;
      }
      // TClassDecoder::Decode(fBuffer,fOffset,fNext,something?)
      // if the data is available or not
      if (fRIDFData.fSegmentedData->GetEntriesFast()) {
         // the data is available
         return kTRUE;
      } else  if (fIsEOB) {
         // no data is available
         return kFALSE;
      }
   }
}
