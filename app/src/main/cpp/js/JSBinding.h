#pragma once

#include "v8.h"
#include "View.h"
#include "SkiaUIContext.h"

class JSBinding {

public:

    JSBinding(std::shared_ptr<SkiaUIContext> &context) {
        this->context = context;
    };

    virtual ~JSBinding() {};

    virtual v8::Local<v8::FunctionTemplate>
    registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                   v8::Local<v8::FunctionTemplate> inherit,
                   v8::Local<v8::External> external) = 0;

protected:

    std::shared_ptr<SkiaUIContext> context = nullptr;

};
