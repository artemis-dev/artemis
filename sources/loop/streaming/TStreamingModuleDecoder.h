#ifndef TSTREAMINGMODULEDECODER_H
#define TSTREAMINGMODULEDECODER_H

#include <string>


namespace art {
   class TStreamingModuleDecoder;
}

class TObjArray;

class art::TStreamingModuleDecoder  {

public:
   TStreamingModuleDecoder();
   TStreamingModuleDecoder(bool isRegistered);
   virtual ~TStreamingModuleDecoder();

   bool IsRegistered() const { return fIsRegistered; }
   
   virtual std::string GetName() = 0;
   virtual int GetID() = 0;
   virtual void SetID(int id) = 0;

   virtual int Decode(char* buffer, const int &size, TObjArray *seg, int femid = 0) = 0;
   // clear hits
   virtual void Clear() = 0;

public:

private:
   // do nothing
//   virtual Int_t Decode(char* buffer, const int &size, TObjArray *seg) {};
   const bool fIsRegistered;
};

#endif // TSTREAMINGMODULEDECODER_H
