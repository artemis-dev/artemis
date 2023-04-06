/**
 * @file   TVDCCluster.h
 * @brief
 *
 * @date   Created:       2013-08-14 10:19:28
 *         Last Modified: 2014-02-24 14:39:51
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TVDCCLUSTER_H
#define TVDCCLUSTER_H

#include "TDataObject.h"
#include "ITiming.h"
#include "ICharge.h"

namespace art{
   class TVDCCluster;
}

class art::TVDCCluster : public TDataObject{
public:
   typedef enum {kID, kCharge, kTiming} ETimingChargeSortType; //!
   typedef enum {kASC, kDESC} ESortOrder; //!

   // Default constructor
   TVDCCluster();
   virtual ~TVDCCluster();
	 void Init();
	 void Init2(int size);
   // Copy constructor
   TVDCCluster(const TVDCCluster& rhs);
   // Assignment operator
//   virtual TVDCCluster& operator=(const TVDCCluster& rhs);

/*
   virtual Double_t GetTiming() const {return fTiming;}
   virtual void     SetTiming(Double_t val){fTiming = val;}
   virtual void     SetTiming(const ITiming& obj){SetTiming(obj.GetTiming());}
	*/
/*
   virtual Double_t GetCharge() const {return fCharge;}
   virtual void     SetCharge(Double_t val){fCharge = val;}
   virtual void     SetCharge(const ICharge& obj){SetCharge(obj.GetCharge());}
	*/

   // obsolete. use TDataObject::GetID()
   virtual Int_t GetDetID() const {return GetID();}
   // obsolete. use TDataObject::SetID()
   virtual void  SetDetID(Int_t val){ SetID(val);}

   static void SetSortOrder(Int_t order){fSortOrder = order;}
   static void SetSortType(Int_t type){fSortType = type;}

   virtual Bool_t IsSortable() const {return kTRUE;}
/*
   virtual void Copy(TObject& dest) const {
      TDataObject::Copy(dest);
      TVDCCluster &cobj = *(TVDCCluster*)&dest;
      cobj.SetTiming(this->GetTiming());
      cobj.SetCharge(this->GetCharge());
   }
	*/
	virtual void SetHitID(int idx, int id){fHitID[idx]=id;}
	virtual int GetHitID(int idx){return fHitID[idx];}

	virtual void SetHitDL(int idx, double dl){fHitDL[idx]=dl;}
	virtual double GetHitDL(int idx){return fHitDL[idx];}

	virtual void SetHitTiming(int idx, double timing){fHitTiming[idx]=timing;}
	virtual double GetHitTiming(int idx){return fHitTiming[idx];}

	virtual void SetHitLR(int idx, int lr){fHitLR[idx]=lr;}
	virtual int GetHitLR(int idx){return fHitLR[idx];}

	virtual void SetHitPos(double pos){cluster_pos=pos;}
	virtual double GetHitPos(){return cluster_pos;}
	
	virtual void SetClustID(double id){Clust_ID=id;}
	virtual double GetClustID(){return Clust_ID;}

	virtual void SetClustnum(double id){valid_cluster=id;}
	virtual double GetClustnum(){return valid_cluster;}

	virtual void SetClustnum2(double id){valid_cluster2=id;}
	virtual double GetClustnum2(){return valid_cluster2;}

	virtual void SetClustSize(double id){fClust_Size=id;}
	virtual double GetClustSize(){return fClust_Size;}

	virtual void SetHitAngle(double angle){cluster_angle=angle;}
	virtual double GetHitAngle(){return cluster_angle;}
//   virtual void Clear(Option_t *opt="");


	 virtual void Print(Option_t*) const;




protected:
   Double_t fTiming;
   Double_t fCharge;
   Double_t fClust_Size;
   static Int_t fSortType;  //!
   static Int_t fSortOrder; //!
	Double_t Clust_ID;
	double valid_cluster;
	double valid_cluster2;

	Double_t cluster_pos;
	Double_t cluster_angle;
 std::vector<int> fHitID; // 
 std::vector<Double_t> fHitDL; // 
 std::vector<Double_t> fHitTiming; // 
 std::vector<Double_t> fHitLR; // 

   ClassDef(TVDCCluster,1) // container for timing information with charge
};

#endif // TTIMINGCHARGEDATA_H
