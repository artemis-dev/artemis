/* $Id:$ */
/**
 * @file   TCatDecoderV1190.cc
 * @date   Created : Jun 22, 2012 19:22:27 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatDecoderV1190.h"

TCatDecoderV1190::TCatDecoderV1190()
: TArtDecoder(kID) {
   for (Int_t i=0; i!=128; i++) {
      fIdx[i] = NULL;
      fLeadingIsFirst[i] = kTRUE;
   }
   fV1190Hits = new TClonesArray("TCatRawDataV1190");
}
TCatDecoderV1190::~TCatDecoderV1190()
{
   delete fV1190Hits;
}

Int_t TCatDecoderV1190::Decode(unsigned char *&buf, const unsigned int& size,
                        TArtRawSegmentObject *rawseg)
{
   unsigned int *evtdata = (unsigned int*) buf;
   int evtsize = size/sizeof(unsigned int);
   unsigned int ih;
   int igeo;
   int ghf,  bncid, evtid;
//   fHits->Clear();
   for (int i=0; i<evtsize; i++) {
      ih = evtdata[i]&kHeaderMask;
      if (ih == kGlobalHeader) {
         ghf = 1;
         igeo = (evtdata[i]&kMaskGeometry)>>kShiftGeometry;
      } else if (ih == kTDCHeader) {
         if (ghf != 1) break;
         bncid = (evtdata[i]&kMaskBunchID)>>kShiftBunchID;
         evtid = (evtdata[i]&kMaskEventCounter)>>kShiftEventCounter;
      } else if (ih == kTDCMeasurement) {
         Int_t ich = (evtdata[i]&kMaskChannel) >> kShiftChannel;
         Int_t edge = (evtdata[i]&kMaskEdgeType) >> kShiftEdgeType;
         Double_t meas = (evtdata[i]&kMaskMeasure)>>kShiftMeasure;
         if (fIdx[ich] == NULL) {
            fIdx[ich] = new ((*fV1190Hits)[fV1190Hits->GetEntriesFast()]) TCatRawDataV1190(igeo,ich);
//            fIdx[ich] = new TCatRawDataV1190(igeo,ich);
         }
         switch(edge) {
         case 0:
            fIdx[ich]->SetLeading(meas);
            if (!fLeadingIsFirst) {
               rawseg->PutData(fIdx[ich]);
               fIdx[ich] = NULL;
            }
            break;
         case 1:
            fIdx[ich]->SetTrailing(meas);
            if (fLeadingIsFirst) {
               rawseg->PutData(fIdx[ich]);
               fIdx[ich] = NULL;
            }
            break;
         }
      } else if (ih == kTDCTrailer) {
         //printf("V1190 [TDC Trailer  ] : 0x%08x\n", evtdata[i]);
         //thf = 0;
      } else if (ih == kTDCError) {
         //printf("V1190 [TDC Error    ] : 0x%08x\n", evtdata[i]);
      } else if (ih == kGlobalTrailer) {
         for (Int_t j=0; j!= 128;j++) {
            if (fIdx[j] != NULL) {
               rawseg->PutData(fIdx[j]);
               fIdx[j] = NULL;
            }
         }
         //printf("V1190 [Global Traile] : 0x%08x\n", evtdata[i]);
         ghf = 0;
      }
   }
   return 0;
}
