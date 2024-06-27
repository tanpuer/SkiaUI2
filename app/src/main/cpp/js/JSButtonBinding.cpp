#include "JSButtonBinding.h"
#include "Button.h"

v8::Local<v8::FunctionTemplate>
JSButtonBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                                v8::Local<v8::FunctionTemplate> inherit,
                                v8::Local<v8::External> external) {
    auto buttonConstructor = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        SkASSERT(args.IsConstructCall() && args.Length() == 0);
        auto data = v8::Local<v8::External>::Cast(args.Data());
        auto binding = static_cast<JSButtonBinding *>(data->Value());
        SkASSERT(binding);
        auto button = new Button();
        button->setContext(binding->context);
        args.This()->SetInternalField(0, v8::External::New(args.GetIsolate(), button));
        args.GetReturnValue().Set(args.This());
    };
    auto buttonTemplate = v8::FunctionTemplate::New(isolate, buttonConstructor, external);
    buttonTemplate->Inherit(inherit);
    buttonTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    buttonTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "Button"));
    v8::Local<v8::Function> constructor = buttonTemplate->GetFunction();
    skiaUI->Set(v8::String::NewFromUtf8(isolate, "Button"), constructor);
    return buttonTemplate;
}
