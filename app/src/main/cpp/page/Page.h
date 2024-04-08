//
// Created by banma-3412 on 2024/3/20.
//

#ifndef SKIAUI_PAGE_H
#define SKIAUI_PAGE_H


#include "ViewGroup.h"
#include "memory"

static int64_t PAGE_ID = 0;

/**
 * One Page can only have one child
 */
class Page : public ViewGroup {

public:

    struct EnterExitInfo {
        int from;
        int to;
        int duration = 1000;

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

    void measure(int widthMeasureSpec, int heightMeasureSpec) override;

    void layout(int l, int t, int r, int b) override;

    void draw(SkCanvas *canvas) override;

    bool dispatchTouchEvent(TouchEvent *touchEvent) override;

    void enterFromRight(const EnterExitInfo &info);

    void exitToLeft(const EnterExitInfo &info);

    void enterFromBottom(const EnterExitInfo &info);

    void exitToTop(const EnterExitInfo &info);

    void setVisibility(bool visible);

    bool getVisibility();

    void setBlackWhiteMode();

private:

    std::unique_ptr<View> rootView;

    unsigned int pageId;

    bool visible = true;

    std::unique_ptr<SkPaint> pagePaint;

};


#endif //SKIAUI_PAGE_H
