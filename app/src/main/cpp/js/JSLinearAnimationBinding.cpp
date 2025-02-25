#include "JSLinearAnimationBinding.h"
#include "LinearAnimator.h"

namespace HYSkiaUI {

v8::Local<v8::FunctionTemplate>
JSLinearAnimationBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                                         v8::Local<v8::FunctionTemplate> inherit,
                                         v8::Local<v8::External> external) {
    auto linearAnimationConstructor = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        SkASSERT(args.IsConstructCall() && args.Length() == 3);
        auto data = v8::Local<v8::External>::Cast(args.Data());
        auto binding = static_cast<JSLinearAnimationBinding *>(data->Value());
        SkASSERT(binding);
        auto view = static_cast<View *>(v8::Local<v8::External>::Cast(
                args[0]->ToObject()->GetInternalField(0))->Value());
        auto start = args[1]->ToNumber()->Value();
        auto end = args[2]->ToNumber()->Value();
        auto animator = new LinearAnimator(view, start, end);
        args.This()->SetInternalField(0, v8::External::New(args.GetIsolate(), animator));
        args.GetReturnValue().Set(args.This());
    };
    auto linearAnimatorTemplate = v8::FunctionTemplate::New(isolate, linearAnimationConstructor,
                                                            external);
    linearAnimatorTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    linearAnimatorTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "LinearAnimator"));
    auto durationSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                             const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsNumber()) {
            throwInvalidError(info.GetIsolate(), "Invalid value for duration; expected a number");
        }
        auto animator = GetTargetView<LinearAnimator>(info);
        animator->setDuration(value->NumberValue());
    };
    linearAnimatorTemplate->InstanceTemplate()->SetAccessor(
            v8::String::NewFromUtf8(isolate, "duration"),
            nullptr, durationSetter);
    auto loopSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                         const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsNumber()) {
            throwInvalidError(info.GetIsolate(), "Invalid value for loop; expected a number");
            return;
        }
        auto animator = GetTargetView<LinearAnimator>(info);
        animator->setLoopCount(value->Int32Value());
    };
    linearAnimatorTemplate->InstanceTemplate()->SetAccessor(
            v8::String::NewFromUtf8(isolate, "loop"),
            nullptr, loopSetter);

    auto start = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        assert(args.Length() == 0);
        auto linearAnimator = GetTargetView<LinearAnimator>(args);
        linearAnimator->start();
    };
    linearAnimatorTemplate->PrototypeTemplate()->Set(
            isolate, "start", v8::FunctionTemplate::New(isolate, start));
    auto pause = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        assert(args.Length() == 0);
        auto linearAnimator = GetTargetView<LinearAnimator>(args);
        linearAnimator->pause();
    };
    linearAnimatorTemplate->PrototypeTemplate()->Set(
            isolate, "pause", v8::FunctionTemplate::New(isolate, pause));
    auto resume = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        assert(args.Length() == 0);
        auto linearAnimator = GetTargetView<LinearAnimator>(args);
        linearAnimator->resume();
    };
    linearAnimatorTemplate->PrototypeTemplate()->Set(
            isolate, "resume", v8::FunctionTemplate::New(isolate, resume));
    auto stop = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        assert(args.Length() == 0);
        auto linearAnimator = GetTargetView<LinearAnimator>(args);
        linearAnimator->stop();
    };
    linearAnimatorTemplate->PrototypeTemplate()->Set(
            isolate, "stop", v8::FunctionTemplate::New(isolate, stop));

    auto setUpdateListener = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        auto isolate = args.GetIsolate();
        assert(args.Length() && args[0]->IsFunction());
        auto callback = args[0].As<v8::Function>();
        auto linearAnimator = GetTargetView<LinearAnimator>(args);
        linearAnimator->jsUpdateCallback = v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>>(
                isolate, callback);
        linearAnimator->setUpdateListener([linearAnimator](View *view, float value) {
            if (linearAnimator->jsUpdateCallback.IsEmpty()) {
                return;
            }
            view->getContext()->getRuntime()->enterContext(
                    [view, linearAnimator, value](v8::Isolate *isolate,
                                                  v8::Local<v8::Object> skiaUI) {
                        v8::Local<v8::Value> argv[1] = {v8::Number::New(isolate, value)};
                        view->getContext()->getRuntime()->performFunction(
                                linearAnimator->jsUpdateCallback, 1, argv);
                    });
        });
    };
    linearAnimatorTemplate->PrototypeTemplate()->Set(
            isolate, "setUpdateListener",
            v8::FunctionTemplate::New(isolate, setUpdateListener));

    v8::Local<v8::Function> constructor = linearAnimatorTemplate->GetFunction();
    skiaUI->Set(v8::String::NewFromUtf8(isolate, "LinearAnimator"), constructor);
    return linearAnimatorTemplate;
}

}
