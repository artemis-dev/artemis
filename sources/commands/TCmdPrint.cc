/**
 * @file   TCmdPrint.cc
 * @brief  print the last-saved figure
 *
 * @date   Created       : 2017-06-13 16:38:43 JST
 *         Last Modified : 2019-11-15 20:45:33 JST (ota)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2017 KAWASE Shoichiro
 */

#include "TCmdPrint.h"
#include "TCmdSave.h"

#include <TSystem.h>
#include <TRegexp.h>

using art::TCmdPrint;

ClassImp(TCmdPrint)

TCmdPrint::TCmdPrint()
: fPrinter("")
{
  SetName("print");
  SetTitle("print the last-saved figure");
}

TCmdPrint::~TCmdPrint()
{
}

namespace {
  Bool_t IsClean(const TString &str)
  {
    const TRegexp &re = TRegexp("[ ~*?!;^$#()%&.|<>(){}[]`\"'\x0A\xFF\\\\]");
    return (str.Index(re) == -1);
  }
}

void TCmdPrint::SetPrinter(const char* printer)
{
  const TString str(printer);
  if(!IsClean(str)) {
    Error("SetPrinter","invalid printer name: %s", printer);
    return;
  }
  fPrinter = printer;
}

void TCmdPrint::SetOption(const char* option)
{
   fOption = option;
}

Long_t TCmdPrint::Cmd(vector<TString>)
{
  return Run();
}

Long_t TCmdPrint::Run()
{
  const TString &filename = TCmdSave::Instance()->GetPrintFileName();
  if(filename.IsNull()) {
    Error("Run","There is no file to be printed.");
    return 1;
  }
  TString cmd;
  if (fPrintCmd.IsNull()) {
     cmd = TString::Format("lpr %s %s %s",
                          fPrinter.IsNull() ? ""
                          : TString::Format("-P%s ",fPrinter.Data()).Data(),
                          fOption.Data(),
                          filename.Data());
  } else {
     cmd = TString::Format(fPrintCmd,filename.Data());
  }
  
  
  Info("Run","%s",cmd.Data());
  gSystem->Exec(cmd.Data());

  return 1;
}
