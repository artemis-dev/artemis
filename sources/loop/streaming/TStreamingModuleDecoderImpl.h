#ifndef TSTREAMINGMODULEDECODERIMPL_H
#define TSTREAMINGMODULEDECODERIMPL_H

#include "TStreamingModuleDecoder.h"
#include "TStreamingModuleDecoderFactory.h"

namespace art {
   template <typename T> class TStreamingModuleDecoderImpl;
}


template <typename T>
class art::TStreamingModuleDecoderImpl : public TStreamingModuleDecoder {

public:
   static TStreamingModuleDecoder* Create() { return new T(); }

   std::string GetName() { return T::fgName; }
   int GetID() { return T::fgID; }
   void SetID(int id) { T::fgID = id; }

protected:
   static const bool fgIsRegistered;
   TStreamingModuleDecoderImpl();
};

template <typename T>
art::TStreamingModuleDecoderImpl<T>::TStreamingModuleDecoderImpl()
: TStreamingModuleDecoder(fgIsRegistered)
{
}

template <typename T>
const bool art::TStreamingModuleDecoderImpl<T>::fgIsRegistered =
   art::TStreamingModuleDecoderFactory::Register(T::fgName, &art::TStreamingModuleDecoderImpl<T>::Create);


#endif // TSTREAMINGMODULEDECODERIMPL_H
