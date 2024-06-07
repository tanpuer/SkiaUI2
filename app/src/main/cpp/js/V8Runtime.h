#pragma once

#include "v8.h"
#include "v8-platform.h"
#include "libplatform/libplatform.h"

class V8Runtime {

public:

    V8Runtime();

    ~V8Runtime();

private:

    v8::Local<v8::Context> CreateGlobalContext(v8::Isolate *isolate);

private:

    v8::Isolate *mIsolate = nullptr;
    std::unique_ptr<v8::ArrayBuffer::Allocator> arrayBufferAllocator_;
    v8::Global<v8::Context> mContext;
    std::unique_ptr<v8::Platform> mPlatform;


};
