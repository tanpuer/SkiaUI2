#pragma once


#include "ITestDraw.h"
#include "Page.h"

class FlexboxLayoutTest : public ITestDraw {

public:

    FlexboxLayoutTest() = default;

    ~FlexboxLayoutTest() = default;

    virtual void doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) override;

    virtual View *getRootView() override;

private:

    Page *initPage(int width, int height);

    void testAbsolute(int drawCount, ViewGroup *root, int width, int height);

    void testWrap(int drawCount, ViewGroup *root, int width, int height);

    void testDisplay(int drawCount, ViewGroup *root, int width, int height);

    void testFlexGrow(int drawCount, ViewGroup *root, int width, int height);

    void testFlexDirection(int drawCount, ViewGroup *root, int width, int height);

    void testGap(int drawCount, ViewGroup *root, int width, int height);

    void testSetFlex(int drawCount, ViewGroup *root, int width, int height);

    void testNested(int drawCount, ViewGroup *root, int width, int height);

    void testLyric(int drawCount, ViewGroup *root, int width, int height);

    void testLyric2(int drawCount, ViewGroup *root, int width, int height);

};

