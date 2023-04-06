/**
 * @file   TVDCTrackingResult.h
 * @brief
 *
 * @date   Created:       2013-10-17 16:02:43
 *         Last Modified: 2014-03-04 17:10:28 (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TVDCTRACKINGRESULT_H
#define TVDCTRACKINGRESULT_H

#include "TDataObject.h"
#include "TTrack.h"

namespace art {
  class TVDCTrackingResult;
}

class art::TVDCTrackingResult : public TDataObject {
  public:
	 // Default constructor
	 TVDCTrackingResult();
	 virtual ~TVDCTrackingResult();
	 // Copy constructor
	 TVDCTrackingResult(const TVDCTrackingResult& rhs);
	 // Assignment operator
	 TVDCTrackingResult& operator=(const TVDCTrackingResult& rhs);

	 enum ETrackingID {kDefault, kGood = 1, kLargeSSR = -1, kLargeCombination = -2};

	 /* override */
	 virtual void Copy(TObject &obj) const;
	 virtual void Clear(Option_t *opt="");
	 /* override end */

	 void SetTrackingID(Int_t val) { SetID(val); }
	 Int_t GetTrackingID() const { return GetID(); }

	 Bool_t IsGood() const {return (GetID() == kGood); }
	 Bool_t IsProcessed() const {return (GetID() != kDefault); }

	 void SetSSR(Double_t val) {fSSR = val;}
	 Double_t GetSSR() const {return fSSR;}

	 void SetSSR2(Double_t val) {fSSR2 = val;}
	 Double_t GetSSR2() const {return fSSR2;}

	 void SetTrack(Double_t x, Double_t y, Double_t z,
		  Double_t a, Double_t b) {
		fTrack.SetPos(x,y,z); fTrack.SetAngle(a,b);
	 }
	 void SetTrack2(Double_t x, Double_t y, Double_t z,
		  Double_t a, Double_t b) {
		fTrack2.SetPos(x,y,z); fTrack2.SetAngle(a,b);
	 }

	 TTrack* GetTrack() { return &fTrack; }
	 const TTrack* GetTrack() const { return &fTrack; }

	 TTrack* GetTrack2() { return &fTrack2; }
	 const TTrack* GetTrack2() const { return &fTrack2; }

	 void SetResidual(Int_t idx, Double_t val){
		fResidual[idx] = val;
	 } 
	 void SetLR(Int_t idx, Int_t val){
		fLR[idx] = val;
	 } 
	 int GetLR(Int_t idx){
		return fLR[idx];
	 }
	 double GetResidual(Int_t idx){
		return fResidual[idx];
	 }

	 void Init(Int_t nPlane);

	 Int_t GetNParameter() const { return fNParameter; }
	 void SetNParameter(Int_t val) { fNParameter = val; }

	 Int_t GetNPlaneValid() const { return fNPlaneValid; }
	 void SetNPlaneValid(Int_t val) { fNPlaneValid = val; }

	 Int_t GetPlaneCombinationID() const { return fPlaneCombinationID; }
	 void SetPlaneCombinationID(Int_t val) { fPlaneCombinationID = val; }

	 Int_t GetNCombination() const { return fNCombination; }
	 void SetNCombination(Int_t val) { fNCombination = val; }

	 Int_t GetNCombinationProcessed() const { return fNCombinationProcessed; }
	 void SetNCombinationProcessed(Int_t val) { fNCombinationProcessed = val; }

	 Double_t GetUSV() const { return fUSV; }
	 void SetUSV(Double_t val) { fUSV = val; }

	 Int_t GetWireIDAdopted(Int_t idx) const { return fWireIDAdopted[idx]; }
	 void SetWireIDAdopted(Int_t idx, Int_t val) { fWireIDAdopted[idx] = val; }
	 void Setmultiplicity(Int_t iplane, Int_t val) { multiplicity[iplane] = val; }

	 Double_t GetDriftLengthAdopted(Int_t idx) const { return fDriftLengthAdopted[idx]; }
	 void SetDriftLengthAdopted(Int_t idx, Double_t val) { fDriftLengthAdopted[idx] = val; }

	 Double_t GetSigma(Int_t idx) const { return fSigma[idx]; }
	 void SetSigma(Int_t idx, Double_t val) { fSigma[idx] = val; }

	 Double_t GetCI(Int_t idx) const { return fCI[idx]; }
	 void SetCI(Int_t idx, Double_t val) { fCI[idx] = val; }

	 Double_t GetTimeCost() const { return fTimeCost; }
	 void SetTimeCost(Double_t val) { fTimeCost = val; }

	 static const Int_t kMAXNPARAMETER = 4;

	 virtual void Print(Option_t*) const;

	 void setvdcres(int ipl,int i, double res){
		VDC_res[ipl][i]=res;
	 }
	 double getvdcres(int ipl,int i){
		return VDC_res[ipl][i];
	 }

	 void setvdclr(int ipl,int i, int lr){
		VDC_lr[ipl][i]=lr;
	 }
	 double getvdclr(int ipl,int i){
		return VDC_lr[ipl][i];
	 }

	 void setvdcadl(int ipl,int i, double adl){
		VDC_adl[ipl][i]=adl;
	 }
	 double getvdcadl(int ipl,int i){
		return VDC_adl[ipl][i];
	 }
//	 void setvdcaid(int ipl,int i, double aid){
//		VDC_aid[ipl][i]=aid;
//	 }
//	 int getvdcaid(int ipl,int i){
//		return VDC_aid[ipl][i];
//	 }
//
	 void setvdcapos(int ipl,int i, double apos){
		VDC_apos[ipl][i]=apos;
	 }
	 double getvdcapos(int ipl,int i){
		return VDC_apos[ipl][i];
	 }
	 void setclustnum(int ipl, int i){
		CLST_num[ipl]=i;
	 }
	 int getclustnum(int ipl){
		return CLST_num[ipl];
	 }
	 void setclustpos(int ipl,int i, double apos){
		CLST_pos[ipl][i]=apos;
	 }
	 double getclustpos(int ipl,int i){
		return	CLST_pos[ipl][i];
	 }
	 void setclustgrad(int ipl,int i, double agrad){
		CLST_grad[ipl][i]=agrad;
	 }
	 double getclustgrad(int ipl,int i){
		return	CLST_grad[ipl][i];
	 }
	 void setclustsize(int ipl,int i, int asize){
		CLST_size[ipl][i]=asize;
	 }
	 int getclustsize(int ipl,int i){
		return	CLST_size[ipl][i];
	 }
//	 void setclustrsize(int ipl,int i, int arsize){
//		CLST_rsize[ipl][i]=arsize;
//	 }
//	 int getclustrsize(int ipl,int i){
//		return	CLST_rsize[ipl][i];
//	 }
	 void setclustsippai(int ipl,int i, int asippai){
		CLST_sippai[ipl][i]=asippai;
	 }
	 int getclustsippai(int ipl,int i){
		return	CLST_sippai[ipl][i];
	 }
	 void setclustoN(int ipl, double aasize){
		CLST_osize[ipl]=aasize;
	 }
	 double getclustoN(int ipl){
		return	CLST_osize[ipl];
	 }
	 void setclustN(int ipl, int aasize){
		CLST_asize[ipl]=aasize;
	 }
	 int getclustN(int ipl){
		return	CLST_asize[ipl];
	 }


	 void setcluststatus(int ipl,int i, int astatus){
		CLST_status[ipl][i]=astatus;
	 }
	 int getcluststatus(int ipl,int i){
		return	CLST_status[ipl][i];
	 }




	 void sethit(int ipl, double hit){
		VDC_hit[ipl]=hit;
	 }
	 double gethit(int ipl){
		return VDC_hit[ipl];
	 }
	 void setclustasize(int ipl, int i){
		VDC_nc[ipl]=i;
	 }
	 int getclustasize(int ipl){
		return VDC_nc[ipl];
	 }

	 void setclustdl(int ipl,int icl, int iwi, double adl){
		CLST_dl[ipl][icl][iwi]=adl;
	 }
	 double getclustdl(int ipl,int icl, int iwi){
		return	CLST_dl[ipl][icl][iwi];
	 }
	 void setclusttiming(int ipl,int icl, int iwi, int atiming){
		CLST_timing[ipl][icl][iwi]=atiming;
	 }
	 int getclusttiming(int ipl,int icl, int iwi){
		return	CLST_timing[ipl][icl][iwi];
	 }

	 void setclustid(int ipl,int icl, int iwi, int aid){
		CLST_id[ipl][icl][iwi]=aid;
	 }
	 int getclustid(int ipl,int icl, int iwi){
		return	CLST_id[ipl][icl][iwi];
	 }
	 void setsumres(double sum){
		sumres = sum;
	 }
	 double getsumres(){
		return	sumres;
	 }
	 void setdispres(double disp){
		dispres = disp;
	 }
	 double getdispres(){
		return	dispres;
	 }
	 void setchi2_plane(int ipl, double disp){
		chi2_plane[ipl] = disp;
	 }
	 double getchi2_plane(int ipl){
		return	chi2_plane[ipl];
	 }





  protected:
	 double VDC_res[4][10];
	 double VDC_lr[4][10];
	 double VDC_adl[4][10];
//	 int VDC_aid[4][10];
	 double VDC_apos[4][10];
	 double VDC_nc[4];
	 double VDC_hit[4];
	 double sumres;
//	 double aaa;
	 double dispres;

	 double chi2_plane[4];
	 double CLST_pos[4][10];
	 double CLST_grad[4][10];
	 int CLST_size[4][10];
	 //int CLST_rsize[4][10];
	 int CLST_osize[4];
	 int CLST_asize[4];
	 int CLST_sippai[4][10];
	 int CLST_num[4];
	 double CLST_dl[4][10][20];
	 int CLST_timing[4][10][20];
	 int CLST_id[4][10][20];
	 int CLST_status[4][10];

	 std::vector<int> multiplicity;
	 TTrack   fTrack;
	 TTrack   fTrack2;

	 Int_t    fNPlane;
	 Int_t    fNPlaneValid;

	 Int_t    fPlaneCombinationID;
	 Int_t    fNParameter;
	 Int_t    fNCombination;
	 Int_t    fNCombinationProcessed;

	 std::vector<Int_t> fWireIDAdopted;      //
	 std::vector<Int_t> fLR;      //
	 std::vector<Double_t> fDriftLengthAdopted; //
	 //	int fWireIDAdopted[100];
	 //	int fLR[100];
	 //	double fDriftLengthAdopted[100];
	 //		double fResidual[100];

	 Double_t fSSR;                // sum of squared residuals
	 Double_t fSSR2;                // sum of squared residuals
	 std::vector<Double_t> fResidual;          
	 //
	 //   	

	 Double_t fUSV;                // unbiased sample variance = SSR/DF. the expectation value is population variance.
	 std::vector<Double_t> fSigma; // Sigma of tracking parameter
	 std::vector<Double_t> fCI;    // Confident Interval of tracking parameter

	 Double_t fTimeCost;                     // time cost in msec. obsolete

	 ClassDef(TVDCTrackingResult,1) // container for MWDC tracking result
};

#endif // TMWDCTRACKINGRESULT_H
