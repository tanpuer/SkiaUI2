#pragma once

#include "ViewGroup.h"
#include "memory"
#include "v8.h"
#include "V8Runtime.h"

namespace HYSkiaUI {

static int64_t PAGE_ID = 0;

/**
 * One Page can only have one child
 */
class Page : public ViewGroup {

public:

    struct EnterExitInfo {
        int from;
        int to;
        int duration = 500;

        EnterExitInfo() = delete;

        EnterExitInfo(int from, int to, int duration) {
            this->from = from;
            this->to = to;
            this->duration = duration;
        }

        EnterExitInfo(int from, int to) {
            this->from = from;
            this->to = to;
        }

    };

public:

    Page();

    ~Page();

    void setContext(std::shared_ptr<SkiaUIContext> context) override;

    void measure() override;

    void layout(int l, int t, int r, int b) override;

    void draw(SkCanvas *canvas) override;

    bool dispatchTouchEvent(TouchEvent *touchEvent) override;

    bool dispatchVelocity(Velocity *velocity) override;

    void enterFromRight(const EnterExitInfo &info);

    void exitToLeft(const EnterExitInfo &info);

    void enterFromBottom(const EnterExitInfo &info);

    void exitToTop(const EnterExitInfo &info);

    void setVisibility(bool visible);

    bool getVisibility();

    void setBlackWhiteMode();

    bool isDestroyed();

    void pageSizeChange(int width, int height);

    void setOnPageSizeChangeListener(std::function<void(int, int)> &&callback);

private:

    std::unique_ptr<View> rootView;

    unsigned int pageId;

    bool visible = true;

    std::unique_ptr<SkPaint> pagePaint;

    bool markDestroyed = false;

    std::vector<std::function<void(int, int)>> pageChangeCallbackList;

public:
    void onShow() override;

    void onHide() override;

    v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>> createCallback;

    v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>> destroyCallback;

    v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>> showCallback;

    v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>> hideCallback;

public:

    virtual void init(std::shared_ptr<SkiaUIContext> &context, int width, int height) {}

    virtual void drawOneFrame(int drawCount) {}

};

}
