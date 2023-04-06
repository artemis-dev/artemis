/* $Id:$ */
/**
 * @file   TCatHitSelection.h
 * @date   Created : Oct 26, 2013 04:26:18 JST
 *   Last Modified : Oct 26, 2013 05:15:49 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATHITSELECTION_H
#define TCATHITSELECTION_H

#include <TProcessor.h>

namespace art {
   class TCatHitSelection;
}
class TClonesArray;

class art::TCatHitSelection  : public art::TProcessor {

public:
   TCatHitSelection();
   ~TCatHitSelection();
   void Init(TEventCollection *col);
   void Process();

protected:
   StringVec_t fInputColName;
   TClonesArray  **fCatHit;

   ClassDef(TCatHitSelection,1);
};
#endif // end of #ifdef TCATHITSELECTION_H
