/* @file TArtLocalUtil.h
 * @brief local utility 
 *
 * @date Create        : 2020-07-28 10:36:19 JST
 *       Last Modified : 2020-07-31 19:11:49 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */

#ifndef INCLUDE_GUARD_UUID_8FC4BB9C_6D84_48D2_940A_7EE6CAB978AA
#define INCLUDE_GUARD_UUID_8FC4BB9C_6D84_48D2_940A_7EE6CAB978AA


#include <vector>

class TString;
class TH1;
class TH2;


namespace art {
   namespace Util {
      TString ZeroFilledDigitForm(int maxNum);
      TH1* ScaleX(TH1* hin, double scale, TH1* hout = 0);
      TH2* ShiftCorrectionKT(TH2* h2, std::vector<double>& scales, std::vector<double>& curvature,
                             TH1* href = 0, double kStepMin = 0.1, double kStepMax = 2., int kNumStep = 10);
      int  EdgeScan(const std::vector<double>& content, double thres, double ref);
      
   }
}

#endif // #ifndef INCLUDE_GUARD_UUID_8FC4BB9C_6D84_48D2_940A_7EE6CAB978AA}
