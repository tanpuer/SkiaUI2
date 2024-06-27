#include "JSViewGroupBinding.h"
#include "ViewGroup.h"
#include "w3c_util.h"

v8::Local<v8::FunctionTemplate>
JSViewGroupBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                                   v8::Local<v8::FunctionTemplate> inherit,
                                   v8::Local<v8::External> external) {
    auto viewGroupConstructor = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        SkASSERT(args.IsConstructCall() && args.Length() == 0);
        auto data = v8::Local<v8::External>::Cast(args.Data());
        auto binding = static_cast<JSViewGroupBinding *>(data->Value());
        SkASSERT(binding);
        auto viewGroup = new ViewGroup();
        viewGroup->setContext(binding->context);
        args.This()->SetInternalField(0, v8::External::New(args.GetIsolate(), viewGroup));
        args.GetReturnValue().Set(args.This());
    };
    auto addView = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        auto isolate = args.GetIsolate();
        assert(args.Length() == 1 && args[0]->IsObject());
        auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
        auto parent = static_cast<ViewGroup *>(wrap->Value());
        auto childWrap = v8::Local<v8::External>::Cast(
                args[0]->ToObject()->GetInternalField(0));
        auto child = static_cast<View *>(childWrap->Value());
        parent->addView(child);
    };
    auto setFlexWrap = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        auto isolate = args.GetIsolate();
        assert(args.Length() == 1 && args[0]->IsString());
        auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
        auto viewGroup = static_cast<ViewGroup *>(wrap->Value());
        v8::String::Utf8Value utf8(isolate, args[0]);
        auto flexWrap = W3CToYGWrap(std::string(*utf8, utf8.length()));
        viewGroup->setFlexWrap(flexWrap);
    };
    auto setFlexDirection = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        auto isolate = args.GetIsolate();
        assert(args.Length() == 1 && args[0]->IsString());
        auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
        auto viewGroup = static_cast<ViewGroup *>(wrap->Value());
        v8::String::Utf8Value utf8(isolate, args[0]);
        auto flexDirection = W3CToYGFlexDirection(std::string(*utf8, utf8.length()));
        viewGroup->setFlexDirection(flexDirection);
    };
    auto viewGroupTemplate = v8::FunctionTemplate::New(isolate, viewGroupConstructor, external);
    viewGroupTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    viewGroupTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "ViewGroup"));
    viewGroupTemplate->Inherit(inherit);
    viewGroupTemplate->PrototypeTemplate()->Set(
            isolate, "addView",
            v8::FunctionTemplate::New(isolate, addView));
    viewGroupTemplate->PrototypeTemplate()->Set(
            isolate, "setFlexWrap",
            v8::FunctionTemplate::New(isolate, setFlexWrap));
    viewGroupTemplate->PrototypeTemplate()->Set(
            isolate, "setFlexDirection",
            v8::FunctionTemplate::New(isolate, setFlexDirection));
    skiaUI->Set(v8::String::NewFromUtf8(isolate, "ViewGroup"),
                viewGroupTemplate->GetFunction());
    return viewGroupTemplate;
}
