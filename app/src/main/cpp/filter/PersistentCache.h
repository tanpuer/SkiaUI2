#pragma once

#include "gpu/ganesh/GrContextOptions.h"
#include "jni.h"

namespace HYSkiaUI {

class PersistentCache : public GrContextOptions::PersistentCache {

public:

    PersistentCache(JNIEnv *env);

    ~PersistentCache();

    sk_sp<SkData> load(const SkData &key) override;

    void store(const SkData &key, const SkData &data) override;

private:

    JNIEnv *jniEnv;

    jobject javaCache = nullptr;

    jmethodID storeMethodId = nullptr;

    jmethodID loadMethodId = nullptr;

};

}
