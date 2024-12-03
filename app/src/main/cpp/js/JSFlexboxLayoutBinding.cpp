#include "JSFlexboxLayoutBinding.h"
#include "w3c_util.h"
#include "FlexboxLayout.h"

namespace HYSkiaUI {

v8::Local<v8::FunctionTemplate>
JSFlexboxLayoutBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                                       v8::Local<v8::FunctionTemplate> inherit,
                                       v8::Local<v8::External> external) {
    auto flexboxConstructor = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        SkASSERT(args.IsConstructCall() && args.Length() == 0);
        auto data = v8::Local<v8::External>::Cast(args.Data());
        auto binding = static_cast<JSFlexboxLayoutBinding *>(data->Value());
        SkASSERT(binding);
        auto flexboxLayout = new FlexboxLayout();
        flexboxLayout->setContext(binding->context);
        args.This()->SetInternalField(0, v8::External::New(args.GetIsolate(), flexboxLayout));
        args.GetReturnValue().Set(args.This());
    };
    auto flexboxTemplate = v8::FunctionTemplate::New(isolate, flexboxConstructor, external);
    flexboxTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    flexboxTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "FlexboxLayout"));
    flexboxTemplate->Inherit(inherit);
    auto setFlexboxFlexDirection = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        auto isolate = args.GetIsolate();
        assert(args.Length() == 1 && args[0]->IsString());
        auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
        auto flexboxLayout = static_cast<FlexboxLayout *>(wrap->Value());
        v8::String::Utf8Value utf8(isolate, args[0]);
        auto flexDirection = W3CToYGFlexDirection(std::string(*utf8, utf8.length()));
        flexboxLayout->setFlexDirection(flexDirection);
    };
    flexboxTemplate->InstanceTemplate()->Set(
            isolate, "setFlexDirection",
            v8::FunctionTemplate::New(isolate, setFlexboxFlexDirection));
    skiaUI->Set(v8::String::NewFromUtf8(isolate, "FlexboxLayout"),
                flexboxTemplate->GetFunction());
    return flexboxTemplate;
}

}
