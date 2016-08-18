/* $Id:$ */
/**
 * @file   TCatCmdHelp.cc
 * @date   Created : Feb 10, 2012 20:10:39 JST
 *   Last Modified : 2016-08-18 13:53:51 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdHelp.h"

#include <TCatCmdFactory.h>

TCatCmdHelp::TCatCmdHelp()
{
   SetName("help");
   SetTitle("show help");
}
TCatCmdHelp::~TCatCmdHelp()
{
}

TCatCmdHelp* TCatCmdHelp::Instance()
{
   static TCatCmdHelp instance;
   return &instance;
}

Long_t TCatCmdHelp::Cmd(vector<TString> args) 
{
   if (args.size() > 1) {
      TCatCmdFactory::Instance()->CmdHelp(args[1]);
   } else {
      TCatCmdFactory::Instance()->CmdHelp();
   }
   return 1;
}

void TCatCmdHelp::Help()
{
   printf("SYNOPSIS:\n");
   printf("   help [command]\n");
   printf("DESCRIPTION:\n");
   printf("   Show the list of commands if no command is given.\n");
   printf("   Otherwize show the help of implied command.\n");
   printf("\n");
}
