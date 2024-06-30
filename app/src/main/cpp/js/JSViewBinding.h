#pragma once

#include "JSBinding.h"

class JSViewBinding : public JSBinding {

public:

    JSViewBinding(std::shared_ptr<SkiaUIContext> &context, std::shared_ptr<V8Runtime> &runtime)
            : JSBinding(context, runtime) {}

    v8::Local<v8::FunctionTemplate>
    registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                   v8::Local<v8::FunctionTemplate> inherit,
                   v8::Local<v8::External> external) override;

protected:

    v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>> clickFunction;

};

