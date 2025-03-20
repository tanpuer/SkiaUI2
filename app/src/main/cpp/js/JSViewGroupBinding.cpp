#include "JSViewGroupBinding.h"
#include "ViewGroup.h"
#include "w3c_util.h"

namespace HYSkiaUI {

v8::Local<v8::FunctionTemplate>
JSViewGroupBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                                   v8::Local<v8::FunctionTemplate> inherit,
                                   v8::Local<v8::External> external) {
    auto viewGroupConstructor = MakeJSViewConstructor<ViewGroup, JSViewGroupBinding>();
    auto viewGroupTemplate = v8::FunctionTemplate::New(isolate, viewGroupConstructor, external);
    viewGroupTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    viewGroupTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "ViewGroup"));
    viewGroupTemplate->Inherit(inherit);
    auto addView = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        assert(args.Length() == 1 && args[0]->IsObject());
        auto parent = GetTargetView<ViewGroup>(args);
        auto childWrap = v8::Local<v8::External>::Cast(
                args[0]->ToObject()->GetInternalField(0));
        auto child = static_cast<View *>(childWrap->Value());
        parent->addView(child);
    };
    viewGroupTemplate->PrototypeTemplate()->Set(
            isolate, "addView",
            v8::FunctionTemplate::New(isolate, addView));
    auto addViewBefore = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        assert(args.Length() == 2);
        auto parent = GetTargetView<ViewGroup>(args);
        auto childWrap = v8::Local<v8::External>::Cast(
                args[0]->ToObject()->GetInternalField(0));
        auto beforeChildWrap = v8::Local<v8::External>::Cast(
                args[1]->ToObject()->GetInternalField(0));
        auto child = static_cast<View *>(childWrap->Value());
        auto beforeChild = static_cast<View *>(beforeChildWrap->Value());
        parent->addViewBefore(child, beforeChild);
    };
    viewGroupTemplate->PrototypeTemplate()->Set(
            isolate, "addViewBefore",
            v8::FunctionTemplate::New(isolate, addViewBefore));
    auto removeView = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        assert(args.Length() == 1 && args[0]->IsObject());
        auto parent = GetTargetView<ViewGroup>(args);
        auto childWrap = v8::Local<v8::External>::Cast(
                args[0]->ToObject()->GetInternalField(0));
        auto child = static_cast<View *>(childWrap->Value());
        parent->removeView(child);
    };
    viewGroupTemplate->PrototypeTemplate()->Set(
            isolate, "removeView",
            v8::FunctionTemplate::New(isolate, removeView));
    auto flexWrapSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                             const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsString()) {
            throwInvalidError(info.GetIsolate(), "Invalid value for flexWrap; expected a string");
        }
        auto viewGroup = GetTargetView<ViewGroup>(info);
        auto flexWrap = stdString(info.GetIsolate(), value);
        viewGroup->setFlexWrap(W3CToYGWrap(flexWrap));
    };
    auto flexWrapGetter = [](v8::Local<v8::String> property,
                             const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto viewGroup = GetTargetView<ViewGroup>(info);
        info.GetReturnValue().Set(
                v8::String::NewFromUtf8(info.GetIsolate(), viewGroup->getFLexWrap()));
    };
    viewGroupTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "flexWrap"),
                                                       flexWrapGetter, flexWrapSetter);

    auto justifyContentSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                                   const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsString()) {
            throwInvalidError(info.GetIsolate(),
                              "Invalid value for justifyContent; expected a string");
        }
        auto viewGroup = GetTargetView<ViewGroup>(info);
        auto justify = stdString(info.GetIsolate(), value);
        viewGroup->setJustifyContent(W3CToYGJustify(justify));
    };
    auto justifyContentGetter = [](v8::Local<v8::String> property,
                                   const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto viewGroup = GetTargetView<ViewGroup>(info);
        info.GetReturnValue().Set(
                v8::String::NewFromUtf8(info.GetIsolate(), viewGroup->getJustifyContent()));
    };
    viewGroupTemplate->InstanceTemplate()->SetAccessor(
            v8::String::NewFromUtf8(isolate, "justifyContent"), justifyContentGetter,
            justifyContentSetter);

    auto alignItemsSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                               const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsString()) {
            throwInvalidError(info.GetIsolate(), "Invalid value for alignItems; expected a string");
        }
        auto viewGroup = GetTargetView<ViewGroup>(info);
        auto align = stdString(info.GetIsolate(), value);
        viewGroup->setAlignItems(W3CToYGAlign(align));
    };
    auto alignItemsGetter = [](v8::Local<v8::String> property,
                               const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto viewGroup = GetTargetView<ViewGroup>(info);
        info.GetReturnValue().Set(
                v8::String::NewFromUtf8(info.GetIsolate(), viewGroup->getAlignItems()));
    };
    viewGroupTemplate->InstanceTemplate()->SetAccessor(
            v8::String::NewFromUtf8(isolate, "alignItems"), alignItemsGetter,
            alignItemsSetter);

    auto flexDirectionSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                                  const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsString()) {
            throwInvalidError(info.GetIsolate(),
                              "Invalid value for flexDirection; expected a string");
        }
        auto viewGroup = GetTargetView<ViewGroup>(info);
        v8::String::Utf8Value utf8(info.GetIsolate(), value);
        auto direction = std::string(*utf8, utf8.length());
        viewGroup->setFlexDirection(W3CToYGFlexDirection(direction));
    };
    auto flexDirectionGetter = [](v8::Local<v8::String> property,
                                  const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto viewGroup = GetTargetView<ViewGroup>(info);
        info.GetReturnValue().Set(
                v8::String::NewFromUtf8(info.GetIsolate(), viewGroup->getFlexDirection()));
    };
    viewGroupTemplate->InstanceTemplate()->SetAccessor(
            v8::String::NewFromUtf8(isolate, "flexDirection"),
            flexDirectionGetter,
            flexDirectionSetter);

    skiaUI->Set(v8::String::NewFromUtf8(isolate, "ViewGroup"), viewGroupTemplate->GetFunction());
    return viewGroupTemplate;
}

}