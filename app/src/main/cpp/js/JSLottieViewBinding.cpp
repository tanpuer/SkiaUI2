#include "JSLottieViewBinding.h"
#include "LottieView.h"

namespace HYSkiaUI {

v8::Local<v8::FunctionTemplate>
JSLottieViewBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                                    v8::Local<v8::FunctionTemplate> inherit,
                                    v8::Local<v8::External> external) {
    auto lottieViewConstructor = MakeJSViewConstructor<LottieView, JSLottieViewBinding>();
    auto lottieTemplate = v8::FunctionTemplate::New(isolate, lottieViewConstructor, external);
    lottieTemplate->Inherit(inherit);
    lottieTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    lottieTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "LottieView"));
    auto srcSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                        const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsString()) {
            throwInvalidError(info.GetIsolate(), "Invalid value for src; expected a string");
        }
        auto lottieView = GetTargetView<LottieView>(info);
        v8::String::Utf8Value utf8(info.GetIsolate(), value);
        lottieView->setSource(std::string(*utf8, utf8.length()).c_str());
    };
    auto srcGetter = [](v8::Local<v8::String> property,
                        const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto lottieView = GetTargetView<LottieView>(info);
        info.GetReturnValue().Set(
                v8::String::NewFromUtf8(info.GetIsolate(), lottieView->getSource()));
    };
    lottieTemplate->InstanceTemplate()->SetAccessor(
            v8::String::NewFromUtf8(isolate, "src"),
            srcGetter,
            srcSetter);
    auto start = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        assert(args.Length() == 0);
        auto lottieView = GetTargetView<LottieView>(args);
        lottieView->start();
    };
    lottieTemplate->PrototypeTemplate()->Set(
            v8::String::NewFromUtf8(isolate, "start"),
            v8::FunctionTemplate::New(isolate, start, v8::External::New(isolate, this)));
    auto pause = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        assert(args.Length() == 0);
        auto lottieView = GetTargetView<LottieView>(args);
        lottieView->pause();
    };
    lottieTemplate->PrototypeTemplate()->Set(
            v8::String::NewFromUtf8(isolate, "pause"),
            v8::FunctionTemplate::New(isolate, pause, v8::External::New(isolate, this)));
    skiaUI->Set(v8::String::NewFromUtf8(isolate, "LottieView"), lottieTemplate->GetFunction());
    return lottieTemplate;
}

}
