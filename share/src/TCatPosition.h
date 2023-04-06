/* $Id:$ */
/**
 * @file   TCatPosition.h
 * @date   Created : Oct 26, 2013 03:26:10 JST
 *   Last Modified : Oct 26, 2013 04:04:50 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATPOSITION_H
#define TCATPOSITION_H

#include <TVector3>

namespace art {
   class TCatPosition;
}

class TCatPosition  : public TVector3 {

public:
   TCatPosition();
   virtual ~TCatPosition();

   void SetPulses(TObject* pulse1, TObject *pulse2) {
      fPulse0 = pulse1;
      fPulse1 = pulse2;
   }

   void GetPulse1() { return fPulse1; }
   void GetPulse2() { return fPulse2; }

private:
   TObject *fPulse1; //->
   TObject *fPulse2; //->


   ClassDef(TCatPosition,1);
};
#endif // end of #ifdef TCATPOSITION_H
