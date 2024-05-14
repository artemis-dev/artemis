#include "TStreamingModuleDecoderFactory.h"


#include <utility>
#include <cstdio>

std::map<std::string, factoryMethod> art::TStreamingModuleDecoderFactory::fgRegisteredNames = { };
std::map<int,art::TStreamingModuleDecoder*> art::TStreamingModuleDecoderFactory::fgDecoders = { };

bool art::TStreamingModuleDecoderFactory::Register(const std::string& name, factoryMethod method)
{
   printf("Registering %s\n",name.c_str());

   std::pair<std::map<std::string,factoryMethod>::iterator, bool> pair =
      TStreamingModuleDecoderFactory::fgRegisteredNames.insert(std::make_pair(name.c_str(),method));
   return pair.second;
}

art::TStreamingModuleDecoder* art::TStreamingModuleDecoderFactory::Create(std::string name)
{
   std::map<std::string,factoryMethod>::iterator pair =
      TStreamingModuleDecoderFactory::fgRegisteredNames.find(name);

   if (pair == TStreamingModuleDecoderFactory::fgRegisteredNames.end()) {
      return NULL;
   }

   return pair->second();
}

void art::TStreamingModuleDecoderFactory::CreateAll()
{
   for (std::pair<std::string,factoryMethod> pair : fgRegisteredNames) {
      TStreamingModuleDecoder* decoder = pair.second();
      TStreamingModuleDecoderFactory::fgDecoders.insert(std::make_pair(decoder->GetID(),decoder));
      printf("Created %s with decoder ID %d\n",pair.first.c_str(),decoder->GetID());
   }
}


art::TStreamingModuleDecoder* art::TStreamingModuleDecoderFactory::Find(int id)
{
   
   decltype(TStreamingModuleDecoderFactory::fgDecoders)& m = TStreamingModuleDecoderFactory::fgDecoders;
   decltype(TStreamingModuleDecoderFactory::fgDecoders)::iterator it = TStreamingModuleDecoderFactory::fgDecoders.find(id);
   if (it != m.end()) {
      return it->second;
   } else {
      return NULL;
   }
}

void art::TStreamingModuleDecoderFactory::Clear()
{
   for (auto& e : fgDecoders) {
      e.second->Clear();
   }
}


bool art::TStreamingModuleDecoderFactory::ResetID(int orgID, int newID) {
   decltype(TStreamingModuleDecoderFactory::fgDecoders)& m = TStreamingModuleDecoderFactory::fgDecoders;
   decltype(TStreamingModuleDecoderFactory::fgDecoders)::iterator it = TStreamingModuleDecoderFactory::fgDecoders.find(orgID);
   decltype(TStreamingModuleDecoderFactory::fgDecoders)::iterator itnew = TStreamingModuleDecoderFactory::fgDecoders.find(newID);
   if (it == m.end()) {
      printf("%s cannot find original id %d\n",__FUNCTION__, orgID);
      return false;
   }

   if (itnew != m.end()) {
      // key exists
      printf("%s cannot swap : id %d exists\n",__FUNCTION__, newID);
      return false;
   }

   it->second->SetID(newID);
   std::swap(m[newID],it->second);
   m.erase(it);
   
   printf("%s OK new id %d for %s\n",__FUNCTION__, m[newID]->GetID(), m[newID]->GetName().c_str());
   return true;
}
