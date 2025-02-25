#pragma once

#include "v8.h"
#include "View.h"
#include "SkiaUIContext.h"
#include "V8Runtime.h"

namespace HYSkiaUI {

class JSBinding {

public:

    template<typename T, typename BindingType>
    auto MakeJSViewConstructor() {
        return [](const v8::FunctionCallbackInfo<v8::Value>& args) {
            SkASSERT(args.IsConstructCall() && args.Length() == 0);
            auto data = v8::Local<v8::External>::Cast(args.Data());
            auto bindingPtr = static_cast<BindingType*>(data->Value());
            SkASSERT(bindingPtr);
            T* instance = new T();
            instance->setContext(bindingPtr->context);
            args.This()->SetInternalField(0, v8::External::New(args.GetIsolate(), instance));
            args.GetReturnValue().Set(args.This());
        };
    }

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
