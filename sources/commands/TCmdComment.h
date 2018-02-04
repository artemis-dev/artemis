/**
 * @file   TCmdComment.h
 * @brief  write comment in pad
 *
 * @date   Created       : 2017-06-18 03:08:31 JST
 *         Last Modified : 2017-06-18 03:08:35 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2017 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_6D807444_2257_4030_844D_5E0907BC4310
#define INCLUDE_GUARD_UUID_6D807444_2257_4030_844D_5E0907BC4310

#include "TCatCmd.h"

namespace art {
   class TCmdComment;
}

class art::TCmdComment : public TCatCmd {
public:
   TCmdComment();
   virtual ~TCmdComment();

   static TCmdComment* Instance();
   
   Long_t Cmd(vector<TString> tokens);
   Long_t Run(const TString &comment);
   void Help();
protected:

private:
   TCmdComment(const TCmdComment&);
   TCmdComment& operator=(const TCmdComment&);

   ClassDef(TCmdComment,1) // write comment in pad
};

#endif // INCLUDE_GUARD_UUID_6D807444_2257_4030_844D_5E0907BC4310
