/**
 * @file   TNArray.h
 * @brief  n-dimension array
 *
 * @date   Created       : 2016-01-29 11:34:04 JST
 *         Last Modified : 2020-12-08 20:51:15 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_E384AEC2_D044_4456_B21F_1AD3607B698B
#define INCLUDE_GUARD_UUID_E384AEC2_D044_4456_B21F_1AD3607B698B

#include "TNtupleD.h"
#include "TMath.h"

namespace art {
   class TNArray;
}


class art::TNArray : public TNtupleD {
public:
   class Variable;
   TNArray();
   TNArray(const char* name, const char* title, const char* varlist, Int_t bufsize = 32000);
   virtual ~TNArray();

   TNArray(const TNArray& rhs);
   TNArray& operator=(const TNArray& rhs);

   virtual bool Load(int verbose = 0); // should be called after all the values are filled
//   virtual void Init(); // should be called after all the index variables are added

   virtual void Add(const char* name, Double_t min, Double_t max, Int_t num);
   virtual Double_t Eval(Double_t *x) const;
   virtual Double_t Eval2(Double_t *x) const;

   virtual const Variable& GetVar(Int_t i) { return fVars[i]; }

   virtual Double_t Value(Int_t i) const { return fValues[i]; }

   virtual double WeightedSum(int ipar, double w, int idx, const std::vector< std::vector< double > >& weights,
                              const std::vector< std::vector< int > > & indexes) const ;
   virtual void CalcBicubicWeights(double dx, std::vector<double>& weight, double param = -0.5) const ;
   virtual void CalcBilinearWeights(double dx, std::vector<double>& weight) const;
   

protected:

private:
   Int_t fNumVars; // number of index variables (dimension)
   std::vector<Variable> fVars; // information of each variables
   std::vector<Int_t> fTotalNumVals; //! sum of values of lower dimensions
   Double_t *fValues; //!

   ClassDef(TNArray,1) // n-dimension array (maximum dimension of index is 9)
};

class art::TNArray::Variable : public TNamed {
public:
   Variable() { }
   virtual ~Variable() { }
   Variable(const char* name, Double_t step, Double_t min, Double_t max, Int_t num);

//   Int_t IndexI(Double_t x) { return TMath::Nint((x-fMin)*(1./fStep)); }
//   Int_t IndexI(Double_t x) { return (Int_t)TMath::Floor(((Float_t)(x-fMin))*((Float_t)(1./fStep))); }
   inline Int_t IndexI(Double_t x) const;
   



   
   Bool_t CheckBounce(Double_t x) const { return (fMin <= x ) && (x <= fMax); }
   Double_t Derivative(Double_t x) const {
      double ret = fNumVals == 1 ? 0 : (x-fMin)*(1/fStep)  - TMath::Floor((TMath::Floor((x-fMin)/fStep * 10 + 0.5))/10.);
      return ret > 0. ? ret : 0.;
   }

   virtual Int_t GetNumVals() const { return fNumVals; }
   virtual Double_t GetMin() const{ return fMin; }
   virtual Double_t GetMax() const{ return fMax; }
   virtual Double_t GetStep()const { return fStep; }

private:
   Double_t fStep; // step of variable
   Double_t fMin; // step of variable
   Double_t fMax; // step of variable
   Int_t    fNumVals; // number of values of each variable
   ClassDef(Variable,1) // information of variable
};


Int_t art::TNArray::Variable::IndexI(double x) const {

   if (fNumVals == 1) return 0;

   int idxc = TMath::Floor((x-fMin)/fStep);


   
   return fNumVals == 1 ? 0 : TMath::FloorNint(TMath::Floor((x-fMin)/fStep*10 + 0.5)/10);    
}

#endif // INCLUDE_GUARD_UUID_E384AEC2_D044_4456_B21F_1AD3607B698B
