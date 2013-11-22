/* $Id:$ */
/**
 * @file   TRIDFEventStore.cc
 * @date   Created : Jul 12, 2013 17:12:35 JST
 *   Last Modified : Nov 22, 2013 13:20:05 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TRIDFEventStore.h"
#include <TConfigFile.h>
#include <TMapTable.h>
#include <TCategorizedData.h>
#include <TSegmentedData.h>
#include <TDataSource.h>
#include <TFileDataSource.h>
#include <TRawDataObject.h>
#include <TLoop.h>
#include <TModuleDecoderFactory.h>
#include <TModuleDecoder.h>
#include <TRunInfo.h>
#include <TTimeStamp.h>
#include <TEventHeader.h>

#include <map>

art::TRIDFEventStore::TRIDFEventStore()
   : fMaxEventNum(0),fEventNum(0),fEventNumTotal(0), fMaxBufSize(kMaxBufSize)
{
   StringVec_t dummy;
   RegisterInputCollection("InputFiles","The names of input files",fFileName,dummy);
   RegisterOutputCollection("SegmentedData","The name of output array for segmented data",
                            fNameSegmented,TString("segdata"));
   RegisterOutputCollection("CategorizedData","The name of output array for categorized data",
                            fNameCategorized,TString("catdata"));
   RegisterOutputCollection("RunHeadersName","the name of output array for run headers, run header will be stored once",
                            fNameRunHeaders,TString("runheader"));
   RegisterOutputCollection("EventHeaderName","the name of event header",
                            fNameEventHeader,TString("eventheader"));
   RegisterOptionalParameter("MapConfig","File for map configuration. Not mapped if the name is blank.",
                             fMapConfigName,TString("mapper.conf"));

   fRIDFData.fSegmentedData = new TSegmentedData;
   fRIDFData.fCategorizedData = new TCategorizedData;
   fRIDFData.fMapTable = NULL;
   fRIDFData.fRunHeaders = new TList;
   fRIDFData.fEventHeader = new TEventHeader;
   fIsOnline = kFALSE;
   fIsEOB = kTRUE;
   fBuffer = new Char_t[fMaxBufSize];


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
   if (fRIDFData.fCategorizedData) delete fRIDFData.fCategorizedData; 
   if (fRIDFData.fMapTable) delete fRIDFData.fMapTable;
   if (fRIDFData.fRunHeaders) delete fRIDFData.fRunHeaders;
   if (fRIDFData.fEventHeader) delete fRIDFData.fEventHeader;
   if (fBuffer) delete [] fBuffer;
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
   col->Add(fNameCategorized,fRIDFData.fCategorizedData,fOutputIsTransparent);
   col->Add(fNameEventHeader,fRIDFData.fEventHeader,kFALSE);
   col->AddInfo(fNameRunHeaders,fRIDFData.fRunHeaders,kFALSE);
   fRIDFData.fRunHeaders->SetName(fNameRunHeaders);

   //--------------------------------------------------
   // Create map table 
   //--------------------------------------------------
   if (!fMapConfigName.IsNull()) {
      if (!fRIDFData.fMapTable) fRIDFData.fMapTable = new TMapTable;
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
               Int_t segid = (((ids[0]&0x3f)<<20) | ((ids[1]&0x3f)<<14) |((ids[2]&0x3f)<<8));
               fRIDFData.fMapTable->SetMap(segid,ids[3],ids[4],cid,did,i);
            }
         }
      }
      printf("mapfile %s is loaded\n",fMapConfigName.Data());
   }
}


void art::TRIDFEventStore::Process()
{
   // try to prepare data source
   fRIDFData.fSegmentedData->Clear("C");
   fRIDFData.fCategorizedData->Clear("C");
   
   if (!fDataSource) {
      if (fIsOnline) {
//         fDataSource = new TShmDataSourceRIDF(fSHMID);
      } else if (fFileName.size()){
         TString filename = fFileName.front();
         FILE *fp = fopen(filename,"r");
         if (fp) {
            fclose(fp);
            fFileName.erase(fFileName.begin());
            fDataSource = new TFileDataSource(filename);
         }
      }
   }

   if (!fDataSource) {
      // no file is available
      // all the input files are analyzed or cannot open file
      SetStopEvent();
      SetStopLoop();
      SetEndOfRun();
      return;
   }

   // check data is available or not for shared memory data source
   if (!fDataSource->IsPrepared()) {
      // no data is available now
      SetStopEvent();
      return;
   }

   // read data block if the data is availab
   if (fIsEOB) {
      // check if header is available
      if (fDataSource->Read((char*)&fHeader,sizeof(fHeader))) {
         // check the header validity
         if (fHeader.Layer() != 0 ||
             (fHeader.ClassID() != 0 &&
              fHeader.ClassID() != 1 &&
              fHeader.ClassID() != 2 )
            ) {
            return;
         }
         // check if block is availalbe
         fBlockSize   = fHeader.Size() - sizeof(fHeader);
         if (fDataSource->Read(fBuffer,fBlockSize)) {
            // block was read
            fIsEOB = kFALSE;
            fOffset = 0;
            // total block size includes the size of header
         } else {
            fOffset = fBlockSize = 0;
            fHeader = 0LL;
         }
      }
   }

   // check if no data is available 
   if (fIsEOB) {
      // no data is available
      SetStopEvent();
      delete fDataSource;
      fDataSource = NULL;
      return;
   }

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
         break;
      } else  if (fIsEOB) {
         // no data is available
         SetStopEvent();
         break;
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
      const Int_t &nData = seg->GetEntriesFast();
      if (ridfdata->fMapTable) {
         for (Int_t i=0; i!=nData; i++) {
            TRawDataObject *obj = (TRawDataObject*) seg->At(i);
            if (obj->GetCatID() != TRawDataObject::kInvalid) {
               // already mapped
               continue;
            }
            ridfdata->fMapTable->Map(obj);
            ridfdata->fCategorizedData->Add(obj);
         }
      }
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
      info.Print();
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
