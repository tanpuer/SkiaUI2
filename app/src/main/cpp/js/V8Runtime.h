#pragma once

#include "v8.h"
#include "v8-platform.h"
#include "libplatform/libplatform.h"
#include "SkiaUIContext.h"
#include "YGConfig.h"
#include "map"

class V8Runtime {

public:

    V8Runtime() = delete;

    explicit V8Runtime(std::shared_ptr<SkiaUIContext> context);

    V8Runtime(V8Runtime &other) = delete;

    V8Runtime &operator=(V8Runtime &other) = delete;

    ~V8Runtime();

    bool evaluateJavaScript(const std::string &buffer, const std::string &sourceURL);

    v8::Local<v8::Object> global();

    void injectClass(const char *className, v8::FunctionCallback constructorFunc, int fieldCount,
                     std::map<const char *, v8::FunctionCallback> methods, void *any,
                     bool globalTarget = false);

    v8::Local<v8::Object> injectObject(v8::Local<v8::Object> host, const char *name,
                                       std::map<std::string, v8::FunctionCallback> functionMap,
                                       std::map<std::string, std::string> constMap,
                                       void *any = nullptr);

    std::string toStdString(const v8::Local<v8::Value> &string);

private:

    v8::Local<v8::Context> CreateGlobalContext(v8::Isolate *isolate);

    void createGlobalSkiaUIObject();

    bool executeScript(const v8::Local<v8::String> &script, const std::string &sourceURL);

    void ReportException(v8::TryCatch *tryCatch);

private:

    std::shared_ptr<SkiaUIContext> skiaUiContext;
    v8::Isolate *mIsolate = nullptr;
    std::unique_ptr<v8::ArrayBuffer::Allocator> arrayBufferAllocator_;
    v8::Global<v8::Context> mContext;
    std::unique_ptr<v8::Platform> mPlatform;
    v8::Local<v8::Object> skiaUI;
};
