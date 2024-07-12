/**
 * @file   TStreamingEventStore.cc
 * @brief  Streaming Data Event Store
 *
 * @date   Created       : 2023-02-11 12:00:00 JST
 *         Last Modified : 2024-07-13 07:29:44 JST
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 *
 *    (C) 2023 Shinsuke OTA
 */

#include "TStreamingEventStoreV1.h"

#include "TEventHeader.h"
#include "TFileDataSource.h"
#include "TObjArray.h"
#include "TRunInfo.h"
#include "TSegmentedData.h"
#include "TStreamingHeaderFSV1.h"
#include "TStreamingHeaderSTFV1.h"
#include "TStreamingHeaderTFV1.h"
#include "TStreamingHeaderHBV1.h"
#include "TStreamingModuleDecoderFactory.h"
#include "TSystem.h"
// #include "TStreamingHeaderFLTCOINV1.h"

#if HAVE_ZMQ_H
#include "zmq.h"
#include "TZmqDataSource.h"
#endif

#if HAVE_REDIS_H
#include "sw/redis++/redis++.h"
#endif


ClassImp(art::v1::TStreamingEventStore)

    using art::v1::TStreamingEventStore;

TStreamingEventStore::TStreamingEventStore()
   : fDataSource(NULL), fBuffer(NULL), fPresentRunInfo(nullptr) {
   auto segdata = fSegmentedData(
       "SegmentedData", "The name of output segmented data", "segdata");
   dynamic_cast<OutputData<TSegmentedData> *>(segdata)->SetDoAuto(kFALSE);
   Register(segdata);
   Register(fRunHeader("RunHeadersName",
                       "the name of output array for run headers",
                       "runheader"));
   Register(fEventHeader("EventHeaderName", "the name of event header",
                         "eventheader"));
   Register(
       fMaxFrames("MaxFrames", "maximum number of frames (no limit if 0)", 0));
   Register(fStartFrame("StartFrame", "start point number of frame", 0));
   Register(fFileName("FileName", "names of input files", StringVec_t()));
   Register(fFEMID("FEMID", "frontend module id", 0));
   Register(fFEMType("FEMType", "frontend module type", 2));
   Register(fIsStandAlone("IsStandAlone", "stand alone mode", 0));
   Register(fDefaultLength("DefaultLength",
                           "Default length for stand alone (kB) ", 256));

#if HAVE_ZMQ_H
   Register(fZmqURI("ZmqURI","specify URI directory (use RedisURI etc if available)",""));
   Register(fIsOnline("IsOnline","0:offline (default), 1:online",0));
#if HAVE_REDIS_H
   Register(fRedisURI("RedisURI","redis server uri","tcp://127.0.0.1:6379/0"));
   Register(fDeviceID("DeviceID","nestdaq process name","TimeFrameSlicerByLogicTiming"));
   Register(fChannelName("ChannelName","channel name of nestdaq process","dqm"));
   Register(fSubChannel("SubChannel","SubChannel of channel","0"));
#endif
#endif
            

   fBuffer = new char[1024 * 1024 * 1024];
   fHeaderFS = new TStreamingHeaderFS;
   fHeaderTF = new TStreamingHeaderTF;
   fHeaderSTF = new TStreamingHeaderSTF;
   fHeaderHB = new TStreamingHeaderHB;
}

TStreamingEventStore::~TStreamingEventStore() {
   delete fBuffer;
   delete fHeaderFS;
   delete fHeaderTF;
   delete fHeaderSTF;
   delete fHeaderHB;
}

TStreamingEventStore::TStreamingEventStore(const TStreamingEventStore &rhs) {}

TStreamingEventStore &TStreamingEventStore::operator=(
    const TStreamingEventStore &rhs) {
   return *this;
}

void TStreamingEventStore::NotifyEndOfRun() {
   SetStopEvent();
   SetStopLoop();
   SetEndOfRun();
}

void TStreamingEventStore::Init(TEventCollection *col) {
   // check if the files exist
   StringVec_t &filenames = fFileName.Value();
   for (StringVec_t::iterator it = filenames.begin(), itend = filenames.end();
        it < itend; ++it) {
      TString filelist = gSystem->GetFromPipe(Form("ls -tr %s*", (*it).Data()));
      TObjArray *files = (filelist.Tokenize("\n"));
      if (!files) {
         Error("Init", "%s", Form("No such files %s", (*it).Data()));
         SetStateError("Init");
         return;
      }
   }

   // initialize segmented data
   if (void **objref = col->GetObjectRef(fSegmentedData.Value())) {
      Info("Init", "Using output segmented data %s\n",
           fSegmentedData.Value().Data());
      *(TSegmentedData **)fSegmentedData.PtrRef() = *(TSegmentedData **)objref;
      fIsMaster = true;
   } else {
      Info("Init", "Createing output segmented data\n");
      *(TSegmentedData **)fSegmentedData.PtrRef() = new TSegmentedData;
      col->Add(fSegmentedData.Value(), fSegmentedData.Data(),
               fOutputIsTransparent);
      fIsMaster = false;
   }

   // prepare runheader
   if (void **objref = col->GetInfoRef(fRunHeader.Value())) {
      *(TList **)fRunHeader.PtrRef() = *(TList **)objref;
   } else {
      *(TList **)fRunHeader.PtrRef() = new TList;
      col->AddInfo(fRunHeader.Value(), fRunHeader.Data(), fOutputIsTransparent);
   }

   //   // set run header name
   //   fRunHeader->SetName(fRunHeader->GetName());
   //   printf("%p\n",fEventHeader.Data());
   fEventHeader->SetEventNumber(fStartFrame);
   fEventHeader->SetEventNumberTotal(fStartFrame);

   fIsEOB = true;
}

void TStreamingEventStore::PreProcess() {
   TStreamingModuleDecoderFactory::Clear();
   fSegmentedData->Clear("C");
}

void TStreamingEventStore::Process() {
   if (fVerboseLevel > 2)
      Info(
          "Process",
          "++++++++++++++++++++++++++++++ Loop ++++++++++++++++++++++++++++++");
   // where the segmented data is cleared in RIDFEventStore ?
   // clear rawdata array

   // stop after exceeding maximum number of event
   if (fMaxFrames > 0 && fEventHeader->GetEventNumberTotal() > fMaxFrames) {
      Info("Process","event number %d reach at maxframe %d",fEventHeader->GetEventNumberTotal(),fMaxFrames);
      SetStopEvent();
      SetStopLoop();
      SetEndOfRun();
      return;
   }

   if (fDataSource && fDataSource->IsPrepared()) {
      if (GetHeartBeatFrame()) {
         fEventHeader->IncrementEventNumber();
         return;
      } else {
         SetStopEvent();
         return;
      }
   } 

   while (1) {
#if HAVE_ZMQ_H         
      if (fIsOnline) {
         OpenZmq();
         SetStopEvent();
         break;
      } 
#endif    
      if (fDataSource) delete fDataSource;
      fDataSource = NULL;
      if (!Open()) {
         Info("Process", "No more file is available");
         NotifyEndOfRun();
         return;
      } else {
	 SetStopEvent();
         break;
      }
   }
}

Bool_t TStreamingEventStore::GetTimeFrame() {
   if (!fDataSource) return kFALSE;
   if (!fDataSource->IsPrepared()) return kFALSE;

   // read header and check if it is time frame header
   bool found = false;
   while (!found) {
      fDataSource->Read(fBuffer, art::streaming::v1::HDR_BASE_LENGTH);
      fDataSource->Seek(-art::streaming::v1::HDR_BASE_LENGTH, SEEK_CUR);
      if (fVerboseLevel > 2) Info("GetTimeFrame", "%016lx", *(uint64_t *)fBuffer);
      fHeaderTF->ReadBaseFrom(fBuffer);
      fDataSource->Read(fBuffer, fHeaderTF->GetHeaderLength());
      if (!fHeaderTF->IsHeaderTF(*(uint64_t *)fBuffer)) {
         if (fVerboseLevel > 2) Info("GetTimeFrame", "Maybe not TF ? %016lx != %016lx", *(uint64_t *)fBuffer,fHeaderTF->Magic());
         // this is not the header, ignore
         return kFALSE;
      }
      fHeaderTF->ReadFrom(fBuffer);
      if (fHeaderTF->IsType(fHeaderTF->META)) {
         if (fVerboseLevel > 2) Info("GetTimeFrame","Type is meta? %d",fHeaderTF->GetType());
         if (fVerboseLevel > 2) fHeaderTF->Print();
         // if the timeframe contains only meta information skip
         int payloadLength = fHeaderTF->GetLength() - fHeaderTF->GetHeaderLength();
         fDataSource->Read(fBuffer, payloadLength);
      } else {
         found = true;
         break;
      }
   }
   if (fVerboseLevel>2) fHeaderTF->Print();
   fNumSources = fHeaderTF->GetNumSources();
   if (fSubTimeFrameSize.size() != fNumSources) {
      fSubTimeFrameSize.resize(fNumSources);
      fSubTimeFrameBuffers.resize(fNumSources);
      fSubTimeFrameHeaders.resize(fNumSources);
      for (auto i = 0u; i < fNumSources; ++i) {
         fSubTimeFrameHeaders[i] = new TStreamingHeaderSTF;
      }
   }

   int payloadLength = fHeaderTF->GetLength() - fHeaderTF->GetHeaderLength();
   int read = fDataSource->Read(fBuffer, payloadLength);

   if (read != payloadLength) {
      // enough data is not available
#if HAVE_ZMQ_H         
      if (!fIsOnline) {
         NotifyEndOfRun();
      }
#else
      NotifyEndOfRun();
#endif      
      return false;
   }
   return true;
}

Bool_t TStreamingEventStore::GetSubTimeFrame() {
   if (fVerboseLevel > 2)
      Info("GetSubTimeFrame", "size = %zu", fSubTimeFrameBuffers.size());
   if (fSubTimeFrameBuffers.size() > 0) {
      if (fVerboseLevel > 2)
         Info("GetSubTimeFrame", "Checking all the data is read");
      int done = 0;
      int n = fSubTimeFrameBuffers.size();
      for (int i = 0; i < n; ++i) {
         if (fSubTimeFrameSize[i]) continue;
         done++;
      }
      if (!done) {
         // data to be analyzed exists
         return kTRUE;
      } else if (done != n) {
         Error("GetSubtimeframe", "Broken subtime frames, try to reset");
      }
   }
   if (fVerboseLevel > 2) Info("GetSubTimeFrame", "reading time frame");
   if (!GetTimeFrame()) {
      if (fVerboseLevel > 2)
         Info("GetSubTimeFrame", "reading subtime frame directly");
      // get subtime frame buffer by myself since no time frame is ready
      if (!fDataSource) return kFALSE;
      if (!fDataSource->IsPrepared()) return kFALSE;
      while (1) {
         int read = fDataSource->Read(fBuffer, sizeof(uint64_t));
         if (read == 0) {
            Warning("GetSubTimeFrame", "No more data is available");
            return kFALSE;
         }
         if (!TStreamingHeaderSTF::IsHeaderSTF(*(uint64_t *)fBuffer)) {
            if (fIsStandAlone) return kFALSE;
            // no time frame nor subtime frame is found, buffer is not ready
            Warning("GetSubTimeFrame", "Header may not be ready %lx",
                    *(uint64_t *)fBuffer);
         } else {
            break;
         }
      }
      fDataSource->Seek(-sizeof(uint64_t), SEEK_CUR);

      // sub time frame is found but only one
      fNumSources = 1;
      if (fSubTimeFrameBuffers.size() == 0) {
         fSubTimeFrameBuffers.resize(1);
         fSubTimeFrameHeaders.resize(1);
         fSubTimeFrameSize.resize(1);
         fSubTimeFrameHeaders[0] = new TStreamingHeaderSTF;
      }
      
      int nread = fDataSource->Read(fBuffer, art::streaming::v1::HDR_BASE_LENGTH);
      fDataSource->Seek(-art::streaming::v1::HDR_BASE_LENGTH, SEEK_CUR);
      if (nread != art::streaming::v1::HDR_BASE_LENGTH) {
#if HAVE_ZMQ_H         
         if (!fIsOnline) {
            NotifyEndOfRun();
         }
#else         
         NotifyEndOfRun();
#endif         
         return kFALSE;
      }
      auto &header = fSubTimeFrameHeaders[0];
      header->ReadBaseFrom(fBuffer);
      nread = fDataSource->Read(fBuffer, header->GetHeaderLength());

      int payloadSize = header->GetLength() - header->GetHeaderLength();
      nread = fDataSource->Read(fBuffer, payloadSize);
      if (nread != payloadSize) {
         NotifyEndOfRun();
         return kFALSE;
      }
      fSubTimeFrameBuffers[0] = fBuffer;
      fSubTimeFrameSize[0] = payloadSize;
   } else {
      if (fVerboseLevel > 2) {
         Info("GetSubtimeFrame", "Read from time frame with %lu sources",
              fNumSources);
      }
      char *buffer = fBuffer;
      for (uint64_t i = 0; i < fNumSources; ++i) {
         if (!TStreamingHeaderSTF::IsHeaderSTF(*(uint64_t *)buffer)) {
            Warning("GetSubtimeFrame", "not STF header[%lu] : %lx", i,
                    *(uint64_t *)buffer);
            fNumSources = i;
            break;
         }
         auto &header = fSubTimeFrameHeaders[i];
         header->ReadFrom(buffer);
         fEventHeader->SetTimestamp(header->GetTimeFrameID());
         fSubTimeFrameBuffers[i] = buffer + header->GetHeaderLength();
	 if (fVerboseLevel > 2) {
	   Info("GetSubTimeFrame","buffer head %016lx",*(uint64_t*)fSubTimeFrameBuffers[i]);
	 }
         fSubTimeFrameSize[i] =
             header->GetLength() - header->GetHeaderLength();
         buffer += header->GetLength();
         if (fVerboseLevel > 2) {
            header->Print();
         }
      }
   }
   return kTRUE;
}

Bool_t TStreamingEventStore::GetHeartBeatFrame() {
   if (fIsStandAlone) {
      if (fVerboseLevel > 2) Info("GetHeartBeatFrame", "Standalone mode");
      if (!fDataSource) return kFALSE;
      if (!fDataSource->IsPrepared()) return kFALSE;
      int femid = fFEMID;
      int femtype = fFEMType;
      if (fVerboseLevel > 2)
         Info("GetHeartBeatFrame", "Standalone mode femid = %x, femtype = %d",
              femid, femtype);
      int segid = 0;
      TObjArray *seg = fSegmentedData->FindSegmentByID(segid);
      if (!seg) seg = fSegmentedData->NewSegment(segid);
      if (fVerboseLevel > 2)
         Info("GetHeartBeatFrame", "Standalone mode start reading");
      while (1) {
         int nread = fDataSource->Read(fBuffer, fDefaultLength * 1024 * 8);
         if (fVerboseLevel > 2)
            Info("GetHeartBeatFrame",
                 "Standalone mode fBuffer = %p, nread = %d", fBuffer, nread);
         if (nread == 0) {
            return kFALSE;
         }
         TStreamingModuleDecoder *decoder =
             TStreamingModuleDecoderFactory::Find(femtype);
         if (!decoder) {
            return kFALSE;
         }
         int used = decoder->Decode(fBuffer, nread, seg, femid);
         if (fVerboseLevel > 2)
            Info("GetHeartBeatFrame", "Standalone mode used =  %d, nread = %d",
                 used, nread);
         if (!used) continue;
         fDataSource->Seek(used - nread, SEEK_CUR);
         break;
      }
      return kTRUE;
   } else if (!GetSubTimeFrame()) {
      if (fVerboseLevel > 1)
         Info("GetHeartBeatFrame", "No subtime frame found");
      // no subtime frame is found
      return kFALSE;
   } else {
      for (uint64_t i = 0; i < fNumSources; ++i) {
         auto headerSTF = fSubTimeFrameHeaders[i];
         auto buffer = fSubTimeFrameBuffers[i];
         auto size = fSubTimeFrameSize[i];
         if (size < 0) {
            Error("GetHeartbeatframe", "Unexpected size %d",
                  fSubTimeFrameSize[i]);
            return kFALSE;
         }
         int femid = (headerSTF->GetFEMId() & 0xffff);
         int femtype = headerSTF->GetFEMType();
         int segid = 0;
         TObjArray *seg = fSegmentedData->FindSegmentByID(segid);
         if (!seg) seg = fSegmentedData->NewSegment(segid);
         if (size == 0) continue;
         TStreamingModuleDecoder *decoder =
             TStreamingModuleDecoderFactory::Find(femtype);
         if (!decoder) {
	   Error("GetHeartbeatframe","No such decoder %d",femtype);
            return kFALSE;
         }
         fHeaderHB->ReadFrom(buffer);
         buffer += fHeaderHB->GetHeaderLength();
	 if (fVerboseLevel > 2) {
	   fHeaderHB->Print();
	 }
         decoder->Decode(buffer, fHeaderHB->GetLength() - fHeaderHB->GetHeaderLength(), seg, femid);
         fSubTimeFrameSize[i] -= fHeaderHB->GetLength();
         fSubTimeFrameBuffers[i] += fHeaderHB->GetLength();
#if 0
         if (used > 0) {
            fSubTimeFrameSize[i] -= used;
            fSubTimeFrameBuffers[i] += used;
            if (fVerboseLevel > 2)
               printf("size = %d, buffer = %p\n", fSubTimeFrameSize[i],
                      fSubTimeFrameBuffers[i]);
         } else {
            fSubTimeFrameSize[i] = 0;
            // Info("GetHeartBeatFrame","used all");
         }
#endif         
      }
   }
   return kTRUE;
}

//////////////////////////////////////////////////////////////////////

Bool_t TStreamingEventStore::Open() {
   if (fDataSource) {
      Error("Open", "Data source is already preapared");
      return kFALSE;
   }

   TString filename = "";
   StringVec_t &files = fFileName.Value();

   while (files.size()) {
      filename = files.front();
      files.erase(files.begin());
      if (gSystem->FindFile("./", filename)) {
         break;
      }
   }
   if (filename == "") {
      return kFALSE;
   }
   fDataSource = new TFileDataSource(filename);
   if (fDataSource->IsPrepared()) {
      Info("Open", "DataSource with %s is preapred\n", filename.Data());
   } else {
      Error("Open", "No such file %s (see also the Error above)",filename.Data());
      return kFALSE;
   }
      

   // read file sink header block
   fDataSource->Read(fBuffer, art::streaming::v1::HDR_BASE_LENGTH);
   fDataSource->Seek(-art::streaming::v1::HDR_BASE_LENGTH,SEEK_CUR);
   fHeaderFS->ReadBaseFrom(fBuffer);
   if (fVerboseLevel > 2) {
     Info("Open","magic          = %016lx",*(uint64_t*)fBuffer);
     Info("Open","header length = %d",fHeaderFS->GetHeaderLength());
   }
   // fDataSource->Read(fBuffer, 304);
   fDataSource->Read(fBuffer, fHeaderFS->GetLength());
   if (fHeaderFS->ReadFrom(fBuffer)) {
     // print header 
     fHeaderFS->Print();
   } else {
     Warning("Open","Header is not file header");
     Info("Open","buffer1         = %016lx",*(uint64_t*)fBuffer);
     Info("Open","buffer2         = %016lx",*(((uint64_t*)fBuffer)+1));
     Info("Open","buffer3         = %016lx",*(((uint64_t*)fBuffer)+2));
     NotifyEndOfRun();
     return false;
   }
   {
      const TString runname("run");
      int runnum = fHeaderFS->GetRunNumber();
      TRunInfo *info = new TRunInfo(runname, runname);
      fPresentRunInfo = info;
      info->SetRunName(runname);
      info->SetRunNumber(runnum);
      fRunHeader->Add(info);
      fEventHeader->SetRunName(runname);
      fEventHeader->SetRunNumber(runnum);
   }
   fIsEOB = false;
#if 0  // file sink comment is disabled
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

#if HAVE_ZMQ_H
bool TStreamingEventStore::OpenZmq()
{

   if (!fDataSource) fDataSource = new TZmqDataSource;

   // this is not suitable, if the multiple timeframe exists in one message
   // prepare shoudl be merged in open zqm
   if (((TZmqDataSource*)fDataSource)->Prepare()) {
      return true;
   }

   
#if HAVE_REDIS_H
   std::string uri;
   if (!GetZmqUri(fRedisURI.Value().Data(),
                  fDeviceID.Value().Data(),
                  fChannelName.Value().Data(),
                  fSubChannel.Value().Data(),uri)) {
      auto ds = dynamic_cast<TZmqDataSource*>(fDataSource);
      ds->SetURI("");
      ds->SetReadoutTimeout(10);
      return false;
   }
   fZmqURI = uri;
#endif
   auto ds = dynamic_cast<TZmqDataSource*>(fDataSource);
   ds->SetURI(fZmqURI.Value().Data());
   ds->SetReadoutTimeout(10);
   return true;
}
#endif



#if HAVE_REDIS_H
bool TStreamingEventStore::GetZmqUri(const std::string& uri,
                                    const std::string& devid,
                                    const std::string& channel,
                                    const std::string& subChannel,
                                    std::string& zmqUri)
{
  // Info("GetZmqUri",uri.c_str());
   try {
      auto redis = sw::redis::Redis(uri);
      std::string keyexpr= "*";
      keyexpr += devid + "*" + channel + "*" + subChannel;
      
      std::string key;
      redis.keys(keyexpr,&key);
      // Info("GetZmqUri","keyexpr = %s", keyexpr.c_str());
      if (key.empty()) {
         // database is accessible but the key is empty means data acquisition maybe stop now
         Warning("GetZmqUri","Data acquisition may be stop");
         return false;
      }
      
      // Info("GetZmqUri","key = %s", key.c_str());
      auto ret = redis.hget(key,"address");
      zmqUri = ret.value();
      // Info("GetZmqUri","zmq uri = %s", zmqUri.c_str());
      return true;
   } catch (const sw::redis::Error& err) {
      Warning("GetZmqUri",err.what());
      return false;
   } catch (...) {
      return false;
   }

   return true;
}
#endif
