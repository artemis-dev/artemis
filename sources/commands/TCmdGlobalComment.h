/* @file TCmdGlobalComment.h
 * @brief add comment in art canvas
 *
 * @date Create        : 2019-11-24 01:50:42 JST
 *       Last Modified : 2019-11-24 02:03:43 JST (ota)
 * @author:  <ota@ata03.atnet>
 */

#ifndef INCLUDE_GUARD_UUID_0D24F1AF_012B_4E88_96A9_FF1C299252FC
#define INCLUDE_GUARD_UUID_0D24F1AF_012B_4E88_96A9_FF1C299252FC

#include "TCatCmd.h"
#include <vector>
namespace art {
   class TCmdGlobalComment;
}


class art::TCmdGlobalComment : public TCatCmd {
public:
   TCmdGlobalComment();
   virtual ~TCmdGlobalComment();

   virtual Long_t Run(const char* comment);
   virtual Long_t Cmd(std::vector<TString> token);
   
   virtual void Help();

   ClassDef(TCmdGlobalComment,1)
}; // end of TCmdGlobalComment

#endif // #ifndef INCLUDE_GUARD_UUID_0D24F1AF_012B_4E88_96A9_FF1C299252FC}
