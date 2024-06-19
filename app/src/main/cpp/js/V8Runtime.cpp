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
    v8::Context::Scope scopedContext(mContext.Get(mIsolate));
    mContext.Reset(mIsolate, CreateGlobalContext(mIsolate));
    createGlobalSkiaUIObject();
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

void V8Runtime::createGlobalSkiaUIObject() {
    v8::Locker locker(mIsolate);
    v8::Isolate::Scope scopedIsolate(mIsolate);
    v8::HandleScope scopedHandle(mIsolate);
    v8::Context::Scope scopedContext(mContext.Get(mIsolate));
    auto global = mContext.Get(mIsolate)->Global();
    skiaUI = v8::Object::New(mIsolate);
    assert(global->Set(mContext.Get(mIsolate),
                       v8::String::NewFromUtf8(mIsolate, "skiaui"),
                       skiaUI).ToChecked());
}

bool V8Runtime::evaluateJavaScript(const std::string &buffer, const std::string &sourceURL) {
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

bool V8Runtime::executeScript(const v8::Local<v8::String> &script, const std::string &sourceURL) {
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
        return false;
    }
    v8::Local<v8::Value> result;
    if (!compiledScript->Run(context).ToLocal(&result)) {
        ReportException(&tryCatch);
        return false;
    }
    ALOGD("AgilV8Runtime::executeScript success")
    return true;
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

void
V8Runtime::injectClass(const char *className, v8::FunctionCallback constructorFunc, int fieldCount,
                       std::map<const char *, v8::FunctionCallback> methods, void *any,
                       bool globalTarget) {
    v8::Locker locker(mIsolate);
    v8::Isolate::Scope scopedIsolate(mIsolate);
    v8::HandleScope scopedHandle(mIsolate);
    v8::Context::Scope scopedContext(mContext.Get(mIsolate));
    // 准备构造函数模板
    v8::Local<v8::External> external_context_data = v8::External::New(mIsolate, any);
    v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(mIsolate, constructorFunc,
                                                                    external_context_data);
    tpl->InstanceTemplate()->SetInternalFieldCount(fieldCount);
    tpl->SetClassName(v8::String::NewFromUtf8(mIsolate, className));
    // 添加方法
    for (const auto &item: methods) {
        v8::Local<v8::FunctionTemplate> readTpl = v8::FunctionTemplate::New(mIsolate, item.second);
        tpl->PrototypeTemplate()->Set(mIsolate, item.first, readTpl);
    }
    // 注册构造函数
    v8::Local<v8::Function> constructor = tpl->GetFunction();
    auto target = globalTarget ? mContext.Get(mIsolate)->Global() : skiaUI;
    target->Set(v8::String::NewFromUtf8(mIsolate, className), constructor);
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
