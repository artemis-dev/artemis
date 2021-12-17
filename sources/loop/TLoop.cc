/** @class art::TLoop
 * A event loop
 *
 * Usually a loop is instantiate by the loop manger
 * (art::TLoopManager::Add) usually invoking the TCatLoopAdd command (add).
 *
 *
 * @date   Created : Apr 26, 2012 20:26:47 JST
 *   Last Modified : 2016-08-23 17:02:06 JST (ota)
 *
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *    Copyright (C) 2012 
 */
#include "TLoop.h"

#include <fstream>
#include <sstream>
#include <yaml-cpp/yaml.h>

#include <algorithm>

#include <TProcessID.h>
#include <TRint.h>
#include <TProcessor.h>

#include <TSystem.h>
#include <TString.h>
#include <TFolder.h>
#include <TROOT.h>
#include <IEventStore.h>
#include <TAnalysisInfo.h>
#include "TDatime.h"

const char* art::TLoop::kConditionName = "condition";
const char* art::TLoop::kAnalysisInfoName = "analysisInfo";

namespace {
   const char* kNodeKeyInclude = "include";
   const char* kNodeKeyReplace = "replace";
   const char* kNodeKeyName = "name";
}


ClassImp(art::TLoop);

art::TLoop::TLoop()
   : fCondition(NULL), fEventCollection(NULL),  fID(0), fAnalysisInfo(NULL),
     fEventStore(NULL)
{
   fEventCollection = new TEventCollection;
   fCondition       = new TConditionBit;
   fAnalysisInfo    = new TAnalysisInfo;
   fAnalysisInfo->SetBit(kMustCleanup);
   
}

art::TLoop::~TLoop()
{
  Info("~TLoop","deconstruct");
   std::list<TProcessor*>::iterator itr;
   std::list<TProcessor*>::iterator itrBegin = fProcessors.begin();
   std::list<TProcessor*>::iterator itrEnd   = fProcessors.end();
   for (itr = itrBegin; itr != itrEnd; itr++) {
      delete *itr;
   }
//   fEventCollection->Delete();
   delete fEventCollection;
   delete fCondition;
//   delete fAnalysisInfo;
}


//////////////////////////////////////////////////////////////////////
/// Load yaml file (for internal use)
///
/// \param[in] dirname the directory name of the file
/// \param[in] basename the base name of the file
/// \param[out] loaded contains if the file is already loaded to avoid the loop
///
/// Load yaml file at [dirname]/[basename]. 
///
Bool_t art::TLoop::Load(const char* dirname, const char* basename, std::list<Long_t> *loaded,
                        std::map<std::string,std::string>* replace )
{
   const char *filename = gSystem->ConcatFileName(dirname, basename);
   if ( 0 == loaded->size() ) {
      // if file is top
      fAnalysisInfo->SetSteeringFileName(TString(dirname) + TString("/") + TString(basename));
   }
   FileStat_t fstat;
   gSystem->GetPathInfo(filename, fstat);
   TString dirsave = fBaseDir;
   fBaseDir = dirname;
   for (std::list<Long_t>::iterator itr = loaded->begin(); itr != loaded->end(); itr++) {
      if (*itr == fstat.fIno) {
         std::cerr << "Include loop found: " << filename << std::endl;
         return kFALSE;
      }
   }

   try {
      TString lines;
      std::ifstream fin(filename);
      lines.ReadFile(fin);
      fin.close();
#if 0
      // list keys
      TPRegexp patt("@[A-Z_]+@");
      TObjArray *
      patt.Match(lines,"",0,100,&pos
#endif
      if (replace) {
         std::map<std::string,std::string>::const_iterator it = replace->begin();
         std::map<std::string,std::string>::const_iterator itend = replace->end();
         for (; it != itend;  ++it) {
            const std::string& key = it->first;
            const std::string& value = it->second;
            fAnalysisInfo->SetStringData(key.c_str(),value.c_str());
            lines.ReplaceAll(Form("@%s@",key.c_str()),value.c_str());
         }
      }
      std::istringstream iss(lines.Data());
      YAML::Parser parser(iss);
      YAML::Node doc;
      std::string name, type, value;
      
      loaded->push_back(fstat.fIno);
      
      parser.GetNextDocument(doc);
      if (!LoadYAMLNode(doc["Processor"],loaded)) {
         return kFALSE;
      }
   } catch (YAML::KeyNotFound& e) {
      std::cout << e.what() << std::endl;
      return kFALSE;
   } catch (YAML::Exception &e) {
      std::cout << "Error Occured while loading " << filename << std::endl;
      std::cout << e.what() << std::endl;
      return kFALSE;
   }
//   Init();
   fBaseDir = dirsave;
   return kTRUE;
}



void ParseSequenceRecursive(const YAML::Node &node, TString& str)
{
   if (node.Type() == YAML::NodeType::Sequence) {
      for (YAML::Iterator itv = node.begin(); itv != node.end(); ++itv) {
         if (itv->Type() == YAML::NodeType::Scalar) {
            std::string value;
            *itv >> value;
            str.Append(TString::Format("\"%s\",",value.c_str()));
         } else if (itv->Type() == YAML::NodeType::Sequence) {
            ParseSequenceRecursive(*itv,str);
         }
      }
   }
}

Bool_t art::TLoop::LoadYAMLNode(const YAML::Node &node, std::list<Long_t>* loaded)
{
   std::string name, type, value;
   // iterate for all the processors
   TString dirsaved = fBaseDir;
   for (YAML::Iterator it = node.begin(); it != node.end(); it++) {
      if (const YAML::Node *include = (*it).FindValue("include")) {
         if (include->Type() == YAML::NodeType::Map) {
            //////////////////////////////////////////////////////////////////////
            // include template by replacing keywords
            //////////////////////////////////////////////////////////////////////
            const YAML::Node *replaceNode = include->FindValue(kNodeKeyReplace);
            const YAML::Node *nameNode = include->FindValue(kNodeKeyName);
            if (!nameNode) {
               Error("LoadYAMLNode","no 'name' node is found in include node somewhere");
               return kFALSE;
            }
            (*nameNode) >> name;
            printf("loading %s\n",name.c_str());
            
            if (!replaceNode) {
               Warning("LoadYAMLNode","no 'replace' node is found while it is expected in '%s'",name.c_str());
            }
            
            TString filename = gSystem->ConcatFileName(fBaseDir,name.c_str());
            const char* dir = gSystem->DirName(gSystem->ConcatFileName(fBaseDir,name.c_str()));
            const char* base = gSystem->BaseName(name.c_str());
            printf("%s %s %s %s\n",fBaseDir.Data(),name.c_str(),dir,base);


            std::ifstream fin(filename.Data());
            if (!fin.is_open()) {
               Error("LoadFile", "Cannot open file: %s",filename.Data());
               return kFALSE;
            }
            YAML::Node included_doc;
            TString lines;
            lines.ReadFile(fin);
            if (replaceNode) {
               for (YAML::Iterator it = replaceNode->begin(), itend = replaceNode->end();
                    it != itend; ++it) {
                  std::string key, value;
                  TString valstr;
                  it.first() >> key;
                  if (it.second().Type() == YAML::NodeType::Scalar) {
                     it.second() >> value;
                     valstr = value;
                  } else if (it.second().Type() == YAML::NodeType::Sequence) {
                     valstr = "[";
                     ParseSequenceRecursive(it.second(),valstr);
                     valstr.Remove(TString::kTrailing,',');
                     valstr.Append("]");
                  }
                  lines.ReplaceAll(TString::Format("@%s@",key.c_str()),valstr);
               }
            }
//            printf("repalced\n");
//            printf("%s\n",lines.Data());
            std::istringstream iss(lines.Data());
            YAML::Parser parser(iss);
            parser.GetNextDocument(included_doc);
            if (!LoadYAMLNode(included_doc["Processor"],loaded)) {
               Error("LoadYAMLNode","Error while loading %s",name.c_str());
               return kFALSE;
            }
         } else {
            //////////////////////////////////////////////////////////////////////
            // include simple file
            //////////////////////////////////////////////////////////////////////
            std::string name;
            *include >> name;
            const char* dir = gSystem->DirName(gSystem->ConcatFileName(fBaseDir,name.c_str()));
            const char* base = gSystem->BaseName(name.c_str());
            printf("%s %s %s %s\n",fBaseDir.Data(),name.c_str(),dir,base);
            if (!Load(dir,base, loaded))
               return kFALSE;
         }
         continue;
      }
      TProcessor *proc = NULL;
      (*it) >> proc;
      if (!proc) return kFALSE;
      printf("%s\n",proc->GetName());
      fProcessors.push_back(proc);
   }
   fBaseDir = dirsaved;
   return kTRUE;
}



Bool_t art::TLoop::Init()
{
   std::list<TProcessor*>::iterator itr;
   std::list<TProcessor*>::iterator itrBegin = fProcessors.begin();
   std::list<TProcessor*>::iterator itrEnd   = fProcessors.end();
   printf("itrBegin = %ld itrEnd = %ld\n",itrBegin, itrEnd);
   // initialization function
//   fEventCollection->Clear();
   fEventCollection->Add(kConditionName,fCondition,kTRUE);
//   fEventCollection->AddInfo(kAnalysisInfoName,fAnalysisInfo,kFALSE);
   fCondition->Set(kBeginOfRun);
   TFolder *topfolder = (TFolder*) gROOT->FindObject("/artemis");
   TFolder *folder = (TFolder*) gROOT->FindObject(TString::Format("/artemis/loops/loop%d",fID));
   topfolder->Add(fAnalysisInfo);
//   fAnalysisInfo->SetProcessors(fProcessors);
   for (itr = itrBegin; itr!=itrEnd; ++itr) {
      TProcessor *proc = (*itr);
      proc->InitProc(fEventCollection);
      if (proc->IsError()) {
         Error("Init","\n\n Process '%s' (%s) %s\n",proc->GetName(),proc->GetTitle(),proc->GetErrorMessage());
         return kFALSE;
      }
      if ( NULL == fEventStore ) {
         fEventStore = dynamic_cast<IEventStore*>(proc);
      }
   }
   if (folder) {
      TFolder *conf = folder->AddFolder("Config","configuration");
      TIter *infoiter = fEventCollection->GetUserInfoIter();
      TEventObject *obj;
      while ((obj = (TEventObject*)infoiter->Next())) {
         conf->Add(*(TObject**)obj->GetObjectRef());
      }
      delete infoiter;
   }

   // prepared
   std::stringstream ss;
   for (itr = itrBegin; itr != itrEnd; ++itr) {
      (*itr)->PrintDescriptionYAML(ss);
   }
   TString yaml(ss.str());
   yaml.ReplaceAll("Processor:\n","");
   yaml.Prepend("Processor:\n");
   fAnalysisInfo->SetProcessors(yaml);
   
   
   return kTRUE;
}

Bool_t art::TLoop::Resume()
{

#ifdef USE_MPI
   {
      int myrank, npe,useMPI = false;
      MPI_Initialized(&useMPI);
      sleep(1);
      printf("resuming\n");
      if (useMPI) {
         MPI_Comm_size(MPI_COMM_WORLD, &npe);
         MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
         MPI_Barrier(MPI_COMM_WORLD);   
      }
   }
   
#endif   
   // get loop iterator
   std::list<TProcessor*>::iterator itr;
   std::list<TProcessor*>::iterator itrBegin = fProcessors.begin();
   std::list<TProcessor*>::iterator itrEnd   = fProcessors.end();
   // if the loop is already running, do nothing
   if (fCondition->IsSet(kRunning)) return kFALSE;
   // set status as running
   fCondition->Set(kRunning);
   // if the loop start at the first time, call BeginOfRun
   if (fCondition->IsSet(kBeginOfRun)) {
      for_each(itrBegin,itrEnd,std::mem_fun(&TProcessor::BeginOfRun));
      fCondition->Unset(kBeginOfRun);
      fAnalysisInfo->SetAnalysisStartTime(TDatime().AsSQLString());
   }
   // do prerun
   // For the special call to remap branch etc.
   for_each(itrBegin,itrEnd,std::mem_fun(&TProcessor::PreLoop));
   // start loop
   while (1) { 
     Int_t objectNumber = TProcessID::GetObjectCount();
     // increment event number
      for (itr = itrBegin; itr != itrEnd; itr++) {
         (*itr)->Process();
         if (fCondition->IsSet(kStopEvent)) {
            break;
         }
      }
      if (fCondition->IsSet(kStopEvent)) {
         fCondition->Unset(kStopEvent);
      } else {
         fAnalysisInfo->IncrementEventNumber();
         if (fEventStore) {
            fAnalysisInfo->SetRunNumber(TString::Format("%04d",fEventStore->GetRunNumber()));
            fAnalysisInfo->SetRunName(TString::Format("%s",fEventStore->GetRunName()));
         } else {
            printf("no event store\n");
         }
      }
      
      TProcessID::SetObjectCount(objectNumber);
      if (fCondition->IsSet(kStopLoop)) {
         // requested to stop run
         fCondition->Unset(kStopLoop);
         break;
      }
   }
   fAnalysisInfo->SetAnalysisEndTime(TDatime().AsSQLString());
   
   // store analysis information
   if (fEventStore) {
      fAnalysisInfo->SetRunNumber(TString::Format("%04d",fEventStore->GetRunNumber()));
      fAnalysisInfo->SetRunName(TString::Format("%s",fEventStore->GetRunName()));
   } else {
      printf("no event store\n");
   }

   for_each(itrBegin,itrEnd,std::mem_fun(&TProcessor::PostLoop));

      
      
      
   if (fCondition->IsSet(kEndOfRun)) {
      fCondition->Unset(kEndOfRun);
      for_each(itrBegin,itrEnd,std::mem_fun(&TProcessor::EndOfRun));
   }
   fCondition->Unset(kRunning);
   ((TRint*)gApplication)->SetPrompt("artemis [%d] ");

   return kTRUE;
}



Bool_t art::TLoop::Terminate()
{
   std::list<TProcessor*>::iterator itr;
   std::list<TProcessor*>::iterator itrBegin = fProcessors.begin();
   std::list<TProcessor*>::iterator itrEnd   = fProcessors.end();
   for_each(itrBegin,itrEnd,std::mem_fun(&TProcessor::Terminate));   
   return kTRUE;
}
