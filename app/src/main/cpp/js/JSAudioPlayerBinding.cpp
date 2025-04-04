#include "JSAudioPlayerBinding.h"
#include "JSAudioPlayer.h"

namespace HYSkiaUI {

v8::Local<v8::FunctionTemplate>
JSAudioPlayerBinding::registerJSView(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI,
                                     v8::Local<v8::FunctionTemplate> inherit,
                                     v8::Local<v8::External> external) {
    auto audioPlayerConstructor = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        SkASSERT(args.IsConstructCall() && args.Length() == 1);
        auto data = v8::Local<v8::External>::Cast(args.Data());
        auto binding = static_cast<JSAudioPlayerBinding *>(data->Value());
        auto path = stdString(args.GetIsolate(), args[0]);
        SkASSERT(binding);
        auto audioPlayer = new JSAudioPlayer(binding->context, path);
        args.This()->SetInternalField(0, v8::External::New(args.GetIsolate(), audioPlayer));
        args.GetReturnValue().Set(args.This());
    };
    auto audioPlayerTemplate = v8::FunctionTemplate::New(isolate, audioPlayerConstructor, external);
    audioPlayerTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    audioPlayerTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "AudioPlayer"));

    auto start = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        assert(args.Length() == 0);
        auto audioPlayer = GetTargetView<JSAudioPlayer>(args);
        if (audioPlayer != nullptr) {
            audioPlayer->start();
        }
    };
    audioPlayerTemplate->PrototypeTemplate()->Set(
            v8::String::NewFromUtf8(isolate, "start"),
            v8::FunctionTemplate::New(isolate, start, v8::External::New(isolate, this)));
    auto pause = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        assert(args.Length() == 0);
        auto audioPlayer = GetTargetView<JSAudioPlayer>(args);
        if (audioPlayer != nullptr) {
            audioPlayer->pause();
        }
    };
    audioPlayerTemplate->PrototypeTemplate()->Set(
            v8::String::NewFromUtf8(isolate, "pause"),
            v8::FunctionTemplate::New(isolate, pause, v8::External::New(isolate, this)));
    auto release = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        assert(args.Length() == 0);
        auto audioPlayer = GetTargetView<JSAudioPlayer>(args);
        if (audioPlayer != nullptr) {
            audioPlayer->release();
        }
    };
    audioPlayerTemplate->PrototypeTemplate()->Set(
            v8::String::NewFromUtf8(isolate, "release"),
            v8::FunctionTemplate::New(isolate, release, v8::External::New(isolate, this)));
    auto getCurrentPosition = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        assert(args.Length() == 0);
        auto audioPlayer = GetTargetView<JSAudioPlayer>(args);
        if (audioPlayer != nullptr) {
            args.GetReturnValue().Set(
                    v8::Number::New(args.GetIsolate(), audioPlayer->getCurrPosition()));
        }
    };
    audioPlayerTemplate->PrototypeTemplate()->Set(
            v8::String::NewFromUtf8(isolate, "getCurrentPosition"),
            v8::FunctionTemplate::New(isolate, getCurrentPosition,
                                      v8::External::New(isolate, this)));
    auto getDuration = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        assert(args.Length() == 0);
        auto audioPlayer = GetTargetView<JSAudioPlayer>(args);
        if (audioPlayer != nullptr) {
            args.GetReturnValue().Set(
                    v8::Number::New(args.GetIsolate(), audioPlayer->getDuration()));
        }
    };
    audioPlayerTemplate->PrototypeTemplate()->Set(
            v8::String::NewFromUtf8(isolate, "getDuration"),
            v8::FunctionTemplate::New(isolate, getDuration,
                                      v8::External::New(isolate, this)));
    auto seek = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
        assert(args.Length() == 1 && args[0]->IsNumber());
        auto audioPlayer = GetTargetView<JSAudioPlayer>(args);
        if (audioPlayer != nullptr) {
            audioPlayer->seek(args[0]->Int32Value());
        }
    };
    audioPlayerTemplate->PrototypeTemplate()->Set(
            v8::String::NewFromUtf8(isolate, "seek"),
            v8::FunctionTemplate::New(isolate, seek, v8::External::New(isolate, this)));

    v8::Local<v8::Function> constructor = audioPlayerTemplate->GetFunction();
    skiaUI->Set(v8::String::NewFromUtf8(isolate, "AudioPlayer"), constructor);
    return audioPlayerTemplate;
}

}
