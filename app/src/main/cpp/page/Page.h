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

    Page();

    ~Page();

    void measure(int widthMeasureSpec, int heightMeasureSpec) override;

    void layout(int l, int t, int r, int b) override;

    void draw(SkCanvas *canvas) override;

    bool dispatchTouchEvent(TouchEvent *touchEvent) override;

    void enterFromRight(int distance);

    void exitToLeft(int distance);

    void enterFromBottom(int distance);

    void exitToTop(int distance);

    void setVisibility(bool visible);

    bool getVisibility();

private:

    std::unique_ptr<View> rootView;

    unsigned int pageId;

    bool visible = true;

};


#endif //SKIAUI_PAGE_H
