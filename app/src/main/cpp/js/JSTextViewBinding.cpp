#include "JSTextViewBinding.h"
#include "TextView.h"
#include "color_util.h"

namespace HYSkiaUI {

v8::Local<v8::FunctionTemplate>
JSTextViewBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                                  v8::Local<v8::FunctionTemplate> inherit,
                                  v8::Local<v8::External> external) {
    auto textViewConstructor = MakeJSViewConstructor<TextView, JSTextViewBinding>();
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
    auto textColorSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                             const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsString()) {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(),
                                                 "Invalid value for textColor; expected a string");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
            return;
        }
        auto textView = static_cast<TextView *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (textView) {
            v8::String::Utf8Value utf8(value);
            textView->setTextColor(std::string(*utf8, utf8.length()));
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    auto textColorGetter = [](v8::Local<v8::String> property,
                             const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto textView = static_cast<TextView *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (textView) {
            info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), textView->getTextColor()));
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    textTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "textColor"),
                                                  textColorGetter, textColorSetter);
    auto setTextGradient = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        auto isolate = args.GetIsolate();
        assert(args.Length() == 2 && args[0]->IsArray() && args[1]->IsArray());
        auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
        auto textView = static_cast<TextView *>(wrap->Value());
        if (textView != nullptr) {
            auto colorsArray = v8::Local<v8::Array>::Cast(args[0]);
            std::vector<SkColor> colors;
            for (uint32_t i = 0; i < colorsArray->Length(); ++i) {
                v8::Local item = colorsArray->Get(i);
                v8::String::Utf8Value utf8(isolate, item);
                int r, g, b, a;
                hexToRGBA(std::string(*utf8, utf8.length()), r, g, b, a);
                colors.push_back(SkColorSetARGB(a, r, g, b));
            }
            auto posArray = v8::Local<v8::Array>::Cast(args[1]);
            std::vector<float> positions;
            for (uint32_t i = 0; i < posArray->Length(); ++i) {
                v8::Local item = posArray->Get(i);
                positions.push_back(item->NumberValue());
            }
            textView->setTextGradient(colors, positions);
        }
    };
    textTemplate->PrototypeTemplate()->Set(
            v8::String::NewFromUtf8(isolate, "setTextGradient"),
            v8::FunctionTemplate::New(isolate, setTextGradient, v8::External::New(isolate, this)));
    v8::Local<v8::Function> constructor = textTemplate->GetFunction();
    skiaUI->Set(v8::String::NewFromUtf8(isolate, "TextView"), constructor);
    return textTemplate;
}

}
