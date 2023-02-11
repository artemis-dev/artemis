#ifndef TSTREAMINGMODULEDECODERFACTORY_H
#define TSTREAMINGMODULEDECODERFACTORY_H

#include "TStreamingModuleDecoder.h"

#include <map>

namespace art {
   class TStreamingModuleDecoderFactory;
}


typedef art::TStreamingModuleDecoder* (*factoryMethod)();

class art::TStreamingModuleDecoderFactory 
{
public:
   static bool Register(std::string name, factoryMethod method);

   static TStreamingModuleDecoder* Create(std::string name);

   static void CreateAll();

protected:
   static std::map<std::string, factoryMethod> fgRegisteredNames;
};




#endif // TSTREAMINGMODULEDECODERFACTORY_H
