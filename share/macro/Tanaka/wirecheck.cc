void wirecheck(int mwdc_number, int plane_number){ 
  //
  if((mwdc_number==4 || mwdc_number==3|| mwdc_number==2||mwdc_number==1)&&(plane_number==1||plane_number==2||plane_number==3)){
    //------------
    int kmax, k;
    char WireConf[2][3]={{'x','u','y'},{'x','u','v'}};
    // 
    //  c11 : Charge
    // 
    
    TCanvas *c11 = new TCanvas("c11","wire charge",10,10,1200,640);
    if(plane_number==3){
      c11->Divide(4,4);
      kmax=16;
    }else{
      c11->Divide(6,4);
      kmax=24;
    }
    //--------  
    /*
    for(k=1;k<(kmax+1);k++){         // (mwdc_number)-th MWDC, (plane_number)-th plane, wire k : 1, 2, 3, 4 ... up to 16 or 24
      c11->cd(k);
      test->Draw(Form("lpmwdc_%d_%c_raw.fCharge>>Charge%d%c_%d(500,0,4000)",mwdc_number,(WireConf[(int)((mwdc_number-1)/2.)][plane_number-1]),mwdc_number,(WireConf[(int)((mwdc_number-1)/2.)][plane_number-1]),k),Form("lpmwdc_%d_%c_raw.fDetectorID==%d",mwdc_number,(WireConf[(int)((mwdc_number-1)/2.)][plane_number-1]),k));
    }
    */
    //--------
    // 
    //  c12 : Timing
    //
    
       TCanvas *c12 = new TCanvas("c12","wire check timing",10,10,1200,640);
       if(plane_number==3){
       c12->Divide(4,4);
       kmax=16;
       }else{
       c12->Divide(6,4);
       kmax=24;
       }
       //--------  
       for(k=1;k<(kmax+1);k++){         // (mwdc_number)-th MWDC, (plane_number)-th plane, wire k : 1, 2, 3, 4 ... to 16 or 24
       c12->cd(k);
       test->Draw(Form("lpmwdc_%d_%c_raw.fTiming>>Timing%d%c_%d(500,18000,22000)",mwdc_number,(WireConf[(int)((mwdc_number-1)/2.)][plane_number-1]),mwdc_number,(WireConf[(int)((mwdc_number-1)/2.)][plane_number-1]),k),Form("lpmwdc_%d_%c_raw.fDetectorID==%d",mwdc_number,(WireConf[(int)((mwdc_number-1)/2.)][plane_number-1]),k));
       }
    
    //-------------
  }        //if bun no owari
}

int tanaka_test(int ggg){
  return ggg;
}
