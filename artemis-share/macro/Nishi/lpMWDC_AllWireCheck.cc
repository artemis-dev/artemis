void lpMWDC_AllWireCheck(){
  //zone 4 3;
  TCanvas *c1 = new TCanvas("c1","wire check",10,32,510,332);
  c1->Divide(4,3);
  int position[12]={1,5,9,2,6,10,3,7,11,4,8,12};
  char WireConf[2][3]={{'x','u','y'},{'x','u','v'}};
  //  
  for(int i=0;i<4;i++){         // MWDC i : 1, 2, 3, 4
    for(int j=0;j<3;j++){       // j-th plane 
      c1->cd(position[i*3+j]);
      test->Draw(Form("lpmwdc_%d_%c_raw.fDetectorID>>hist_DetID%d%c",i+1,(WireConf[(int)(i/2.)][j]),i+1,(WireConf[(int)(i/2.)][j])),Form("lpmwdc_%d_%c_raw.fDetectorID",i+1,(WireConf[(int)(i/2.)][j])));
    }
  }
  // TCanvas *c2 = new TCanvas("c2","wire charge check",10,32,700,500);
}

void lpMWDC_AllWireCheck(char* s){
  //zone 4 3;
  TCanvas *c1 = new TCanvas("c1","wire check",10,32,510,332);
  c1->Divide(4,3);
  int position[12]={1,5,9,2,6,10,3,7,11,4,8,12};
  char WireConf[2][3]={{'x','u','y'},{'x','u','v'}};
  //  
  for(int i=0;i<4;i++){         // MWDC i : 1, 2, 3, 4
    for(int j=0;j<3;j++){       // j-th plane 
      c1->cd(position[i*3+j]);
      test->Draw(Form("lpmwdc_%d_%c_raw.%s>>hist%s%d%c(200,450,2000)",i+1,(WireConf[(int)(i/2.)][j]),s,s,i+1,(WireConf[(int)(i/2.)][j])));
    }
  }
  // TCanvas *c2 = new TCanvas("c2","wire charge check",10,32,700,500);
}


