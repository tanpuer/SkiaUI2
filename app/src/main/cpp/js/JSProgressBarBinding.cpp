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
            auto error = v8::String::NewFromUtf8(info.GetIsolate(),
                                                 "Invalid value for barType; expected a string");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
            return;
        }
        auto progressBar = static_cast<ProgressBar *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (progressBar) {
            v8::String::Utf8Value utf8(value);
            auto type = std::string(*utf8, utf8.length());
            if (type == "circle") {
                progressBar->setType(ProgressBar::ProgressBarType::CIRCLE);
            } else if (type == "linear") {
                progressBar->setType(ProgressBar::ProgressBarType::LINEAR);
            } else {
                ALOGE("unsupported barType %s", type.c_str());
            }
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    auto barTypeGetter = [](v8::Local<v8::String> property,
                            const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto progressBar = static_cast<ProgressBar *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (progressBar) {
            auto type = progressBar->getType();
            auto barType = "";
            if (type == ProgressBar::ProgressBarType::CIRCLE) {
                barType = "circle";
            } else if (type == ProgressBar::ProgressBarType::LINEAR) {
                barType = "linear";
            }
            info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), barType));
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    progressBarTemplate->InstanceTemplate()->SetAccessor(
            v8::String::NewFromUtf8(isolate, "barType"), barTypeGetter, barTypeSetter);
    v8::Local<v8::Function> constructor = progressBarTemplate->GetFunction();

    auto barColorSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                             const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsString()) {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(),
                                                 "Invalid value for barColor; expected a number");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
            return;
        }
        auto progressBar = static_cast<ProgressBar *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (progressBar) {
            v8::String::Utf8Value utf8(info.GetIsolate(), value);
            auto hexColor = std::string(*utf8, utf8.length());
            progressBar->setBarColor(hexColor);
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    auto barColorGetter = [](v8::Local<v8::String> property,
                             const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto progressBar = static_cast<ProgressBar *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (progressBar) {
            info.GetReturnValue().Set(
                    v8::String::NewFromUtf8(info.GetIsolate(), progressBar->getBarColor()));
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    progressBarTemplate->InstanceTemplate()->SetAccessor(
            v8::String::NewFromUtf8(isolate, "barColor"),
            barColorGetter,
            barColorSetter);

    auto autoModeSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                             const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsBoolean()) {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(),
                                                 "Invalid value for autoMode; expected a number");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
            return;
        }
        auto progressBar = static_cast<ProgressBar *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (progressBar) {
            progressBar->setAutoMode(value->BooleanValue());
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    auto autoModeGetter = [](v8::Local<v8::String> property,
                             const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto progressBar = static_cast<ProgressBar *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (progressBar) {
            info.GetReturnValue().Set(
                    v8::Boolean::New(info.GetIsolate(), progressBar->getAutoMode()));
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    progressBarTemplate->InstanceTemplate()->SetAccessor(
            v8::String::NewFromUtf8(isolate, "autoMode"),
            autoModeGetter,
            autoModeSetter);

    skiaUI->Set(v8::String::NewFromUtf8(isolate, "ProgressBar"), constructor);
    return progressBarTemplate;
}

}
