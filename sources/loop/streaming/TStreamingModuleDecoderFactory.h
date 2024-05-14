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
   static bool Register(const std::string& name, factoryMethod method);

   static TStreamingModuleDecoder* Create(std::string name);

   static void CreateAll();

   static TStreamingModuleDecoder* Find(int femtype);

   static void Clear();

   static bool ResetID(int orgID, int newID);

protected:
   static std::map<std::string, factoryMethod> fgRegisteredNames;
   static std::map<int, TStreamingModuleDecoder*> fgDecoders;
};




#endif // TSTREAMINGMODULEDECODERFACTORY_H
