{ 
  //
  int position[12]={1,5,9,2,6,10,3,7,11,4,8,12};
  char WireConf[2][3]={{'x','u','y'},{'x','u','v'}};
  // 
  //  c1 : DetID
  // 
  TCanvas *c1 = new TCanvas("c1","wire check",200,10,800,600);
  c1->Divide(4,3);
  //--------  
  for(int i=0;i<4;i++){         // MWDC i : 1, 2, 3, 4
    for(int j=0;j<3;j++){       // j-th plane 
      c1->cd(position[i*3+j]);
      test->Draw(Form("lpmwdc_%d_%c_raw.fDetectorID>>DetID%d%c",i+1,(WireConf[(int)(i/2.)][j]),i+1,(WireConf[(int)(i/2.)][j])));
    }
  }
  
  //--------- 
  //
  //  c2 : charge 
  //
  TCanvas *c2 = new TCanvas("c2","wire charge check",100,10,800,600);
  c2->Divide(4,3);
  //--------
  for(int i=0;i<4;i++){         // MWDC i : 1, 2, 3, 4
    for(int j=0;j<3;j++){       // j-th plane 
      c2->cd(position[i*3+j]);
      test->Draw(Form("lpmwdc_%d_%c_raw.fCharge>>Charge%d%c(500,0,4000)",i+1,(WireConf[(int)(i/2.)][j]),i+1,(WireConf[(int)(i/2.)][j])));
    }
  }
 //--------- 
  //
  //  c3 : timing 
  //
  TCanvas *c3 = new TCanvas("c3","wire timing check",100,10,800,600);
  c3->Divide(4,3);
  //--------
  for(int i=0;i<4;i++){         // MWDC i : 1, 2, 3, 4
    for(int j=0;j<3;j++){       // j-th plane 
      c3->cd(position[i*3+j]);
      test->Draw(Form("lpmwdc_%d_%c_raw.fTiming>>Timing%d%c(500,18000,22000)",i+1,(WireConf[(int)(i/2.)][j]),i+1,(WireConf[(int)(i/2.)][j])));
    }
  }
  
 //--------- 
  //
  //  c4 : charge vs DetID 
  //
  TCanvas *c4 = new TCanvas("c4","wire charge vs DetID ",100,10,800,600);
  c4->Divide(4,3);
  //--------
  for(int i=0;i<4;i++){         // MWDC i : 1, 2, 3, 4
    for(int j=0;j<3;j++){       // j-th plane 
      c4->cd(position[i*3+j]);
      test->Draw(Form("lpmwdc_%d_%c_raw.fCharge:lpmwdc_%d_%c_raw.fDetectorID>>DetIDCharge%d%c(48,1,25,500,0,4000)",i+1,(WireConf[(int)(i/2.)][j]),i+1,(WireConf[(int)(i/2.)][j]),i+1,(WireConf[(int)(i/2.)][j])),"","col");
    }
  }
  /*

  */
}
