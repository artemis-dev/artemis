/* @file TCmdGlobalComment.cc
 * @brief add comment in canvas
 *
 * @date Create        : 2019-11-24 02:02:52 JST
 *       Last Modified : 2019-11-24 03:41:57 JST (ota)
 * @author:  <ota@ata03.atnet>
 */


#include "TCmdGlobalComment.h"
#include "TCatPadManager.h"

ClassImp(art::TCmdGlobalComment)

using art::TCmdGlobalComment;

TCmdGlobalComment::TCmdGlobalComment()
{
   SetName("gcom");
   SetTitle("add comment to global header of artcanvas");
}

TCmdGlobalComment::~TCmdGlobalComment()
{
}

Long_t TCmdGlobalComment::Cmd(std::vector<TString> args)
{
   if (args.size() < 2 || args.size() > 2) {
      Help();
      return 1;
   }
   Run(args[1].Data());
   return 1;
}

Long_t TCmdGlobalComment::Run(const char* label)
{
   TCatPadManager::Instance()->Current();
   TCatPadManager::SetComment(label);
   return 1;
}

void TCmdGlobalComment::Help() {
   TString help = "";
   help.Append("NAME\n");
   help.Append("\n");
   help.Append("     gcom -- add global comment in canvas");
   help.Append("SYNOPSIS\n");
   help.Append("     gcom [comment]\n");
   help.Append("\n");
   help.Append("DESCRIPTION\n");
   help.Append("     A command gcom add your comment in an artemis canvas.");
   help.Append("     If no artemis canvas is available, create canvas");
   printf("%s",help.Data());
   
}
