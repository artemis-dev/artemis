/**
 * @file   TVDCTrackingProcessor_gr.cc
 * @brief
 *
 * @date   Created:       2013-10-19 17:01:56
 *         Last Modified: 2014-05-20 08:58:47 JST (kawase)
 *         
 *         this program was created by modifing Kawase-san's tracking processor
 *         
 * @author Yuni Watanabe<ywatanabe@nucl.phys.s.u-tokyo.ac.jp>
 *
 *  
 */

#include "TVDCTrackingProcessor_gr.h"
#include "TVDCTrackingResult.h"
#include "TMWDCParameter.h"
#include "TMWDCPlaneInfo.h"
#include "TMWDCHitData.h"
#include "TVDCCluster.h"
#include "TFile.h"
#include "TTree.h"
#include "constant.h"

#include <numeric>

#include <TStopwatch.h>
#include <TClonesArray.h>
#include <TMath.h>
#include <TMatrixD.h>

using art::TVDCTrackingProcessor_gr;

ClassImp(art::TVDCTrackingProcessor_gr)

  // Default constructor
  TVDCTrackingProcessor_gr::TVDCTrackingProcessor_gr()
: fPlaneIn(NULL), fPlaneInfo(NULL), fMWDCTrackOut(NULL),
  fMatrix(NULL), fPlaneData(NULL), fCos(NULL), fSin(NULL),
  fZ(NULL), fX(NULL), fY(NULL), fStopwatch(NULL)
{
  StringVec_t defInput;
  StringVec_t defInput2;
  defInput.push_back("mwdc1_x");
  defInput.push_back("mwdc1_u");
  defInput.push_back("mwdc1_v");
  defInput2.push_back("mwdc1_x");
  defInput2.push_back("mwdc1_u");
  defInput2.push_back("mwdc1_v");
  RegisterInputCollection("InputCollection","input",
		fInputColName,defInput);
  RegisterInputCollection("InputCollection2","input2",
		fInputColName2,defInput2);
  RegisterOutputCollection("OutputCollection","output",
		fOutputColName,TString("mwdc1_track"));
  RegisterProcessorParameter("UseLocalPos","default: 0",
		fUseLocalPos,0);
  RegisterProcessorParameter("TrackingMethod","not impremented (default: 0)",
		fTrackingMethod,0);
  RegisterProcessorParameter("SSRMax","maximum limit for SSR",
		fSSRMax,1000);
}

TVDCTrackingProcessor_gr::~TVDCTrackingProcessor_gr()
{
  delete fPlaneIn;
  fPlaneIn = NULL;
  delete fPlaneInfo;
  fPlaneInfo = NULL;
  delete fMWDCTrackOut;
  fMWDCTrackOut = NULL;
  delete [] fCos;
  fCos = NULL;
  delete [] fSin;
  fSin = NULL;
  delete [] fZ;
  fZ = NULL;
  delete [] fX;
  fX = NULL;
  delete [] fY;
  fY = NULL;
  delete fStopwatch;
  fStopwatch = NULL;
}

void TVDCTrackingProcessor_gr::Init(TEventCollection *col)
{
  //  double x,y,a,b;
  //printf("init_tracking\n");
  fPlaneIn = new std::vector<TClonesArray**>;
  fPlaneInfo = new std::vector<TMWDCPlaneInfo*>;


  fNPlane = fInputColName.size();
  printf("fNPlane=%d\n",fNPlane);

  if(fNPlane < MIN_NPLANE) {
	 Error("Init","Number of plane is insufficient for tracking (min: 3 planes). abort.");
	 fNPlane = 0;
	 return;
  }
  {
	 TString mwdcnameCache("");
	 for (Int_t i = 0;i!=fNPlane;++i) {
		const TString &inputName = fInputColName2[i];
		const TString &inputName2 = fInputColName[i];

		TClonesArray** const ref = reinterpret_cast<TClonesArray**>(col->GetObjectRef(inputName2.Data()));

		const Int_t delimIndex = inputName.Last('_');
		const TString &mwdcname = inputName(0,delimIndex);
		const TString &planename = inputName(delimIndex+1,inputName.Length());

		TMWDCParameter *const mwdcprm =
		  reinterpret_cast<TMWDCParameter*>(col->GetInfo(mwdcname));

		if (!mwdcprm) {
		  Error("Init","mwdc parameter object '%s' not found",mwdcname.Data());
		  delete fPlaneIn;
		  delete fPlaneInfo;
		  fPlaneIn = NULL;
		  fPlaneInfo = NULL;
		  return;
		}

		TMWDCPlaneInfo *const info = mwdcprm->GetPlane(planename, fUseLocalPos);
		if (!info) {
		  Error("Init","info for plane '%s' not found",planename.Data());
		}
		info->Print();

		if (fUseLocalPos) {
		  if (mwdcnameCache.Length() && !mwdcnameCache.EqualTo(mwdcname)) {
			 Warning("Init","Planes from more than one MWDC are used. fUseLocalPos setting will be ignored.");
			 fUseLocalPos = kFALSE;
		  }
		  mwdcnameCache = mwdcname;
		}

		if (!ref || !info) {
		  delete fPlaneIn;
		  delete fPlaneInfo;
		  fPlaneIn = NULL;
		  fPlaneInfo = NULL;
		  return;
		}

		fPlaneIn->push_back(ref);
		fPlaneInfo->push_back(info);
	 }
  }

  fMWDCTrackOut = new TClonesArray(TVDCTrackingResult::Class(),1);
  fMWDCTrackOut->SetName(fOutputColName);
  col->Add(fOutputColName,fMWDCTrackOut,fOutputIsTransparent);

  fCos = new Double_t[fNPlane];
  fSin = new Double_t[fNPlane];
  fZ   = new Double_t[fNPlane];
  fX   = new Double_t[fNPlane];
  fY   = new Double_t[fNPlane];

  for(Int_t i = 0; i!=fNPlane; ++i) {
	 const TMWDCPlaneInfo *const planeInfo = fPlaneInfo->at(i);
	 fCos[i] = planeInfo->Cos();
	 fSin[i] = planeInfo->Sin();
	 fZ[i]   = planeInfo->GetZ();
	 fY[i]   = planeInfo->GetY();
  }
  fX[0]=0; 
  fX[1]=0; 
  fX[2]=250; 
  fX[3]=250; 

  TString outputStr = TString("(");
  for(Int_t i = 0; i!=fNPlane; ++i) {
	 if(i) outputStr.Append(", ");
	 outputStr.Append(fInputColName[i]);
  }
  outputStr.Append(") => ");
  outputStr.Append(fOutputColName);

  Info("Init","%s",outputStr.Data());

  fStopwatch = new TStopwatch;
}

void TVDCTrackingProcessor_gr::Process()
{
  //input : clusters of each planes
  //output : tracking result
  //function
  //1 initialization
  //2 count the number of cluster combinations
  //3 do tracking procedure for all combinations 
  //4 select the best combination whose chi2 is minimum
  //5 output the tracking result

  int debug =0;
//  if(debug)getchar();
  if(!fNPlane || !fPlaneIn) return;
  /* initialization */
  fStopwatch->Start();
  fMWDCTrackOut->Clear("C");
  Int_t nHitPlane = 0;
  Int_t nData = 0;

  //initialize output 
  TVDCTrackingResult *const trackingResult =
	 static_cast<TVDCTrackingResult*>(fMWDCTrackOut->ConstructedAt(nData++));
  trackingResult->Init(200);	


  double bairitsu[4]={1.077,1.134,1.069,1.13};//adjustment of drift length
  int valid_cluster[4]={0,0,0,0}; //number of valid cluster for each plane
  for (Int_t iPlane = 0 ;iPlane !=fNPlane;++iPlane) {
	 TClonesArray *const plane = *(fPlaneIn->at(iPlane));
	 for(int jk=0;jk<plane->GetEntriesFast();jk++){
		TVDCCluster *dummy= (TVDCCluster*)plane->At(jk);
		if(dummy->GetClustID()==1)continue;
		valid_cluster[iPlane]++;
	 }
  }
  //number of combination 
  int combination = valid_cluster[0]*valid_cluster[1]* valid_cluster[2]* valid_cluster[3];

  //tracking ID (same definition as Tamii-analyzer)
  int tracking_ID=0;
  for(int i=0;i<4;i++){
	 if(valid_cluster[i]==0){
		int  bairitu=1;
		for(int j=0;j<i;j++){
		  bairitu=	bairitu*2;
		}
		tracking_ID+=bairitu;
	 }
  }
  trackingResult->SetTrackingID(tracking_ID);//sippai

  //アルゴリズムの流れとしては、
  //1. 各コンビネーションごとにメンゴとのhitid, dl, トラッキングされた位置などを回収し、
  //2. トラッキングしてSSRなどを計算する
  //3. SSRが最小になる組み合わせを選ぶ
  //
  //  int selected_cluster[4]={0,0,0,0};
  if(debug)  printf("trapro_gr 1.5\n");
  int id[4][50][50];
  double dl[4][50][50];
  double cluster_pos[4][50];
  int cluster_size[4][50];
  for(int iPlane = 0;iPlane<fNPlane;iPlane++){
	 TClonesArray *const plane = *(fPlaneIn->at(iPlane));
	 int index=0;
	 for(int jk=0;jk<plane->GetEntriesFast();jk++){
		TVDCCluster *dummy= (TVDCCluster*)plane->At(jk);
		if(dummy->GetClustID()==1)continue;
		for(int k=0;k<dummy->GetClustSize();k++){
		  id[iPlane][index][k]=dummy->GetHitID(k);
		  dl[iPlane][index][k]=dummy->GetHitDL(k)/10.*bairitsu[iPlane];
		}
		cluster_size[iPlane][index]=dummy->GetClustSize();
		cluster_pos[iPlane][index]=dummy->GetHitPos();
		if(debug)	 printf("clustsize[%d][%d]=%d,clustpos=%lf\n",iPlane,index,cluster_size[iPlane][index],cluster_pos[iPlane][index]);
		index++;
	 }
  }
  // detector configuration (same as Tamii-analyzer)
  double	pli[4][6]={{573.0,0,0,-6,0.0,10.},{538.9+64,0,20,-4,-48.19,10.},{323.0,0,250,-6,0.0,10.},{288.7+64,0,270,-4,-48.19,10.}};

  double bestx=-10000;
  double besty=-10000;
  double besta=-1000;
  double bestb=-1000;
  double fSSR=10000;
  double bestsumres;
  double disp_res1 = 1000;
  double fSSR_2=1000;
  int bestcombi=-100;
  double bestres[4][50];
  int best_clust_size[4]={0,0,0,0};
  double bestdl[4][50];
  int bestid[4][50];


  int selected_cluster[4]={0,0,0,0};
  if(combination==0){
	 //  trackingResult->SetTrackingID(-1);//sippai
	 trackingResult->SetTrack(-10000,-10000,-10000,-10000,-10000);
	 trackingResult->SetSSR(-10000);
	 return;
  }
  if(combination>50){
	 trackingResult->SetTrackingID(-2);//sippai
	 trackingResult->SetTrack(-10000,-10000,-10000,-10000,-10000);
	 trackingResult->SetSSR(-10000);
	 return;
  }

  if(debug)  printf("trapro_gr 2\n");


  for(int c = 0;c<combination;c++){
	 if(debug)printf("combination = %d,%d,%d,%d,%d\n",combination,selected_cluster[0],selected_cluster[1],selected_cluster[2],selected_cluster[3]);
	 double m[4][5];
	 for(int ipl = 0;ipl<4;ipl++){
		double disp = pli[ipl][3];
		double rot = pli[ipl][4]*3.14159265358979323846/180.;
		//		double dl = pli[ipl][5];
		double dispx = disp*cos(rot);
		double dispy = disp*sin(rot);
		double dispz = 0;
		double dirx=-sin(rot);
		double diry=cos(rot);
		double a[3];
		a[0]= pli[ipl][0]+dispx*cluster_pos[ipl][selected_cluster[ipl]];
		a[1]= pli[ipl][1]+dispy*cluster_pos[ipl][selected_cluster[ipl]];
		a[2]= pli[ipl][2]+dispz*cluster_pos[ipl][selected_cluster[ipl]];
		m[ipl][0]=diry;
		m[ipl][1]=-dirx;
		m[ipl][2]=diry*a[2];
		m[ipl][3]=-dirx*a[2];
		m[ipl][4]=diry*a[0]-dirx*a[1];
	 }
	 //calculate the inverse matrix
	 double f;
	 for(int i=3;i>=0;i--){
		for(int j=i-1;j>=0;j--){
		  f = m[j][i]/m[i][i];
		  for(int k=0;k<5;k++){
			 m[j][k]-=m[i][k]*f;
		  }
		}
	 }
	 for(int i=0;i<4;i++){
		for(int j=0;j<i;j++){
		  m[i][4]-=m[i][j]*m[j][4];
		}
		m[i][4]/=m[i][i];
	 }
	 double s[4][5];
	 for(int ii=0;ii<4;ii++){
		for(int jj=0;jj<5;jj++){
		  s[ii][jj]=m[ii][jj];
		}
	 }

	 double dx = s[2][4];
	 double dy =s[3][4];
	 double ares[4] = {1.05*3.14159265358979323846/180., 1.97*3.14159265358979323846/180., 1.97*3.14159265358979323846/180., 1.05*3.14159265358979323846/180.}; 
	 double chi2=0;


	 double tempx=s[0][4];
	 double tempy=s[1][4];
	 double tempa=s[2][4];
	 double tempb=s[3][4];
// cross check
	 double posx1 = (cluster_pos[0][selected_cluster[0]] - pli[0][0]/6.)*(-6);
	 double posu1 = (cluster_pos[1][selected_cluster[1]] - pli[1][0]/6.)*(-6);
	 double posx2 = (cluster_pos[2][selected_cluster[2]] - pli[2][0]/6.)*(-6);
	 double posu2 = (cluster_pos[3][selected_cluster[3]] - pli[3][0]/6.)*(-6);

	 double temp2x =posx1;
	 double temp2a =(posx2-posx1)/(pli[2][2]-pli[0][2]); 
	 double temp2b = (tempa - (posu2-posu1)/(pli[3][2]-pli[1][2]))/tan(-48.19/180.*3.141592);
	 double temp2y = (tempx + (pli[1][2]*posu2-pli[3][2]*posu1)/(pli[3][2]-pli[1][2]))/tan(-48.19/180.*3.141592);


	 if(debug)	 printf("x=%lf,y=%lf,a=%lf,b=%lf\n",tempx,tempy,tempa,tempb);
///	 	 printf("x=%lf,y=%lf,a=%lf,b=%lf\n",tempx,tempy,tempa,tempb);
//	 	 printf("x2=%lf,y2=%lf,a2=%lf,b2=%lf\n",temp2x,temp2y,temp2a,temp2b);
	 double SSR=0;
	 double sumres=0;

	 double res[4][50];
	 for(int i=0;i<4;i++){
		double aangle=pli[i][4];
		double space = -pli[i][3];
		double z= pli[i][2];
		for(int jk=0;jk<cluster_size[i][selected_cluster[i]];jk++){
		  res[i][jk] = ((pli[i][0]/6.0-id[i][selected_cluster[i]][jk])*space-((tempx+tempa*z)*cos(aangle*3.141592/180.)+(tempy+tempb*z)*sin(aangle*3.141592/180.)))/(tempa*cos(aangle*3.141592/180.)+tempb*sin(aangle*3.141592/180.)) - dl[i][selected_cluster[i]][jk]*10;
		  SSR+=res[i][jk]*res[i][jk];
		  sumres+=res[i][jk];
		  		  if(debug)printf("id=%d,dl=%lf,res[%d][%d]=%lf\n",id[i][selected_cluster[i]][jk],dl[i][selected_cluster[i]][jk]*10,i,jk,res[i][jk]);
		  //printf("id=%d,dl=%lf,res[%d][%d]=%lf\n",id[i][selected_cluster[i]][jk],dl[i][selected_cluster[i]][jk]*10,i,jk,res[i][jk]);
		}
	 }
	 //printf("SSR=%lf\n",SSR);
	 if(SSR<fSSR){
		fSSR=SSR;
		bestsumres=sumres;
		bestx=tempx;
		besta=tempa;
		bestb=tempb;
		besty=tempy;
		for(int i=0;i<4;i++){
		  for(int jk=0;jk<cluster_size[i][selected_cluster[i]];jk++){
			 bestres[i][jk]=res[i][jk];
			 bestdl[i][jk]=dl[i][selected_cluster[i]][jk]*10;
			 bestid[i][jk]=id[i][selected_cluster[i]][jk]*10;
		  }
		  if(debug)	  printf("selected_cluster[%d]=%d\n",i,selected_cluster[i]);
		  best_clust_size[i]=cluster_size[i][selected_cluster[i]];
		}
	 }

	 for(int i=0;i<4;i++){
		if(selected_cluster[i]==valid_cluster[i]-1){
		  selected_cluster[i]=0;
		  continue;
		}else{
		  selected_cluster[i]=selected_cluster[i]+1;
		  break;
		}
	 }
  } 
  //		  trackingResult->SetTrack(bestx,besty,0,atan(besta)+3.14159265/4.,atan(bestb/sqrt(2)));//sqrt(2)は回転のぶん
  trackingResult->SetTrack(bestx,besty,0,(atan(besta)+3.14159265/4.)*180./3.141592,atan(bestb/sqrt(2))/3.141592*180.);//sqrt(2)は回転のぶん
  trackingResult->SetSSR(fSSR);
  trackingResult->setsumres(bestsumres);
  if(debug)printf("bestSSR=%lf\n",fSSR);
  for(int i=0;i<4;i++){
	 if(debug)  printf("best_clust_size[%d]=%d\n",i,best_clust_size[i]);
	 for(int jk=0;jk<best_clust_size[i];jk++){
		trackingResult->setvdcres(i,jk,bestres[i][jk]);
		trackingResult->setvdcadl(i,jk,bestdl[i][jk]);
	 }
	 trackingResult->setclustN(i,best_clust_size[i]);
  }
  if(debug)printf("tracking proceesor end\n\n");
}
