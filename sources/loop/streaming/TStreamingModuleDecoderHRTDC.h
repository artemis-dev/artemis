/*
 * Created       : 2023-02-11 11:54:42 JST
 * Last Modified : 2023-02-17 21:06:56 JST
 */

#ifndef TSTREAMINGMODULEDECODERHRTDC_H
#define TSTREAMINGMODULEDECODERHRTDC_H

#include "TStreamingModuleDecoderImpl.h"

namespace art {
   class TStreamingModuleDecoderHRTDC;
}

class TClonesArray;
class TObjArray;

class art::TStreamingModuleDecoderHRTDC :
   public TStreamingModuleDecoderImpl<TStreamingModuleDecoderHRTDC> {
public:
   TStreamingModuleDecoderHRTDC();
   virtual ~TStreamingModuleDecoderHRTDC();

   // utility
   unsigned int DecodeBits(unsigned long long data, unsigned long long shift, unsigned long long mask) {
      return ((data >> shift) & mask);
   }
   
   static const std::string fgName;
   static int fgID;
   static const int kID = {2};
   static const int fgChannelHBD = {128};
   static const int fgChannelSD = {129};

   virtual int Decode(char* buffer, const int &size, TObjArray *seg, int femid = 0);

   // clear hits
   virtual void Clear();

   static const unsigned int kShiftHeader = 58;
   static const unsigned int kHeaderMask = 0x3f; // header mask
   static const unsigned int kHeaderTDC  = 0x0b; // tdc data
   static const unsigned int kHeaderHBD  = 0x1c; // heartbeat delimiter
   static const unsigned int kHeaderSPND = 0x18; // spill on delimiter 
   static const unsigned int kHeaderSPFD = 0x14; // spill off delimiter

   static const unsigned long long kShiftChannel = 51;
   static const unsigned long long kMaskChannel = 0x7f;
   static const unsigned long long kShiftTOT    = 29;
   static const unsigned long long kMaskTOT     = 0x3fffff;
   static const unsigned long long kShiftTime   = 0;
   static const unsigned long long kMaskTime    = 0x1fffffff;

   // heartbeat or spill
   static const unsigned long long kShiftFlag = 48;
   static const unsigned long long kMaskFlag  = 0x3ff;
   static const unsigned long long kShiftSN = 40;
   static const unsigned long long kMaskSN  = 0xff;
   static const unsigned long long kShiftHB  = 24;
   static const unsigned long long kMaskHB   = 0xffff;
   
   

private:
   TClonesArray *fTDC; // tdc data
   TClonesArray *fHBD; // heartbeat frame delimiter
   TClonesArray *fSD; // spill delimiter
   
};

 

#endif // TSTREAMINGMODULEDECODERHRTDC_H
