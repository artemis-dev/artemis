/**
 * @file   TStreamingEventStore.cc
 * @brief  Streaming Data Event Store
 *
 * @date   Created       : 2023-02-11 12:00:00 JST
 *         Last Modified : 2023-02-18 19:28:40 JST
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 *
 *    (C) 2023 Shinsuke OTA
 */

#include "TStreamingEventStore.h"

#include "TSystem.h"
#include "TObjArray.h"

#include "TEventHeader.h"
#include "TRunInfo.h"
#include "TSegmentedData.h"
#include "TFileDataSource.h"
#include "TStreamingModuleDecoderFactory.h"
#include "TStreamingHeaderFS.h"
#include "TStreamingHeaderSTF.h"
#include "TStreamingHeaderTF.h"

ClassImp(art::TStreamingEventStore)


art::TStreamingEventStore::TStreamingEventStore()
: fDataSource(NULL), fBuffer(NULL)
{

   Register(fSegmentedData("SegmentedData","The name of output segmented data","segdata"));
   Register(fRunHeader("RunHeadersName","the name of output array for run headers","runheader"));
   Register(fEventHeader("EventHeaderName","the name of event header","eventheader"));
   Register(fMaxFrames("MaxFrames","maximum number of frames (no limit if 0)",0));
   Register(fStartFrame("StartFrame","start point number of frame",0));
   Register(fFileName("FileName","names of input files",StringVec_t()));

   fBuffer = new char[1024*1024*1024];
   fHeaderFS = new TStreamingHeaderFS;
   fHeaderTF = new TStreamingHeaderTF;
   fHeaderSTF = new TStreamingHeaderSTF;
   fSTFClones = new TClonesArray(art::TStreamingHeaderSTF::Class());
}

art::TStreamingEventStore::~TStreamingEventStore()
{
   delete fBuffer;
   delete fHeaderFS;
   delete fHeaderTF;
   delete fHeaderSTF;
}

art::TStreamingEventStore::TStreamingEventStore(const art::TStreamingEventStore& rhs)
{

}

art::TStreamingEventStore& art::TStreamingEventStore::operator=(const art::TStreamingEventStore& rhs)
{
   return *this;
}

void art::TStreamingEventStore::NotifyEndOfRun()
{
   SetStopEvent();
   SetStopLoop();
   SetEndOfRun();
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

   fIsEOB = true;
}

void art::TStreamingEventStore::PreLoop()
{
   TStreamingModuleDecoderFactory::Clear();
}

void art::TStreamingEventStore::Process()
{
   // Info("Process","++++++++++++++++++++++++++++++ Loop ++++++++++++++++++++++++++++++");
   // where the segmented data is cleared in RIDFEventStore ?
   fSegmentedData->Clear("C");
   // clear rawdata array

   // stop after exceeding maximum number of event
   if (fMaxFrames > 0 && fEventHeader->GetEventNumberTotal() > fMaxFrames) {
      SetStopEvent();
      SetStopLoop();
      SetEndOfRun();
      return;
   }

   GetHeartBeatFrame();
   
   
   while (1) {
      delete fDataSource;
      fDataSource = NULL;
      if (!Open()) {
         Info("Process","No more file is available");
         NotifyEndOfRun();
         return;
      } else {
         // SetStopEvent();
         break;
      }
   }

   
   fEventHeader->IncrementEventNumber();
}


Bool_t art::TStreamingEventStore::GetTimeFrame()
{
   if (!fDataSource) return kFALSE;
   if (!fDataSource->IsPrepared()) return kFALSE;

   // read header and check if it is time frame header
   fDataSource->Read(fBuffer,sizeof(uint64_t));
   fDataSource->Seek(-sizeof(uint64_t),SEEK_CUR);
   // Info("GetTimeFrame","%lx",*(uint64_t*)fBuffer);
   if(!TStreamingHeaderTF::IsHeaderTF(*(uint64_t*)fBuffer)) {
      // this is not the header, ignore
      return kFALSE;
   }

   fDataSource->Read(fBuffer,TStreamingHeaderTF::kHeaderSize);
   fHeaderTF->ReadFrom(fBuffer);
   fNumSources = fHeaderTF->GetNumSources();
   if (fSubTimeFrameSize.size() != fNumSources) {
      fSubTimeFrameSize.resize(fNumSources);
      fSubTimeFrameBuffers.resize(fNumSources);
      fSubTimeFrameHeaders.resize(fNumSources);
      for (int i = 0; i < fNumSources; ++i) {
         fSubTimeFrameHeaders[i] = (TStreamingHeaderSTF*)fSTFClones->ConstructedAt(i);
      }
   }
   
   // fHeaderTF->Print();

   int read = fDataSource->Read(fBuffer,fHeaderTF->GetLength());
   
   if (read != fHeaderTF->GetLength()) {
      // enough data is not available
      NotifyEndOfRun();
   }
}

Bool_t art::TStreamingEventStore::GetSubTimeFrame()
{
   // Info("GetSubTimeFrame","size = %d",fSubTimeFrameBuffers.size());
   if (fSubTimeFrameBuffers.size() > 0) {
      // Info("GetSubTimeFrame","Checking all the data is read");
      int done = 0;
      int n  = fSubTimeFrameBuffers.size();
      for (int i = 0;  i < n; ++i ) {
         if (fSubTimeFrameSize[i]) continue;
         done ++;
      }
      if (!done) {
         // data to be analyzed exists      
         return kTRUE;
      } else if (done != n) {
         Error("GetSubtimeframe","Broken subtime frames, try to reset");
      }
   }
   // Info("GetSubTimeFrame","reading time frame");
   if (!GetTimeFrame()) {
      // Info("GetSubTimeFrame","reading subtime frame directly");
      // get subtime frame buffer by myself since no time frame is ready
      if (!fDataSource) return kFALSE;
      if (!fDataSource->IsPrepared()) return kFALSE;
      fDataSource->Read(fBuffer,sizeof(uint64_t));
      fDataSource->Seek(-sizeof(uint64_t),SEEK_CUR);
      if(!TStreamingHeaderSTF::IsHeaderSTF(*(uint64_t*)fBuffer)) {
         // no time frame nor subtime frame is found, buffer is not ready
         Warning("","Header may not be ready %lx",*(uint64_t*)fBuffer);
         return kFALSE;
      }
      // sub time frame is found but only one
      fNumSources = 1;
      if (fSubTimeFrameBuffers.size() == 0) {
         fSubTimeFrameBuffers.resize(1);
         fSubTimeFrameHeaders.resize(1);
         fSubTimeFrameSize.resize(1);
         fSubTimeFrameHeaders[0] = (TStreamingHeaderSTF*)fSTFClones->ConstructedAt(0);
      }
      
      int nread = fDataSource->Read(fBuffer,TStreamingHeaderSTF::kHeaderSize);
      if (nread != TStreamingHeaderSTF::kHeaderSize) {
         NotifyEndOfRun();
         return kFALSE;
      }
      auto& header = fSubTimeFrameHeaders[0];
      header->ReadFrom(fBuffer);
      // header->Print();
      int payloadSize = header->GetLength() - TStreamingHeaderSTF::kHeaderSize;
      nread = fDataSource->Read(fBuffer,payloadSize);
      if (nread != payloadSize) {
         NotifyEndOfRun();
         return kFALSE;
      }
      fSubTimeFrameBuffers[0] = fBuffer;
      fSubTimeFrameSize[0] = payloadSize;
   } else {
      char *buffer = fBuffer;
      for (int i = 0; i < fNumSources; ++i) {
         auto& header = fSubTimeFrameHeaders[i];
         header->ReadFrom(buffer);
         fSubTimeFrameBuffers[i] = buffer+TStreamingHeaderSTF::kHeaderSize;
         fSubTimeFrameSize[i] = header->GetLength() - TStreamingHeaderSTF::kHeaderSize;
         buffer += header->GetLength();
      }
   }
   return kTRUE;
}


Bool_t art::TStreamingEventStore::GetHeartBeatFrame()
{
   if (!GetSubTimeFrame()) {
      // no subtime frame is found
      return kFALSE;
   } else {
      for (int i = 0; i < fNumSources; ++i) {
         auto headerSTF = fSubTimeFrameHeaders[i];
         auto buffer    = fSubTimeFrameBuffers[i];
         auto size    = fSubTimeFrameSize[i];
         if (size <= 0) {
            Error("GetHeartbeatframe","Unexpected size %d",fSubTimeFrameSize[i]);
            return kFALSE;
         }
         int femid = (headerSTF->GetFEMId() & 0xffff);
         int femtype = headerSTF->GetFEMType();
         int segid = 0;
         TObjArray *seg = fSegmentedData->FindSegmentByID(segid);
         if (!seg) seg = fSegmentedData->NewSegment(segid);
         
         TStreamingModuleDecoder *decoder = TStreamingModuleDecoderFactory::Find(femtype);
         int used = decoder->Decode(buffer,size,seg,femid);
         if (used > 0) {
            fSubTimeFrameSize[i] -= used;
            fSubTimeFrameBuffers[i] += used;
            // printf("size = %d, buffer = %p\n",fSubTimeFrameSize[i],fSubTimeFrameBuffers[i]);
         } else {
            fSubTimeFrameSize[i] = 0;
            // Info("GetHeartBeatFrame","used all");
         }
      }
   }
   return kTRUE;
}

//////////////////////////////////////////////////////////////////////



Bool_t art::TStreamingEventStore::Open()
{

   if (fDataSource) {
      Error("Open","Data source is already preapared");
      return kFALSE;
   }

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
   Info("Open","DataSource with %s is preapred\n",filename.Data());

   // read file sink header block
   fDataSource->Read(fBuffer,0x130);
   fHeaderFS->ReadFrom(fBuffer);
   fHeaderFS->Print();
   fIsEOB = false;
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

