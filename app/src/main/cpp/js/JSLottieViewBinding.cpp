#include "JSLottieViewBinding.h"
#include "LottieView.h"

v8::Local<v8::FunctionTemplate>
JSLottieViewBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                                    v8::Local<v8::FunctionTemplate> inherit,
                                    v8::Local<v8::External> external) {
    auto lottieViewConstructor = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        SkASSERT(args.IsConstructCall() && args.Length() == 0);
        auto data = v8::Local<v8::External>::Cast(args.Data());
        auto binding = static_cast<JSLottieViewBinding *>(data->Value());
        SkASSERT(binding);
        auto lottieView = new LottieView();
        lottieView->setContext(binding->context);
        args.This()->SetInternalField(0, v8::External::New(args.GetIsolate(), lottieView));
        args.GetReturnValue().Set(args.This());
    };
    auto lottieTemplate = v8::FunctionTemplate::New(isolate, lottieViewConstructor, external);
    lottieTemplate->Inherit(inherit);
    lottieTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    lottieTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "LottieView"));
    auto srcSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                        const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsString()) {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(),
                                                 "Invalid value for src; expected a number");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
            return;
        }
        auto lottieView = static_cast<LottieView *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (lottieView) {
            v8::String::Utf8Value utf8(info.GetIsolate(), value);
            lottieView->setSource(std::string(*utf8, utf8.length()).c_str());
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    auto srcGetter = [](v8::Local<v8::String> property,
                        const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto lottieView = static_cast<LottieView *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (lottieView) {
            info.GetReturnValue().Set(
                    v8::String::NewFromUtf8(info.GetIsolate(), lottieView->getSource()));
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    lottieTemplate->InstanceTemplate()->SetAccessor(
            v8::String::NewFromUtf8(isolate, "src"),
            srcGetter,
            srcSetter);
    skiaUI->Set(v8::String::NewFromUtf8(isolate, "LottieView"), lottieTemplate->GetFunction());
    return lottieTemplate;
}
