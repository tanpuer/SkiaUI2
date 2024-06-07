#include "V8Runtime.h"
#include "native_log.h"

V8Runtime::V8Runtime() {
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
    ALOGD("AgilV8Runtime init success")
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
