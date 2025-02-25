#include "JSIconBinding.h"
#include "Icon.h"

namespace HYSkiaUI {

v8::Local<v8::FunctionTemplate>
JSIconBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                              v8::Local<v8::FunctionTemplate> inherit,
                              v8::Local<v8::External> external) {
    auto textViewConstructor = MakeJSViewConstructor<Icon, JSIconBinding>();
    auto iconTemplate = v8::FunctionTemplate::New(isolate, textViewConstructor, external);
    iconTemplate->Inherit(inherit);
    iconTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    iconTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "Icon"));

    auto iconSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                         const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsNumber()) {
            throwInvalidError(info.GetIsolate(), "Invalid value for icon; expected a number");
        }
        auto icon = GetTargetView<Icon>(info);
        icon->setIcon(value->Int32Value());
    };
    auto iconGetter = [](v8::Local<v8::String> property,
                         const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto icon = GetTargetView<Icon>(info);
        info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), icon->getIcon()));
    };
    iconTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "icon"),
                                                  iconGetter, iconSetter);
    auto colorSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                          const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsString()) {
            throwInvalidError(info.GetIsolate(), "Invalid value for color; expected a string");
        }
        auto icon = GetTargetView<Icon>(info);
        icon->setIconColor(stdString(info.GetIsolate(), value).c_str());
    };
    auto colorGetter = [](v8::Local<v8::String> property,
                          const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto icon = GetTargetView<Icon>(info);
        info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), icon->getIconColor()));
    };
    iconTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "color"),
                                                  colorGetter, colorSetter);
    auto sizeSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                         const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsNumber()) {
            throwInvalidError(info.GetIsolate(), "Invalid value for size; expected a number");
        }
        auto icon = GetTargetView<Icon>(info);
        icon->setIconSize(value->Int32Value());
    };
    auto sizeGetter = [](v8::Local<v8::String> property,
                         const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto icon = GetTargetView<Icon>(info);
        info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), icon->getIconSize()));
    };
    iconTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "size"),
                                                  sizeGetter, sizeSetter);
    v8::Local<v8::Function> constructor = iconTemplate->GetFunction();
    skiaUI->Set(v8::String::NewFromUtf8(isolate, "Icon"), constructor);
    return iconTemplate;
}

}
