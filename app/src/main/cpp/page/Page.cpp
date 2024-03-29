//
// Created by banma-3412 on 2024/3/20.
//

#include "Page.h"
#include "core/SkPictureRecorder.h"
#include "core/SkPicture.h"
#include "LinearAnimator.h"
#include "TranslateAnimator.h"
#include "PageStackManager.h"

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
    animator->start();
}

void Page::exitToLeft(const EnterExitInfo &info) {
    ALOGD("exitToLeft %d %d %d", info.from, info.to, info.duration)
    animator = std::make_unique<TranslateAnimator>(this, info.from, info.to, 0, 0);
    animator->setDuration(info.duration);
    animator->start();
    animator->addListener([]() {
        auto page = PageStackManager::getInstance()->pop();
        delete page;
    });
}

void Page::measure(int widthMeasureSpec, int heightMeasureSpec) {
    SkASSERT(children.size() == 1);
    auto root = children[0];
    measureChild(root, widthMeasureSpec, heightMeasureSpec);
    ViewGroup::setMeasuredDimension(MeasureSpec::getSize(widthMeasureSpec),
                                    MeasureSpec::getSize(heightMeasureSpec));
    YGNodeCalculateLayout(node, YGNodeStyleGetWidth(node).value, YGNodeStyleGetHeight(node).value,
                          YGDirectionLTR);
}

void Page::layout(int l, int t, int r, int b) {
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
