#include "V8Runtime.h"
#include "native_log.h"
#include "SkiaUIContext.h"
#include <ostream>
#include <sstream>

V8Runtime::V8Runtime(std::shared_ptr<SkiaUIContext> context) {
    this->skiaUiContext = context;
    v8::V8::SetFlagsFromString("--nolazy");
    v8::V8::Initialize();
    arrayBufferAllocator_.reset(
            v8::ArrayBuffer::Allocator::NewDefaultAllocator());
    mPlatform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializeICU();
    v8::V8::InitializePlatform(mPlatform.get());
    v8::Isolate::CreateParams createParams;
    createParams.array_buffer_allocator = arrayBufferAllocator_.get();
    mIsolate = v8::Isolate::New(createParams);
    v8::Locker locker(mIsolate);
    v8::Isolate::Scope scopedIsolate(mIsolate);
    v8::HandleScope scopedHandle(mIsolate);
    mContext.Reset(mIsolate, CreateGlobalContext(mIsolate));
    v8::Context::Scope scopedContext(mContext.Get(mIsolate));
    v8::Local<v8::Object> skiaUIObj = v8::Object::New(mIsolate);
    skiaUIObj->Set(v8::String::NewFromUtf8(mIsolate, "version"), v8::Number::New(mIsolate, 0.01));
    auto global = mContext.Get(mIsolate)->Global();
    auto result = global->Set(v8::String::NewFromUtf8(mIsolate, "SkiaUI"), skiaUIObj);
    skiaUI.Reset(mIsolate, skiaUIObj);
    ALOGD("V8Runtime init success")
}

V8Runtime::~V8Runtime() {
    v8::Locker locker(mIsolate);
    v8::Isolate::Scope scopedIsolate(mIsolate);
    v8::HandleScope scopedHandle(mIsolate);
    mContext.Reset();
    mIsolate->Dispose();
}

v8::Local<v8::Context> V8Runtime::CreateGlobalContext(v8::Isolate *isolate) {
    v8::EscapableHandleScope scopedHandle(isolate);
    v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
    return scopedHandle.Escape(v8::Context::New(isolate, nullptr, global));
}

v8::Local<v8::Value>
V8Runtime::evaluateJavaScript(const std::string &buffer, const std::string &sourceURL) {
    v8::Locker locker(mIsolate);
    v8::Isolate::Scope scopedIsolate(mIsolate);
    v8::HandleScope scopedHandle(mIsolate);
    v8::Context::Scope scopedContext(mContext.Get(mIsolate));
    auto string = v8::String::NewFromUtf8(
            mIsolate,
            reinterpret_cast<const char *>(buffer.c_str()),
            v8::NewStringType::kNormal,
            buffer.size()).ToLocalChecked();
    return executeScript(string, sourceURL);
}

v8::Local<v8::Value>
V8Runtime::executeScript(const v8::Local<v8::String> &script, const std::string &sourceURL) {
    v8::HandleScope scopedHandle(mIsolate);
    v8::TryCatch tryCatch(mIsolate);
    v8::MaybeLocal<v8::String> sourceURLValue = v8::String::NewFromUtf8(
            mIsolate,
            sourceURL.c_str(),
            v8::NewStringType::kNormal,
            static_cast<int>(sourceURL.length()));
    v8::ScriptOrigin origin(sourceURLValue.ToLocalChecked());
    v8::Local<v8::Context> context(mIsolate->GetCurrentContext());
    std::unique_ptr<v8::ScriptCompiler::Source> source =
            std::make_unique<v8::ScriptCompiler::Source>(script, origin);
    v8::Local<v8::Script> compiledScript;
    if (!v8::ScriptCompiler::Compile(
            context,
            source.release(),
            v8::ScriptCompiler::kNoCompileOptions)
            .ToLocal(&compiledScript)) {
        ReportException(&tryCatch);
        return v8::Undefined(mIsolate);
    }
    v8::Local<v8::Value> result;
    if (!compiledScript->Run(context).ToLocal(&result)) {
        ReportException(&tryCatch);
        return result;
    }
    ALOGD("AgilV8Runtime::executeScript success")
    return v8::Undefined(mIsolate);
}

void V8Runtime::ReportException(v8::TryCatch *tryCatch) {
    v8::HandleScope scopedHandle(mIsolate);
    std::string exception = toStdString(tryCatch->Message()->Get());
    v8::Local<v8::Message> message = tryCatch->Message();
    if (message.IsEmpty()) {
        // V8 didn't provide any extra information about this error; just
        // print the exception.
        ALOGE("JSError: %s", exception.c_str())
        return;
    } else {
        std::ostringstream ss;
        v8::Local<v8::Context> context(mIsolate->GetCurrentContext());

        // Print (filename):(line number): (message).
        ss << toStdString(message->GetScriptOrigin().ResourceName())
           << ":" << message->GetLineNumber(context).FromJust() << ": " << exception
           << std::endl;

        // Print line of source code.
        ss << toStdString(message->GetSourceLine(context).ToLocalChecked())
           << std::endl;

        // // Print wavy underline (GetUnderline is deprecated).
        int start = message->GetStartColumn(context).FromJust();
        for (int i = 0; i < start; i++) {
            ss << " ";
        }
        int end = message->GetEndColumn(context).FromJust();
        for (int i = start; i < end; i++) {
            ss << "^";
        }
        ss << std::endl;

        v8::Local<v8::Value> stackTraceString;
        if (tryCatch->StackTrace(context).ToLocal(&stackTraceString) &&
            stackTraceString->IsString() &&
            v8::Local<v8::String>::Cast(stackTraceString)->Length() > 0) {
            v8::String::Utf8Value stackTrace(mIsolate, stackTraceString);
            ss << std::string(*stackTrace, stackTrace.length()) << std::endl;
        }
        ALOGE("JSError: %s", ss.str().c_str())
    }
}


v8::Local<v8::Object> V8Runtime::global() {
    v8::Locker locker(mIsolate);
    v8::Isolate::Scope scopedIsolate(mIsolate);
    v8::HandleScope scopedHandle(mIsolate);
    v8::Context::Scope scopedContext(mContext.Get(mIsolate));
    return mContext.Get(mIsolate)->Global();
}

v8::Local<v8::Object> V8Runtime::injectObject(v8::Local<v8::Object> host, const char *name,
                                              std::map<std::string, v8::FunctionCallback> functionMap,
                                              std::map<std::string, std::string> constMap,
                                              void *any) {
    v8::Locker locker(mIsolate);
    v8::Isolate::Scope scopedIsolate(mIsolate);
    v8::HandleScope scopedHandle(mIsolate);
    v8::Context::Scope scopedContext(mContext.Get(mIsolate));
    v8::Local<v8::Object> object = v8::Object::New(mIsolate);
    for (std::pair<std::string, v8::FunctionCallback> pair: functionMap) {
        v8::Local<v8::External> external_context_data = v8::External::New(mIsolate, any);
        auto function = v8::FunctionTemplate::New(mIsolate, pair.second,
                                                  external_context_data)->GetFunction();
        auto result = object->Set(v8::String::NewFromUtf8(mIsolate, pair.first.c_str()), function);
        ALOGD("%s set %s result: %d", name, pair.first.c_str(), result)
    }
    for (std::pair<std::string, std::string> pair: constMap) {
        object->Set(v8::String::NewFromUtf8(mIsolate, pair.first.c_str()),
                    v8::String::NewFromUtf8(mIsolate, pair.second.c_str()));
    }
    auto global = mContext.Get(mIsolate)->Global();
    auto result = global->Set(v8::String::NewFromUtf8(mIsolate, name), object);
    ALOGD("global set object: %s result: %d", name, result)
    return object;
}

std::string V8Runtime::toStdString(const v8::Local<v8::Value> &string) {
    v8::HandleScope scopedHandle(mIsolate);
    assert(string->IsString());
    v8::String::Utf8Value utf8(mIsolate, string);
    if (*utf8) {
        return std::string(*utf8, utf8.length());
    }
    return {};
}

void V8Runtime::enterContext(
        const std::function<void(v8::Isolate *isolate, v8::Local<v8::Object> skiaUI)> &callback) {
    v8::Locker locker(mIsolate);
    v8::Isolate::Scope scopedIsolate(mIsolate);
    v8::HandleScope scopedHandle(mIsolate);
    v8::Context::Scope scopedContext(mContext.Get(mIsolate));
    callback(mIsolate, skiaUI.Get(mIsolate));
}

v8::Local<v8::Value>
V8Runtime::callFunction(const char *func, int argc, v8::Local<v8::Value> *argv) {
    v8::Locker locker(mIsolate);
    v8::Isolate::Scope scopedIsolate(mIsolate);
    v8::HandleScope scopedHandle(mIsolate);
    v8::Context::Scope scopedContext(mContext.Get(mIsolate));
    auto global = mContext.Get(mIsolate)->Global();
    v8::Local<v8::Value> func_val = global->Get(v8::String::NewFromUtf8(mIsolate, func));
    assert(func_val->IsFunction());
    auto callback = func_val.As<v8::Function>();
    v8::TryCatch try_catch(mIsolate);
    auto result = callback->Call(mIsolate->GetCurrentContext(), global, argc, argv);
    if (try_catch.HasCaught()) {
        v8::String::Utf8Value exception(mIsolate, try_catch.Exception());
        auto info = std::string(*exception, exception.length());
        ALOGE("callFunction error %s", info.c_str())
    }
    return result.ToLocalChecked();
}

v8::Local<v8::External> V8Runtime::createExternal(void *any) {
    v8::Locker locker(mIsolate);
    v8::Isolate::Scope scopedIsolate(mIsolate);
    v8::HandleScope scopedHandle(mIsolate);
    v8::Context::Scope scopedContext(mContext.Get(mIsolate));
    return v8::External::New(mIsolate, any);
}

void V8Runtime::injectFunction(const char *name, v8::FunctionCallback callback, void *any) {
    v8::Locker locker(mIsolate);
    v8::Isolate::Scope scopedIsolate(mIsolate);
    v8::HandleScope scopedHandle(mIsolate);
    v8::Context::Scope scopedContext(mContext.Get(mIsolate));
    v8::Local<v8::External> external_context_data = v8::External::New(mIsolate, any);
    v8::Local<v8::FunctionTemplate> funcTemplate = v8::FunctionTemplate::New(mIsolate, callback,
                                                                             external_context_data);
    v8::Local<v8::Function> function = funcTemplate->GetFunction();
    v8::Local<v8::String> funcName = v8::String::NewFromUtf8(mIsolate, name);
    auto global = mContext.Get(mIsolate)->Global();
    auto result = global->Set(funcName, function);
    ALOGD("global set function: %s result: %d", name, result)
}

v8::Local<v8::Value> V8Runtime::performFunction(
        v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>> function, int argc,
        v8::Local<v8::Value> *argv) {
    v8::Locker locker(mIsolate);
    v8::Isolate::Scope scopedIsolate(mIsolate);
    v8::HandleScope scopedHandle(mIsolate);
    v8::Context::Scope scopedContext(mContext.Get(mIsolate));

    v8::Local<v8::Function> callback = v8::Local<v8::Function>::New(mIsolate, function);
    v8::TryCatch try_catch(mIsolate);
    auto result = callback->Call(mIsolate->GetCurrentContext(),
                                 mIsolate->GetCurrentContext()->Global(), argc,
                                 argv);
    if (try_catch.HasCaught()) {
        v8::String::Utf8Value exception(mIsolate, try_catch.Exception());
        auto info = std::string(*exception, exception.length());
        ALOGD("performFunction error %s", info.c_str())
    }
    return result.ToLocalChecked();
}

void V8Runtime::injectNumber(const char *name, int number) {
    v8::Locker locker(mIsolate);
    v8::Isolate::Scope scopedIsolate(mIsolate);
    v8::HandleScope scopedHandle(mIsolate);
    v8::Context::Scope scopedContext(mContext.Get(mIsolate));
    auto result = skiaUI.Get(mIsolate)->Set(v8::String::NewFromUtf8(mIsolate, name),
                                            v8::Number::New(mIsolate, number));
}

v8::Platform *V8Runtime::getPlatform() {
    return mPlatform.get();
}
