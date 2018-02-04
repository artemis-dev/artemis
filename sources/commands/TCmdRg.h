/**
 * @file   TCmdRg.h
 * @brief  command for setting axis range
 *
 * @date   Created       : 2016-10-09 13:25:34 JST
 *         Last Modified : 2018-02-04 11:23:17 JST (ota)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 * @comment
 *
 *    (C) 2016 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_19C75B15_0593_40A3_AABD_32006D8655B4
#define INCLUDE_GUARD_UUID_19C75B15_0593_40A3_AABD_32006D8655B4

#include "TCatCmd.h"

namespace art {
   class TCmdRg;
}

class TH1;
class TAxis;

class art::TCmdRg : public TCatCmd {
public:
   typedef enum {kX, kY, kZ} EAxis;
   TCmdRg(const EAxis axis);
   virtual ~TCmdRg();

   virtual Long_t Cmd(vector<TString> tokens);
   virtual Long_t Run(TH1 *h, Double_t ufirst, Double_t ulast) const;
   void Help();

private:
   typedef TAxis* (TH1::*AxisGetter)();
   AxisGetter fGetAxis; // pointer to Getter method of axis

   TCmdRg(const TCmdRg&);            // undefined
   TCmdRg& operator=(const TCmdRg&); // undefined

   ClassDef(TCmdRg,0) // command for setting axis range
};

#endif // INCLUDE_GUARD_UUID_19C75B15_0593_40A3_AABD_32006D8655B4
