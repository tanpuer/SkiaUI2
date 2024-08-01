#pragma once

#include "v8.h"
#include "JavascriptTest.h"

auto nowCallback = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    auto isolate = args.GetIsolate();
    if (args.Length() != 0) {
        isolate->ThrowException(v8::String::NewFromUtf8(isolate, "Invalid argument"));
        return;
    }
    auto data = v8::Local<v8::External>::Cast(args.Data());
    auto app = static_cast<JavascriptTest *>(data->Value());
    auto result = app->getContext()->getCurrentTimeMills();
    args.GetReturnValue().Set(static_cast<double>(result));
};
