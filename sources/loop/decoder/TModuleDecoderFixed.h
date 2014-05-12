/**
 * @file   TModuleDecoderFixed.h
 * @brief
 *
 * @date   Created:       2013-07-30 16:13:56
 *         Last Modified: 2013-11-20 10:05:53
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TMODULEDECODERFIXED_H
#define TMODULEDECODERFIXED_H

#include <TModuleDecoder.h>

namespace art{

template <typename T> class TModuleDecoderFixed : public TModuleDecoder {
public:
   TModuleDecoderFixed();
   TModuleDecoderFixed(Int_t id, Int_t size);
   virtual ~TModuleDecoderFixed();

   virtual Int_t Decode(char* buffer, const int &size, TObjArray *seg);
   // Copy this mask to obj
   virtual void Copy(TObject& obj) const;
protected:
   T          fMaskMeasure;
   TObjArray *fHitData;

private:
   void CalcMaskMeasure(Int_t size);

   // Copy constructor (prohibited)
   TModuleDecoderFixed(const TModuleDecoderFixed<T>&);
   // Assignment operator (prohibited)
   TModuleDecoderFixed<T>& operator=(const TModuleDecoderFixed<T>&);

   ClassDef(TModuleDecoderFixed,0);
};

} // namespace art

#endif // TMODULEDECODERFIXED_H
