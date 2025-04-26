#include "JSImageViewBinding.h"
#include "ImageView.h"
#include "w3c_util.h"

namespace HYSkiaUI {

v8::Local<v8::FunctionTemplate>
JSImageViewBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                                   v8::Local<v8::FunctionTemplate> inherit,
                                   v8::Local<v8::External> external) {
    auto imageViewConstructor = MakeJSViewConstructor<ImageView, JSImageViewBinding>();
    auto imageTemplate = v8::FunctionTemplate::New(isolate, imageViewConstructor, external);
    imageTemplate->Inherit(inherit);
    imageTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    imageTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "ImageView"));
    auto sourceSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                           const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsString()) {
            throwInvalidError(info.GetIsolate(), "Invalid value for source; expected a string");
        }
        auto imageView = GetTargetView<ImageView>(info);
        imageView->setSource(stdString(info.GetIsolate(), value).c_str());
    };
    auto sourceGetter = [](v8::Local<v8::String> property,
                           const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto imageView = GetTargetView<ImageView>(info);
        info.GetReturnValue().Set(
                v8::String::NewFromUtf8(info.GetIsolate(), imageView->getSource()));
    };
    imageTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "src"),
                                                   sourceGetter, sourceSetter);
    auto objectFitSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                              const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsString()) {
            throwInvalidError(info.GetIsolate(), "Invalid value for objectFit; expected a string");
        }
        auto imageView = GetTargetView<ImageView>(info);
        auto scaleType = W3CToScaleType(stdString(info.GetIsolate(), value).c_str());
        imageView->setScaleType(scaleType);
    };
    auto objectFitGetter = [](v8::Local<v8::String> property,
                              const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto imageView = GetTargetView<ImageView>(info);
        auto scaleType = imageView->getScaleType();
        auto objectFit = scaleTypeToW3c(scaleType);
        info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), objectFit));
    };
    imageTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "objectFill"),
                                                   objectFitGetter, objectFitSetter);
    auto blurSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                         const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsNumber()) {
            throwInvalidError(info.GetIsolate(), "Invalid value for blur; expected a number");
        }
        auto imageView = GetTargetView<ImageView>(info);
        imageView->setBlur(value->NumberValue());
    };
    imageTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "blur"),
                                                   nullptr, blurSetter);
    auto start = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        assert(args.Length() == 0);
        auto imageView = GetTargetView<ImageView>(args);
        imageView->start();
    };
    imageTemplate->PrototypeTemplate()->Set(
            v8::String::NewFromUtf8(isolate, "start"),
            v8::FunctionTemplate::New(isolate, start, v8::External::New(isolate, this)));
    auto pause = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        assert(args.Length() == 0);
        auto imageView = GetTargetView<ImageView>(args);
        imageView->pause();
    };
    imageTemplate->PrototypeTemplate()->Set(
            v8::String::NewFromUtf8(isolate, "pause"),
            v8::FunctionTemplate::New(isolate, pause, v8::External::New(isolate, this)));
    v8::Local<v8::Function> constructor = imageTemplate->GetFunction();
    skiaUI->Set(v8::String::NewFromUtf8(isolate, "ImageView"), constructor);
    return imageTemplate;
}

}
