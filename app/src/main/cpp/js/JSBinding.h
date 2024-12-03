#pragma once

#include "v8.h"
#include "View.h"
#include "SkiaUIContext.h"
#include "V8Runtime.h"

namespace HYSkiaUI {

class JSBinding {

public:

    JSBinding(std::shared_ptr<SkiaUIContext> &context, std::shared_ptr<V8Runtime> &runtime) {
        this->context = context;
        this->runtime = runtime;
    };

    virtual ~JSBinding() {};

    virtual v8::Local<v8::FunctionTemplate>
    registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                   v8::Local<v8::FunctionTemplate> inherit,
                   v8::Local<v8::External> external) = 0;

protected:

    std::shared_ptr<SkiaUIContext> context = nullptr;
    std::shared_ptr<V8Runtime> runtime = nullptr;

};

}
