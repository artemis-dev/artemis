#include "TStreamingModuleDecoderFactory.h"


#include <utility>
#include <cstdio>

std::map<std::string, factoryMethod> art::TStreamingModuleDecoderFactory::fgRegisteredNames = { };

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
      printf("Creating %s\n",pair.first.c_str());
      pair.second();
   }
}
