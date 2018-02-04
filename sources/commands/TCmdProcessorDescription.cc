/**
 * @file   TCmdProcessorDescription.cc
 * @brief  show Print Description
 *
 * @date   Created       : 2018-01-24 05:38:02 JST
 *         Last Modified : 2018-01-24 07:27:47 JST (mas)
 * @author Shoichiro Masuoka <masuoka@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shoichiro Masuoka
 */

#include "TCmdProcessorDescription.h"

#include "TClass.h"
#include "TProcessor.h"
//#include "TClass.h"

using art::TCmdProcessorDescription;

ClassImp(TCmdProcessorDescription)

TCmdProcessorDescription::TCmdProcessorDescription()
{
   SetName("processordescription");
   SetTitle("show PrintDescriptionYAML");
}

TCmdProcessorDescription::~TCmdProcessorDescription()
{
}
Long_t TCmdProcessorDescription::Cmd(vector<TString> tokens)
{
   if (tokens.size() != 2) {
      Help();
      return 1;
   }
//   TString token("");
   TString &className = tokens[1];
      
   
   // for (Int_t i = 0; i != n; ++i)
   // {
   //    if(i) token += ", ";
   //    token += tokens[i+1];
   // }
   
//   TString s = TString::Format("");
//   art::TProcessor *proc = TClass::GetClass(className)->New();
   TClass* cls = TClass::GetClass(className);
   if (!cls) {
      Error("Cmd" , "Invalid class");
      return 1;
   }
   
      
//   art::TProcessor *proc = static_cast<art::TProcessor*>(TClass::GetClass(className)->New());
   art::TProcessor *proc = static_cast<art::TProcessor*>(cls->New());
   proc->PrintDescriptionYAML();
   return 1;
}



TCmdProcessorDescription::TCmdProcessorDescription(const TCmdProcessorDescription& rhs)
{
}

TCmdProcessorDescription& TCmdProcessorDescription::operator=(const TCmdProcessorDescription& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TCmdProcessorDescription::Help()
{
   printf( "Processor Description \n");
   printf( "NAME \n");
   printf( "          processordescription\n");
   printf( "SYNOPSIS \n");
   printf( "          processordescription [ClassName]\n");
   printf( "DESCRIPTION \n");
   printf( "          show YAML description\n");

//   printf(" type description art::TxxxProcessor, and Run art::TxxxProcessor::PrintDescriptionYAML() \n" );
}
