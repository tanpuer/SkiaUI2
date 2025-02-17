#include "JSVideoViewBinding.h"
#include "ExoPlayerView.h"

namespace HYSkiaUI {

v8::Local<v8::FunctionTemplate>
JSVideoViewBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                                   v8::Local<v8::FunctionTemplate> inherit,
                                   v8::Local<v8::External> external) {
    auto videoViewConstructor = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        SkASSERT(args.IsConstructCall() && args.Length() == 0);
        auto data = v8::Local<v8::External>::Cast(args.Data());
        auto binding = static_cast<JSVideoViewBinding *>(data->Value());
        SkASSERT(binding);
        auto videoView = new ExoPlayerView();
        videoView->setContext(binding->context);
        args.This()->SetInternalField(0, v8::External::New(args.GetIsolate(), videoView));
        args.GetReturnValue().Set(args.This());
    };
    auto videoTemplate = v8::FunctionTemplate::New(isolate, videoViewConstructor, external);
    videoTemplate->Inherit(inherit);
    videoTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    videoTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "VideoView"));
    auto sourceSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                           const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsString()) {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(),
                                                 "Invalid value for source; expected a string");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
            return;
        }
        auto videoView = static_cast<ExoPlayerView *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (videoView) {
            v8::String::Utf8Value utf8(value);
            videoView->setSource(std::string(*utf8, utf8.length()).c_str());
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    auto sourceGetter = [](v8::Local<v8::String> property,
                           const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto videoView = static_cast<ExoPlayerView *>(v8::Local<v8::External>::Cast(
                info.Holder()->GetInternalField(0))->Value());
        if (videoView) {
            info.GetReturnValue().Set(
                    v8::String::NewFromUtf8(info.GetIsolate(), videoView->getSource()));
        } else {
            auto error = v8::String::NewFromUtf8(info.GetIsolate(), "Invalid object");
            info.GetIsolate()->ThrowException(v8::Exception::TypeError(error));
        }
    };
    videoTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "src"),
                                                   sourceGetter, sourceSetter);
    v8::Local<v8::Function> constructor = videoTemplate->GetFunction();
    skiaUI->Set(v8::String::NewFromUtf8(isolate, "VideoView"), constructor);
    return videoTemplate;
}

}
