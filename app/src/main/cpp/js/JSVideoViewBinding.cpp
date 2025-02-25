#include "JSVideoViewBinding.h"
#include "ExoPlayerView.h"

namespace HYSkiaUI {

v8::Local<v8::FunctionTemplate>
JSVideoViewBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                                   v8::Local<v8::FunctionTemplate> inherit,
                                   v8::Local<v8::External> external) {
    auto videoViewConstructor = MakeJSViewConstructor<ExoPlayerView, JSVideoViewBinding>();
    auto videoTemplate = v8::FunctionTemplate::New(isolate, videoViewConstructor, external);
    videoTemplate->Inherit(inherit);
    videoTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    videoTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "VideoView"));
    auto sourceSetter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value,
                           const v8::PropertyCallbackInfo<void> &info) {
        if (!value->IsString()) {
            throwInvalidError(info.GetIsolate(), "Invalid value for source; expected a string");
        }
        auto videoView = GetTargetView<ExoPlayerView>(info);
        v8::String::Utf8Value utf8(value);
        videoView->setSource(std::string(*utf8, utf8.length()).c_str());
    };
    auto sourceGetter = [](v8::Local<v8::String> property,
                           const v8::PropertyCallbackInfo<v8::Value> &info) {
        auto videoView = GetTargetView<ExoPlayerView>(info);
        info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), videoView->getSource()));
    };
    videoTemplate->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "src"),
                                                   sourceGetter, sourceSetter);
    v8::Local<v8::Function> constructor = videoTemplate->GetFunction();
    skiaUI->Set(v8::String::NewFromUtf8(isolate, "VideoView"), constructor);
    return videoTemplate;
}

}
