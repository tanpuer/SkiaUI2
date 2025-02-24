#pragma once

#include "v8.h"
#include "JavascriptTest.h"

using namespace HYSkiaUI;

auto backPressedCallback = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    v8::Isolate *isolate = args.GetIsolate();
    v8::HandleScope handle_scope(isolate);
    if (args.Length() < 1 || !args[0]->IsFunction()) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Invalid argument"));
        return;
    }
    auto data = v8::Local<v8::External>::Cast(args.Data());
    auto app = static_cast<JavascriptTest *>(data->Value());
    assert(app);
    v8::Local<v8::Function> function = v8::Local<v8::Function>::Cast(args[0]);
    auto callbackId = app->BACK_PRESSED_INDEX++;
    app->backPressCallbackMap[callbackId] = v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>>(
            isolate, function);
    args.GetReturnValue().Set(callbackId);
};
