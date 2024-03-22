//
// Created by banma-3412 on 2024/3/20.
//

#include "Page.h"
#include "core/SkPictureRecorder.h"
#include "core/SkPicture.h"
#include "LinearAnimator.h"

Page::Page() {
    pageId = PAGE_ID++;
}

Page::~Page() {

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
    canvas->drawPicture(picture);
    canvas->restore();
}

void Page::enterFromRight(int distance) {
    animator = std::make_unique<LinearAnimator>(distance, 0);
    animator->setDuration(1000);
    animator->start();
}

void Page::exitToLeft(int distance) {
    animator = std::make_unique<LinearAnimator>(-distance, 0);
    animator->setDuration(1000);
    animator->start();
}

void Page::enterFromBottom(int distance) {
    auto animation = new LinearAnimator(0, distance);
}

void Page::exitToTop(int distance) {

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
        }
    }
    root->layout(left + skRect.left(), top + skRect.top(), left + skRect.left() + width,
                 top + skRect.top() + height);
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
