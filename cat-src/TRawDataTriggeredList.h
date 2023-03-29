/* $Id:$ */
/**
 * @file   TRawDataTriggeredList.h
 * @author Rin Yokoyama	 <yokoyama@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2016
 */
#ifndef TRAWDATATRIGGEREDLIST_H
#define TRAWDATATRIGGEREDLIST_H

#include <TRawDataObject.h>

namespace art {
   class TRawDataTriggeredList;
}

class art::TRawDataTriggeredList  : public TRawDataObject {

public:
   TRawDataTriggeredList();
   virtual ~TRawDataTriggeredList();

   virtual void SetADC(const UInt_t adc) {
      fADC = adc;
   }

   virtual void SetTSI(const UInt_t hi, const UInt_t lo) {
      fTSI_Hi = hi;
      fTSI_Lo = lo;
      fTSI = lo+(((unsigned long long)hi)<<29);
   }

   virtual void SetEventCount(const UInt_t eventcount) {
      fEventCount = eventcount;
   }

   UInt_t GetADC() const { return fADC; }
   UInt_t GetTSI_Hi() const { return fTSI_Hi; }
   UInt_t GetTSI_Lo() const { return fTSI_Lo; }
   unsigned long long GetTSI() const { return fTSI; }
   UInt_t GetEventCount() const { return fEventCount; }
#if 1
   void Clear(Option_t *opt) {
      TRawDataObject::Clear(opt);
      fADC = 0.;
      fTSI_Hi = 0;
      fTSI_Lo = 0;
      fTSI  = 0;
      fEventCount = 0;
   }
#endif

protected:
   UInt_t fADC;
   UInt_t fTSI_Hi;
   UInt_t fTSI_Lo;
   unsigned long long fTSI;
   UInt_t fEventCount;

   ClassDef(TRawDataTriggeredList,1);
};
#endif // end of #ifdef TRAWDATATRIGGEREDLIST_H
