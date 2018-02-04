/**
 * @file   TCmdClassInfo.cc
 * @brief  show artemis class information
 *
 * @date   Created       : 2015-04-09 21:01:43 JST
 *         Last Modified : Apr 10, 2015 18:19:19 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015
 */

#include "TCmdClassInfo.h"
#include <TClass.h>
#include <TList.h>
#include <TRegexp.h>
#include <TDataMember.h>
#include <TMethod.h>
#include <TBaseClass.h>
#include <TDataType.h>

namespace {
   const char* const excludedMethods[] = {
      "Class",
      "Class_Name",
      "Class_Version",
      "Dictionary",
      "IsA",
      "ShowMembers",
      "Streamer",
      "StreamerNVirtual",
      "DeclFileName",
      "ImplFileLine",
      "ImplFileName",
      "DeclFileLine"
   };

   const char* const excludedMembers[] = {
      "fgIsA"
   };
}

using art::TCmdClassInfo;

ClassImp(TCmdClassInfo)

TCmdClassInfo::TCmdClassInfo()
{
   SetName("classinfo");
   SetTitle("show class information");
}

TCmdClassInfo::~TCmdClassInfo()
{
}

TCmdClassInfo::TCmdClassInfo(const TCmdClassInfo& rhs)
{
}

TCmdClassInfo& TCmdClassInfo::operator=(const TCmdClassInfo& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

Long_t TCmdClassInfo::Cmd(std::vector<TString> args)
{
   if (args.size() != 2) {
      Help();
      return 1;
   }
   Run(args[1].Data());
   return 1;
}

Long_t TCmdClassInfo::Run(const char* name)
{
   TClass *cls = TClass::GetClass(name);
   if (!cls) {
      Error("Run","Cannot find class '%s'",name);
      return 1;
   }
   // get only overrided methods
   
   
   TList *methods = new TList;
   TList *members = new TList;

   // Clone of TList cannot used since the data type of TDataMember is null.
   { // copy 
      TIter it(cls->GetListOfMethods());
      while (TObject *obj = it()) {
         methods->Add(obj);
      }
   }
   { // copy members
      TIter it(cls->GetListOfDataMembers());
      while (TObject *obj = it()) {
         members->Add(obj);
      }
   }

   // remove constructors
   TString clsname = name;
   clsname.Remove(0,clsname.Index(TRegexp("[^:]+$")));
   while (TObject *obj = methods->FindObject(clsname)) {
      methods->Remove(obj);
   }


   // remove destructor
   clsname.Insert(0,"~");
   methods->Remove(methods->FindObject(clsname));

   // remove methods in the exclude list
   Int_t n = sizeof(excludedMethods) / sizeof(const char*);
   for (Int_t i=0; i!=n; ++i) {
      methods->Remove(methods->FindObject(excludedMethods[i]));
   }

   // remove members in the exclude list
   n = sizeof(excludedMembers) / sizeof(const char*);
   for (Int_t i=0; i!=n; ++i) {
      members->Remove(members->FindObject(excludedMembers[i]));
   }
   
   // display items
   printf("\n");
   printf(" %s\n",name);
   { 
      TIter it(members);
      printf("\n");
      printf(" Data Members\n");
      printf("\n");
      while (TDataMember *mem = (TDataMember*)it()) {
         // mem->Print();
         TString type;
         if (mem->IsBasic()) {
            type = mem->GetDataType()->GetTypeName();
         } else if (mem->IsEnum()) {
            type = mem->GetTypeName();
         }
         type = mem->GetTypeName();
         type.Remove(0,type.Index(TRegexp("[^:]+$")));
         printf("%5s%20s%3s%-20s%3s%-20s\n","",type.Data(),"",mem->GetName(),"",mem->GetTitle());
      }
   }   

   { 
      TIter it(methods);
      printf("\n");
      printf(" Methods\n");
      printf("\n");
      while (TMethod *obj = (TMethod*)it()) {
         // mem->Print();
         TString type = obj->GetReturnTypeName();
         type.Remove(0,type.Index(TRegexp("[^:]+$")));
         printf("%5s%20s%3s%-20s%3s%-20s\n","",type.Data(),"",obj->GetName(),"",obj->GetTitle());
      }
   }   

   // see also
   if (cls->GetListOfBases() != NULL){
      TIter it(cls->GetListOfBases());
      printf("\n");
      printf(" See also\n");
      printf("\n");
      while (TBaseClass *obj = (TBaseClass*)it()) {
         printf("%5s%-20s%3s%-20s\n","",obj->GetName(),"",obj->GetTitle());
      }
   }


   return 1;
}


void TCmdClassInfo::Help() {
   printf(" classinfo [classname]\n");
   
}
