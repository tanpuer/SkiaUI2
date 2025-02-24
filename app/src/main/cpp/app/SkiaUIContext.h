#pragma once

#include "jni.h"
#include "AssetManager.h"
#include "memory"
#include "PageStackManager.h"
#include "PluginManager.h"
#include "YGConfig.h"
#include "core/SkCanvas.h"
#include "ResourcesLoader.h"
#include "V8Runtime.h"
#include "ports/SkFontMgr_android.h"
#include "skparagraph/include/TypefaceFontProvider.h"
#include "skparagraph/include/ParagraphBuilder.h"
#include "MeasureTime.h"

using namespace skia::textlayout;

namespace HYSkiaUI {

struct TimerData {
    std::function<void()> function;
    bool repeat;
    long delay;

    TimerData(std::function<void()> &&function, bool repeat, long delay) {
        this->function = std::move(function);
        this->repeat = repeat;
        this->delay = delay;
    }

};

class SkiaUIContext {

public:

    SkiaUIContext() {

    }

    ~SkiaUIContext() {
        jniEnv->DeleteGlobalRef(javaSkiaEngine);
    }

    void setJavaAssetManager(JNIEnv *env, jobject javaAssetManager) {
        this->jniEnv = env;
        assetManager = std::make_shared<AssetManager>(env, javaAssetManager);
        this->intFont();
    }

    std::shared_ptr<AssetManager> getAssetManager() {
        return assetManager;
    }

    void setTimeMills(long time) {
        currentTimeMills = time;
    }

    long getCurrentTimeMills() {
        return currentTimeMills;
    }

    void setConfigRef(YGConfigRef config) {
        this->config = config;
    }

    const YGConfigRef getConfig() {
        return config;
    }

    const std::shared_ptr<PageStackManager> getPageStackManager() {
        return pageStackManager;
    }

    const std::shared_ptr<PluginManager> getPluginManager() {
        return pluginManager;
    }

    JNIEnv *getJniEnv() {
        return jniEnv;
    }

    void setJavaSkiaEngine(jobject instance) {
        javaSkiaEngine = jniEnv->NewGlobalRef(instance);
        resourcesLoader = std::make_shared<ResourcesLoader>(jniEnv, javaSkiaEngine);
        auto jClazz = jniEnv->FindClass("com/temple/skiaui/HYSkiaEngine");
        setTimeoutMethodId = jniEnv->GetMethodID(jClazz, "setTimeout", "(JJ)V");
    }

    const jobject getJavaSkiaEngine() {
        return javaSkiaEngine;
    }

    void setCanvas(SkCanvas *canvas) {
        this->canvas = canvas;
    }

    SkCanvas *getCanvas() const {
        return canvas;
    }

    void setV8Runtime(std::shared_ptr<V8Runtime> runtime) {
        this->v8Runtime = runtime;
    }

    std::shared_ptr<V8Runtime> &getRuntime() {
        return v8Runtime;
    }

    void intFont() {
        MeasureTime("initFont");
        fontMgr = SkFontMgr_New_Android(nullptr);
        auto fontProvider = sk_make_sp<TypefaceFontProvider>();
        {
            auto fontData = assetManager->readImage("font/AlimamaFangYuanTiVF-Thin.ttf");
            auto data = SkData::MakeWithCopy(fontData->content, fontData->length);
            auto typeface = fontMgr->makeFromData(std::move(data));
            fontProvider->registerTypeface(typeface, SkString("Alimama"));
            delete fontData;
        }
        {
            auto fontData = assetManager->readImage("font/NotoColorEmoji.ttf");
            auto data = SkData::MakeWithCopy(fontData->content, fontData->length);
            auto typeface = fontMgr->makeFromData(std::move(data));
            fontProvider->registerTypeface(typeface, SkString("ColorEmoji"));
            delete fontData;
        }
        {
            auto fontData = assetManager->readImage("font/iconfont.woff");
            auto data = SkData::MakeWithCopy(fontData->content, fontData->length);
            iconFontTypeFace = fontMgr->makeFromData(std::move(data));
            delete fontData;
        }
        fontCollection = sk_make_sp<FontCollection>();
        fontCollection->setAssetFontManager(std::move(fontProvider));
        fontCollection->setDefaultFontManager(fontMgr);
        fontCollection->enableFontFallback();
    }

    sk_sp<FontCollection> getFontCollection() {
        return fontCollection;
    }

    sk_sp<SkTypeface> getIconFontTypeFace() {
        return iconFontTypeFace;
    }

    void markDirty() {
        dirty = true;
    }

    void clearDirty() {
        dirty = false;
    }

    bool isDirty() {
        return dirty;
    }

    long setTimer(std::function<void()> &&callback, long delay, bool repeat) {
        if (jniEnv == nullptr || javaSkiaEngine == nullptr || setTimeoutMethodId == nullptr) {
            return -1L;
        }
        auto id = timerId++;
        jniEnv->CallVoidMethod(javaSkiaEngine, setTimeoutMethodId, id, delay);
        timerMap.emplace(id, TimerData(std::move(callback), repeat, delay));
        return id;
    }

    void clearTimer(long id) {
        auto itr = timerMap.find(id);
        if (itr == timerMap.end()) {
            return;
        }
        timerMap.erase(id);
    }

    void performTimer(long id) {
        auto itr = timerMap.find(id);
        if (itr == timerMap.end()) {
            return;
        }
        auto data = itr->second;
        data.function();
        if (data.repeat) {
            jniEnv->CallVoidMethod(javaSkiaEngine, setTimeoutMethodId, id, data.delay);
        }
    }

    void setBackPressedInterceptor(std::function<void()>&& callback) {
        this->backPressedInterceptor = std::move(callback);
    }

    std::function<void()>& getBackPressedInterceptor() {
        return this->backPressedInterceptor;
    }

public:
    std::shared_ptr<ResourcesLoader> resourcesLoader;

private:

    std::shared_ptr<AssetManager> assetManager;

    long currentTimeMills = 0L;

    YGConfigRef config;

    std::shared_ptr<PageStackManager> pageStackManager = std::make_shared<PageStackManager>();

    std::shared_ptr<PluginManager> pluginManager = std::make_shared<PluginManager>();

    JNIEnv *jniEnv = nullptr;

    jobject javaSkiaEngine = nullptr;

    SkCanvas *canvas;

    std::shared_ptr<V8Runtime> v8Runtime = nullptr;

    sk_sp<SkFontMgr> fontMgr = nullptr;

    sk_sp<FontCollection> fontCollection = nullptr;

    sk_sp<SkTypeface> iconFontTypeFace = nullptr;

    bool dirty = true;

    std::unordered_map<long, TimerData> timerMap;

    jmethodID setTimeoutMethodId = nullptr;

    long timerId = 0;

    std::function<void()> backPressedInterceptor = nullptr;

};

}
