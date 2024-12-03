#include "JSTextViewBinding.h"
#include "TextView.h"

namespace HYSkiaUI {

v8::Local<v8::FunctionTemplate>
JSTextViewBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                                  v8::Local<v8::FunctionTemplate> inherit,
                                  v8::Local<v8::External> external) {
    auto textViewConstructor = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        SkASSERT(args.IsConstructCall() && args.Length() == 0);
        auto data = v8::Local<v8::External>::Cast(args.Data());
        auto binding = static_cast<JSTextViewBinding *>(data->Value());
        SkASSERT(binding);
        auto textView = new TextView();
        textView->setContext(binding->context);
        args.This()->SetInternalField(0, v8::External::New(args.GetIsolate(), textView));
        args.GetReturnValue().Set(args.This());
    };
    auto textTemplate = v8::FunctionTemplate::New(isolate, textViewConstructor, external);
    textTemplate->Inherit(inherit);
    textTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    textTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "TextView"));
    auto textSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                         const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsString()) {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(),
                                                 "Invalid value for text; expected a string");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
            return;
        }
        auto textView = static_cast<TextView *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (textView) {
            v8::String::Utf8Value utf8(value);
            textView->setText(SkString(std::string(*utf8, utf8.length())));
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    auto textGetter = [](v8::Local<v8::String> property,
                         const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto textView = static_cast<TextView *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (textView) {
            info.GetReturnValue().Set(
                    v8::String::NewFromUtf8(info.GetIsolate(), textView->getText().c_str()));
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    textTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "text"),
                                                  textGetter, textSetter);
    auto textSizeSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                             const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsNumber()) {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(),
                                                 "Invalid value for textSize; expected a number");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
            return;
        }
        auto textView = static_cast<TextView *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (textView) {
            textView->setTextSize(value->Int32Value());
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    auto textSizeGetter = [](v8::Local<v8::String> property,
                             const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto textView = static_cast<TextView *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (textView) {
            info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), textView->getTextSize()));
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    textTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "textSize"),
                                                  textSizeGetter, textSizeSetter);
    v8::Local<v8::Function> constructor = textTemplate->GetFunction();
    skiaUI->Set(v8::String::NewFromUtf8(isolate, "TextView"), constructor);
    return textTemplate;
}

}
