/**
 *
 * {{{ Copyright (C) 2017 The AliOS Project. All rights reserved. }}}
 *
 */

#ifndef INCLUDE_XTS_H_
#define INCLUDE_XTS_H_

#include "v8.h"  // NOLINT(build/include)

#define XTS_MAJOR_VERSION 2
#define XTS_MINOR_VERSION 1
#define XTS_BUILD_NUMBER  0

#define ETSC_MAJOR_VERSION 2
#define ETSC_MINOR_VERSION 1
#define ETSC_BUILD_NUMBER  0

#define XTS_VERSION_STRING \
  V8_S(XTS_MAJOR_VERSION)  \
  "." V8_S(XTS_MINOR_VERSION) "." V8_S(XTS_BUILD_NUMBER)
#define ETSC_VERSION_STRING \
  V8_S(ETSC_MAJOR_VERSION)  \
  "." V8_S(ETSC_MINOR_VERSION) "." V8_S(ETSC_BUILD_NUMBER)

namespace v8 {

class Isolate;
class VexPackage;
/**
 * The Loader to load ta files for xts.
 */
typedef MaybeLocal<String> (*XTSSourceGetterCallback)(Isolate* isolate,
                                                      const char* full_path);

enum XTSCompilationMode {
  kNoneMode,
  kCompileBytecodeMode,
  kCompileNativeCodeMode,
  kCompileHybridCodeMode
};

#ifdef ENABLE_JSAOT_

class V8_EXPORT JsoContext {
 public:
  JsoContext(Isolate* isolate, JsoGenerator& generator, const char* tex_path);
  ~JsoContext();

  void CompileAndSerialize(std::vector<std::pair<Local<String>, Local<String>>>& sources,
                           bool source_wrapped_node);

  void DecodeAndSerialize(const VexPackage* vex,
                          const char* prefix);

  static void DumpAOTCodeCache(Isolate* isolate, std::ostream& os);
  static size_t AOTCodeCacheSize(Isolate* isolate);

  static void DumpJso(Isolate* isolate, std::ostream& os);
 private:
  Isolate* isolate_;
  JsoGenerator& generator_;
  const char* tex_path_;
};
#endif  // ENABLE_JSAOT_

class V8_EXPORT XTSContext {
 public:
  // Set the 'Callback' which will be called by Taload.
  // 'Callback' need return the actual ta file path to TaLoad
  static void RegisterSourceGetterCallback(Isolate* isolate,
                                           XTSSourceGetterCallback callback);
  static void DumpAOTCodeCache(Isolate* isolate, const char* out_file);
  static void LoadAOTCodeCache(Isolate *isolate,
                               const char *in_file,
                               int fd = -1,
                               size_t size = 0,
                               size_t offset = 0);
  static void SetCompilationMode(Isolate* isolate,
                                 const XTSCompilationMode mode);
  static void AOTCompile(Isolate* isolate,
                         const char* tex_path,
                         std::vector<std::pair<Local<String>, Local<String>> >& sources,
                         std::vector<Local<Script>>* compiled_scripts = nullptr,
                         bool need_serialize = true);

  static void SetCrossCompile();
};

}  // namespace v8

#endif  // INCLUDE_XTS_H_

