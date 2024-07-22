#pragma once

#include "ITestDraw.h"
#include "Page.h"

class PageTest : public ITestDraw {

public:

    PageTest() = default;

    ~PageTest() = default;

    virtual void doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) override;

    virtual View *getRootView() override;

    void onShow() override;

    void onHide() override;

private:

    Page *initPage(int width, int height);

    void initChildren(int drawCount, ViewGroup *root, int width, int height);

    bool blackWhiteMode = false;

};
