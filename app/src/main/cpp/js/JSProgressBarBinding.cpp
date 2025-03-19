#include "JSProgressBarBinding.h"
#include "ProgressBar.h"

namespace HYSkiaUI {

v8::Local<v8::FunctionTemplate>
JSProgressBarBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                                     v8::Local<v8::FunctionTemplate> inherit,
                                     v8::Local<v8::External> external) {
    auto progressBarConstructor = MakeJSViewConstructor<ProgressBar, JSProgressBarBinding>();
    auto progressBarTemplate = v8::FunctionTemplate::New(isolate, progressBarConstructor, external);
    progressBarTemplate->Inherit(inherit);
    progressBarTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    progressBarTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "ProgressBar"));
    auto barTypeSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                            const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsString()) {
            throwInvalidError(info.GetIsolate(), "Invalid value for barType; expected a string");
        }
        auto progressBar = GetTargetView<ProgressBar>(info);
        auto type = stdString(info.GetIsolate(), value);
        if (type == "circle") {
            progressBar->setType(ProgressBar::ProgressBarType::CIRCLE);
        } else if (type == "linear") {
            progressBar->setType(ProgressBar::ProgressBarType::LINEAR);
        } else {
            ALOGE("unsupported barType %s", type.c_str());
        }
    };
    auto barTypeGetter = [](v8::Local<v8::String> property,
                            const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto progressBar = GetTargetView<ProgressBar>(info);
        auto type = progressBar->getType();
        auto barType = "";
        if (type == ProgressBar::ProgressBarType::CIRCLE) {
            barType = "circle";
        } else if (type == ProgressBar::ProgressBarType::LINEAR) {
            barType = "linear";
        }
        info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), barType));
    };
    progressBarTemplate->InstanceTemplate()->SetAccessor(
            v8::String::NewFromUtf8(isolate, "barType"), barTypeGetter, barTypeSetter);

    auto barColorSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                             const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsString()) {
            throwInvalidError(info.GetIsolate(), "Invalid value for barColor; expected a string");
        }
        auto progressBar = GetTargetView<ProgressBar>(info);
        auto hexColor = stdString(info.GetIsolate(), value);
        progressBar->setBarColor(hexColor);
    };
    auto barColorGetter = [](v8::Local<v8::String> property,
                             const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto progressBar = GetTargetView<ProgressBar>(info);
        info.GetReturnValue().Set(
                v8::String::NewFromUtf8(info.GetIsolate(), progressBar->getBarColor()));
    };
    progressBarTemplate->InstanceTemplate()->SetAccessor(
            v8::String::NewFromUtf8(isolate, "barColor"),
            barColorGetter,
            barColorSetter);

    auto autoModeSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                             const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsBoolean()) {
            throwInvalidError(info.GetIsolate(), "Invalid value for barColor; expected a bool");
        }
        auto progressBar = GetTargetView<ProgressBar>(info);
        progressBar->setAutoMode(value->BooleanValue());
    };
    auto autoModeGetter = [](v8::Local<v8::String> property,
                             const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto progressBar = GetTargetView<ProgressBar>(info);
        info.GetReturnValue().Set(v8::Boolean::New(info.GetIsolate(), progressBar->getAutoMode()));
    };
    progressBarTemplate->InstanceTemplate()->SetAccessor(
            v8::String::NewFromUtf8(isolate, "autoMode"),
            autoModeGetter,
            autoModeSetter);

    auto progressSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                             const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsNumber()) {
            throwInvalidError(info.GetIsolate(), "Invalid value for progress; expected a number");
        }
        auto progressBar = GetTargetView<ProgressBar>(info);
        progressBar->setProgress(value->NumberValue());
    };
    auto progressGetter = [](v8::Local<v8::String> property,
                             const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto progressBar = GetTargetView<ProgressBar>(info);
        info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), progressBar->getProgress()));
    };
    progressBarTemplate->InstanceTemplate()->SetAccessor(
            v8::String::NewFromUtf8(isolate, "progress"),
            progressGetter,
            progressSetter);

    auto progressChange = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        auto isolate = args.GetIsolate();
        assert(args.Length() == 1 && (args[0]->IsFunction() || args[0]->IsNullOrUndefined()));
        auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
        auto targetView = static_cast<ProgressBar *>(wrap->Value());
        auto function = v8::Local<v8::Function>::Cast(args[0]);
        auto data = v8::Local<v8::External>::Cast(args.Data());
        auto binding = static_cast<JSProgressBarBinding *>(data->Value());
        if (args[0]->IsNullOrUndefined()) {
            targetView->progressFunction.Reset();
            targetView->setProgressCallback(nullptr);
            return;
        }
        auto newCallback = v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>>(
                isolate, function);
        targetView->progressFunction.Reset(isolate, newCallback);
        assert(binding);
        targetView->setProgressCallback([binding, targetView](int progress, bool finished) {
            binding->context->getRuntime()->enterContext(
                    [targetView, progress, finished](v8::Isolate *isolate,
                                                     v8::Local<v8::Object> skiaUI) {
                        v8::Local<v8::Value> argv[2] = {
                                v8::Number::New(isolate, progress),
                                v8::Boolean::New(isolate, finished)
                        };
                        auto callback = targetView->progressFunction.Get(isolate);
                        if (!callback.IsEmpty()) {
                            callback->Call(isolate->GetCurrentContext(),
                                           isolate->GetCurrentContext()->Global(), 2, argv);
                        } else {
                            ALOGE("error: miss js progress callback for ProgressBar");
                        }
                    });
        });
    };
    progressBarTemplate->PrototypeTemplate()->Set(
            v8::String::NewFromUtf8(isolate, "setProgressCallback"),
            v8::FunctionTemplate::New(isolate, progressChange, v8::External::New(isolate, this)));

    v8::Local<v8::Function> constructor = progressBarTemplate->GetFunction();
    skiaUI->Set(v8::String::NewFromUtf8(isolate, "ProgressBar"), constructor);
    return progressBarTemplate;
}

}
