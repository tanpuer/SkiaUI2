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
        v8::String::Utf8Value utf8(value);
        auto type = std::string(*utf8, utf8.length());
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
    v8::Local<v8::Function> constructor = progressBarTemplate->GetFunction();

    auto barColorSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                             const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsString()) {
            throwInvalidError(info.GetIsolate(), "Invalid value for barColor; expected a string");
        }
        auto progressBar = GetTargetView<ProgressBar>(info);
        v8::String::Utf8Value utf8(info.GetIsolate(), value);
        auto hexColor = std::string(*utf8, utf8.length());
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

    skiaUI->Set(v8::String::NewFromUtf8(isolate, "ProgressBar"), constructor);
    return progressBarTemplate;
}

}
