/**
 * @file   TCmdMpol.h
 * @brief  polynomial fitting by mouse
 *
 * @date   Created       : 2017-06-14 13:11:30 JST
 *         Last Modified : 2017-06-14 13:11:32 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2017 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_3986406D_ADA9_4CD0_A276_532685940236
#define INCLUDE_GUARD_UUID_3986406D_ADA9_4CD0_A276_532685940236

#include "TCatCmd.h"

namespace art {
   class TCmdMpol;
}

class TClonesArray;

class art::TCmdMpol : public TCatCmd {
public:
   static TCmdMpol* Instance();
   virtual ~TCmdMpol();

   Long_t Run(Int_t degree);
   Long_t Cmd(vector<TString> tokens);
   void Help();
   void SetMarkerStyle(Int_t val){fMarkerStyle = val;}
   
protected:
   static const char* kFuncNameBase;
   static const char* kLabelNameBase;
   TClonesArray *fMarkers;
   Int_t         fMarkerStyle;

private:
   TCmdMpol();

   TCmdMpol(const TCmdMpol&); // undefined
   TCmdMpol& operator=(const TCmdMpol& rhs); // undefined;

   ClassDef(TCmdMpol,0) // polynomial fitting by mouse
};

#endif // INCLUDE_GUARD_UUID_3986406D_ADA9_4CD0_A276_532685940236
