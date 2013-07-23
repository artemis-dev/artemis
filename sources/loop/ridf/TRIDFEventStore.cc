/* $Id:$ */
/**
 * @file   TRIDFEventStore.cc
 * @date   Created : Jul 12, 2013 17:12:35 JST
 *   Last Modified : Jul 23, 2013 09:42:13 JST
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

//ClassImp(art::TRIDFEventStore);

art::TRIDFEventStore::TRIDFEventStore()
   : fMaxEventNum(0),fEventNum(0),fMaxBufSize(kMaxBufSize)
{
   StringVec_t dummy;
   RegisterInputCollection("InputFiles","The names of input files",fFileName,dummy);
   RegisterOutputCollection("SegmentedData","The name of output array for segmented data",
                            fNameSegmented,TString("segdata"));
   RegisterOutputCollection("CategorizedData","The name of output array for categorized data",
                            fNameCategorized,TString("catdata"));
   RegisterOptionalParameter("MapConfig","File for map configuration. Not mapped if the name is blank.",
                             fMapConfigName,TString("mapper.conf"));

   fRIDFData.fSegmentedData = new TSegmentedData;
   fRIDFData.fCategorizedData = new TCategorizedData;
   fRIDFData.fMapTable = NULL;
   fIsOnline = kFALSE;
   fIsEOB = kTRUE;
   fBuffer = new Char_t[fMaxBufSize];


   // register class decoders
   // Class 3 : event header  w/o timestamp
   fClassDecoder[3] = ClassDecoder03;
   // Class 4 : event segment
   fClassDecoder[4] = ClassDecoder04;
   // Class 5 : comment block
   fClassDecoder[5] = ClassDecoder05;
   // Class 6 : event header w/ timestamp
   fClassDecoder[6] = ClassDecoderSkip;
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
   if (fRIDFData.fMapTable) delete fRIDFData.fMapTable;
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
   fCondition = (TConditionBit**)(col->Get(TLoop::kConditionName)->GetObjectRef());

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
         TString& filename = fFileName.front();
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
   printf("Unkown Class\n");
   ClassDecoderSkip(buf,offset,ridfdata);
}


//----------------------------------------
// Class : 03
// decode the event header
void art::TRIDFEventStore::ClassDecoder03(Char_t *buf, Int_t& offset, struct RIDFData* ridfdata)
{
   RIDFHeader header;
   memcpy(&header,buf+offset,sizeof(header));
   offset += sizeof(header);
   offset += sizeof(int);
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
      for (Int_t i=0; i!=nData; i++) {
         TRawDataObject *obj = (TRawDataObject*) seg->At(i);
         ridfdata->fMapTable->Map(obj);
         ridfdata->fCategorizedData->Add(obj);
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
   RIDFCommentRunInfo info;
   offset += sizeof(RIDFHeader) + sizeof(int)*2;
   memcpy(&info,buf+offset,sizeof(info));
   info.Print();
}

//----------------------------------------
// Class : 06
// decode the time stamp event header
void art::TRIDFEventStore::ClassDecoder06(Char_t *buf, Int_t& offset, struct RIDFData* ridfdata)
{
   printf("Decoder 06\n");
}
