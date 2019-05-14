/* @file TMacroReplacer.h
 * @brief MacroReplacer
 *
 * @date Create        : 2019-05-10 13:15:52 JST
 *       Last modified : 
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */


#ifndef INCLUDE_GUARD_UUID_81659E06_4243_439A_92B5_38CFBB0419DB
#define INCLUDE_GUARD_UUID_81659E06_4243_439A_92B5_38CFBB0419DB

#include <string>
#include <map>
#include <vector>

namespace art {
   class TMacroReplacer;
}

class art::TMacroReplacer {
public:
   TMacroReplacer(const char* delimiter = "@");
   virtual ~TMacroReplacer() {;}
   

   std::string operator() (const std::string& input);
   
   typedef std::map<std::string,std::string> Map_t;

   static const std::string fgMacroFormat;

   // add macro
   bool Add(const char* key, const char* value);
   // replace macro
   std::string Replace(const std::string& input);
   // find the macro
   std::vector<std::string> Parse(const std::string& input);
   

protected:
   Map_t fMacros;
   std::string fDelimiter;



}; // end of TMacroReplacer

#endif // #ifndef INCLUDE_GUARD_UUID_81659E06_4243_439A_92B5_38CFBB0419DB}
