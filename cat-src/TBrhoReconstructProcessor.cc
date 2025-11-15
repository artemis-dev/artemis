/**
 * @file   TBrhoReconstructProcessor.cc
 * @brief  reconstruct Brho35, Brho57 using F3, F5, F7 information
 *
 * @date   Created       : 2015-04-15 15:31:28 JST
 *         Last Modified : Jun 07, 2015 06:37:49 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 Shinsuke OTA
 */

#include "TBrhoReconstructProcessor.h"
#include <TClonesArray.h>
#include <TMWDCTrackingResult.h>
#include <TSimpleData.h>
#include <TMath.h>
#include <TArtSystemOfUnit.h>


using art::TBrhoReconstructProcessor;
using namespace TArtSystemOfUnit;

ClassImp(TBrhoReconstructProcessor)

TBrhoReconstructProcessor::TBrhoReconstructProcessor()
{
   RegisterInputCollection("InputName1","name of tracking result at entrance focus",
                           fInputName1,TString("f3"),
                           &fInput1,TClonesArray::Class_Name(),TMWDCTrackingResult::Class_Name());
   RegisterInputCollection("InputName2","name of tracking result at exit focus",
                           fInputName2,TString("f5"),
                           &fInput2,TClonesArray::Class_Name(),TMWDCTrackingResult::Class_Name());

   RegisterOutputCollection("OutputName","name of output collection",
                            fOutputName,TString("brho35"),
                            &fOutput,TClonesArray::Class_Name(),TSimpleData::Class_Name());

   RegisterProcessorParameter("Brho0","brho of central ray",
                              fBrho0,(Float_t)0.);

   RegisterProcessorParameter("Z","z position of focus of exit (mode 1) or entrance (mode 2)",
                              fZ,(Float_t)0.);

   RegisterProcessorParameter("Mode","0: both tracks on focus, 1: only entrance track on focus, 2: only exit track on focus",
                              fMode,(Int_t)0);

   RegisterProcessorParameter("Section","section 35: F3-F5, 57: F5-F7",
                              fSection,(Int_t)35);

   
}

TBrhoReconstructProcessor::~TBrhoReconstructProcessor()
{
}

TBrhoReconstructProcessor::TBrhoReconstructProcessor(const TBrhoReconstructProcessor& rhs)
{
}

TBrhoReconstructProcessor& TBrhoReconstructProcessor::operator=(const TBrhoReconstructProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}
/*
  (x|x)         (a|x)         (y|x)         (b|x)         (l|x)
  (x|a)         (a|a)         (y|a)         (b|a)         (l|a)
  (x|y)         (a|y)         (y|y)         (b|y)         (l|y)
  (x|b)         (a|b)         (y|b)         (b|b)         (l|b)
  (x|l)         (a|l)         (y|l)         (b|l)         (l|l)
  (x|d)         (a|d)         (y|d)         (b|d)         (l|d)

  (F3-F5) from http://ribf.riken.jp/BigRIPSInfo/optics/fig/matrix_1st_7Tm.txt
  0.926591E+00 -0.196513E-01  0.            0.           -0.361262E-01  100000
 -0.471245E-02  0.107932E+01  0.            0.            0.194078E+01  010000
  0.            0.            0.103406E+01  0.291468E+00  0.            001000
  0.            0.           -0.222120E-01  0.960798E+00  0.            000100
  0.            0.            0.            0.            0.100000E+01  000010
  0.316690E+02  0.150266E-01  0.            0.            0.785482E+02  000001

  (F5-F7) from http://ribf.riken.jp/BigRIPSInfo/optics/fig/matrix_1st_7Tm.txt
  0.108043E+01 -0.182343E-01  0.            0.           -0.476105E-02  100000
  0.226346E-01  0.925174E+00  0.            0.           -0.179602E+01  010000
  0.            0.            0.962937E+00  0.294048E+00  0.            001000
  0.            0.           -0.269719E-01  0.103025E+01  0.            000100
  0.            0.            0.            0.            0.100000E+01  000010
 -0.341741E+02  0.654360E+00  0.            0.            0.785442E+02  000001  
 */
void TBrhoReconstructProcessor::Init(TEventCollection* )
{
   fMatrix = new TMatrixD(6,6);
   TMatrixD &m = *fMatrix;
   if (fSection == 35) {
      m[0][0] = 0.926591; // xx
      m[0][1] = -0.00471245 * mm / mrad; // xa
      m[1][0] = -0.0196513 * mrad / mm; // ax
      m[1][1] = 1.07932; // aa
      m[0][5] = 31.6690; // xd
      m[1][5] = 0.015266; // ad
   } else if (fSection == 57) {
      m[0][0] = 1.08043; // xx
      m[0][1] = 0.0226346 * mm / mrad; // xa
      m[1][0] = -0.0182343 * mrad / mm; // ax
      m[1][1] = 0.925174; // aa
      m[0][5] = -34.1741; // xd
      m[1][5] = 0.654360; // ad
   } else {
      SetStateError(TString::Format("Unknown section %d\n",fSection));
      return;
   }

}

void TBrhoReconstructProcessor::Process()
{
   TMWDCTrackingResult *track1 = ((TMWDCTrackingResult*) (*fInput1)->At(0));
   TMWDCTrackingResult *track2 = ((TMWDCTrackingResult*) (*fInput2)->At(0));

   fOutput->Clear("C");
   if (track1 && track2) {
      // obtain brho35
      Double_t x1 = track1->GetTrack()->X();
      Double_t a1 = track1->GetTrack()->A();
      Double_t z1 = track1->GetTrack()->Z();
      Double_t l1 = fZ - z1;
      Double_t x2 = track2->GetTrack()->X();
      Double_t a2 = track2->GetTrack()->A();
      Double_t z2 = track2->GetTrack()->Z();
      Double_t l2 = fZ - z2;
      TMatrixD &m = *fMatrix;
      Double_t d, a;
      // 0:X 1:A 2:Y 3:B 4:L 5:D
      switch (fMode) {
      case 0: // both tracks on focus
         d = ( x2 - m[0][0]*x1 - m[0][1]*a1) / m[0][5];
//         printf("d = %f, x1 = %f, x2 = %f, a1 = %f, m[0][0] = %f, m[0][1] = %f, m[0][5] = %f\n",
//                d,x1,x2,a1,m[0][0],m[0][1],m[0][5]);
         a = a2;
         break;
      case 1:
         d = ( x2 - (m[0][0] - m[1][0]*l1) * x1 + (m[0][1] - l1*m[1][1]) * a1) / (m[0][5] - l1 * m[1][5]);
         break;
      case 2:
         d = (( x2 - m[0][0] * x1) * (m[1][0]*l2+m[1][1]) - (a2 - m[1][0]*x1)*(m[0][0]*l2 + m[0][1])) 
            / ((m[1][0]*l2+m[1][1])*m[0][5] - (m[1][0]*l2 + m[1][1])*m[1][5]);
         break;
      default:
         printf("unknown mode\n");
         return;
         break;
      }

      TSimpleData *data = (TSimpleData*)fOutput->ConstructedAt(fOutput->GetEntriesFast());
      data->SetValue(fBrho0 * (1+d/100.));
   }

   return;

      
}
