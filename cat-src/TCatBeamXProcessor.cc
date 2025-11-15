/* $Id:$ */
/**
 * @file   TCatBeamXProcessor.cc
 * @date   Created : May 13, 2015 16:13:24 JST
 *   Last Modified : 2015-05-14 11:02:06 JST by tokieda
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2015
 */
#include "TCatBeamXProcessor.h"
#include <TCatPulseShape.h>
#include <TClonesArray.h>
#include <TMath.h>

using art::TCatBeamXProcessor;

ClassImp(TCatBeamXProcessor)

TCatBeamXProcessor::TCatBeamXProcessor()
: fOutputP0(NULL), fOutputP1(NULL)
{
  StringVec_t def;
  def.push_back("atroughbeam");
  RegisterInputCollection("InputCollection","pulse data (TCatPulseShape)",
			  fInputColName, def);
  RegisterOutputCollection("OutputP0Collection","output beam p0 data (TSimpleData)",
			   fOutputP0ColName,TString("beamxzp0"));
  RegisterOutputCollection("OutputP1Collection","output beam p1 data (TSimpleData)",
			   fOutputP1ColName,TString("beamxzp1"));
  RegisterProcessorParameter("LogFilePath","Path of Log File",fLogFilePath,TString("../log"));
  RegisterProcessorParameter("LogFileName","Name of Log File",fLogFileName,TString("beamxproc.log"));
  RegisterProcessorParameter("DebugMode","Flag For Debug",fDebug,false);
  RegisterProcessorParameter("DetailDebugMode","Flag For Detail Debug",fDetailDebug,false);
  RegisterInputInfo("RunHeadersName","run headers",fNameRunHeaders,TString("runheader"),&fRunHeaders,"TList","TList");
}
TCatBeamXProcessor::~TCatBeamXProcessor()
{
  delete fOutputP0;
  delete fOutputP1;
}

TCatBeamXProcessor::TCatBeamXProcessor(const TCatBeamXProcessor& rhs)
{
  MayNotUse("TCatBeamXProcessor");
}

TCatBeamXProcessor& TCatBeamXProcessor::operator=(const TCatBeamXProcessor& rhs)
{
  MayNotUse("operator=");
  if (this != &rhs) {

  }
  return *this;
}

void TCatBeamXProcessor::Init(TEventCollection *col)
{
  TEventObject *obj = NULL;
  if (!(obj = col->Get(fInputColName.front()))) {
    Info("Init","No such input %s",fInputColName.front().Data());
    return;
  }
  // Prepare Log/Fig File
  if (fDebug || fDetailDebug) {
     const Char_t *LogNamePath = Form("%s/%s",fLogFilePath.Data(),fLogFileName.Data());
     fp = fopen(LogNamePath,"w");
     if (fp == NULL) {
        Error("Init","Cannot open %s/%s",fLogFilePath.Data(),fLogFileName.Data());
        exit(1);
     }
  }
  fInput = (TClonesArray**) obj->GetObjectRef();
  fOutputP0 = new TSimpleData();
  col->Add(fOutputP0ColName,fOutputP0,fOutputIsTransparent);
  fOutputP1 = new TSimpleData();
  col->Add(fOutputP1ColName,fOutputP1,fOutputIsTransparent);
  fEvt = 0;
}

void TCatBeamXProcessor::Process()
{
   if ((*fRunHeaders)->Last()) {
      fRunInfo   = dynamic_cast<TRunInfo*>((*fRunHeaders)->Last());
      fRunName   = fRunInfo->GetRunName();
      fRunNumber = fRunInfo->GetRunNumber();
   } else {
      printf("Failed to call runname,number from runinfo\n");
   }
   TClonesArray *input = *fInput;
   Int_t nHits = (input)->GetEntriesFast();

   TCatPulseShape::SetSortType(TCatPulseShape::kTiming);
   TCatPulseShape::SetSortOrder(TCatPulseShape::kASC);
   (input)->Sort();
   (input)->Compress();
  
   if (nHits) {
      if (fDebug) fprintf(fp,"-----------\n*Run: %s%04d, Evt = %d, nHits = %3d\n",fRunName.Data(),fRunNumber,fEvt,nHits);

      TObjArray beam_array;
      Int_t cls_i = 0;
      Int_t nBeamHitsRow[20] = {};
      for (Int_t iHit = 0; iHit != nHits; iHit++) {
         TCatPulseShape *hit = (TCatPulseShape*) (input)->At(iHit);
         //Int_t cls_i = hit->GetAuxID();
         Int_t id = hit->GetID();
         //Float_t off = hit->GetOffset();
         //Double_t charge = hit->GetCharge();
         Int_t col = id / 10;
         Int_t row = id % 10;
         if ((id > 69 && id < 330) && (col%2 == 0)) {
            col = col - 1;
            row = row + 10;
         }
         beam_array.Add(hit);
         nBeamHitsRow[row]++;
      }

      // Derive Linear Equation of Beam (x,z)
      if (fDebug) fprintf(fp,"// Derive Linear Equation of Beam\n");
      if (fDebug) fprintf(fp,"// nBeamHitsRow: 2 -> charge division, >2 -> center of charge\n");
      Double_t x_b[20],z_b[20],q_b[20];
      Double_t sqx[20];
      for (Int_t i=0; i<20; i++) {
         x_b[i] = -1000.;
         z_b[i] = -1000.;
         q_b[i] = 0.;
         sqx[i] = 0.;
      }
      Double_t Q_b = 0.;
      Double_t Sx  = 0.;
      Double_t Sz  = 0.;
      Double_t Sxx = 0.;
      Double_t Sxz = 0.;
      Double_t a_bz  = 0.;
      Double_t b_bz  = 0.;
      Int_t    n   = 0;
      Int_t    first_col = -1;
      const Double_t gap = 0.15 / TMath::Sqrt(3) * 2;

      TCatPulseShape::SetSortType(TCatPulseShape::kID);
      TCatPulseShape::SetSortOrder(TCatPulseShape::kASC);
      beam_array.Sort();    
      Int_t nBeamHits = beam_array.GetEntriesFast();
      if (fDebug) fprintf(fp,"cls_ i = %d, nBeamHits = %d\n",cls_i,nBeamHits);
      for (Int_t iHit=0; iHit<nBeamHits; iHit++) {
         TCatPulseShape *hit = (TCatPulseShape*) beam_array[iHit];
         Int_t id = hit->GetID();
         Int_t col = id / 10;
         Int_t row = id % 10;
         if ((id > 69 && id < 330) && (col%2 == 0)) {
            col = col - 1;
            row = row + 10;
         }
         Double_t x = hit->GetX();
         Double_t z = hit->GetZ();
         Double_t q = hit->GetCharge();

      
         if (fDebug) fprintf(fp,"cls_i = %2d, id = %3d, row = %2d, nBeamHitsRow[%2d] = %2d\n",cls_i,id,row,row,nBeamHitsRow[row]);
         if (nBeamHitsRow[row]==2) { // Charge Division
            if (x_b[row]==-1000.) {
               first_col = col;
               x_b[row] = x;
               z_b[row] = z;
               q_b[row] = q;
            } else {
               if ((first_col>=6) && (first_col<=33)) { // 5 mm pad
                  Double_t z_bh = (5./2 * TMath::Sqrt(3) / 3) / 2;
                  Double_t x_bh = (5./2 + gap) / 2;
                  if ((first_col%4==3) || (first_col==6) || (first_col==33)) { // first_col = 6,7,11,15,19,23,27,31,33
                     if (row%2==0) { /* 1\//\2 */
                        if (fDebug) fprintf(fp,"/* first_col : %2d,row :even, 1\\//\\2 */\n",first_col);
                        x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
                        z_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * z_bh + (z_b[row] - z_bh);
                     } else if (row%2==1) { /* 1/\\/2 */
                        if (fDebug) fprintf(fp,"/* first_col : %2d,row : odd, 1/\\\\/2 */\n",first_col);
                        x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
                        z_b[row] = ((q - q_b[row]) / (q + q_b[row])) * z_bh + (z - z_bh);
                     }
                  } else if ((first_col%4==1) && (first_col!=33)) { // first_col = 9,13,17,21,25,29
                     if (row%2==0) { /* 1/\\/2 */
                        if (fDebug) fprintf(fp,"/* first_col : %2d,row :even, 1/\\\\/2 */\n",first_col);
                        x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
                        z_b[row] = ((q - q_b[row]) / (q + q_b[row])) * z_bh + (z - z_bh);
                     } else if (row%2==1) { /* 1\//\2 */
                        if (fDebug) fprintf(fp,"/* first_col : %2d,row : odd, 1\\//\\2 */\n",first_col);
                        x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
                        z_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * z_bh + (z_b[row] - z_bh);
                     }
                  }
               } else if ((first_col<=5) || (first_col>=34)) { // 10 mm pad
                  Double_t z_bh = (10./2 * TMath::Sqrt(3) / 3) / 2;
                  Double_t x_bh = (10./2 + gap) / 2;
                  if ((first_col<=5) && (first_col%2==0)) { // first_col = 0,2,4
                     if (row%2==0) { /* 1\//\2 */
                        if (fDebug) fprintf(fp,"/* first_col :even,row :even, 1\\//\\2 */\n");
                        x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
                        z_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * z_bh + (z_b[row] - z_bh);
                     } else if (row%2==1) { /* 1/\\/2 */
                        if (fDebug) fprintf(fp,"/* first_col :even,row : odd, 1/\\\\/2 */\n");
                        x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
                        z_b[row] = ((q - q_b[row]) / (q + q_b[row])) * z_bh + (z - z_bh);
                     }
                  } else if ((first_col<=5) && (first_col%2==1)) { // first_col = 1,3,5
                     if (row%2==0) { /* 1/\\/2 */
                        if (fDebug) fprintf(fp,"/* first_col : odd,row :even, 1/\\\\/2 */\n");
                        x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
                        z_b[row] = ((q - q_b[row]) / (q + q_b[row])) * z_bh + (z - z_bh);
                     } else if (row%2==1) { /* 1\//\2 */
                        if (fDebug) fprintf(fp,"/* first_col : odd,row : odd, 1\\//\\2 */\n");
                        x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
                        z_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * z_bh + (z_b[row] - z_bh);
                     }
                  } else if ((first_col>=34) && (first_col%2==1)) { // first_col = 35,37,39
                     if (row%2==0) { /* 1\//\2 */
                        if (fDebug) fprintf(fp,"/* first_col :even,row :even, 1\\//\\2 */\n");
                        x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
                        z_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * z_bh + (z_b[row] - z_bh);
                     } else if (row%2==1) { /* 1/\\/2 */
                        if (fDebug) fprintf(fp,"/* first_col :even,row : odd, 1/\\\\/2 */\n");
                        x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
                        z_b[row] = ((q - q_b[row]) / (q + q_b[row])) * z_bh + (z - z_bh);
                     }
                  } else if ((first_col>=34) && (first_col%2==0)) { // first_col = 34,36,38
                     if (row%2==0) { /* 1/\\/2 */
                        if (fDebug) fprintf(fp,"/* first_col : odd,row :even, 1/\\\\/2 */\n");
                        x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
                        z_b[row] = ((q - q_b[row]) / (q + q_b[row])) * z_bh + (z - z_bh);
                     } else if (row%2==1) { /* 1\//\2 */
                        if (fDebug) fprintf(fp,"/* first_col : odd,row : odd, 1\\//\\2 */\n");
                        x_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * x_bh + (x - x_bh);
                        z_b[row] = ((q_b[row] - q) / (q_b[row] + q)) * z_bh + (z_b[row] - z_bh);
                     }
                  }
               }
               q_b[row] += q;
            }

            if (fDetailDebug) fprintf(fp,"x_b[%2d] = %5.1f, z_b[%2d] = %5.1f, q_b[%2d] = %5.1f, x = %5.1f, z = %5.1f, q = %5.1f\n",row,x_b[row],row,z_b[row],row,q_b[row],x,z,q);

         } else if (nBeamHitsRow[row]>=3) { // Center of Charge
            sqx[row] += q * x;
            z_b[row] =  z; 
            q_b[row] += q;

            if (fDetailDebug) fprintf(fp,"x_b[%2d] = %5.1f, z_b[%2d] = %5.1f, q_b[%2d] = %5.1f, x = %5.1f, z = %5.1f, q = %5.1f, sqx[%2d] = %5.1f\n",row,x_b[row],row,z_b[row],row,q_b[row],x,z,q,row,sqx[row]);

         } else if (nBeamHitsRow[row]==1) {
            x_b[row] = x;
            z_b[row] = z;
            q_b[row] = q;

            if (fDetailDebug) fprintf(fp,"x_b[%2d] = %5.1f, z_b[%2d] = %5.1f, q_b[%2d] = %5.1f, x = %5.1f, z = %5.1f, q = %5.1f\n",row,x_b[row],row,z_b[row],row,q_b[row],x,z,q);

         }
      }

      if (fDebug) fprintf(fp,"\n* Only row = 0,1,18,19 join Beam fitting\n");      
      for (Int_t iRow=0; iRow<20; iRow++) {
         if (nBeamHitsRow[iRow]>=3) x_b[iRow] = sqx[iRow] / q_b[iRow];
         if ((x_b[iRow]==-1000.) || (nBeamHitsRow[iRow]<=1)) continue;
         if (!((iRow==0) || (iRow==1) || (iRow==18) || (iRow==19))) continue;
         n++;
         Sx += x_b[iRow];
         Sz += z_b[iRow];
         Sxx += TMath::Power(x_b[iRow],2);
         Sxz += x_b[iRow] * z_b[iRow];
         Q_b += q_b[iRow];
      
         if (fDetailDebug) fprintf(fp,"x_b[%2d] = %5.1f, z_b[%2d] = %5.1f, q_b[%2d] = %5.1f\n",iRow,x_b[iRow],iRow,z_b[iRow],iRow,q_b[iRow]);
      }

      if (n<=2) {
         if (fDebug) fprintf(fp,"Skip: n <= 2\n");      
         fEvt++;
         return;
      }

      if (fDebug) fprintf(fp,"Beam: Least Square Method selected.\n");
      Double_t d   = 0;
      Double_t X_b = 0;
   
      d = n * Sxx - TMath::Power(Sx,2);
      if (d!=0) {
         a_bz= (n * Sxz - Sx * Sz) / d;
         b_bz= (Sxx * Sz - Sxz * Sx) / d;
         X_b = (z_b[0] - b_bz) / a_bz;
      } else {
         a_bz= b_bz= 0;
         X_b = x_b[0];
      }

      if (fDebug) fprintf(fp,"x_b[0] = %5.1f, Q_b = %5.1f, a_bz= %5.1f, b_bz= %5.1f\n",x_b[0],Q_b,a_bz,b_bz);

      if (TMath::Abs(a_bz) < 4.) {
         if (fDebug) fprintf(fp,"Skip: |a_bz| < 4.\n");
         fEvt++;
         return;
      }
   
      //printf("b_bz = %f, a_bz = %f\n",b_bz,a_bz);

      // Save
      fOutputP0->SetValue(b_bz);
      fOutputP1->SetValue(a_bz);
      fOutputP0->Validate();
      fOutputP1->Validate();
      
      //printf("p0 = %f, p1 = %f\n",fOutputP0->GetValue(),fOutputP1->GetValue());
      if (fDebug) fprintf(fp,"p1 = a_bz = %5.1f, p0 = b_bz = %5.1f\n",fOutputP1->GetValue(),fOutputP0->GetValue());
   }
   fEvt++;
}
