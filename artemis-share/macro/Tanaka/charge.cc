{ 
  // 
  //  c31 : charge check
  // 
  /*
  TCanvas *c31 = new TCanvas("c31","charge check",10,10,1000,800);
  c31->Divide(2,2);
  c31->cd(1);
  test->Draw("lpmwdc_1_x_raw.fCharge>>Charge1x(1000,0,4000)");
  c31->cd(2);
  test->Draw("lpmwdc_1_u_raw.fCharge>>Charge1u(1000,0,4000)");
  c31->cd(3);
  test->Draw("lpmwdc_1_y_raw.fCharge>>Charge1y(1000,0,4000)");
  c31->cd(4);
  test->Draw("lpmwdc_2_x_raw.fCharge>>Charge2x(1000,0,4000)");
  //
  //----------------------
  // 
  //  c32 : charge correlation
  //
  TCanvas *c32 = new TCanvas("c32","charge correlation check",10,10,1000,800);
  c32->Divide(2,2);
  c32->cd(1);
  // test->Draw("lpmwdc_1_x_raw.fCharge:lpmwdc_1_x_raw.fTiming>>Charge1xTiming1x(500,19000,21500,1000,0,2000)","","colz");
  c32->cd(2);
  test->Draw("lpmwdc_1_x_raw.fCharge:lpmwdc_1_u_raw.fCharge>>Charge1x1u(1000,0,2000,1000,0,2000)","","colz");
  c32->cd(3);
  test->Draw("lpmwdc_1_x_raw.fCharge:lpmwdc_1_y_raw.fCharge>>Charge1x1y(1000,0,2000,1000,0,2000)","","colz");
  c32->cd(4);
  test->Draw("lpmwdc_1_x_raw.fCharge:lpmwdc_2_x_raw.fCharge>>Charge1x2x(1000,0,2000,1000,0,2000)","","colz");
  //---------------------
  // 
  //  c33 : timing check
  // 
  TCanvas *c33 = new TCanvas("c33","timing check",10,10,1000,800);
  c33->Divide(2,2);
  c33->cd(1);
  test->Draw("lpmwdc_1_x_raw.fTiming>>Timing1x(500,19000,21500)");
  c33->cd(2);
  test->Draw("lpmwdc_1_u_raw.fTiming>>Timing1u(500,19000,21500)");
  c33->cd(3);
  test->Draw("lpmwdc_1_y_raw.fTiming>>Timing1y(500,19000,21500)");
  c33->cd(4);
  test->Draw("lpmwdc_2_x_raw.fTiming>>Timing2x(500,19000,21500)");
  //
  //----------------------
  // 
  //  c34 : timing correlation
  //
  */
  TCanvas *c34 = new TCanvas("c34","timing correlation check",10,10,1000,800);
  c34->Divide(2,2);
  c34->cd(1);
  test->Draw("lpmwdc_1_x_raw.fTiming:lpmwdc_2_x_raw.fTiming>>Timing1x2x(100,19000,21500,100,19000,21500)","","colz");
  c34->cd(2);
  test->Draw("lpmwdc_3_x_raw.fTiming:lpmwdc_4_x_raw.fTiming>>Timing3x4x(100,19000,21500,100,19000,21500)","","colz");
  c34->cd(3);
  test->Draw("lpmwdc_1_x_raw.fTiming:lpmwdc_3_x_raw.fTiming>>Timing1x3x(100,19000,21500,100,19000,21500)","","colz");
  c34->cd(4);
  test->Draw("lpmwdc_2_x_raw.fTiming:lpmwdc_4_x_raw.fTiming>>Timing2x4x(100,19000,21500,100,19000,21500)","","colz");
  //---------------------
  // 
}
