#include "JSScrollViewBinding.h"
#include "ScrollView.h"
#include "w3c_util.h"

namespace HYSkiaUI {

v8::Local<v8::FunctionTemplate>
JSScrollViewBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                                    v8::Local<v8::FunctionTemplate> inherit,
                                    v8::Local<v8::External> external) {
    auto scrollViewConstructor = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        SkASSERT(args.IsConstructCall() && args.Length() == 0);
        auto data = v8::Local<v8::External>::Cast(args.Data());
        auto binding = static_cast<JSScrollViewBinding *>(data->Value());
        SkASSERT(binding);
        auto scrollView = new ScrollView();
        scrollView->setContext(binding->context);
        args.This()->SetInternalField(0, v8::External::New(args.GetIsolate(), scrollView));
        args.GetReturnValue().Set(args.This());
    };
    auto scrollTemplate = v8::FunctionTemplate::New(isolate, scrollViewConstructor, external);
    scrollTemplate->Inherit(inherit);
    scrollTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    scrollTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "ScrollView"));
    auto setScrollViewFlexDirection = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        auto isolate = args.GetIsolate();
        assert(args.Length() == 1 && args[0]->IsString());
        auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
        auto flexboxLayout = static_cast<FlexboxLayout *>(wrap->Value());
        v8::String::Utf8Value utf8(isolate, args[0]);
        auto flexDirection = W3CToYGFlexDirection(std::string(*utf8, utf8.length()));
        flexboxLayout->setFlexDirection(flexDirection);
    };
    scrollTemplate->InstanceTemplate()->Set(
            isolate, "setFlexDirection",
            v8::FunctionTemplate::New(isolate, setScrollViewFlexDirection));
    skiaUI->Set(v8::String::NewFromUtf8(isolate, "ScrollView"), scrollTemplate->GetFunction());
    return scrollTemplate;
}

}
