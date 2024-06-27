#include "JSImageViewBinding.h"
#include "ImageView.h"

v8::Local<v8::FunctionTemplate>
JSImageViewBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                                   v8::Local<v8::FunctionTemplate> inherit,
                                   v8::Local<v8::External> external) {
    auto imageViewConstructor = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        SkASSERT(args.IsConstructCall() && args.Length() == 0);
        auto data = v8::Local<v8::External>::Cast(args.Data());
        auto binding = static_cast<JSImageViewBinding *>(data->Value());
        SkASSERT(binding);
        auto imageView = new ImageView();
        imageView->setContext(binding->context);
        args.This()->SetInternalField(0, v8::External::New(args.GetIsolate(), imageView));
        args.GetReturnValue().Set(args.This());
    };
    auto imageTemplate = v8::FunctionTemplate::New(isolate, imageViewConstructor, external);
    imageTemplate->Inherit(inherit);
    imageTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    imageTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "ImageView"));
    auto sourceSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                           const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsString()) {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(),
                                                 "Invalid value for source; expected a string");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
            return;
        }
        auto imageView = static_cast<ImageView *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (imageView) {
            v8::String::Utf8Value utf8(value);
            imageView->setSource(std::string(*utf8, utf8.length()).c_str());
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    auto sourceGetter = [](v8::Local<v8::String> property,
                           const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto imageView = static_cast<ImageView *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (imageView) {
            info.GetReturnValue().Set(
                    v8::String::NewFromUtf8(info.GetIsolate(), imageView->getSource()));
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    imageTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "src"),
                                                   sourceGetter, sourceSetter);
    v8::Local<v8::Function> constructor = imageTemplate->GetFunction();
    skiaUI->Set(v8::String::NewFromUtf8(isolate, "ImageView"), constructor);
    return imageTemplate;
}
