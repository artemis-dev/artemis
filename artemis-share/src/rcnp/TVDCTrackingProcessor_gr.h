/**
 * @file   TVDCTrackingProcessor_gr.h
 * @brief
 *
 * @date   Created:       2013-10-19 16:59:46
 *         Last Modified: 2014-03-13 16:03:56 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TVDCTRACKINGPROCESSOR_gr_H
#define TVDCTRACKINGPROCESSOR_gr_H

#include <vector>

#include <TProcessor.h>
#include <TTree.h>

namespace art {
   class TVDCTrackingProcessor_gr;
   class TMWDCPlaneInfo;
   class TMWDCHitData;
   class TVDCTrackingResult;
}

class TClonesArray;
class TStopwatch;
template <typename T> class TMatrixT;
typedef TMatrixT<double> TMatrixD;

class art::TVDCTrackingProcessor_gr : public TProcessor {
public:
   // Default constructor
   TVDCTrackingProcessor_gr();
   virtual ~TVDCTrackingProcessor_gr();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   StringVec_t fInputColName;
   TString fOutputColName;
//	  TString fInputColName2;
   StringVec_t fInputColName2;
	 TClonesArray **fpreTrack;
   std::vector<TClonesArray**> *fPlaneIn; //!
   std::vector<TMWDCPlaneInfo*> *fPlaneInfo; //! TODO: separate this vector into small ones
   TClonesArray *fMWDCTrackOut;
   Int_t fTrackingMethod;
   Int_t fNPlane;
   TMatrixD *fMatrix;
   TMWDCHitData **fPlaneData; //!
   Int_t fUseLocalPos;
   std::vector<int> bestwirepos;
   std::vector<int> numberofire;

//   Float_t fSSRMax;
   Int_t fSSRMax;
//	 Int_t *clust;
//	 Int_t *clustmax;
	 int clustmax[4];  
	 double clust_grad[4][10];
	 double clust_pos[4][10];
	 int clust_size[4][10];  
	 int clust_rsize[4][10];  
	 int clust_id[4][10];  
	 int clust_sippai[4][10];  
	 int clust_timing[4][10][20];  
	 double clust_dl[4][10][20];  
	 int clust_wire[4][10][20];  
	 double zposition[4][10];
	 int total_hit;

	 TTree *tree;
	 double x,y,a,b;
   Double_t *fCos; //[fNPlane]
   Double_t *fSin; //[fNPlane]
   Double_t *fZ;   //[fNPlane]

   Double_t *fX;   //[fNPlane]
   Double_t *fY;   //[fNPlane]
   TStopwatch *fStopwatch; //!
//   virtual void GenerateMatrix(std::vector<int> bangou);
//   virtual void GenerateMatrix(int bangou);
//   virtual void FindTrack(TVDCTrackingResult *tr);
   //virtual void ChoseWire();
 //  virtual int ChoseWire(TVDCTrackingResult *tr);
  // virtual Double_t CalcSSR(const std::vector<Double_t> &pos,
	//		    const std::vector<Double_t> &trackPrm,
	//		    std::vector<Double_t> &residual) const;
//virtual Double_t CalcSSR(FloatVec_t &pos, FloatVec_t &trackPrm,FloatVec_t &residual);
   static const Int_t MIN_NPLANE = 3;
   static const Int_t CRITICAL_NPLANE = 0;

private:
   // Copy constructor (prohibited)
   TVDCTrackingProcessor_gr(const TVDCTrackingProcessor_gr&);
   // Assignment operator (prohibited)
   TVDCTrackingProcessor_gr& operator=(const TVDCTrackingProcessor_gr&);

   ClassDef(TVDCTrackingProcessor_gr,0) // processor for MWDC tracking
};

#endif // TMWDCTRACKINGPROCESSOR_H
