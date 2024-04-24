//
// Created by banma-3412 on 2024/2/12.
//

#include "SkiaUIApp.h"
#include "core/SkPictureRecorder.h"
#include "core/SkPicture.h"
#include "ViewGroup.h"
#include "ScrollView.h"
#include "BaseListView.h"
#include "ScrollViewTest.h"
#include "core/SkGraphics.h"
#include "SkiaUIContext.h"
#include "PageTest.h"
#include "PageStackManager.h"
#include "FlexboxLayoutTest.h"

SkiaUIApp::SkiaUIApp() {
    SkGraphics::Init();
//    testDraw = std::make_unique<ScrollViewTest>();
    testDraw = std::make_unique<PageTest>();
//    testDraw = std::make_unique<FlexboxLayoutTest>();
}

SkiaUIApp::~SkiaUIApp() {

}

long SkiaUIApp::doFrame(long time) {
    SkiaUIContext::getInstance()->setTimeMills(time);
    drawCount++;
    IAnimator::currTime = time;
    SkPictureRecorder recorder;
    auto recordingCanvas = recorder.beginRecording(mWidth, mHeight);
    testDraw->doDrawTest(drawCount, recordingCanvas, mWidth, mHeight);
    auto picture = recorder.finishRecordingAsPicture();
    picture->ref();
    return reinterpret_cast<long >(picture.get());
}

void SkiaUIApp::dispatchTouchEvent(TouchEvent *touchEvent) {
    mTouchEvent = std::unique_ptr<TouchEvent>(touchEvent);
    auto root = testDraw->getRootView();
    if (root == nullptr) {
        return;
    }
    dynamic_cast<ViewGroup *>(root)->dispatchTouchEvent(mTouchEvent.get());
}

void SkiaUIApp::setVelocity(Velocity *velocity) {
    auto root = testDraw->getRootView();
    auto scrollView = dynamic_cast<ScrollView *>(root);
    if (scrollView != nullptr) {
        scrollView->setVelocity(velocity->xVelocity, velocity->yVelocity);
        return;
    }
    auto listView = dynamic_cast<BaseListView<void *> *>(root);
    if (listView != nullptr) {
        listView->setVelocity(velocity->xVelocity, velocity->yVelocity);
    }
}

void SkiaUIApp::setWindowSize(int width, int height) {
    mWidth = width;
    mHeight = height;
}

bool SkiaUIApp::onBackPressed() {
    auto pageTest = dynamic_cast<PageTest *>(testDraw.get());
    if (pageTest != nullptr) {
        if (PageStackManager::getInstance()->getPages().size() <= 1) {
            ALOGE("pop failed due to empty pages")
            return false;
        }
        auto page = PageStackManager::getInstance()->back();
        page->exitToLeft(Page::EnterExitInfo(0, mWidth));
        return true;
    }
    return false;
}
