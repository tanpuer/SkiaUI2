#include "JSScrollViewBinding.h"
#include "ScrollView.h"
#include "w3c_util.h"

namespace HYSkiaUI {

v8::Local<v8::FunctionTemplate>
JSScrollViewBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                                    v8::Local<v8::FunctionTemplate> inherit,
                                    v8::Local<v8::External> external) {
    auto scrollViewConstructor = MakeJSViewConstructor<ScrollView, JSScrollViewBinding>();
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
    auto getDistanceByIndex = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        assert(args.Length() == 1 && args[0]->IsNumber());
        auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
        auto scrollView = static_cast<ScrollView *>(wrap->Value());
        if (scrollView != nullptr) {
            args.GetReturnValue().Set(
                    v8::Number::New(args.GetIsolate(),
                                    scrollView->getDistanceByIndex(args[0]->Int32Value())));
        }
    };
    scrollTemplate->PrototypeTemplate()->Set(
            v8::String::NewFromUtf8(isolate, "getDistanceByIndex"),
            v8::FunctionTemplate::New(isolate, getDistanceByIndex,
                                      v8::External::New(isolate, this)));
    auto scrollTo = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        assert(args.Length() == 1 && args[0]->IsNumber());
        auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
        auto scrollView = static_cast<ScrollView *>(wrap->Value());
        if (scrollView != nullptr) {
            scrollView->scrollTo(args[0]->NumberValue());
        }
    };
    scrollTemplate->PrototypeTemplate()->Set(
            v8::String::NewFromUtf8(isolate, "scrollTo"),
            v8::FunctionTemplate::New(isolate, scrollTo, v8::External::New(isolate, this)));
    skiaUI->Set(v8::String::NewFromUtf8(isolate, "ScrollView"), scrollTemplate->GetFunction());
    return scrollTemplate;
}

}
