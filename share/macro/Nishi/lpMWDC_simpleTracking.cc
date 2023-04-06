double wire2pos_X(int wire, char conf){
  double spacing = 9.0;
  double angle[4]={0.,30.,-45.,90.};
  switch(s){
    case 'x'
    case 'u'
    case 'v'
    case 'y'
  }
}

int range(double branch_val,double min,double max){
  if(branch_val<=min&&branch_val>max)return 1;
  else return 0;
}

TCut lpMWDCcut(){
  TCut MWDC1x = "mwdc_1_x_raw.fDetectorID>0&&mwdc_1_x_raw.fDetectorID<25";
  TCut MWDC1u = "mwdc_1_u_raw.fDetectorID>0&&mwdc_1_u_raw.fDetectorID<25";
  TCut MWDC1y = "mwdc_1_y_raw.fDetectorID>0&&mwdc_1_y_raw.fDetectorID<25";
  TCut MWDC2x = "mwdc_2_x_raw.fDetectorID>0&&mwdc_2_x_raw.fDetectorID<25";
  TCut MWDC2u = "mwdc_2_u_raw.fDetectorID>0&&mwdc_2_u_raw.fDetectorID<25";
  TCut MWDC2y = "mwdc_2_y_raw.fDetectorID>0&&mwdc_2_y_raw.fDetectorID<25";
  TCut MWDC3x = "mwdc_3_x_raw.fDetectorID>0&&mwdc_3_x_raw.fDetectorID<25";
  TCut MWDC3u = "mwdc_3_u_raw.fDetectorID>0&&mwdc_3_u_raw.fDetectorID<25";
  TCut MWDC3v = "mwdc_3_v_raw.fDetectorID>0&&mwdc_3_v_raw.fDetectorID<25";
  TCut MWDC4x = "mwdc_4_x_raw.fDetectorID>0&&mwdc_4_x_raw.fDetectorID<25";
  TCut MWDC4u = "mwdc_4_u_raw.fDetectorID>0&&mwdc_4_u_raw.fDetectorID<25";
  TCut MWDC4v = "mwdc_4_v_raw.fDetectorID>0&&mwdc_4_v_raw.fDetectorID<25";

  TCut MWDC1 = MWDC1x&&MWDC1u;
  //  MWDC1 += MWDC1y;
  TCut MWDC2 = MWDC2x&&MWDC2u;
  MWDC2 += MWDC2y;
  TCut MWDC3 = MWDC1x&&MWDC1u;
  MWDC1 += MWDC1y;
  TCut MWDC4 = MWDC1x&&MWDC1u;
  MWDC1 += MWDC1y;
  test->Draw("9.0*mwdc_1_x_raw.fDetectorID - 112.5",MWDC1);
  return  MWDC1;
  //  return  MWDC1&&MWDC2&&MWDC3&&MWDC4;

}

double lpMWDC_simpleTracking(){
  TCut MWDC1x = "mwdc_1_x_raw.fDetectorID>0&&mwdc_1_x_raw.fDetectorID<25";
  TCut MWDC1u = "mwdc_1_u_raw.fDetectorID>0&&mwdc_1_u_raw.fDetectorID<25";
  TCut MWDC1y = "mwdc_1_y_raw.fDetectorID>0&&mwdc_1_y_raw.fDetectorID<17";
  TCut MWDC2x = "mwdc_2_x_raw.fDetectorID>0&&mwdc_2_x_raw.fDetectorID<25";
  TCut MWDC2u = "mwdc_2_u_raw.fDetectorID>0&&mwdc_2_u_raw.fDetectorID<25";
  TCut MWDC2y = "mwdc_2_y_raw.fDetectorID>0&&mwdc_2_y_raw.fDetectorID<17";
  TCut MWDC3x = "mwdc_3_x_raw.fDetectorID>0&&mwdc_3_x_raw.fDetectorID<25";
  TCut MWDC3u = "mwdc_3_u_raw.fDetectorID>0&&mwdc_3_u_raw.fDetectorID<25";
  TCut MWDC3v = "mwdc_3_v_raw.fDetectorID>0&&mwdc_3_v_raw.fDetectorID<17";
  TCut MWDC4x = "mwdc_4_x_raw.fDetectorID>0&&mwdc_4_x_raw.fDetectorID<25";
  TCut MWDC4u = "mwdc_4_u_raw.fDetectorID>0&&mwdc_4_u_raw.fDetectorID<25";
  TCut MWDC4v = "mwdc_4_v_raw.fDetectorID>0&&mwdc_4_v_raw.fDetectorID<17";

  TCut MWDC1 = MWDC1x&&MWDC1u;
  MWDC1+=MWDC1y;
  TCut MWDC2 = MWDC2x&&MWDC2u;
  MWDC2+=MWDC2y;
  TCut MWDC3 = MWDC1x&&MWDC1u;
  MWDC3 += MWDC1y;
  TCut MWDC4 = MWDC1x&&MWDC1u;
  MWDC4 += MWDC1y;


  test->Draw("9.0*mwdc_1_x_raw.fDetectorID - 112.5",MWDC1);
}

double wire2pos(int wireID){
  double pos,ang,offset;
  double spacing = 9.0;
  char conf = 'x';
  if(conf == 'x'){
    ang = 0.;
    offset = -112.5;
    pos = wireID*spacing + offset; 
  }
  else if(conf == 'u'){
    ang = 30.;
  }
  else if(conf == 'v'){
    ang = -45.;
  }
  else if(conf == 'y'){
    ang = 90.;
  }
  else {
    printf("conf should in x,u,v,y\n");
    break;
  }
  return pos;
}
