/**
 * @file   TCmdPadZoom.h
 * @brief  zoom subpad
 *
 * @date   Created       : 2017-06-19 14:51:12 JST
 *         Last Modified : 2017-06-19 14:52:18 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2017 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_B218643A_1DF9_4B3C_A702_6DA65E1B8851
#define INCLUDE_GUARD_UUID_B218643A_1DF9_4B3C_A702_6DA65E1B8851

#include "TCatCmd.h"

namespace art {
   class TCmdPadZoom;
}

class TCanvas;

class art::TCmdPadZoom : public TCatCmd {
public:
   static TCmdPadZoom *Instance();
   virtual ~TCmdPadZoom();

   Long_t Cmd(vector<TString> tokens);
   Long_t Run(Int_t id);

   void Help();
   void Closed();
   
protected:
   TCanvas *fCanvas;

   void CreateCanvas();

private:
   TCmdPadZoom();
   TCmdPadZoom(const TCmdPadZoom&); // prohibited
   TCmdPadZoom& operator=(const TCmdPadZoom&); // prohibited

   ClassDef(TCmdPadZoom,0) // zoom subpad
};

#endif // INCLUDE_GUARD_UUID_B218643A_1DF9_4B3C_A702_6DA65E1B8851
