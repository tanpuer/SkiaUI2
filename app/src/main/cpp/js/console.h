#pragma once

#include "v8.h"

auto handleConsole = [](const v8::FunctionCallbackInfo<v8::Value> &args) -> std::string {
    std::string result;
    v8::Isolate *isolate = args.GetIsolate();
    v8::HandleScope handle_scope(isolate);
    for (int i = 0; i < args.Length(); ++i) {
        v8::Local<v8::Value> arg = args[i];
        v8::String::Utf8Value value(isolate, arg);
        auto logInfo = std::string(*value, value.length());
        result += logInfo;
    }
    return result;
};
auto logCallback = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    ALOGD("%s", handleConsole(args).c_str())
};
auto errorCallback = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    ALOGE("%s", handleConsole(args).c_str())
};
auto infoCallback = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    ALOGI("%s", handleConsole(args).c_str())
};
auto warnCallback = [](const v8::FunctionCallbackInfo<v8::Value> &args) {
    ALOGW("%s", handleConsole(args).c_str())
};