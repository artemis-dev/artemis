/*
 * Created       : 2023-02-11 11:54:42 JST
 * Last Modified : 2024/04/13 02:50:51
 */

#ifndef TSTREAMINGMODULEDECODERHRTDC_H
#define TSTREAMINGMODULEDECODERHRTDC_H

#include "TStreamingModuleDecoderImpl.h"

namespace art {
namespace streaming {
namespace v1 {
class TStreamingModuleDecoderHRTDC;
}
}  // namespace streaming
}  // namespace art

class TClonesArray;
class TObjArray;

class art::streaming::v1::TStreamingModuleDecoderHRTDC
    : public TStreamingModuleDecoderImpl<TStreamingModuleDecoderHRTDC> {
  public:
   TStreamingModuleDecoderHRTDC();
   virtual ~TStreamingModuleDecoderHRTDC();

   // utility
   unsigned int DecodeBits(unsigned long long data, unsigned long long shift,
                           unsigned long long mask) {
      return ((data >> shift) & mask);
   }

   inline static std::string fgName{"art::streaming::v1::TStreamingModuleDecoderHRTDC"};
   static int fgID;
   static const int kID = {5};
   static const int fgChannelHBD = {128};
   static const int fgChannelSD = {129};

   virtual int Decode(char *buffer, const int &size, TObjArray *seg,
                      int femid = 0);

   // clear hits
   virtual void Clear();

   static const unsigned int kShiftHeader = 58;
   static const unsigned int kHeaderMask = 0x3f;  // header mask
   static const unsigned int kHeaderTDC = 0x0b;   // tdc data
   static const unsigned int kHeaderTDCT = 0x0d;  // (b001101) tdc trailing data
   static const unsigned int kHeaderTDCT1S =
       0x19;  // (b011001) input throttling t1 start
   static const unsigned int kHeaderTDCT1E =
       0x11;  // (b010001) input throttling t1 end
   static const unsigned int kHeaderTDCT2S =
       0x1a;  // (b011010) input throttling t2 start
   static const unsigned int kHeaderTDCT2E =
       0x12;  // (b010010) input throttling t2 end
   static const unsigned int kHeaderHBD = 0x1c;  // heartbeat delimiter

   static const unsigned long long kShiftChannel = 51;
   static const unsigned long long kMaskChannel = 0x7f;
   static const unsigned long long kShiftTOT = 29;
   static const unsigned long long kMaskTOT = 0x3fffff;
   static const unsigned long long kShiftTime = 0;
   static const unsigned long long kMaskTime = 0x1fffffff;

  private:
   TClonesArray *fTDC;  // tdc data
   TClonesArray *fHBD;  // heartbeat frame delimiter
};

#endif  // TSTREAMINGMODULEDECODERHRTDC_H
