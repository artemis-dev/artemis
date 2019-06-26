/**
 * @file   TCmdHcol.h
 * @brief  change the line color of histogram
 *
 * @date   Created       : 2018-02-13 15:02:23 JST
 *         Last Modified :
 * @author Shoichiro Masuoka <masuoka@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shoichiro Masuoka
 */

#ifndef INCLUDE_GUARD_UUID_8761BDED_5B13_4AC2_9C42_B97414EAAD4E
#define INCLUDE_GUARD_UUID_8761BDED_5B13_4AC2_9C42_B97414EAAD4E

#include "TCatCmd.h"
//#include <

namespace art {
   class TCmdHcol;
}

class art::TCmdHcol : public TCatCmd {
public:
   TCmdHcol();
   virtual ~TCmdHcol();

   virtual Long_t Cmd(vector<TString> tokens); 
//   virtual Long_t Run(Int_t id,TString &col);
   virtual Long_t Run(Int_t id, Int_t cid);   
   virtual void Help(); 
protected:

private:

   ClassDef(TCmdHcol,1) // change the line color of histogram
};

#endif // INCLUDE_GUARD_UUID_8761BDED_5B13_4AC2_9C42_B97414EAAD4E
