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

const char* art::TLoop::kConditionName = "condition";
const char* art::TLoop::kAnalysisInfoName = "analysisInfo";

namespace {
   const char* kNodeKeyInclude = "include";
   const char* kNodeKeyReplace = "replace";
   const char* kNodeKeyName = "name";
}


ClassImp(art::TLoop);

art::TLoop::TLoop()
   : fCondition(NULL), fEventCollection(NULL), fProcessors(NULL), fID(0), fAnalysisInfo(NULL),
     fEventStore(NULL)
{
   fEventCollection = new TEventCollection;
   fCondition       = new TConditionBit;
   fAnalysisInfo    = new TAnalysisInfo;
}

art::TLoop::~TLoop()
{
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
Bool_t art::TLoop::Load(const char* dirname, const char* basename, std::list<Long_t> *loaded)
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
      std::ifstream fin(filename);
      YAML::Parser parser(fin);
      YAML::Node doc;
      std::string name, type, value;
      
      loaded->push_back(fstat.fIno);
      
      parser.GetNextDocument(doc);
      fin.close();
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
   // initialization function
//   fEventCollection->Clear();
   fEventCollection->Add(kConditionName,fCondition,kTRUE);
//   fEventCollection->AddInfo(kAnalysisInfoName,fAnalysisInfo,kFALSE);
   fCondition->Set(kBeginOfRun);
   TFolder *folder = (TFolder*) gROOT->FindObject(TString::Format("/artemis/loops/loop%d",fID));
   folder->Add(fAnalysisInfo);
//   fAnalysisInfo->SetProcessors(fProcessors);
   for (itr = itrBegin; itr!=itrEnd; itr++) {
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



