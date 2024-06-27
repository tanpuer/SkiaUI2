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
    auto lottieSetSource = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        auto isolate = args.GetIsolate();
        assert(args.Length() == 1 && args[0]->IsString());
        auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
        auto lottieView = static_cast<LottieView *>(wrap->Value());
        v8::String::Utf8Value utf8(isolate, args[0]);
        auto path = std::string(*utf8, utf8.length()).c_str();
        lottieView->setSource(path);
    };
    lottieTemplate->PrototypeTemplate()->Set(isolate, "setSource",
                                             v8::FunctionTemplate::New(isolate,
                                                                       lottieSetSource));
    skiaUI->Set(v8::String::NewFromUtf8(isolate, "LottieView"), lottieTemplate->GetFunction());
    return lottieTemplate;
}
