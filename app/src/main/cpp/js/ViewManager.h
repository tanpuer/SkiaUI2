#pragma once

#include "native_log.h"
#include "memory"
#include "SkiaUIContext.h"
#include "V8Runtime.h"


class ViewManager {

public:

    ViewManager(std::shared_ptr<SkiaUIContext> &context, std::shared_ptr<V8Runtime> &runtime);

    ~ViewManager();

    void registerHYViews();

private:

    std::shared_ptr<SkiaUIContext> context;

    std::shared_ptr<V8Runtime> runtime;

    static inline void createView(const v8::FunctionCallbackInfo<v8::Value> &args, int type);

    v8::Local<v8::FunctionTemplate>
    registerHYView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                   v8::Local<v8::External> external);

    v8::Local<v8::FunctionTemplate>
    registerHYViewGroup(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                        v8::Local<v8::FunctionTemplate> inherit,
                        v8::Local<v8::External> external);

    v8::Local<v8::FunctionTemplate>
    registerHYFlexboxLayout(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                            v8::Local<v8::FunctionTemplate> inherit,
                            v8::Local<v8::External> external);

    v8::Local<v8::FunctionTemplate>
    registerScrollView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                       v8::Local<v8::FunctionTemplate> inherit,
                       v8::Local<v8::External> external);

    v8::Local<v8::FunctionTemplate>
    registerHYLottieView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                         v8::Local<v8::FunctionTemplate> inherit,
                         v8::Local<v8::External> external);

    v8::Local<v8::FunctionTemplate>
    registerHYShaderView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                         v8::Local<v8::FunctionTemplate> inherit,
                         v8::Local<v8::External> external);

};
