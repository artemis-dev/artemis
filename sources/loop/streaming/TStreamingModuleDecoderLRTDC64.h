/*
 * Created       : 2023-02-11 11:54:42 JST
 * Last Modified : 2023-02-17 21:07:37 JST
 */

#ifndef TSTREAMINGMODULEDECODERLRTDC64_H
#define TSTREAMINGMODULEDECODERLRTDC64_H

#include "TStreamingModuleDecoderImpl.h"

namespace art {
   class TStreamingModuleDecoderLRTDC64;
}

class TClonesArray;
class TObjArray;

class art::TStreamingModuleDecoderLRTDC64 :
   public TStreamingModuleDecoderImpl<TStreamingModuleDecoderLRTDC64> {
public:
   TStreamingModuleDecoderLRTDC64();
   virtual ~TStreamingModuleDecoderLRTDC64();

   unsigned int DecodeBits(unsigned long long data, unsigned long long shift, unsigned long long mask) {
      return ((data >> shift) & mask);
   }
   
   static const std::string fgName;
   static int fgID;
   static const int kID = {3}; 
   static const int fgChannelHBD = {128};
   static const int fgChannelSD = {129} ;

   virtual int Decode(char* buffer, const int &size, TObjArray *seg, int femid = 0);

   // clear hits
   virtual void Clear();

   static const unsigned int kShiftHeader = 58;
   static const unsigned int kHeaderMask = 0x3f; // header mask
   static const unsigned int kHeaderTDC     = 0x0b; // (b001011) tdc leading data 
   static const unsigned int kHeaderTDCT    = 0x0d; // (b001101) tdc trailing data 
   static const unsigned int kHeaderTDCT1S  = 0x19; // (b011001) input throttling t1 start 
   static const unsigned int kHeaderTDCT1E  = 0x11; // (b010001) input throttling t1 end
   static const unsigned int kHeaderTDCT2S  = 0x1a; // (b011010) input throttling t2 start 
   static const unsigned int kHeaderTDCT2E  = 0x12; // (b010010) input throttling t2 end
   static const unsigned int kHeaderHBD  = 0x1c; // heartbeat delimiter
   static const unsigned int kHeaderSPND = 0x18; // spill on delimiter 
   static const unsigned int kHeaderSPFD = 0x14; // spill off delimiter

   static const unsigned long long kShiftChannel = 51;
   static const unsigned long long kMaskChannel = 0x7f;
   static const unsigned long long kShiftTOT    = 35;
   static const unsigned long long kMaskTOT     = 0xffff;
   static const unsigned long long kShiftTime   = 16;
   static const unsigned long long kMaskTime    = 0x7ffff;

   // heartbeat
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

 

#endif // TSTREAMINGMODULEDECODERLRTDC64_H