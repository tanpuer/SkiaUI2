#include "JSViewGroupBinding.h"
#include "ViewGroup.h"
#include "w3c_util.h"

v8::Local<v8::FunctionTemplate>
JSViewGroupBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                                   v8::Local<v8::FunctionTemplate> inherit,
                                   v8::Local<v8::External> external) {
    auto viewGroupConstructor = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        SkASSERT(args.IsConstructCall() && args.Length() == 0);
        auto data = v8::Local<v8::External>::Cast(args.Data());
        auto binding = static_cast<JSViewGroupBinding *>(data->Value());
        SkASSERT(binding);
        auto viewGroup = new ViewGroup();
        viewGroup->setContext(binding->context);
        args.This()->SetInternalField(0, v8::External::New(args.GetIsolate(), viewGroup));
        args.GetReturnValue().Set(args.This());
    };
    auto addView = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        auto isolate = args.GetIsolate();
        assert(args.Length() == 1 && args[0]->IsObject());
        auto wrap = v8::Local<v8::External>::Cast(args.Holder()->GetInternalField(0));
        auto parent = static_cast<ViewGroup *>(wrap->Value());
        auto childWrap = v8::Local<v8::External>::Cast(
                args[0]->ToObject()->GetInternalField(0));
        auto child = static_cast<View *>(childWrap->Value());
        parent->addView(child);
    };
    auto viewGroupTemplate = v8::FunctionTemplate::New(isolate, viewGroupConstructor, external);
    viewGroupTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    viewGroupTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "ViewGroup"));
    viewGroupTemplate->Inherit(inherit);
    viewGroupTemplate->PrototypeTemplate()->Set(
            isolate, "addView",
            v8::FunctionTemplate::New(isolate, addView));
    auto flexWrapSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                             const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsString()) {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(),
                                                 "Invalid value for flexWrap; expected a String");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
            return;
        }
        auto viewGroup = static_cast<ViewGroup *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (viewGroup) {
            v8::String::Utf8Value utf8(info.GetIsolate(), value);
            auto flexWrap = std::string(*utf8, utf8.length());
            viewGroup->setFlexWrap(W3CToYGWrap(flexWrap));
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    auto flexWrapGetter = [](v8::Local<v8::String> property,
                             const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto viewGroup = static_cast<ViewGroup *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (viewGroup) {
            info.GetReturnValue().Set(
                    v8::String::NewFromUtf8(info.GetIsolate(), viewGroup->getFLexWrap()));
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    viewGroupTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "flexWrap"),
                                                       flexWrapGetter, flexWrapSetter);

    auto justifyContentSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                                   const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsString()) {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(),
                                                 "Invalid value for justifyContent; expected a String");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
            return;
        }
        auto viewGroup = static_cast<ViewGroup *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (viewGroup) {
            v8::String::Utf8Value utf8(info.GetIsolate(), value);
            auto justify = std::string(*utf8, utf8.length());
            viewGroup->setJustifyContent(W3CToYGJustify(justify));
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    auto justifyContentGetter = [](v8::Local<v8::String> property,
                                   const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto viewGroup = static_cast<ViewGroup *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (viewGroup) {
            info.GetReturnValue().Set(
                    v8::String::NewFromUtf8(info.GetIsolate(), viewGroup->getJustifyContent()));
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    viewGroupTemplate->InstanceTemplate()->SetAccessor(
            v8::String::NewFromUtf8(isolate, "justifyContent"), justifyContentGetter,
            justifyContentSetter);

    auto alignItemsSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                               const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsString()) {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(),
                                                 "Invalid value for alignItems; expected a String");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
            return;
        }
        auto viewGroup = static_cast<ViewGroup *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (viewGroup) {
            v8::String::Utf8Value utf8(info.GetIsolate(), value);
            auto align = std::string(*utf8, utf8.length());
            viewGroup->setAlignItems(W3CToYGAlign(align));
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    auto alignItemsGetter = [](v8::Local<v8::String> property,
                               const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto viewGroup = static_cast<ViewGroup *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (viewGroup) {
            info.GetReturnValue().Set(
                    v8::String::NewFromUtf8(info.GetIsolate(), viewGroup->getAlignItems()));
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    viewGroupTemplate->InstanceTemplate()->SetAccessor(
            v8::String::NewFromUtf8(isolate, "alignItems"), alignItemsGetter,
            alignItemsSetter);

    auto flexDirectionSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                                  const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsString()) {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(),
                                                 "Invalid value for flexDirection; expected a String");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
            return;
        }
        auto viewGroup = static_cast<ViewGroup *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (viewGroup) {
            v8::String::Utf8Value utf8(info.GetIsolate(), value);
            auto direction = std::string(*utf8, utf8.length());
            viewGroup->setFlexDirection(W3CToYGFlexDirection(direction));
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    auto flexDirectionGetter = [](v8::Local<v8::String> property,
                                  const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto viewGroup = static_cast<ViewGroup *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (viewGroup) {
            info.GetReturnValue().Set(
                    v8::String::NewFromUtf8(info.GetIsolate(), viewGroup->getFlexDirection()));
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    viewGroupTemplate->InstanceTemplate()->SetAccessor(
            v8::String::NewFromUtf8(isolate, "flexDirection"),
            flexDirectionGetter,
            flexDirectionSetter);

    skiaUI->Set(v8::String::NewFromUtf8(isolate, "ViewGroup"), viewGroupTemplate->GetFunction());
    return viewGroupTemplate;
}
