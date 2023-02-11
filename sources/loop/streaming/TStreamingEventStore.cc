/**
 * @file   TStreamingEventStore.cc
 * @brief  Streaming Data Event Store
 *
 * @date   Created       : 2017-12-21 00:29:39 JST
 *         Last Modified : 2023-02-11 15:15:10 JST
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 *
 *    (C) 2023 Shinsuke OTA
 */

#include "TStreamingEventStore.h"

#include "TSystem.h"
#include "TEventHeader.h"
#include "TRunInfo.h"
#include "TSegmentedData.h"
#include "TFileDataSource.h"

ClassImp(art::TStreamingEventStore)


art::TStreamingEventStore::TStreamingEventStore()
{

   Register(fSegmentedData("SegmentedData","The name of output segmented data","segdata"));
   Register(fRunHeader("RunHeadersName","the name of output array for run headers","runheader"));
   Register(fEventHeader("EventHeaderName","the name of event header","eventheader"));
   Register(fMaxFrames("MaxFrames","maximum number of frames (no limit if 0)",0));
   Register(fStartFrame("StartFrame","start point number of frame",0));
   Register(fFileName("FileName","names of input files",StringVec_t()));

   fBuffer = new char[1024*1024*1024];
   
}

art::TStreamingEventStore::~TStreamingEventStore()
{
   delete fBuffer;
}

art::TStreamingEventStore::TStreamingEventStore(const art::TStreamingEventStore& rhs)
{

}

art::TStreamingEventStore& art::TStreamingEventStore::operator=(const art::TStreamingEventStore& rhs)
{
   return *this;
}

void art::TStreamingEventStore::Init(TEventCollection *col)
{

   // check if the files exist
   StringVec_t &filenames = fFileName.Value();
   for (StringVec_t::iterator it = filenames.begin(), itend = filenames.end(); it < itend; ++it) {
      TString filelist = gSystem->GetFromPipe(Form("ls -tr %s*",(*it).Data()));
      TObjArray *files = (filelist.Tokenize("\n"));
      if (!files) {
         Error("Init",Form("No such files %s",(*it).Data()));
         SetStateError("Init");
         return;
      } 

//      for (Int_t i = 0, n = files->GetEntries(); i < n; ++i) {
//         TString file = static_cast<TObjString*>(files->At(i))->GetName();
//         Info("Init","Adding file %s",file.Data());
//      }
   }

//   // set run header name
//   fRunHeader->SetName(fRunHeader->GetName());
//   printf("%p\n",fEventHeader.Data());
   fEventHeader->SetEventNumber(fStartFrame);
   fEventHeader->SetEventNumberTotal(fStartFrame);
}

void art::TStreamingEventStore::Process()
{
   // where the segmented data is cleared in RIDFEventStore ?
   fSegmentedData->Clear("C");
//   fData->Clear("C");

   // stop after exceeding maximum number of event
   if (fMaxFrames > 0 && fEventHeader->GetEventNumberTotal() > fMaxFrames) {
      SetStopEvent();
      SetStopLoop();
      SetEndOfRun();
      return;
   }

   fEventHeader->IncrementEventNumber();
   printf("Event %d\n",fEventHeader->GetEventNumberTotal());

//   while (!GetFrame()) {
//      if ((*fCondition)->IsSet(TLoop::kStopLoop)) {
//         SetStopEvent();
//         return;
//      }
//
//      if (!Open()) {
//         NotifyEndOfRun();
//         return;
//      }
//   }
}

Bool_t art::TStreamingEventStore::ReadTimeFrame()
{
#if 0   
   constexpr int length = 100;
   fDataSource->Read(fBuffer,length);
   int used = 0;
   if (!(used = fTimeFrameHeader->ReadFrom(fBuffer))) {
      // no time frame header is found but may not be error
      return  kFALSE;
   }
   fDataSource->Seek(used - length, SEEK_CUR);
#endif
   return kTRUE;
}

Bool_t art::TStreamingEventStore::ReadSubTimeFrame()
{
#if 0   
   // @TODO subtime frame should be read until the end of time frame or its own length
   int totalLength = fTimeFrameHeader->IsPrepared() ? fTimeFrameHeader->GetLength() : 0;
   if (fTimeFrameHeader->IsPrepared()) {
   } else {
      ReadSubTimeFrameOnce();
   }
#endif   
}


Bool_t art::TStreamingEventStore::ReadSubTimeFrameOnce()
{
   #if 0
   constexpr int hsize = TSubTimeFrameHeader::fgSize;
   int used = 0;
   if (fDataSource->Read(fBuffer,hsize) != hsize) {
      // end of file
      fEOB = kTRUE;
      return 0;
   }
      
   if (!(used = fSubTimeFrameHeader->ReadFrom(fBuffer))) {
      // no subtime frame header is found but may not be error
      fDataSource->Seek(used - length, SEEK_CUR);
      return 0;
   }
   // @TODO here the subtime frame header should be copied

   // reading data
   ReadData(fSubTimeFrameHeader->GetLength(),
            fSubTimeFrameHeader->GetFEMID(),
            fSubTimeFrameHeader->GetFEMType());
#endif
   return kTRUE;
}
/// header のデコーダをマップしておいて、まずはヘッダの解析をしてからその後を考えさせるか
/// Streaming (fairmq 自体はデータフォーマットの構造化を前提としていない）
Bool_t art::TStreamingEventStore::ReadData(int length, int femid, int femtype) {

#if 0   
   int used = 0;
   // use only segid = 0 for now
   constexpr int segid = 0;

   fDataSource->Read(fBuffer,length);
   
   TObjArray *seg = fSegmentedData->FindSegmentByID(segid);
   if (!seg) seg =  fSegmentedData->NewSegment(segid);
   TStreamingModuleDecoder *decoder = fDecoderFactory->Get(femtype);
   if (decoder) {
      // each decoder may output HBD and SDs with channels over the last channel of each module
      decoder->Decode(fBuffer,length,seg,femid);
   } else {
      printf("No such decoder for Module %d\n",fFENType);
      return kFALSE;
   }
#endif   
   return kTRUE;
}


Bool_t art::TStreamingEventStore::Open()
{
   TString filename = "";
   StringVec_t& files = fFileName.Value();
   
   while (files.size()) {
      filename = files.front();
      files.erase(files.begin());
      if (gSystem->FindFile("./",filename)) {
         break;
      }
   }
   if (filename == "") {
      return kFALSE;
   }
   fDataSource = new TFileDataSource(filename);
#if 0 // file sink comment is disabled
   // @TODO how long we have to read
   constexpr int length = 100;
   constexpr TString runname("data");
   fDataSource->Read(fBuffer,length); 
   int used = fFileSinkComment->ReadFrom(fBuffer);
   fDataSource->Seek(used - length,SEEK_CUR);
   TRunInfo *info = new TRunInfo(runname,runname);
   info->SetRunName(runname);
   info->SetRunNumber(fFileSinkComment->GetRunNumber());
   fRunHeaders->Add(info);
   fEventHeader->SetRunName(runname);
   fEventHeader->SetRunNumber(fFileSinkComment->GetRunNumber());
#endif

   return kTRUE;
}



