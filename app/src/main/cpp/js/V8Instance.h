#pragma once

#include "v8.h"
#include "v8-platform.h"
#include "libplatform/libplatform.h"

namespace HYSkiaUI {

class V8Instance {

public:

    static V8Instance* getInstance() {
        static V8Instance instance;
        return &instance;
    }

    v8::Isolate* getIsolate() {
        return mIsolate;
    }

    v8::Platform* getPlatform() {
        return mPlatform.get();
    }

private:

    V8Instance() {
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
    }

    /**
     * can only dispose v8 when app exit
     */
    ~V8Instance() {
        mIsolate->Dispose();
    }

    V8Instance(V8Instance& instance) = delete;

    V8Instance operator=(V8Instance& instance) = delete;

private:

    v8::Isolate *mIsolate = nullptr;
    std::unique_ptr<v8::ArrayBuffer::Allocator> arrayBufferAllocator_;
    std::unique_ptr<v8::Platform> mPlatform;
};

}
