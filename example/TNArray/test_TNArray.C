#include "TNArray.h"
#include "TF2.h"
#include "TMath.h"
#include "TRandom.h"
#include "TNtupleD.h"
void test_TNArray() {
   art::TNArray *array = new art::TNArray("table","gaus2d","a:r:value");
   array->Add("angle",0.,90.,181);
   array->Add("radius",0.,10,501);
   double tstep = array->GetVar(0).GetStep();
   double rstep = array->GetVar(1).GetStep();
   
   for (int it = 0, nt = array->GetVar(0).GetNumVals(); it < nt; ++it) {
      for (int ir = 0, nr = array->GetVar(1).GetNumVals(); ir < nr; ++ir) {
         double r = ir * rstep;
         double adeg = it * tstep;
         double a = adeg * TMath::DegToRad();
         double x = r * TMath::Cos(a);
         double y = r * TMath::Sin(a);
         array->Fill(adeg,r, TMath::Gaus(x,0.,1.,1)*TMath::Gaus(y,0.,1.,1) );
      }
   }
   array->Load();

   TNtupleD *tuple = new TNtupleD("tuple","tuple","x:y:r:a:tval:tval2:ival:diff:diff2");
   
   for (int ix = 0; ix < 100; ++ix) {
      for (int iy = 0; iy < 100; ++iy) {
         double x = gRandom->Uniform(0.,5.);
         double y = gRandom->Uniform(0.,5.);
         double r = TMath::Sqrt(x*x + y*y);
         double a = TMath::ATan(y/x) * TMath::RadToDeg();
         double xval [] = {a,r};
         double tval = array->Eval(xval);
         double tval2 = array->Eval2(xval);
         double ival = TMath::Gaus(x,0.,1.,1)*TMath::Gaus(y,0.,1.,1);
         tuple->Fill(x,y,r,a,tval,tval2,ival,tval-ival,tval2-ival);
      }
   }
}
