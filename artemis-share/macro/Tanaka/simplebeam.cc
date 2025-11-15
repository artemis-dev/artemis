{
  double par_x1[2]={-112.5,9.0};
  double par_x2[2]={-112.5,9.0};
  double par_y1[2]={-76.5,9.0};
  double par_y2[2]={-76.5,9.0};
  double par_x3[2]={-112.5,9.0};
  double par_x4[2]={-112.5,9.0};
  double par_v3[2]={-76.5,9.0};
  double par_v4[2]={-76.5,9.0};
  TCut cut_x1 ="mwdc_1_x_raw.fDetectorID >16 &&mwdc_1_x_raw.fDetectorID<25 ";
  TCut cut_x2 ="mwdc_2_x_raw.fDetectorID >16 &&mwdc_2_x_raw.fDetectorID<25 ";
  TCut cut_y1 ="mwdc_1_y_raw.fDetectorID >12 &&mwdc_1_y_raw.fDetectorID<17 ";
  TCut cut_y2 ="mwdc_2_y_raw.fDetectorID >12 &&mwdc_2_y_raw.fDetectorID<17 ";
  TCut cut_x3 ="mwdc_3_x_raw.fDetectorID >0 &&mwdc_3_x_raw.fDetectorID<25 ";
  TCut cut_x4 ="mwdc_4_x_raw.fDetectorID >0 &&mwdc_4_x_raw.fDetectorID<25 ";
  TCut cut_v3 ="mwdc_3_v_raw.fDetectorID >0 &&mwdc_3_v_raw.fDetectorID<17 ";
  TCut cut_v4 ="mwdc_4_v_raw.fDetectorID >0 &&mwdc_4_v_raw.fDetectorID<17 ";
  //
  TCanvas *c21 = new TCanvas("c21","very simple beam profile",10,10,1000,600);
  c21->Divide(2,2);
  c21->cd(1);
  test->Draw(Form("%f+%f*((mwdc_1_y_raw.fDetectorID)):%f+%f*((mwdc_1_x_raw.fDetectorID))>>1x1y(40,-100,100,30,-75,75)",par_y1[0],par_y1[1],par_x1[0],par_x1[1]),cut_x1&&cut_y1,"colz");
  c21->cd(2);
  test->Draw(Form("%f+%f*((mwdc_2_y_raw.fDetectorID)):%f+%f*((mwdc_2_x_raw.fDetectorID))>>2x2y(22,-99,99,18,-81,81)",par_y2[0],par_y2[1],par_x2[0],par_x2[1]),cut_x2&&cut_y2,"colz");
  c21->cd(3);
  test->Draw(Form("(%f+%f*((mwdc_3_v_raw.fDetectorID)))*%f-(%f+%f*((mwdc_3_x_raw.fDetectorID))):%f+%f*((mwdc_3_x_raw.fDetectorID))>>3x3v(40,-100,100,30,-75,75)",par_v3[0],par_v3[1],sqrt(2.0),par_x3[0],par_x3[1],par_x3[0],par_x3[1]),cut_x3 && cut_v3 && cut_x2 && cut_y2 ,"col");
  c21->cd(4);
  test->Draw(Form("%f+%f*((mwdc_4_v_raw.fDetectorID)):%f+%f*((mwdc_4_x_raw.fDetectorID))>>4x4v(22,-99,99,18,-81,81)",par_v4[0],par_v4[1],par_x4[0],par_x4[1]),cut_x4&&cut_v4,"colz");
  //
}
