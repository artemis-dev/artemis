#include "TString.h"
double MKhistNaI(){
  const int n = 84;
  //char buff[2][n][256];
  TString s[n][2];
  for(int i=0;i<n;i++){
    //sprintf(buff[0][i],"naiwallH.fCharge>>Charge%d",i);
    //sprintf(buff[1][i],"naiwallH.fCharge>>Charge%d",i);
    s[i][0] = TString::Format("naiwallL.fCharge[]>>Charge%d(410,0.,4100)",i+1);
    s[i][1] = TString::Format("naiwallL.fDetectorID[]==%d",i+1);
    test->Draw(s[i][0].Data(),s[i][1].Data(),"goff");
  }
}
