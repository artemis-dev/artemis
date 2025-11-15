/**
 * @file   TGateInfo.h
 * @brief
 *
 * @date   Created:       2014-01-16 12:11:39
 *         Last Modified: 2014-01-20 10:25:18
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro All rights reserved
 */

#ifndef TGATEINFO_H
#define TGATEINFO_H

#include <TNamed.h>

namespace art {
   class TGateInfo;
}


class art::TGateInfo : public TNamed {
public:
   // Default constructor
   TGateInfo();
   TGateInfo(Int_t gateid, const TString &name, const TString &description);
   virtual ~TGateInfo();
   // Copy constructor
   TGateInfo(const TGateInfo& rhs);
   // Assignment operator
   TGateInfo& operator=(const TGateInfo& rhs);

   Int_t GetID() const { return fID; }

   virtual void Print(Option_t *opt = "") const;

protected:
   Int_t fID; // gate ID

private:
   
   ClassDef(TGateInfo,1) // gate infomation
};

#endif // TGATEINFO_H
