#pragma once

#include "JSBinding.h"

namespace HYSkiaUI {

class JSVideoViewBinding : public JSBinding {

public:

    explicit JSVideoViewBinding(std::shared_ptr<SkiaUIContext> &context): JSBinding(context) {}

    v8::Local<v8::FunctionTemplate>
    registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                   v8::Local<v8::FunctionTemplate> inherit,
                   v8::Local<v8::External> external) override;

};

}
