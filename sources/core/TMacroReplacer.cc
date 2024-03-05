/* @file TMacroReplacer.cc
 * @brief MacroReplacer
 *
 * @date Create        : 2019-05-10 13:15:52 JST
 *       Last modified :
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */
#include "TMacroReplacer.h"

#include <algorithm>
#include "TString.h"
#include "TRegexp.h"
#include "yaml-cpp/yaml.h"

using art::TMacroReplacer;

const std::string TMacroReplacer::fgMacroFormat = "[a-zA-Z_]+";

TMacroReplacer::TMacroReplacer(const char *delimiter)
    : fDelimiter(delimiter)
{
}

bool TMacroReplacer::Add(const char *key, const char *value)
{
   if (fMacros.find(key) != fMacros.end())
   {
      printf("TMacroReplacer::Add : key '%s' already exists\n", key);
      return false;
   }
   // @TODO to be replaced when upgraded
   if (!TString(key).Contains(TRegexp("^" + fgMacroFormat + "$")))
   {
      printf("TMacroReplacer::Add : key '%s' contains invalid character\n", key);
      return false;
   }
   fMacros[key] = std::string(value);
   return true;
}

bool TMacroReplacer::Add(const YAML::Node &node)
{

   for (YAML::const_iterator it = node.begin(), itend = node.end(); it != itend; ++it)
   {
      std::string key, value;
      key = it->first.as<std::string>();
      if (it->second.Type() != YAML::NodeType::Scalar)
      {
         printf("TMacroReplacer::Add : key '%s' has non scalar value", key.c_str());
         return false;
      }
      value = it->second.as<std::string>();

      Add(key.c_str(), value.c_str());
   }
   return true;
}

std::string TMacroReplacer::Replace(const std::string &input)
{
   std::string output = input;
   for (Map_t::iterator it = fMacros.begin(), itend = fMacros.end(); it != itend; ++it)
   {
      std::string key = fDelimiter + it->first + fDelimiter;
      std::string value = it->second;
      std::string::size_type pos = std::string::npos;
      while ((pos = output.find(key)) != std::string::npos)
      {
         output.replace(pos, key.length(), value);
      }
   }
   return output;
}

std::vector<std::string> TMacroReplacer::Parse(const std::string &input)
{
   TRegexp reg(fDelimiter + fgMacroFormat + fDelimiter);
   TString str(input);
   Ssiz_t start = 0;
   std::vector<std::string> output;
   while ((start = str.Index(reg, start)) != -1)
   {
      Ssiz_t end = str.Index(TRegexp(fDelimiter), start + 1);
      output.push_back(std::string(TString(str(start + 1, end - start - 1)).Data()));
      start = end + 1;
   }

   std::sort(output.begin(), output.end());
   output.erase(std::unique(output.begin(), output.end()), output.end());
   return output;
}
