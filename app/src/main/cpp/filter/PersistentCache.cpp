#include "PersistentCache.h"
#include "MeasureTime.h"

namespace HYSkiaUI {

sk_sp<SkData> PersistentCache::load(const SkData &key) {
    MeasureTime PersistentCache("PersistentCache load");
    if (loadMethodId != nullptr) {
        jobject buffer = jniEnv->NewDirectByteBuffer(const_cast<void *>(key.data()), key.size());
        auto result = jniEnv->CallObjectMethod(javaCache, loadMethodId, buffer);
        if (result != nullptr) {
            return SkData::MakeWithoutCopy(
                    jniEnv->GetDirectBufferAddress(result),
                    jniEnv->GetDirectBufferCapacity(result));
        }
    }
    ALOGD("PersistentCache no cache")
    return nullptr;
}

void PersistentCache::store(const SkData &key, const SkData &data) {
    MeasureTime PersistentCache("PersistentCache store");
    if (storeMethodId != nullptr) {
        auto jKey = jniEnv->NewDirectByteBuffer(const_cast<void *>(key.data()), key.size());
        auto jValue = jniEnv->NewDirectByteBuffer(const_cast<void *>(data.data()), data.size());
        jniEnv->CallVoidMethod(javaCache, storeMethodId, jKey, jValue);
    }
}

PersistentCache::PersistentCache(JNIEnv *env) {
    jniEnv = env;
    auto clazz = jniEnv->FindClass("com/temple/skiaui/cache/PersistentCache");
    jfieldID instanceField = jniEnv->GetStaticFieldID(clazz, "INSTANCE",
                                                      "Lcom/temple/skiaui/cache/PersistentCache;");
    javaCache = jniEnv->NewGlobalRef(jniEnv->GetStaticObjectField(clazz, instanceField));
    loadMethodId = jniEnv->GetMethodID(clazz, "load",
                                       "(Ljava/nio/ByteBuffer;)Ljava/nio/ByteBuffer;");
    storeMethodId = jniEnv->GetMethodID(clazz, "store",
                                        "(Ljava/nio/ByteBuffer;Ljava/nio/ByteBuffer;)V");
}

PersistentCache::~PersistentCache() {
    jniEnv->DeleteGlobalRef(javaCache);
}

}
