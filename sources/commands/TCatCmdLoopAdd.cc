/* $Id:$ */
/**
 * @file   TCatCmdLoopAdd.cc
 * @date   Created : May 02, 2012 16:02:35 JST
 *   Last Modified : May 02, 2012 17:28:12 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdLoopAdd.h"
#include "TCatLoopManager.h"
#include <TROOT.h>
#include <TFolder.h>
#include <yaml-cpp/yaml.h>

#include <fstream>

#include <TCatRIDFEventStore.h>

using namespace std;


ClassImp(TCatCmdLoopAdd);

TCatCmdLoopAdd::TCatCmdLoopAdd()
{
   SetName("add");
   SetTitle("Add loop");
}
TCatCmdLoopAdd::~TCatCmdLoopAdd()
{
}

TCatCmdLoopAdd* TCatCmdLoopAdd::Instance()
{
   static TCatCmdLoopAdd instance;
   return &instance;
}

Long_t TCatCmdLoopAdd::Cmd(vector<TString> args)
{
   Bool_t retval = kFALSE;
   
   if (args.size()!=2) {
      Help();
      return 1;
   }
   TString &filename = args[1];
   // check if file exists
   ifstream fin(filename);
   if (!fin) {
      printf("Catnnot open %s\n",filename.Data());
      return 1;
   }
   fin.close();
   
   // preparation for loop
   TCatLoopManager *lm = TCatLoopManager::Instance();
   TCatLoop *l = lm->Add();
   TFolder *top = (TFolder*) gROOT->GetListOfBrowsables()->FindObject("artemis");
   TFolder *thisLoop = top->AddFolder("loop","loop");
   l->SetName("loop");
   thisLoop->Add(l);

   // yaml loader should be more sophystocated
   if (filename.EndsWith(".yaml")) {
      retval = LoadYAML(filename,thisLoop);
   } 

   // rollback if load is failed
   if (!retval) {
      top->Remove(thisLoop);
      // not implimented yet
      // lm->Remove(thisLoop);
      // delete l
   }

   l->Init();

   return 1;
}

void TCatCmdLoopAdd::Help()
{
}

void operator >> (const YAML::Node &node, TCatParameterStrings *&str) {
   vector <TString> prm;
   std::string name,value;
   for (YAML::Iterator it = node.begin(); it != node.end(); it++) {
      prm.clear();
      const YAML::Node &param = *it;
      param["name"] >> name;
      if (param["value"].Type() == YAML::NodeType::Scalar) {
         param["value"] >> value;
         prm.push_back(value);
      } else if (param["value"].Type() == YAML::NodeType::Sequence) {
         for (YAML::Iterator itv = param["value"].begin(); itv != param["value"].end(); itv++) {
            (*itv) >> value;
            prm.push_back(value);
         }
      }
      str->Add(name.data(),prm);
   }
}

void operator >> (const YAML::Node &node, TCatProcessor *&proc)
{
   std::string name, type;
   proc = NULL;
   try {
      node["name"] >> name;
      node["type"] >> type;
   } catch (YAML::KeyNotFound& e) {
      cout << e.what() << endl;
      return;
   }
   TClass *cls = gROOT->GetClass(type.data());
   if (!cls) {
      cout << "  no such processor or processor is not register in dictionary" << endl;
      cout << "  " << type  << endl;
      return;
   }
   proc = (TCatProcessor*) cls->New();
   TCatParameterStrings *str = new TCatParameterStrings;
   try {
      node["parameter"] >> str;
   } catch (YAML::KeyNotFound& e) {
      // nothing to do with no paramter for now
      // cout << e.what() << endl;
   }
   proc->SetParameters(str);
}

   
Bool_t TCatCmdLoopAdd::LoadYAML(TString filename, TFolder *thisLoop)
{
   // initialize parser

   ifstream fin(filename);
   YAML::Parser parser(fin);
   fin.close();
   YAML::Node doc;
   parser.GetNextDocument(doc);
   std::string name, type, value;
   TCatLoop *l = (TCatLoop*) thisLoop->FindObject("loop");
   try {
      const YAML::Node &node = doc["Processor"];
      // iterate for all the processors
      for (YAML::Iterator it = node.begin(); it != node.end(); it++) {
         TCatProcessor *proc = NULL;
         (*it) >> proc;
         l->AddProcess(proc->GetName(),proc);
      }
   } catch (YAML::KeyNotFound& e) {
      cout << e.what() << endl;
   }

   // try check event store
   TCatRIDFEventStore *evt = NULL;
   try {
      const YAML::Node &node = doc["EventStore"];
      evt = new TCatRIDFEventStore;
      for (YAML::Iterator it = node.begin(); it != node.end(); it++) {
         // only ridf is accepted now
         (*it) >> value;
         evt->AddInputFile(value.data());
      }
      l->SetEventStore(evt);
   } catch (YAML::KeyNotFound& e){
      delete evt;
   }
   return kTRUE;
}




