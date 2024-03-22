//
// Created by banma-3412 on 2024/3/21.
//

#ifndef SKIAUI_PAGETEST_H
#define SKIAUI_PAGETEST_H


#include "ITestDraw.h"
#include "Page.h"

class PageTest : public ITestDraw {

public:

    PageTest() = default;

    ~PageTest() = default;

    virtual void doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) override;

    virtual View *getRootView() override;

private:

    Page *initPage(int width, int height);

    void initChildren(ViewGroup *root, int width, int height);

};


#endif //SKIAUI_PAGETEST_H
