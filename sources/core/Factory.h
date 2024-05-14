#ifndef FACTORY_H
#define FACTORY_H

namespace art {
   template <typename T> class Factory;
}

template <typename T>
class art::Factory
{
public:
   typedef T* (*factoryMethod)();
   static bool Register(std::string name, factoryMethod method) {
      T::fgRegisteredNames[name] = method;
      return true;
   }
 
   static void CreateAll() {
      for (std::pair<std::string,factoryMethod> pair : T::fgRegisteredNames) {
         T* base = pair.second();
         int id = base->GetID();
         T::fgDecoders[id] = base;
      }
   }
};

#endif // FACTORY_H