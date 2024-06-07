#ifndef INCLUDE_V8_VEX_H_
#define INCLUDE_V8_VEX_H_

#include "v8.h"

namespace v8 {

class V8_EXPORT Vex {
 public:
  static std::vector<uint8_t> CreateVexCode(
      Isolate* isolate, Local<UnboundScript> unbound_script);

  static std::vector<uint8_t> CreateVexCode(
      Isolate* isolate, Local<Function> func);
};

class V8_EXPORT VexPackage {
 public:
  MaybeLocal<UnboundScript> GetScript(Isolate* isolate,
                                      const char* resource_name) const;

  bool Has(const char* resource_name) const;

  static const VexPackage* Load(const uint8_t* data,
                                size_t size);
};

}

#endif // INCLUDE_V8_VEX_H_
