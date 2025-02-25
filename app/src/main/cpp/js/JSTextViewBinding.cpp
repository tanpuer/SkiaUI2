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
            throwInvalidError(info.GetIsolate(), "Invalid value for text; expected a string");
        }
        auto textView = GetTargetView<TextView>(info);
        textView->setText(SkString(stdString(info.GetIsolate(), value)));
    };
    auto textGetter = [](v8::Local<v8::String> property,
                         const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto textView = GetTargetView<TextView>(info);
        info.GetReturnValue().Set(
                v8::String::NewFromUtf8(info.GetIsolate(), textView->getText().c_str()));
    };
    textTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "text"),
                                                  textGetter, textSetter);
    auto textSizeSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                             const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsNumber()) {
            throwInvalidError(info.GetIsolate(), "Invalid value for textSize; expected a number");
        }
        auto textView = GetTargetView<TextView>(info);
        textView->setTextSize(value->Int32Value());
    };
    auto textSizeGetter = [](v8::Local<v8::String> property,
                             const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto textView = GetTargetView<TextView>(info);
        info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), textView->getTextSize()));
    };
    textTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "textSize"),
                                                  textSizeGetter, textSizeSetter);
    auto textColorSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                             const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsString()) {
            throwInvalidError(info.GetIsolate(), "Invalid value for textColor; expected a string");
        }
        auto textView = GetTargetView<TextView>(info);
        textView->setTextColor(stdString(info.GetIsolate(), value));
    };
    auto textColorGetter = [](v8::Local<v8::String> property,
                             const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto textView = GetTargetView<TextView>(info);
        info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), textView->getTextColor()));
    };
    textTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "textColor"),
                                                  textColorGetter, textColorSetter);
    auto setTextGradient = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        auto isolate = args.GetIsolate();
        assert(args.Length() == 2 && args[0]->IsArray() && args[1]->IsArray());
        auto textView = GetTargetView<TextView>(args);
        if (textView != nullptr) {
            auto colorsArray = v8::Local<v8::Array>::Cast(args[0]);
            std::vector<SkColor> colors;
            for (uint32_t i = 0; i < colorsArray->Length(); ++i) {
                v8::Local item = colorsArray->Get(i);
                int r, g, b, a;
                hexToRGBA(stdString(isolate, item), r, g, b, a);
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
