#pragma once

#include "v8.h"
#include "v8-platform.h"
#include "libplatform/libplatform.h"
#include "YGConfig.h"
#include "map"

namespace HYSkiaUI {

class V8Runtime {

public:
    V8Runtime();

    V8Runtime(V8Runtime &other) = delete;

    V8Runtime &operator=(V8Runtime &other) = delete;

    ~V8Runtime();

    v8::Local<v8::Value>
    evaluateJavaScript(const std::string &buffer, const std::string &sourceURL);

    v8::Local<v8::Object> global();

    v8::Local<v8::Object> injectObject(v8::Local<v8::Object> host, const char *name,
                                       std::map<std::string, v8::FunctionCallback> functionMap,
                                       std::map<std::string, std::string> constMap,
                                       void *any = nullptr);

    std::string toStdString(const v8::Local<v8::Value> &string);

    void enterContext(const std::function<void(v8::Isolate *isolate,
                                               v8::Local<v8::Object> skiaUI)> &callback);

    v8::Local<v8::Value> callFunction(const char *func, int argc, v8::Local<v8::Value> *argv);

    v8::Local<v8::External> createExternal(void *any);

    void injectFunction(const char *name, v8::FunctionCallback callback, void *any);

    void injectFunctionToSkiaUI(const char *name, v8::FunctionCallback callback, void *any);

    v8::Local<v8::Value>
    performFunction(
            v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>> function,
            int argc, v8::Local<v8::Value> argv[]);

    void injectNumber(const char *name, int number);

    v8::Platform *getPlatform();

private:

    v8::Local<v8::Context> CreateGlobalContext(v8::Isolate *isolate);

    v8::Local<v8::Value>
    executeScript(const v8::Local<v8::String> &script, const std::string &sourceURL);

    void ReportException(v8::TryCatch *tryCatch);

private:
    v8::Isolate *mIsolate = nullptr;
    v8::Global<v8::Context> mContext;
    v8::Global<v8::Object> skiaUI;
};

}
