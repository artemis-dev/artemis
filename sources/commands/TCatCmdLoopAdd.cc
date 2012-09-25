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
#include <yaml-cpp/yaml.h>

#include <fstream>
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
   if (args.size()!=2) {
      Help();
      return 1;
   }
   TString &filename = args[1];

   // yaml loader should be more sophystocated
   if (filename.EndsWith(".yaml")) {
      LoadYAML(filename);
   } 

   
   return 1;
}

void TCatCmdLoopAdd::Help()
{
}


void TCatCmdLoopAdd::LoadYAML(TString filename)
{
   // check if file exists
   ifstream fin(filename);
   if (!fin) {
      printf("Catnnot open %s\n",filename.Data());
      return;
   }
   // initialize parser
   YAML::Parser parser(fin);
   YAML::Node doc;
   parser.GetNextDocument(doc);
   std::string name, type, value;

   TCatLoopManager *lm = TCatLoopManager::Instance();
   TCatLoop *l = lm->Add();

   // try check processor
   try {
      const YAML::Node &node = doc["Processor"];
      // iterate for all the processors
      for (YAML::Iterator it = node.begin(); it != node.end(); it++) {
         const YAML::Node &prm = *it;
         prm["name"] >> name;
         prm["type"] >> type;
         TClass *cls = gROOT->GetClass(type.data());
         if (!cls) {
            cout << "  no such processor or processor is not register in dictionary" << endl;
            cout << "  " << type  << endl;
            break;
         }
         TCatProcessor *proc = (TCatProcessor*) cls->New();
         if (!proc) break;
         proc->SetName(name.data());
         TCatParameterStrings *str = new TCatParameterStrings;
         try {
            const YAML::Node &parameters = (*it)["parameter"];
            vector<TString> prm;
            Int_t nParam = parameters.size();
            for (Int_t iParam = 0; iParam != nParam; iParam++) {
               prm.clear();
               const YAML::Node &param = parameters[iParam];
               param["name"] >> name;
               param["type"] >> type;
               switch (param["value"].Type()) {
               case YAML::NodeType::Scalar:
                  param["value"] >> value;
                  prm.push_back(value);
                  break;
               case YAML::NodeType::Sequence:
                  for (Int_t ival = 0; ival != param["value"].size(); ival++) {
                     param["value"][ival] >> value;
                     prm.push_back(value);
                  }
                  break;
               default:
                  break;
               }
               str->Add(name.data(),prm);
            }
            proc->SetParameters(str);
            l->AddProcess(proc->GetName(),proc);
         } catch (YAML::KeyNotFound& e) {
            cout << e.what() << endl;
         }
      }
   } catch (YAML::KeyNotFound& e) {
      cout << e.what() << endl;
   }

   // only YAML format is accepted
//   YAML::Parser
}




