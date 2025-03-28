#include "JSShaderViewBinding.h"
#include "ShaderView.h"

namespace HYSkiaUI {

v8::Local<v8::FunctionTemplate>
JSShaderViewBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                                    v8::Local<v8::FunctionTemplate> inherit,
                                    v8::Local<v8::External> external) {
    auto shaderViewConstructor = MakeJSViewConstructor<ShaderView, JSShaderViewBinding>();
    auto shaderTemplate = v8::FunctionTemplate::New(isolate, shaderViewConstructor, external);
    shaderTemplate->Inherit(inherit);
    shaderTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    shaderTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "ShaderView"));
    auto shaderSetPath = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        auto isolate = args.GetIsolate();
        assert(args.Length() == 2 && args[0]->IsString() && args[1]->IsArray());
        auto shaderView = GetTargetView<ShaderView>(args);
        auto path = stdString(isolate, args[0]);
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

}
