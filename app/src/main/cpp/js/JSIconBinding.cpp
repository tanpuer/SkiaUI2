#include "JSIconBinding.h"
#include "Icon.h"

namespace HYSkiaUI {

v8::Local<v8::FunctionTemplate>
JSIconBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                              v8::Local<v8::FunctionTemplate> inherit,
                              v8::Local<v8::External> external) {
    auto textViewConstructor = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        SkASSERT(args.IsConstructCall() && args.Length() == 0);
        auto data = v8::Local<v8::External>::Cast(args.Data());
        auto binding = static_cast<JSIconBinding *>(data->Value());
        SkASSERT(binding);
        auto icon = new Icon();
        icon->setContext(binding->context);
        args.This()->SetInternalField(0, v8::External::New(args.GetIsolate(), icon));
        args.GetReturnValue().Set(args.This());
    };
    auto iconTemplate = v8::FunctionTemplate::New(isolate, textViewConstructor, external);
    iconTemplate->Inherit(inherit);
    iconTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    iconTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "Icon"));

    auto iconSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                         const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsNumber()) {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(),
                                                 "Invalid value for icon; expected a number");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
            return;
        }
        auto icon = static_cast<Icon *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (icon) {
            icon->setIcon(value->Int32Value());
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    auto iconGetter = [](v8::Local<v8::String> property,
                         const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto icon = static_cast<Icon *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (icon) {
            info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), icon->getIcon()));
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    iconTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "icon"),
                                                  iconGetter, iconSetter);
    auto colorSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                          const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsString()) {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(),
                                                 "Invalid value for text; expected a string");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
            return;
        }
        auto icon = static_cast<Icon *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (icon) {
            v8::String::Utf8Value utf8(value);
            icon->setIconColor(std::string(*utf8, utf8.length()).c_str());
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    auto colorGetter = [](v8::Local<v8::String> property,
                          const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto icon = static_cast<Icon *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (icon) {
            info.GetReturnValue().Set(
                    v8::String::NewFromUtf8(info.GetIsolate(), icon->getIconColor()));
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    iconTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "color"),
                                                  colorGetter, colorSetter);
    auto sizeSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                         const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsNumber()) {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(),
                                                 "Invalid value for textSize; expected a number");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
            return;
        }
        auto icon = static_cast<Icon *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (icon) {
            icon->setIconSize(value->Int32Value());
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    auto sizeGetter = [](v8::Local<v8::String> property,
                         const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto icon = static_cast<Icon *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (icon) {
            info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), icon->getIconSize()));
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    iconTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "size"),
                                                  sizeGetter, sizeSetter);
    v8::Local<v8::Function> constructor = iconTemplate->GetFunction();
    skiaUI->Set(v8::String::NewFromUtf8(isolate, "Icon"), constructor);
    return iconTemplate;
}

}
