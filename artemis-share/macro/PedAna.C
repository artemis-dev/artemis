// Pedestal check macro for NaI
// usage: .x macro/PedAna.C
#include <iostream>
void PedAna(){

  ifstream inf1;
  Int_t n=84, ID, chL, chH, dummy;
  char ofile[200],ifile[200];
  Double_t meanL, sigmaL, meanH, sigmaH, threesigmaL, threesigmaH;

  sprintf(ifile,"%s","prm/adcch_naiid.txt");
  sprintf(ofile,"%s","prm/pedestal.dat");
  inf1.open(ifile);
  std::fstream oFstrm(ofile,std::ios::out);

  test->Draw("naiwallL.fCharge[]:naiwallL.fDetectorID[]>>pedL(84,0.5,84.5,300,0.,300.)");
  test->Draw("naiwallH.fCharge[]:naiwallH.fDetectorID[]>>pedH(84,0.5,84.5,300,0.,300.)");
  pedL->FitSlicesY();
  pedH->FitSlicesY();

  for(Int_t i=1; i<=n; i++){
    inf1 >> ID >> chL >> chH;
    meanL=pedL_1->GetBinContent(i);
    sigmaL=pedL_2->GetBinContent(i);
    meanH=pedH_1->GetBinContent(i);
    sigmaH=pedH_2->GetBinContent(i);

    threesigmaL = meanL + 3 * sigmaL;
    threesigmaH = meanH + 3 * sigmaH;

    oFstrm << ID << "   " << chL << "   " << chH << "   " ; 
    oFstrm <<  meanL << "   " << sigmaL << "   ";
    oFstrm <<  meanH << "   " << sigmaH << "   ";
    oFstrm<< threesigmaL << "   " << threesigmaH << std::endl;
  }

  inf1.close();
  oFstrm.close();

}
  
