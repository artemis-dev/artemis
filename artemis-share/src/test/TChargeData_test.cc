/**
 * @file   TChargeData_test.cc
 * @brief
 *
 * @date   Created:       2013-11-15 14:13:33
 *         Last Modified: 2013-11-29 10:06:04
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TChargeData.h"
#include "constant.h"
#include "gtest/gtest.h"

using art::TChargeData;
using art::kInvalidD;
using art::IsValid;

namespace {
   static const Int_t NDATA = 3;  
   const Int_t valI[NDATA] = {0,-43,4363};
   const Double_t valD[NDATA] = {0.,-46.34,967.3};
}

class TChargeDataTest : public ::testing::Test {
public:
   virtual void SetUp() {
      for(Int_t i = 0; i!= NDATA; ++i) {
	 data[i].SetCharge(valD[i]);
	 data[i].SetDetID(valI[i]);
      }
   }

//   virtual void TearDown() {}

protected:
   const TChargeData default_data;
   TChargeData data[NDATA];
};

TEST_F(TChargeDataTest,IsInvalidInitially) {
   EXPECT_FALSE(IsValid(default_data.GetCharge()));
   EXPECT_FALSE(IsValid(default_data.GetDetID()));
}

TEST_F(TChargeDataTest,IsInvalidAfterClear) {
   for(Int_t i = 0;i!=NDATA;++i) {
      data[i].Clear();
      EXPECT_FALSE(IsValid(data[i].GetCharge()));
      EXPECT_FALSE(IsValid(data[i].GetDetID()));
   }
}

TEST_F(TChargeDataTest,SetterGetter) {
   for(Int_t i = 0;i!=NDATA;++i) {
      EXPECT_DOUBLE_EQ(valD[i],data[i].GetCharge());
      EXPECT_EQ(valI[i],data[i].GetDetID());

      TChargeData cdata;
      cdata.SetCharge(data[i]);
      EXPECT_DOUBLE_EQ(valD[i],cdata.GetCharge());      
   }
}

TEST_F(TChargeDataTest,CopyConstructor) {
   const TChargeData bad_cdata(default_data);
   EXPECT_FALSE(IsValid(bad_cdata.GetCharge()));
   EXPECT_FALSE(IsValid(bad_cdata.GetDetID()));

   for(Int_t i = 0;i!=NDATA;++i) {
      const TChargeData cdata(data[i]);
      EXPECT_DOUBLE_EQ(valD[i],cdata.GetCharge());
      EXPECT_EQ(valI[i],cdata.GetDetID());
   }
}

TEST_F(TChargeDataTest,AssignmentOperator) {
   const TChargeData bad_adata = default_data;
   EXPECT_FALSE(IsValid(bad_adata.GetCharge()));
   EXPECT_FALSE(IsValid(bad_adata.GetDetID()));

   for(Int_t i = 0;i!=NDATA;++i) {
      const TChargeData adata = data[i];
      EXPECT_DOUBLE_EQ(valD[i],adata.GetCharge());
      EXPECT_EQ(valI[i],adata.GetDetID());
   }
}

TEST_F(TChargeDataTest,CopyMethod) {
   TChargeData bad_cdata;
   default_data.Copy(bad_cdata);
   EXPECT_FALSE(IsValid(bad_cdata.GetCharge()));
   EXPECT_FALSE(IsValid(bad_cdata.GetDetID()));

   for(Int_t i = 0;i!=NDATA;++i) {
      TChargeData cdata;
      data[i].Copy(cdata);
      EXPECT_DOUBLE_EQ(valD[i],cdata.GetCharge());
      EXPECT_EQ(valI[i],cdata.GetDetID());
   }
}

TEST_F(TChargeDataTest,Compare) {
   TChargeData a,b;
   a.SetCharge(1.);
   a.SetDetID(1);

   b.SetCharge(1.);
   b.SetDetID(1);   

   const Int_t SMALL = -548;
   const Int_t LARGE = 430;

   TChargeData::SetSortType(TChargeData::kID);
   TChargeData::SetSortOrder(TChargeData::kASC);
   b.SetDetID(SMALL);
   EXPECT_EQ(-1,b.Compare(&a));   
   b.SetDetID(1);
   EXPECT_EQ( 0,b.Compare(&a));   
   b.SetDetID(LARGE);
   EXPECT_EQ( 1,b.Compare(&a));   

   TChargeData::SetSortOrder(TChargeData::kDESC);
   b.SetDetID(SMALL);
   EXPECT_EQ( 1,b.Compare(&a));   
   b.SetDetID(1);
   EXPECT_EQ( 0,b.Compare(&a));   
   b.SetDetID(LARGE);
   EXPECT_EQ(-1,b.Compare(&a));   

   TChargeData::SetSortType(TChargeData::kCharge);
   TChargeData::SetSortOrder(TChargeData::kASC);
   b.SetCharge(SMALL);
   EXPECT_EQ(-1,b.Compare(&a));   
   b.SetCharge(1);
   EXPECT_EQ( 0,b.Compare(&a));   
   b.SetCharge(LARGE);
   EXPECT_EQ( 1,b.Compare(&a));   

   TChargeData::SetSortOrder(TChargeData::kDESC);
   b.SetCharge(SMALL);
   EXPECT_EQ( 1,b.Compare(&a));   
   b.SetCharge(1);
   EXPECT_EQ( 0,b.Compare(&a));   
   b.SetCharge(LARGE);
   EXPECT_EQ(-1,b.Compare(&a));   
}

TEST_F(TChargeDataTest,CompareWithInvalid) {
   TChargeData a;
   a.SetCharge(1.);
   a.SetDetID(1);

   TChargeData::SetSortType(TChargeData::kID);
   TChargeData::SetSortOrder(TChargeData::kASC);
   EXPECT_EQ(-1,a.Compare(NULL));   
   EXPECT_EQ(-1,a.Compare(&default_data));   
   EXPECT_EQ( 1,default_data.Compare(&a));
   EXPECT_EQ( 0,default_data.Compare(&default_data));

   TChargeData::SetSortOrder(TChargeData::kDESC);
   EXPECT_EQ(-1,a.Compare(NULL));   
   EXPECT_EQ(-1,a.Compare(&default_data));   
   EXPECT_EQ( 1,default_data.Compare(&a));
   EXPECT_EQ( 0,default_data.Compare(&default_data));

   TChargeData::SetSortType(TChargeData::kCharge);
   TChargeData::SetSortOrder(TChargeData::kASC);
   EXPECT_EQ(-1,a.Compare(NULL));   
   EXPECT_EQ(-1,a.Compare(&default_data));   
   EXPECT_EQ( 1,default_data.Compare(&a));
   EXPECT_EQ( 0,default_data.Compare(&default_data));

   TChargeData::SetSortOrder(TChargeData::kDESC);
   EXPECT_EQ(-1,a.Compare(NULL));   
   EXPECT_EQ(-1,a.Compare(&default_data));   
   EXPECT_EQ( 1,default_data.Compare(&a));
   EXPECT_EQ( 0,default_data.Compare(&default_data));
}

TEST_F(TChargeDataTest,Operators) {

   const Double_t CONSTANT1 = 44.665;
   const Double_t CONSTANT2 = -73.325;

   for(Int_t i = 0;i!=NDATA;++i) {   
      EXPECT_DOUBLE_EQ(valD[i],(Double_t)data[i]);
      data[i] += CONSTANT1;
      EXPECT_DOUBLE_EQ(valD[i] + CONSTANT1,(Double_t)data[i]);
      data[i] -= CONSTANT2;
      EXPECT_DOUBLE_EQ(valD[i] + CONSTANT1 - CONSTANT2,(Double_t)data[i]);
   }
}
