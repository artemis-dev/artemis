/**
 * @file   TTimingData_test.cc
 * @brief
 *
 * @date   Created:       2013-11-15 14:13:33
 *         Last Modified: 2013-11-29 10:03:18
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TTimingData.h"
#include "constant.h"
#include "gtest/gtest.h"

using art::TTimingData;
using art::kInvalidD;
using art::IsValid;

namespace {
   static const Int_t NDATA = 3;  
   const Int_t valI[NDATA] = {0,-43,4363};
   const Double_t valD[NDATA] = {0.,-46.34,967.3};
}

class TTimingDataTest : public ::testing::Test {
public:
   virtual void SetUp() {
      for(Int_t i = 0; i!= NDATA; ++i) {
	 data[i].SetTiming(valD[i]);
	 data[i].SetDetID(valI[i]);
      }
   }

//   virtual void TearDown() {}

protected:
   const TTimingData default_data;
   TTimingData data[NDATA];
};

TEST_F(TTimingDataTest,IsInvalidInitially) {
   EXPECT_FALSE(IsValid(default_data.GetTiming()));
   EXPECT_FALSE(IsValid(default_data.GetDetID()));
}

TEST_F(TTimingDataTest,IsInvalidAfterClear) {
   for(Int_t i = 0;i!=NDATA;++i) {
      data[i].Clear();
      EXPECT_FALSE(IsValid(data[i].GetTiming()));
      EXPECT_FALSE(IsValid(data[i].GetDetID()));
   }
}

TEST_F(TTimingDataTest,SetterGetter) {
   for(Int_t i = 0;i!=NDATA;++i) {
      EXPECT_DOUBLE_EQ(valD[i],data[i].GetTiming());
      EXPECT_EQ(valI[i],data[i].GetDetID());

      TTimingData cdata;
      cdata.SetTiming(data[i]);
      EXPECT_DOUBLE_EQ(valD[i],cdata.GetTiming());      
   }
}

TEST_F(TTimingDataTest,CopyConstructor) {
   const TTimingData bad_cdata(default_data);
   EXPECT_FALSE(IsValid(bad_cdata.GetTiming()));
   EXPECT_FALSE(IsValid(bad_cdata.GetDetID()));

   for(Int_t i = 0;i!=NDATA;++i) {
      const TTimingData cdata(data[i]);
      EXPECT_DOUBLE_EQ(valD[i],cdata.GetTiming());
      EXPECT_EQ(valI[i],cdata.GetDetID());
   }
}

TEST_F(TTimingDataTest,AssignmentOperator) {
   const TTimingData bad_adata = default_data;
   EXPECT_FALSE(IsValid(bad_adata.GetTiming()));
   EXPECT_FALSE(IsValid(bad_adata.GetDetID()));

   for(Int_t i = 0;i!=NDATA;++i) {
      const TTimingData adata = data[i];
      EXPECT_DOUBLE_EQ(valD[i],adata.GetTiming());
      EXPECT_EQ(valI[i],adata.GetDetID());
   }
}

TEST_F(TTimingDataTest,CopyMethod) {
   TTimingData bad_cdata;
   default_data.Copy(bad_cdata);
   EXPECT_FALSE(IsValid(bad_cdata.GetTiming()));
   EXPECT_FALSE(IsValid(bad_cdata.GetDetID()));

   for(Int_t i = 0;i!=NDATA;++i) {
      TTimingData cdata;
      data[i].Copy(cdata);
      EXPECT_DOUBLE_EQ(valD[i],cdata.GetTiming());
      EXPECT_EQ(valI[i],cdata.GetDetID());
   }
}

TEST_F(TTimingDataTest,Compare) {
   TTimingData a,b;
   a.SetTiming(1.);
   a.SetDetID(1);

   b.SetTiming(1.);
   b.SetDetID(1);   

   const Int_t SMALL = -548;
   const Int_t LARGE = 430;

   TTimingData::SetSortType(TTimingData::kID);
   TTimingData::SetSortOrder(TTimingData::kASC);
   b.SetDetID(SMALL);
   EXPECT_EQ(-1,b.Compare(&a));   
   b.SetDetID(1);
   EXPECT_EQ( 0,b.Compare(&a));   
   b.SetDetID(LARGE);
   EXPECT_EQ( 1,b.Compare(&a));   

   TTimingData::SetSortOrder(TTimingData::kDESC);
   b.SetDetID(SMALL);
   EXPECT_EQ( 1,b.Compare(&a));   
   b.SetDetID(1);
   EXPECT_EQ( 0,b.Compare(&a));   
   b.SetDetID(LARGE);
   EXPECT_EQ(-1,b.Compare(&a));   

   TTimingData::SetSortType(TTimingData::kTiming);
   TTimingData::SetSortOrder(TTimingData::kASC);
   b.SetTiming(SMALL);
   EXPECT_EQ(-1,b.Compare(&a));   
   b.SetTiming(1);
   EXPECT_EQ( 0,b.Compare(&a));   
   b.SetTiming(LARGE);
   EXPECT_EQ( 1,b.Compare(&a));   

   TTimingData::SetSortOrder(TTimingData::kDESC);
   b.SetTiming(SMALL);
   EXPECT_EQ( 1,b.Compare(&a));   
   b.SetTiming(1);
   EXPECT_EQ( 0,b.Compare(&a));   
   b.SetTiming(LARGE);
   EXPECT_EQ(-1,b.Compare(&a));   
}

TEST_F(TTimingDataTest,CompareWithInvalid) {
   TTimingData a;
   a.SetTiming(1.);
   a.SetDetID(1);

   TTimingData::SetSortType(TTimingData::kID);
   TTimingData::SetSortOrder(TTimingData::kASC);
   EXPECT_EQ(-1,a.Compare(NULL));   
   EXPECT_EQ(-1,a.Compare(&default_data));   
   EXPECT_EQ( 1,default_data.Compare(&a));
   EXPECT_EQ( 0,default_data.Compare(&default_data));

   TTimingData::SetSortOrder(TTimingData::kDESC);
   EXPECT_EQ(-1,a.Compare(NULL));   
   EXPECT_EQ(-1,a.Compare(&default_data));   
   EXPECT_EQ( 1,default_data.Compare(&a));
   EXPECT_EQ( 0,default_data.Compare(&default_data));

   TTimingData::SetSortType(TTimingData::kTiming);
   TTimingData::SetSortOrder(TTimingData::kASC);
   EXPECT_EQ(-1,a.Compare(NULL));   
   EXPECT_EQ(-1,a.Compare(&default_data));   
   EXPECT_EQ( 1,default_data.Compare(&a));
   EXPECT_EQ( 0,default_data.Compare(&default_data));

   TTimingData::SetSortOrder(TTimingData::kDESC);
   EXPECT_EQ(-1,a.Compare(NULL));   
   EXPECT_EQ(-1,a.Compare(&default_data));   
   EXPECT_EQ( 1,default_data.Compare(&a));
   EXPECT_EQ( 0,default_data.Compare(&default_data));
}

TEST_F(TTimingDataTest,Operators) {

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
