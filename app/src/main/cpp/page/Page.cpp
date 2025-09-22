#include "Page.h"
#include "core/SkPictureRecorder.h"
#include "core/SkPicture.h"
#include "LinearAnimator.h"
#include "TranslateAnimator.h"
#include "PageStackManager.h"
#include "core/SkColorFilter.h"
#include "PluginManager.h"

namespace HYSkiaUI {

Page::Page() {
    pageId = PAGE_ID++;
    ALOGD("Page create %d", pageId)
    pagePaint = std::make_unique<SkPaint>();
}

void Page::setContext(std::shared_ptr<SkiaUIContext> &context) {
    View::setContext(context);
    auto runtime = context->getRuntime();
    if (runtime != nullptr && !createCallback.IsEmpty()) {
        runtime->performFunction(createCallback, 0, {});
    }
    setBackgroundColor(SK_ColorTRANSPARENT);
}

Page::~Page() {
    auto runtime = context->getRuntime();
    if (runtime != nullptr && !destroyCallback.IsEmpty()) {
        runtime->performFunction(destroyCallback, 0, {});
    }
    createCallback.Reset();
    destroyCallback.Reset();
    showCallback.Reset();
    hideCallback.Reset();
    ALOGD("page destroy %d", pageId)
}

void Page::enterFromRight(const EnterExitInfo &info) {
    ALOGD("enterFromRight %d %d %d", info.from, info.to, info.duration)
    auto animator = new TranslateAnimator(this, info.from, info.to, 0, 0);
    if (context->getPageStackManager()->getPages().size() == 1) {
        animator->setDuration(0);
    } else {
        animator->setDuration(info.duration);
    }
    animator->addListener([this]() {
        context->getPageStackManager()->hideLastPage();
    });
    animator->start();
    context->getPageStackManager()->showCurrentPage();
    context->markDirty();
}

void Page::exitToLeft(const EnterExitInfo &info) {
    ALOGD("exitToLeft %d %d %d", info.from, info.to, info.duration)
    auto animator = new TranslateAnimator(this, info.from, info.to, 0, 0);
    animator->setDuration(info.duration);
    animator->addListener([this]() {
        this->markDestroyed = true;
    });
    animator->start();
    context->getPageStackManager()->showLastPage();
    context->getPluginManager()->invokeMethod("toast", "show", "pop");
    context->markDirty();
}

void Page::enterFromBottom(const Page::EnterExitInfo &info) {
    ALOGD("enterFromBottom %d %d %d", info.from, info.to, info.duration)
    auto animator = new TranslateAnimator(this, 0, 0, info.from, info.to);
    animator->setDuration(info.duration);
    animator->addListener([this]() {
        context->getPageStackManager()->hideLastPage();
    });
    animator->start();
    context->getPageStackManager()->showCurrentPage();
    context->getPluginManager()->invokeMethod("toast", "show", "push");
    context->markDirty();
}

void Page::exitToTop(const Page::EnterExitInfo &info) {
    ALOGD("exitToTop %d %d %d", info.from, info.to, info.duration)
    auto animator = new TranslateAnimator(this, 0, 0, info.from, info.to);
    animator->setDuration(info.duration);
    animator->addListener([this]() {
        this->markDestroyed = true;
    });
    animator->start();
    context->getPageStackManager()->showLastPage();
    context->getPluginManager()->invokeMethod("toast", "show", "pop");
    context->markDirty();
}

void Page::measure() {
    if (!visible || children.size() <= 0) {
        return;
    }
    SkASSERT(children.size() == 1);
    auto root = children[0];
    measureChild(root);
    YGNodeCalculateLayout(node, this->width, this->height, YGDirectionLTR);
}

void Page::layout(int l, int t, int r, int b) {
    if (!visible || children.size() <= 0) {
        return;
    }
    View::layout(l, t, r, b);
    SkASSERT(children.size() == 1);
    auto root = children[0];
    auto childNode = root->getNode();
    auto left = static_cast<int>(YGNodeLayoutGetLeft(childNode));
    auto top = static_cast<int>(YGNodeLayoutGetTop(childNode));
    auto width = static_cast<int>(YGNodeLayoutGetWidth(childNode));
    auto height = static_cast<int>(YGNodeLayoutGetHeight(childNode));
    root->layout(left + animTranslateX,
                 top + animTranslateY,
                 left + animTranslateX + width,
                 top + animTranslateY + height);
}

void Page::draw(SkCanvas *canvas) {
    if (!visible || children.size() <= 0) {
        return;
    }
    SkPictureRecorder recorder;
    auto skCanvas = recorder.beginRecording(width, height);
    View::draw(skCanvas);
    SkASSERT(children.size() == 1);
    auto root = children[0];
    root->draw(skCanvas);
    auto picture = recorder.finishRecordingAsPicture();
    canvas->save();
    canvas->translate(left, top);
    canvas->drawPicture(picture, nullptr, pagePaint.get());
    canvas->restore();
}

bool Page::dispatchTouchEvent(TouchEvent *touchEvent) {
    if (children.size() == 1) {
        auto root = static_cast<ViewGroup *>(children[0]);
        return root->dispatchTouchEvent(touchEvent);
    }
    return false;
}

bool Page::dispatchVelocity(Velocity *velocity) {
    if (children.size() == 1) {
        auto root = static_cast<ViewGroup *>(children[0]);
        return root->dispatchVelocity(velocity);
    }
    return false;
}

void Page::setVisibility(bool visible) {
    this->visible = visible;
}

bool Page::getVisibility() const {
    return visible;
}

void Page::setBlackWhiteMode() {
    auto rowMajor = {
            0.2126f, 0.7152f, 0.0722f, 0.0f, 0.0f,
            0.2126f, 0.7152f, 0.0722f, 0.0f, 0.0f,
            0.2126f, 0.7152f, 0.0722f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f, 0.0f
    };
    auto colorFilter = SkColorFilters::Matrix(data(rowMajor));
    pagePaint->setColorFilter(colorFilter);
}

void Page::onShow() {
    auto runtime = context->getRuntime();
    if (runtime != nullptr && !showCallback.IsEmpty()) {
        runtime->performFunction(showCallback, 0, {});
    }
    if (globalJavaViewRef != nullptr) {
        auto jniEnv = context->getJniEnv();
        if (pageOnShowMethodId == nullptr) {
            auto jClazz = jniEnv->FindClass("com/temple/skiaui/compose/ui/HYComposePage");
            pageOnShowMethodId = jniEnv->GetMethodID(jClazz, "onShowFromNative", "()V");
        }
        jniEnv->CallVoidMethod(globalJavaViewRef, pageOnShowMethodId);
    }
    ViewGroup::onShow();
}

void Page::onHide() {
    auto runtime = context->getRuntime();
    if (runtime != nullptr && !hideCallback.IsEmpty()) {
        runtime->performFunction(hideCallback, 0, {});
    }
    if (globalJavaViewRef != nullptr) {
        auto jniEnv = context->getJniEnv();
        if (pageOnHideMethodId == nullptr) {
            auto jClazz = jniEnv->FindClass("com/temple/skiaui/compose/ui/HYComposePage");
            pageOnHideMethodId = jniEnv->GetMethodID(jClazz, "onHideFromNative", "()V");
        }
        jniEnv->CallVoidMethod(globalJavaViewRef, pageOnHideMethodId);
    }
    ViewGroup::onHide();
}

bool Page::isDestroyed() {
    return markDestroyed;
}

void Page::pageSizeChange(int width, int height) {
    setWidth(width);
    setHeight(height);
    for (const auto &item: pageChangeCallbackList) {
        item(width, height);
    }
}

void Page::setOnPageSizeChangeListener(std::function<void(int, int)> &&callback) {
    pageChangeCallbackList.emplace_back(std::move(callback));
}

const char *Page::name() {
    return "Page";
}

}
