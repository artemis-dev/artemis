/* $Id:$ */
/**
 * @file   TRCNPEventStore_ts.cc
 * @date   Created : Jul 12, 2013 17:12:35 JST
 *   Last Modified : 2024-10-02 16:14:27 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 * 
 *  
 *    Copyright (C)2013
 */
#include "TRCNPEventStore_ts.h"
#include <TSegmentedData.h>
#include <TDataSource.h>
#include <TFileDataSource.h>
#include <TFileDataSourceGZ.h>
#include <TSharedMemoryDataSource.h>
#include <TRawDataObject.h>
#include <TLoop.h>
#include <TModuleDecoderFactory.h>
#include <TModuleDecoder.h>
#include <TRCNPRunInfo.h>
#include <TTimeStamp.h>
#include <TRCNPEventHeader.h>
#include <TSystem.h>

#include <map>

ClassImp(art::TRCNPEventStore_ts)

art::TRCNPEventStore_ts::TRCNPEventStore_ts()
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
   fRIDFData.fEventHeader = new TRCNPEventHeader;
   fRIDFData.fVerboseLevel = &fVerboseLevel;
   fRIDFData.fDecoderFactory = TModuleDecoderFactory::Instance();
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
art::TRCNPEventStore_ts::~TRCNPEventStore_ts()
{
   if (fRIDFData.fSegmentedData) delete fRIDFData.fSegmentedData;
   if (fRIDFData.fRunHeaders) delete fRIDFData.fRunHeaders;
   if (fRIDFData.fEventHeader) delete fRIDFData.fEventHeader;
   if (fBuffer) delete [] fBuffer;
   if (fDataSource) delete fDataSource;
}


void art::TRCNPEventStore_ts::Init(TEventCollection *col)
{
   Int_t n = fFileName.size();
   // online mode if no input file exists
   if (!n) fIsOnline = kTRUE;
   fDataSource = NULL;

   for (Int_t i=0; i!=n;i++) {
      //printf("file = %s\n",fFileName[i].Data());
   }

   col->Add(fNameSegmented,fRIDFData.fSegmentedData,fOutputIsTransparent);
   col->Add(fNameEventHeader,fRIDFData.fEventHeader,kFALSE);
   col->AddInfo(fNameRunHeaders,fRIDFData.fRunHeaders,kFALSE);
   fRIDFData.fRunHeaders->SetName(fNameRunHeaders);
	 nblock=0;
	 evtnum=0;

}


void art::TRCNPEventStore_ts::Process()
{
//  printf("evtnum=%d, analysis start\n",(fRIDFData.fEventHeader)->GetEventNumber());
   // try to prepare data source
   fRIDFData.fSegmentedData->Clear("C");
   fRIDFData.fSegmentedData->Clear();
   fRIDFData.fEventHeader->Clear("C");
   fRIDFData.fRunHeaders->Clear("C");
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
//	getchar();
//fRIDFData.fSegmentedData->Clear("C");
	//printf("seg=%d\n",((TClonesArray*)fRIDFData.fSegmentedData->At(0))->GetEntriesFast());

}


//----------------------------------------
// Class : any (to be skip)
// dummy decoder to skip the data for debug
void art::TRCNPEventStore_ts::ClassDecoderSkip(Char_t *buf, Int_t& offset, struct RIDFData* ridfdata)
{
   RIDFHeader header;
   memcpy(&header,buf+offset,sizeof(header));
   offset += header.Size();
}


//----------------------------------------
// Class : unknown
// unknown class decoder to skip the data
void art::TRCNPEventStore_ts::ClassDecoderUnknown(Char_t *buf, Int_t& offset, struct RIDFData* ridfdata)
{
   RIDFHeader header;
   memcpy(&header,buf+offset,sizeof(header));
  printf("Unkown Class ID = %d, size = %d\n",header.ClassID(),header.Size());
   ClassDecoderSkip(buf,offset,ridfdata);
}


//----------------------------------------
// Class : 03
// decode the event header
void art::TRCNPEventStore_ts::ClassDecoder03(Char_t *buf, Int_t& offset, struct RIDFData* ridfdata)
{
   RIDFHeader header;
   memcpy(&header,buf+offset,sizeof(header));
   Int_t last = offset + header.Size();
   offset += sizeof(header);
   offset += sizeof(int);
   ridfdata->fEventHeader->IncrementEventNumber();
   ((TRCNPRunInfo*)ridfdata->fRunHeaders->Last())->IncrementEventNumber();
   while (offset < last) {
      memcpy(&header,buf+offset,sizeof(header));
      if (header.ClassID() == 4) {
         ClassDecoder04(buf,offset,ridfdata);
      } else {
         //printf("offset = %d, last = %d\n",offset,last);
         ClassDecoderUnknown(buf,offset,ridfdata);
      }
   }
}
//----------------------------------------
// Class : 04
// decode the segment
void art::TRCNPEventStore_ts::ClassDecoder04_2(Char_t *buf, Int_t& offset, struct RIDFData* ridfdata, int sizee, int module)
{
	 SegID segid;
	 Int_t index = offset;
	 Int_t size = sizee;
         TModuleDecoder *decoder = (TModuleDecoder*)TModuleDecoderFactory::Instance()->Get(module);
                        // printf("ClassDecoder04_2: decoder %p for module %d\n",decoder, module);
			//printf("module num = %d\n",module);
			if(module==24){
			  //segid.SetSegID((0x8DCE00));//((72&0x3f)<<20)|((55&0x3f)<<14)|((78&0x3f)<<8)
			  segid.SetSegID((24<<8)); 
			}else if(module==2){
			  //segid.SetSegID((0x7D8D00));//((71&0x3f)<<20)|((54&0x3f)<<14)|((77&0x3f)<<8)//feret
			  segid.SetSegID((2<<8)); 
			}else if(module==1){
			  //segid.SetSegID((0x6D4C00));//((70&0x3f)<<20)|((53&0x3f)<<14)|((76&0x3f)<<8)//fera
			  segid.SetSegID((1<<8)); 
			}else if(module==42){
			  //segid.SetSegID((0x8DCE00));//((72&0x3f)<<20)|((55&0x3f)<<14)|((78&0x3f)<<8)
			  segid.SetSegID((42<<8)); 
			}else if(module==3){
			  //segid.SetSegID((0x5D0B00));//((69&0x3f)<<20)|((52&0x3f)<<14)|((75&0x3f)<<8)
			  segid.SetSegID((3<<8)); 
			}else if(module==32){
			  //segid.SetSegID((0x4CCA00));//((68&0x3f)<<20)|((51&0x3f)<<14)|((74&0x3f)<<8)
			  segid.SetSegID((32<<8)); 
			} else {
                           printf("Classdecoder04_2 : Unexpected module %d (decoder %p)\n",module,decoder);
                           segid.SetSegID(module);
                        }
//			if(module==72){
			UInt_t mapsegid=segid.Get();
			TObjArray *seg = ridfdata->fSegmentedData->FindSegmentByID(mapsegid);
			if(!seg){
			  seg = ridfdata->fSegmentedData->NewSegment(mapsegid);
			 // printf("aa\n")f
			}
			//getchar();
			//					printf("1\n");
//			if(decoder&&(module==72||module==2)){
//			if(decoder&&(module==72)){
			if(decoder){
                           decoder->Decode(&buf[index],size,seg);
			}else{
//				 printf("no ddecoder = %d\n",module);
			}
			//}
			//}
//			delete decoder;
}


//----------------------------------------
// Class : 04
// decode the segment
void art::TRCNPEventStore_ts::ClassDecoder04(Char_t *buf, Int_t& offset, struct RIDFData* ridfdata)
{
  /*
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
	 TModuleDecoder *decoder = ridfdata->fDecoderFactory->Get(segid.Module());
	 if (decoder) {
			decoder->Decode(&buf[index],size,seg);
	 } else {
			printf("No such decoder for Module %d\n",segid.Module());
	 }
	 */
//	 delete seg,decoder;
}

//----------------------------------------
// Class : 05
// decode the comment header
void art::TRCNPEventStore_ts::ClassDecoder05(Char_t *buf, Int_t& offset, struct RIDFData* ridfdata)
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
            TRCNPRunInfo *runinfo = new TRCNPRunInfo(runName+runNumber,runName+runNumber);
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
void art::TRCNPEventStore_ts::ClassDecoder06(Char_t *buf, Int_t& offset, struct RIDFData* ridfdata)
{
	 RIDFHeader header;
	 memcpy(&header,buf+offset,sizeof(header));
	 Int_t last = offset + header.Size();
	 ridfdata->fEventHeader->IncrementEventNumber();
	 ((TRCNPRunInfo*)ridfdata->fRunHeaders->Last())->IncrementEventNumber();
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
Bool_t art::TRCNPEventStore_ts::Open()
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
      TRCNPRunInfo *runinfo = new TRCNPRunInfo("Online","Online");
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
         printf("nofile\n");
         return kFALSE;
      }
      if (filename.EndsWith("gz")) {
         fDataSource = new TFileDataSourceGZ(filename);
      } else {
         fDataSource = new TFileDataSource(filename);
      }
      return kTRUE;
   }
   
   // unexpected to reach here
   Error("Open","Unexpected error at %d",__LINE__);
	 return kFALSE;
}

//----------------------------------------
// GetNextBlock to get next block
Bool_t art::TRCNPEventStore_ts::GetNextBlock()
{
	 if (!fIsEOB) {
			Error("GetNextBlock","Unexpectedly called. Event block contains more data.");
			return kFALSE;
	 }
	 if (!fDataSource) {
			// data source is not ready
			printf("block faulse2\n");
			return kFALSE;
	 }
	 //printf("start of nextblock, nblock=%d,evtnum=%d\n",nblock,evtnum);
	 fDataSource->Lock();
	 fHeader=0LL;
	 //	 if (!fDataSource->Read((char*)&fHeader,2)){

         // read bld header
	 fDataSource->Read(fBuffer,bld1_header::Size());
         fBLD1Header.ReadFrom((int*)fBuffer);
         // fBLD1Header.Print();
         fDataSource->Read(fBuffer,blkheader::Size());
         fBlkHeader.ReadFrom((short*)fBuffer);
         // fBlkHeader.Print();
         
#if 0         
	 int header = fHeader.hedder();

	 //	 if(header==0x424c){
	 if(header==0x424c){
            
			//block header
			//	fDataSource->Read((char*)&fHeader,1);	
			fDataSource->Read((char*)&fHeader,2);//4431	
			//								int header_length=fHeader.getnum();
			fDataSource->Read((char*)&fHeader,2);	
			int temp_blk_num= fHeader.getnum();
			fDataSource->Read((char*)&fHeader,2);	
			int blk_num=fHeader.getnum()+temp_blk_num*0x10000;
			fDataSource->Read((char*)&fHeader,2);	
			int temp_blk_size= fHeader.getnum();
			fDataSource->Read((char*)&fHeader,2);	
			blksize= fHeader.getnum()+temp_blk_size*0x10000;//blockの長さ

			for(int jk = 0;jk<10;jk++){
				 fDataSource->Read((char*)&fHeader,2);	
			}

			fDataSource->Read((char*)&fHeader,2);	//ffff
			fDataSource->Read((char*)&fHeader,2);	//block header長
			fDataSource->Read((char*)&fHeader,2);	
			fDataSource->Read((char*)&fHeader,2);	
			fDataSource->Read((char*)&fHeader,2);	
			fDataSource->Read((char*)&fHeader,2);	
			evtnum+=fHeader.getnum();
			fDataSource->Read((char*)&fHeader,2);	
			fDataSource->Read((char*)&fHeader,2);	
	 }else{
			printf("invalid header=%X, end\n",header);
			fDataSource->Unlock();
			delete fDataSource;
			fDataSource=NULL;
			return kFALSE;
	 }
#endif
	 //	 printf("block size=%X,%d\n",blksize-16,blksize-16);
         // printf("new block size = %d, %d, %d\n",fBLD1Header.fBsize , fBLD1Header.Size() , fBlkHeader.Size());
         blksize = fBLD1Header.fBsize - fBlkHeader.Size();
	 if(fDataSource->Read(fBuffer,fBLD1Header.fBsize -  fBlkHeader.Size())){
			evtsize=0;
			// block was read
			fIsEOB = kFALSE;
			fOffset = 0;
			// total block size includes the size of header
			fDataSource->Unlock();
			nblock++;
			return kTRUE;
	 }else{
			printf("faild to read block\n");
			fDataSource->Unlock();
			fHeader=0LL;
			return kFALSE;
	 }
}


Bool_t art::TRCNPEventStore_ts::GetNextEvent()
{

   if (fIsEOB){
      printf("faulse1\n"); 
      return kFALSE;
   }
   fRIDFData.fDecoderFactory->Clear();
   evtsize++;
   //まずブロックの一番最初の記号をとってくる
   memcpy(&fHeader,fBuffer+fOffset,2);	
   fOffset = fOffset+2;
   int header = fHeader.hedder();
   // printf("event header : 0x%x \n",header);
   if(header!=0xffdf){
      printf("hdear = %X, sofar %d event, goto next block\n",header,evtsize);
      fIsEOB = kTRUE;
      if(header!=0x400){
         while(1){
            memcpy(&fHeader,fBuffer+fOffset,2);	
            printf("fHeader=%X\n",fHeader.hedder());
            fOffset = fOffset+2;
            //						getchar();
            break;
         }
	     
      }
      return kFALSE;
   }
   if((fRIDFData.fEventHeader)->GetEventNumber()%100000==0)printf("evt:%d\n",(fRIDFData.fEventHeader)->GetEventNumber());
   (fRIDFData.fEventHeader)->IncrementEventNumber();
   (fRIDFData.fEventHeader)->SetBlockNumber(nblock);


   //         memcpy(&fHeader,fBuffer+fOffset,10);	//event headerは全て読んだ
   //         fOffset = fOffset+10;
   for (int i = 0; i < 4; ++i) {
      memcpy(&fHeader,fBuffer+fOffset,2);	//event headerは全て読んだ
      fOffset = fOffset+2;
      // printf("header[%d] = %x\n",i, fHeader.hedder());
   }
   memcpy(&fHeader,fBuffer+fOffset,2);	//event headerは全て読んだ
   int num_field = fHeader.hedder();
   fOffset = fOffset+2;
   // printf ("num_field =%d\n",num_field);
   for(int i_field = 0; i_field < num_field; ++i_field){

      //	   printf("filed %d start \n",i_field+1);
      
      memcpy(&fHeader,fBuffer+fOffset,6);	//field headerの最初3つ
      fOffset = fOffset+6;	
      memcpy(&fHeader,fBuffer+fOffset,2);	//field headerの最初3つ
      fOffset = fOffset+2;
      int field_size = fHeader.getnum();
      int	 total_data_size=0;
	   
      //printf("filed size : %d \n",field_size);

      while(1){

         //printf("region start\n");
	     
         //regionヘッダを読み込み->サイズを増やし、データをとる
         memcpy(&fHeader,fBuffer+fOffset,2);	// region header
         fOffset = fOffset+2;
         int module = fHeader.get_module_header();
         int  module_size = fHeader.get_module_size();
         // printf("module : %d, module_size : %d \n", module, module_size);
         total_data_size+=1+module_size*1;
         int decoder_num=0;
         if(module==0x01){ // V1190
            decoder_num=24;
            //				 printf("7\n")
         }else if(module==0x07){ 	
            decoder_num=42;
         }else if(module==0xe){ //feret, 下に同じ
            decoder_num=2;
            //			 printf("feret\n");
//				printf("1, size =%d\n",module_size);
         }else if(module==0xd){ //fera// //feret, tamii analyzerを見るとferaになっているが、実際は逆
            decoder_num=1;
            //			 printf("fera\n");
         }else if(module==0x08){ 
            decoder_num=3;
            //				printf("time stamp, size =%d\n",module_size);
         }else if(module==0x03){ // MQDC 
            decoder_num=32;
         } else {
            // printf("unknown module %d\n",module);
            // do nothing
         }
         if(decoder_num==24||decoder_num==1||decoder_num==2||decoder_num==42||decoder_num==3||decoder_num==32){
            if(decoder_num==24 || decoder_num==32){
               ClassDecoder04_2(fBuffer,fOffset,&fRIDFData,module_size*2,decoder_num);//V1190は1eventが32bit
            }else{
               ClassDecoder04_2(fBuffer,fOffset,&fRIDFData,module_size,decoder_num);//CAMACは1event16bit
            }
         }
         fOffset+=module_size*2;

         //printf("region_end \n");
	     
         if(total_data_size+1>field_size){
            break;
         }
      }
      //printf("field_size : %d, total_size : %d, field end\n", field_size, total_data_size);
   }

   if(fOffset+1>=blksize-20){ //20を引いているのはblock header とender分
      fIsEOB= kTRUE;
   }
   if(fIsEOB){
      return		kFALSE;
   }else{
      return kTRUE;
   }
}
