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


void Page::setContext(std::shared_ptr<SkiaUIContext>& context) {
    View::setContext(context);
    auto runtime = context->getRuntime();
    if (runtime != nullptr && !createCallback.IsEmpty()) {
        runtime->performFunction(createCallback, 0, {});
    }
    setBackgroundColor(SK_ColorTRANSPARENT);
}

Page::~Page() {
    auto runtime = context->getRuntime();
    if (runtime != nullptr && !hideCallback.IsEmpty()) {
        runtime->performFunction(hideCallback, 0, {});
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
    animator->setDuration(info.duration);
    animator->addListener([this]() {
        context->getPageStackManager()->hideLastPage();
    });
    animator->start();
    context->getPageStackManager()->showCurrentPage();
    context->getPluginManager()->invokeMethod("toast", "show", "push");
    context->markDirty();
}

void Page::exitToLeft(const EnterExitInfo &info) {
    ALOGD("exitToLeft %d %d %d", info.from, info.to, info.duration)
    auto animator = new TranslateAnimator(this, info.from, info.to, 0, 0);
    animator->setDuration(info.duration);
    animator->addListener([this]() {
        this->markDestroyed = true;
        context->getPageStackManager()->removeDestroyedPage();
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
        context->getPageStackManager()->removeDestroyedPage();
    });
    animator->start();
    context->getPageStackManager()->showLastPage();
    context->getPluginManager()->invokeMethod("toast", "show", "pop");
    context->markDirty();
}

void Page::measure() {
    if (!visible) {
        return;
    }
    SkASSERT(children.size() == 1);
    auto root = children[0];
    measureChild(root);
    YGNodeCalculateLayout(node, this->width, this->height, YGDirectionLTR);
}

void Page::layout(int l, int t, int r, int b) {
    if (!visible) {
        return;
    }
    View::layout(l, t, r, b);
    SkASSERT(children.size() == 1);
    auto root = children[0];
    auto left = static_cast<int>(YGNodeLayoutGetLeft(root->node));
    auto top = static_cast<int>(YGNodeLayoutGetTop(root->node));
    auto width = static_cast<int>(YGNodeLayoutGetWidth(root->node));
    auto height = static_cast<int>(YGNodeLayoutGetHeight(root->node));
    root->layout(left + animTranslateX,
                 top + animTranslateY,
                 left + animTranslateX + width,
                 top + animTranslateY + height);
}

void Page::draw(SkCanvas *canvas) {
    if (!visible) {
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

bool Page::getVisibility() {
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

#pragma mark LifeCycle Callback start

void Page::onShow() {
    auto runtime = context->getRuntime();
    if (runtime != nullptr && !showCallback.IsEmpty()) {
        runtime->performFunction(showCallback, 0, {});
    }
    ViewGroup::onShow();
}

void Page::onHide() {
    auto runtime = context->getRuntime();
    if (runtime != nullptr && !hideCallback.IsEmpty()) {
        runtime->performFunction(hideCallback, 0, {});
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

#pragma mark LifeCycle Callback end

}
