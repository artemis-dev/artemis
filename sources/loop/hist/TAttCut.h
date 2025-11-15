/**
 * @file   TAttCut.h
 * @brief  Attribute to deal with the collection of TCut
 *
 * @date   Created       : 2014-03-03 16:32:41 JST
 *         Last Modified : 2024-06-11 19:22:18 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_873CBE25_13CF_453C_A1EA_F83A8BE6A9CB
#define INCLUDE_GUARD_UUID_873CBE25_13CF_453C_A1EA_F83A8BE6A9CB

#include <TCut.h>

namespace art {
   class TAttCut;
}

class art::TAttCut {
public:
   TAttCut(const char* cut = "");
   virtual ~TAttCut();

   TAttCut(const TAttCut& rhs);
   TAttCut& operator=(const TAttCut& rhs);

   const TCut& GetCut() const { return fCut; } 
   TCut& GetCut() { return fCut; } 
   void  SetCut(const char* cut = "") { fCut = cut; }
   void AppendCut(const char* cut) { fCut += cut; }

   virtual void Copy(TAttCut &att) const;
   
protected:
   TCut fCut;
private:
   
   ClassDef(TAttCut,1) // Attribute to deal with the collection of TCut
};

#endif // INCLUDE_GUARD_UUID_873CBE25_13CF_453C_A1EA_F83A8BE6A9CB
