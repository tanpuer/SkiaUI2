#pragma once

#include "v8.h"
#include "JavascriptTest.h"

using namespace HYSkiaUI;

auto setTimeout = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 2 || !args[0]->IsFunction() || !args[1]->IsNumber()) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Invalid argument"));
        return;
    }
    v8::Local<v8::Function> function = v8::Local<v8::Function>::Cast(args[0]);
    auto delay = v8::Local<v8::Int32>::Cast(args[1]);
    auto data = v8::Local<v8::External>::Cast(args.Data());
    auto app = static_cast<JavascriptTest *>(data->Value());
    assert(app);
    auto callbackId = app->TIMER_INDEX++;
    app->timerCallbackMap[callbackId] = v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>>(
            isolate, function);
    args.GetReturnValue().Set(callbackId);
    app->getContext()->setTimer([app, callbackId](){
        auto itr = app->timerCallbackMap.find(callbackId);
        if (itr != app->timerCallbackMap.end()) {
            const int argc = 0;
            v8::Local<v8::Value> argv[argc] = {};
            app->getV8Runtime()->performFunction(itr->second, argc, argv);
            app->timerCallbackMap.erase(callbackId);
        }
    }, delay->Int32Value(), false);
};

auto clearTimeout = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 1 || !args[0]->IsNumber()) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Invalid argument"));
        return;
    }
    auto timerId = v8::Local<v8::Int32>::Cast(args[0]);
    auto data = v8::Local<v8::External>::Cast(args.Data());
    auto app = static_cast<JavascriptTest *>(data->Value());
    assert(app);
    app->getContext()->clearTimer(timerId->Int32Value());
};
