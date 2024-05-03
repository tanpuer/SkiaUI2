//
// Created by banma-3412 on 2024/3/20.
//

#include "Page.h"
#include "core/SkPictureRecorder.h"
#include "core/SkPicture.h"
#include "LinearAnimator.h"
#include "TranslateAnimator.h"
#include "PageStackManager.h"
#include "core/SkColorFilter.h"
#include "PluginManager.h"

Page::Page() {
    pageId = PAGE_ID++;
    ALOGD("Page create %d", pageId)
    pagePaint = std::make_unique<SkPaint>();
}

Page::~Page() {
    ALOGD("page destroy %d", pageId)
}

void Page::enterFromRight(const EnterExitInfo &info) {
    ALOGD("enterFromRight %d %d %d", info.from, info.to, info.duration)
    animator = std::make_unique<TranslateAnimator>(this, info.from, info.to, 0, 0);
    animator->setDuration(info.duration);
    animator->addListener([this]() {
        context->getPageStackManager()->updateVisibility(true);
    });
    animator->start();
    context->getPageStackManager()->updateVisibility(false);
    PluginManager::getInstance()->invokeMethod("toast", "show", "push");
}

void Page::exitToLeft(const EnterExitInfo &info) {
    ALOGD("exitToLeft %d %d %d", info.from, info.to, info.duration)
    animator = std::make_unique<TranslateAnimator>(this, info.from, info.to, 0, 0);
    animator->setDuration(info.duration);
    animator->addListener([this]() {
        auto page = context->getPageStackManager()->pop();
        context->getPageStackManager()->updateVisibility(true);
        delete page;
    });
    animator->start();
    context->getPageStackManager()->updateVisibility(false);
    PluginManager::getInstance()->invokeMethod("toast", "show", "pop");
}

void Page::enterFromBottom(const Page::EnterExitInfo &info) {
    ALOGD("enterFromBottom %d %d %d", info.from, info.to, info.duration)
    animator = std::make_unique<TranslateAnimator>(this, 0, 0, info.from, info.to);
    animator->setDuration(info.duration);
    animator->addListener([this]() {
        auto page = context->getPageStackManager()->pop();
        context->getPageStackManager()->updateVisibility(true);
        delete page;
    });
    animator->start();
    context->getPageStackManager()->updateVisibility(false);
    PluginManager::getInstance()->invokeMethod("toast", "show", "push");
}

void Page::exitToTop(const Page::EnterExitInfo &info) {
    ALOGD("exitToTop %d %d %d", info.from, info.to, info.duration)
    animator = std::make_unique<TranslateAnimator>(this, 0, 0, info.from, info.to);
    animator->setDuration(info.duration);
    animator->addListener([this]() {
        auto page = context->getPageStackManager()->pop();
        context->getPageStackManager()->updateVisibility(true);
        delete page;
    });
    animator->start();
    context->getPageStackManager()->updateVisibility(false);
    PluginManager::getInstance()->invokeMethod("toast", "show", "pop");
}

void Page::measure() {
    if (!visible) {
        return;
    }
    SkASSERT(children.size() == 1);
    auto root = children[0];
    measureChild(root);
    YGNodeCalculateLayout(node, this->width, this->height,YGDirectionLTR);
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
    if (animator != nullptr) {
        if (animator->isEnd()) {
            animator.reset();
        } else {
            animator->update(skRect);
            ALOGD("page animator update %f %f", animTranslateX, animTranslateY)
        }
    }
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
