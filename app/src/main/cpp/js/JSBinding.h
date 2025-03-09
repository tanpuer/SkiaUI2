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
        return [](const v8::FunctionCallbackInfo<v8::Value> &args) {
            SkASSERT(args.IsConstructCall() && args.Length() == 0);
            auto data = v8::Local<v8::External>::Cast(args.Data());
            auto bindingPtr = static_cast<BindingType *>(data->Value());
            SkASSERT(bindingPtr);
            T *instance = new T();
            instance->setContext(bindingPtr->context);
            args.This()->SetInternalField(0, v8::External::New(args.GetIsolate(), instance));
            args.GetReturnValue().Set(args.This());
        };
    }

    static inline void throwInvalidError(v8::Isolate *isolate, const char *msg = "Invalid object") {
        auto error = v8::String::NewFromUtf8(isolate, msg);
        isolate->ThrowException(v8::Exception::TypeError(error));
    }

    static inline std::string stdString(v8::Isolate *isolate, const v8::Local<v8::Value>& value) {
        v8::String::Utf8Value utf8(value);
        return std::string(*utf8, utf8.length()).c_str();
    }

    template<typename T>
    static T* GetTargetView(const v8::PropertyCallbackInfo<v8::Value>& info) {
        auto targetView = static_cast<T*>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        assert(targetView);
        return targetView;
    }

    template<typename T>
    static T* GetTargetView(const v8::PropertyCallbackInfo<void>& info) {
        auto targetView = static_cast<T*>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        assert(targetView);
        return targetView;
    }

    template<typename T>
    static T* GetTargetView(const v8::FunctionCallbackInfo<v8::Value> &args) {
        auto targetView = static_cast<T*>(v8::Local<v8::External>::Cast(
                args.Holder()->GetInternalField(0))->Value());
        assert(targetView);
        return targetView;
    }

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

}
