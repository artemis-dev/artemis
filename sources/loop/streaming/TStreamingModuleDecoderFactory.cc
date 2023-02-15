#include "TStreamingModuleDecoderFactory.h"


#include <utility>
#include <cstdio>

std::map<std::string, factoryMethod> art::TStreamingModuleDecoderFactory::fgRegisteredNames = { };
std::map<int,art::TStreamingModuleDecoder*> art::TStreamingModuleDecoderFactory::fgDecoders = { };

bool art::TStreamingModuleDecoderFactory::Register(std::string name, factoryMethod method)
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


