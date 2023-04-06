/**
 * @file   TCmdMWDCCalib.h
 * @brief
 *
 * @date   Created:       2014-01-16 14:19:54
 *         Last Modified: 2014-07-02 13:14:38 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro All rights reserved
 */

#ifndef TCMDMWDCCALIB_H
#define TCMDMWDCCALIB_H

#include <TCatCmd.h>

namespace art {
   class TCmdMWDCCalib;
}

class art::TCmdMWDCCalib : public TCatCmd {
public:
   virtual ~TCmdMWDCCalib();

   virtual Long_t Cmd(vector<TString>);
   virtual Long_t Run();

   static TCmdMWDCCalib* Instance();

protected:
   TCmdMWDCCalib();
private:

   TCmdMWDCCalib(const TCmdMWDCCalib&);
   TCmdMWDCCalib& operator=(const TCmdMWDCCalib&);

   ClassDef(TCmdMWDCCalib,0)
};

#endif // TCMDMWDCCALIB_H
