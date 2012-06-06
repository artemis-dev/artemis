/* $Id:$ */
/**
 * @file   TCatV1190Processor.h
 * @date   Created : Jun 07, 2012 06:07:04 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATV1190PROCESSOR_H
#define TCATV1190PROCESSOR_H

#include <TCatProcessor.h>

class TCatV1190Processor  : public TCatProcessor {

public:
   TCatV1190Processor();
   virtual ~TCatV1190Processor();

   virtual void Init(TCatEventCollection *col);
   virtual void Process();

   
   TArtRawDataObject *fRaw;
   Int_t fTrailingChOffset;
   Int_t fRefGeo;
   Int_t fRefCh;
   Int_t fRefDevice;
   Int_t fRefFp;
   Int_t fRefDetType;

   Int_t fGeo;
   Int_t fDev;
   Int_t fFp;
   Int_t fDetType;

   TString fNameOfRawData;
   
   TClonesArray *fHits;
};
#endif // end of #ifdef TCATV1190PROCESSOR_H
