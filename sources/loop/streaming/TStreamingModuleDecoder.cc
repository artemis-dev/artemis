#include "TStreamingModuleDecoder.h"



art::TStreamingModuleDecoder::TStreamingModuleDecoder()
   : fIsRegistered(false)
{
}



art::TStreamingModuleDecoder::TStreamingModuleDecoder(bool isRegistered)
   : fIsRegistered(isRegistered)
{
}

art::TStreamingModuleDecoder::~TStreamingModuleDecoder()
{
}

