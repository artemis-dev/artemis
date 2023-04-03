void PedFit(){
  Int_t n;
  Double_t p[3];
  char s[1];
  
  ifstream inf1;
  Int_t n=84, ID, chL, chH, dummy;
  char ifile[200];
  //  char ofileL[200],ofileH[200];
  Double_t meanL, sigmaL, meanH, sigmaH, threesigmaL, threesigmaH;

  sprintf(ifile,"%s","prm/adcch_naiid.txt");
  FILE *fpL;
  if((fpL = fopen("prm/tmppedestalL.dat","w")) ==  NULL){
    fprintf(stderr, "Can not open file\n");
    exit(1);
  }

  //  fpL = fopen("prm/tmppedestalL.dat","w");

  FILE *fpH;
  if((fpH = fopen("prm/tmppedestalH.dat","w")) ==  NULL){
    fprintf(stderr, "Can not open file\n");
    exit(1);
  }
  //  fpH = fopen("prm/tmppedestalH.dat","w");

//  sprintf(ofileL,"%s","prm/tmppedestalL.dat");
//  sprintf(ofileH,"%s","prm/tmppedestalH.dat");

  inf1.open(ifile);

//  std::fstream oFstrmL(ofileL,std::ios::out);
//  std::fstream oFstrmH(ofileH,std::ios::out);

  //  test->Draw("naiwallL.fCharge[]:naiwallL.fDetectorID[]>>pedL(84,0.5,84.5,300,0.,300.)");
  //  test->Draw("naiwallH.fCharge[]:naiwallH.fDetectorID[]>>pedH(84,0.5,84.5,300,0.,300.)");
  //  pedL->FitSlicesY();
  //  pedH->FitSlicesY();

  TCatCmdSly::Instance()->Run(pedL);
  TCatCmdSly::Instance()->Run(pedH);

  TDirectory *slydirL = gDirectory->Get("pedL_sly");
  
  slydirL->Print();
  Int_t nHist = slydirL->GetList()->GetEntries();
  for (Int_t i=0; i!=nHist; i++) {
    inf1 >> ID >> chL >> chH;
    TH1F* h1 = (TH1F*) slydirL->GetList()->At(i);
    h1->Draw();
    h1->Fit("gaus","","",60.,150.);
    TF1* func=h1->GetFunction("gaus");
    func->GetParameters(p);
    meanL = p[1];
    sigmaL = p[2];
    threesigmaL = meanL/2 + 5 * sigmaL;
    printf("%2u  %2u  %2u  %7.3f  %7.3f  %5.0f\n",ID, chL, chH, meanL, sigmaL, threesigmaL);
    fprintf(fpL, "%2u  %2u  %2u  %7.3f  %7.3f  %5.0f\n",ID, chL, chH, meanL, sigmaL, threesigmaL);

//    oFstrmL << ID << "   " << chL << "   " << chH << "   " ; 
//    oFstrmL <<  meanL << "   " << sigmaL << "   ";
//    oFstrmL << threesigmaL << std::endl;
  }

  inf1.close();
  //  oFstrmL.close();
  fclose(fpL);
  inf1.open(ifile);

  TDirectory *slydirH = gDirectory->Get("pedH_sly");
  
  slydirH->Print();
  Int_t nHist = slydirH->GetList()->GetEntries();
  for (Int_t i=0; i!=nHist; i++) {
    inf1 >> ID >> chL >> chH;
    TH1F* h1 = (TH1F*) slydirH->GetList()->At(i);
    h1->Draw();
    h1->Fit("gaus","","",10.,260.);
    TF1* func=h1->GetFunction("gaus");
    func->GetParameters(p);
    meanH = p[1];
    sigmaH = p[2];
    threesigmaH = meanH/2 + 5 * sigmaH;
    //    cout << ID << "   " << chL << "   " << chH << "   " << endl; 
    printf("%2u  %2u  %2u  %7.3f  %7.3f  %5.0f\n",ID, chL, chH, meanH, sigmaH, threesigmaH);
    fprintf(fpH, "%2u  %2u  %2u  %7.3f  %7.3f  %5.0f\n",ID, chL, chH, meanH, sigmaH, threesigmaH);

//    oFstrmH << ID << "   " << chL << "   " << chH << "   " ; 
//    oFstrmH <<  meanH << "   " << sigmaH << "   ";
//    oFstrmH << threesigmaH << std::endl;
  }

  inf1.close();
  fclose(fpH);

  //  oFstrmH.close();
}

