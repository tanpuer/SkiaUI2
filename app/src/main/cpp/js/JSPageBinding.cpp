#include "JSPageBinding.h"
#include "Page.h"

v8::Local<v8::FunctionTemplate>
JSPageBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                              v8::Local<v8::FunctionTemplate> inherit,
                              v8::Local<v8::External> external) {
    auto pageConstructor = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        SkASSERT(args.IsConstructCall() && args.Length() == 0);
        auto data = v8::Local<v8::External>::Cast(args.Data());
        auto binding = static_cast<JSPageBinding *>(data->Value());
        SkASSERT(binding);
        auto page = new Page();
        page->setContext(binding->context);
        args.This()->SetInternalField(0, v8::External::New(args.GetIsolate(), page));
        args.GetReturnValue().Set(args.This());
    };
    auto pageTemplate = v8::FunctionTemplate::New(isolate, pageConstructor, external);
    pageTemplate->Inherit(inherit);
    pageTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    pageTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "Page"));
    skiaUI->Set(v8::String::NewFromUtf8(isolate, "Page"), pageTemplate->GetFunction());
    return pageTemplate;
}
