/* $Id:$ */
/**
 * @file   TArtFadcRawDataObject.h
 * @date   Created : Dec 25, 2011 00:25:53 JST
 *   Last Modified : Dec 25, 2011 02:24:03 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2011
 */
#ifndef TARTFADCRAWDATAOBJECT_H
#define TARTFADCRAWDATAOBJECT_H

#include "TArtRawDataObject.h"
#include <vector>
using namespace std;


class TArtFadcRawDataObject  : public TArtRawDataObject {

public:
   TArtFadcRawDataObject();
   TArtFadcRawDataObject(int ingeo, int inch, unsigned int timestamp, unsigned int pattern) 
     : TArtRawDataObject(ingeo,inch,timestamp), nSample(0), fTimestamp(timestamp), fPattern(pattern)
      { ; }
   ~TArtFadcRawDataObject() {}

   void AddSample(int adc) {
      fADC[nSample] = adc;
      fClock[nSample] = nSample;
      nSample++;
   }
   unsigned short &operator[] (int idx) { return fADC[idx]; }
   
   int GetNumSample() { return nSample; }
   unsigned int GetTimestamp() { return fTimestamp; }
   unsigned int GetPattern() { return fPattern; }

   virtual void Clear(const Option_t* = "");
protected:
   static const int kMaxSample = 1000;
   int nSample;
   unsigned short fADC[kMaxSample]; //[nSample]
   unsigned short fClock[kMaxSample]; //[nSample]
   unsigned int fTimestamp;
   unsigned int fPattern;
   //   vector<int> fADC;
   
   ClassDef(TArtFadcRawDataObject,1);
};
#endif // end of #ifdef TARTFADCRAWDATAOBJECT_H

