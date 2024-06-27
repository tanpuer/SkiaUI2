#include "JSShaderViewBinding.h"
#include "ShaderView.h"

v8::Local<v8::FunctionTemplate>
JSShaderViewBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                                    v8::Local<v8::FunctionTemplate> inherit,
                                    v8::Local<v8::External> external) {
    auto shaderViewConstructor = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        SkASSERT(args.IsConstructCall() && args.Length() == 0);
        auto data = v8::Local<v8::External>::Cast(args.Data());
        auto binding = static_cast<JSShaderViewBinding *>(data->Value());
        SkASSERT(binding);
        auto shaderView = new ShaderView();
        shaderView->setContext(binding->context);
        args.This()->SetInternalField(0, v8::External::New(args.GetIsolate(), shaderView));
        args.GetReturnValue().Set(args.This());
    };
    auto shaderTemplate = v8::FunctionTemplate::New(isolate, shaderViewConstructor, external);
    shaderTemplate->Inherit(inherit);
    shaderTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    shaderTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "ShaderView"));
    auto shaderSetPath = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        auto isolate = args.GetIsolate();
        assert(args.Length() == 2 && args[0]->IsString() && args[1]->IsArray());
        auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
        auto shaderView = static_cast<ShaderView *>(wrap->Value());
        v8::String::Utf8Value utf8(isolate, args[0]);
        auto path = std::string(*utf8, utf8.length());
        v8::Local<v8::Array> inputArray = v8::Local<v8::Array>::Cast(args[1]);
        uint32_t arrayLength = inputArray->Length();
        std::vector<std::string> images;
        images.reserve(arrayLength);
        for (uint32_t i = 0; i < arrayLength; ++i) {
            auto jsElement = inputArray->Get(isolate->GetCurrentContext(), i).ToLocalChecked();
            if (jsElement->IsString()) {
                v8::String::Utf8Value utf8String(isolate, jsElement);
                images.emplace_back(*utf8String, utf8String.length());
            } else {
                ALOGE("setShaderPath images must be string")
            }
        }
        shaderView->setShaderPath(path.c_str(), images);
    };
    shaderTemplate->PrototypeTemplate()->Set(isolate, "setShaderPath",
                                             v8::FunctionTemplate::New(isolate, shaderSetPath));
    skiaUI->Set(v8::String::NewFromUtf8(isolate, "ShaderView"), shaderTemplate->GetFunction());
    return shaderTemplate;
}
