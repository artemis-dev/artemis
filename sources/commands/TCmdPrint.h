/**
 * @file   TCmdPrint.h
 * @brief  print the last-saved figure
 *
 * @date   Created       : 2017-06-13 16:38:24 JST
 *         Last Modified : 2017-12-26 21:37:19 JST (ota)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2017 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_9784A220_859C_4809_9B48_A9561F2CD114
#define INCLUDE_GUARD_UUID_9784A220_859C_4809_9B48_A9561F2CD114

#include "TCatCmd.h"

namespace art {
   class TCmdPrint;
}

class art::TCmdPrint : public TCatCmd {
public:
   TCmdPrint();
   virtual ~TCmdPrint();

   Long_t Run();
   Long_t Cmd(vector<TString>);
   void SetPrinter(const char* printer);
   void SetOption(const char* option);

protected:
   TString fPrinter; // printer name
   TString fOption; // option

private:
   TCmdPrint(const TCmdPrint&); // undefined 
   TCmdPrint& operator=(const TCmdPrint&); // undefined

   ClassDef(TCmdPrint,0) // print the last-saved figure
};

#endif // INCLUDE_GUARD_UUID_9784A220_859C_4809_9B48_A9561F2CD114
