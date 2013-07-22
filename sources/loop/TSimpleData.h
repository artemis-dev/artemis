/* $Id:$ */
/**
 * @file   TSimpleData.h
 * @date   Created : Jul 11, 2013 22:11:59 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TSIMPLEDATA_H
#define TSIMPLEDATA_H

namespace art {
   class TSimpleData;
}

class art::TSimpleData  : public TObject {

public:
   TSimpleData(){;}
   ~TSimpleData(){;}

   Double_t fValue;
   ClassDef(TSimpleData,1);
};


#endif // end of #ifdef TSIMPLEDATA_H
