#include "FlexboxLayoutTest.h"
#include "PageStackManager.h"

void FlexboxLayoutTest::doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) {
    if (root == nullptr) {
        ALOGD("doDrawTest %d %d", width, height)
        auto page = initPage(width, height);
        root = page;
        testNested(drawCount, root, width, height);
        PageStackManager::getInstance()->push(page);
        page->enterFromRight(Page::EnterExitInfo(width, 0));
    }
    auto widthSpec = MeasureSpec::makeMeasureSpec(width, EXACTLY);
    auto heightSpec = MeasureSpec::makeMeasureSpec(height, EXACTLY);
    for (const auto &item: PageStackManager::getInstance()->getPages()) {
        if (!item->getVisibility()) {
            continue;
        }
        item->measure(widthSpec, heightSpec);
        item->layout(0, 0, width, height);
        item->draw(canvas);
    }
}

/**
 * test absolute
 */
void FlexboxLayoutTest::testAbsolute(int drawCount, ViewGroup *root, int width, int height) {
    auto flexboxLayout = new FlexboxLayout();
    config = YGConfigNew();
    flexboxLayout->setConfig(config);
    flexboxLayout->setFlexWrap(YGWrapNoWrap);
    flexboxLayout->setFlexDirection(YGFlexDirectionColumn);
    flexboxLayout->setJustifyContent(YGJustifyFlexStart);
    flexboxLayout->setAlignItems(YGAlignCenter);
    flexboxLayout->setAlignContent(YGAlignCenter);
    flexboxLayout->setStyle(SkPaint::kFill_Style);
    flexboxLayout->setBackgroundColor(SK_ColorWHITE);
    root->addView(flexboxLayout, LayoutParams::makeExactlyLayoutParams(width, height));

    {
        auto view = new View();
        view->setConfig(flexboxLayout->config);
        auto colors = std::vector<SkColor>();
        colors.push_back(SK_ColorYELLOW);
        colors.push_back(SK_ColorBLUE);
        view->setLinearGradient(colors);
        flexboxLayout->addView(view, LayoutParams::makeExactlyLayoutParams(200, 200));
    }

    {
        auto view = new View();
        view->setConfig(flexboxLayout->config);
        view->setBackgroundColor(SK_ColorBLUE);
        view->setPositionType(YGPositionTypeAbsolute);
        view->setAlignSelf(YGAlign::YGAlignFlexStart);
        auto lp = LayoutParams::makeExactlyLayoutParams(200, 200);
        lp->setMargin({0, 50, 0, 50});
        flexboxLayout->addView(view, lp);
    }
}

/**
 * test wrap/wrap-reverse
 */
void FlexboxLayoutTest::testWrap(int drawCount, ViewGroup *root, int width, int height) {
    auto flexboxLayout = new FlexboxLayout();
    config = YGConfigNew();
    flexboxLayout->setConfig(config);
    flexboxLayout->setFlexWrap(YGWrapWrap);
    flexboxLayout->setFlexDirection(YGFlexDirectionColumn);
    flexboxLayout->setStyle(SkPaint::kFill_Style);
    flexboxLayout->setBackgroundColor(SK_ColorWHITE);
    root->addView(flexboxLayout, LayoutParams::makeExactlyLayoutParams(width, height));

    for (int i = 0; i < 13; ++i) {
        {
            auto view = new View();
            view->setConfig(flexboxLayout->config);
            view->setBackgroundColor(SK_ColorBLUE);
            auto lp = LayoutParams::makeExactlyLayoutParams(200, 200);
            lp->setMargin({0, 50, 50, 50});
            flexboxLayout->addView(view, lp);
        }
    }
}

/**
 *
 * test display: 'none'
 */
void FlexboxLayoutTest::testDisplay(int drawCount, ViewGroup *root, int width, int height) {
    auto flexboxLayout = new FlexboxLayout();
    config = YGConfigNew();
    flexboxLayout->setConfig(config);
    flexboxLayout->setFlexWrap(YGWrapNoWrap);
    flexboxLayout->setFlexDirection(YGFlexDirectionColumn);
    flexboxLayout->setJustifyContent(YGJustifyFlexStart);
    flexboxLayout->setAlignItems(YGAlignCenter);
    flexboxLayout->setAlignContent(YGAlignCenter);
    flexboxLayout->setStyle(SkPaint::kFill_Style);
    flexboxLayout->setBackgroundColor(SK_ColorWHITE);
    root->addView(flexboxLayout, LayoutParams::makeExactlyLayoutParams(width, height));

    {
        auto view = new View();
        view->setConfig(flexboxLayout->config);
        auto colors = std::vector<SkColor>();
        colors.push_back(SK_ColorYELLOW);
        colors.push_back(SK_ColorBLUE);
        view->setLinearGradient(colors);
        flexboxLayout->addView(view, LayoutParams::makeExactlyLayoutParams(200, 200));
    }

    {
        auto view = new View();
        view->setConfig(flexboxLayout->config);
        view->setBackgroundColor(SK_ColorBLUE);
        view->setAlignSelf(YGAlign::YGAlignFlexStart);
        view->setDisplay(YGDisplayNone);
        auto lp = LayoutParams::makeExactlyLayoutParams(200, 200);
        lp->setMargin({0, 50, 0, 50});
        flexboxLayout->addView(view, lp);
    }
}

//todo flexGrow conflicts with layoutParams
void FlexboxLayoutTest::testFlexGrow(int drawCount, ViewGroup *root, int width, int height) {
    auto flexboxLayout = new FlexboxLayout();
    config = YGConfigNew();
    flexboxLayout->setConfig(config);
    flexboxLayout->setFlexWrap(YGWrapNoWrap);
    flexboxLayout->setFlexDirection(YGFlexDirectionColumn);
    flexboxLayout->setJustifyContent(YGJustifyFlexStart);
    flexboxLayout->setAlignItems(YGAlignCenter);
    flexboxLayout->setAlignContent(YGAlignCenter);
    flexboxLayout->setStyle(SkPaint::kFill_Style);
    flexboxLayout->setBackgroundColor(SK_ColorWHITE);
    root->addView(flexboxLayout, LayoutParams::makeExactlyLayoutParams(width, height));

    {
        auto view = new View();
        view->setConfig(flexboxLayout->config);
        auto colors = std::vector<SkColor>();
        colors.push_back(SK_ColorYELLOW);
        colors.push_back(SK_ColorBLUE);
        view->setLinearGradient(colors);
        view->setFlexGrow(0.3);
        flexboxLayout->addView(view, LayoutParams::makeExactlyLayoutParams(WRAP_CONTENT, WRAP_CONTENT));
    }

    {
        auto view = new View();
        view->setConfig(flexboxLayout->config);
        view->setBackgroundColor(SK_ColorBLUE);
        view->setAlignSelf(YGAlign::YGAlignFlexStart);
        view->setDisplay(YGDisplayFlex);
        auto lp = LayoutParams::makeExactlyLayoutParams(WRAP_CONTENT, WRAP_CONTENT);
        lp->setMargin({50, 50, 50, 50});
        view->setFlexGrow(0.7);
        flexboxLayout->addView(view, lp);
    }

}

/**
 * test flex-direction: column-reverse, row-reverse
 */
void FlexboxLayoutTest::testFlexDirection(int drawCount, ViewGroup *root, int width, int height) {
    auto flexboxLayout = new FlexboxLayout();
    config = YGConfigNew();
    flexboxLayout->setConfig(config);
    flexboxLayout->setFlexWrap(YGWrapNoWrap);
    flexboxLayout->setFlexDirection(YGFlexDirectionColumnReverse);
    flexboxLayout->setJustifyContent(YGJustifyFlexStart);
    flexboxLayout->setAlignItems(YGAlignCenter);
    flexboxLayout->setAlignContent(YGAlignCenter);
    flexboxLayout->setStyle(SkPaint::kFill_Style);
    flexboxLayout->setBackgroundColor(SK_ColorWHITE);
    root->addView(flexboxLayout, LayoutParams::makeExactlyLayoutParams(width, height));

    {
        auto view = new View();
        view->setConfig(flexboxLayout->config);
        auto colors = std::vector<SkColor>();
        colors.push_back(SK_ColorYELLOW);
        colors.push_back(SK_ColorBLUE);
        view->setLinearGradient(colors);
        flexboxLayout->addView(view, LayoutParams::makeExactlyLayoutParams(200, 200));
    }

    {
        auto view = new View();
        view->setConfig(flexboxLayout->config);
        view->setBackgroundColor(SK_ColorBLUE);
        auto lp = LayoutParams::makeExactlyLayoutParams(200, 200);
        lp->setMargin({0, 50, 0, 50});
        flexboxLayout->addView(view, lp);
    }
}

//todo gap not supported
void FlexboxLayoutTest::testGap(int drawCount, ViewGroup *root, int width, int height) {
    auto flexboxLayout = new FlexboxLayout();
    config = YGConfigNew();
    flexboxLayout->setConfig(config);
    flexboxLayout->setFlexWrap(YGWrapWrap);
    flexboxLayout->setFlexDirection(YGFlexDirectionColumn);
    flexboxLayout->setStyle(SkPaint::kFill_Style);
    flexboxLayout->setBackgroundColor(SK_ColorWHITE);
    root->addView(flexboxLayout, LayoutParams::makeExactlyLayoutParams(width, height));

    for (int i = 0; i < 18; ++i) {
        {
            auto view = new View();
            view->setConfig(flexboxLayout->config);
            view->setBackgroundColor(SK_ColorBLUE);
            view->setGap(YGGutterAll, 20);
            auto lp = LayoutParams::makeExactlyLayoutParams(200, 200);
            flexboxLayout->addView(view, lp);
        }
    }
}

void FlexboxLayoutTest::testSetFlex(int drawCount, ViewGroup *root, int width, int height) {
    auto flexboxLayout = new FlexboxLayout();
    config = YGConfigNew();
    flexboxLayout->setConfig(config);
    flexboxLayout->setFlexWrap(YGWrapNoWrap);
    flexboxLayout->setFlexDirection(YGFlexDirectionColumn);
    flexboxLayout->setJustifyContent(YGJustifyFlexStart);
    flexboxLayout->setAlignItems(YGAlignCenter);
    flexboxLayout->setAlignContent(YGAlignCenter);
    flexboxLayout->setStyle(SkPaint::kFill_Style);
    flexboxLayout->setBackgroundColor(SK_ColorWHITE);
    root->addView(flexboxLayout, LayoutParams::makeExactlyLayoutParams(width, height));

    {
        auto view = new View();
        view->setConfig(flexboxLayout->config);
        auto colors = std::vector<SkColor>();
        colors.push_back(SK_ColorYELLOW);
        colors.push_back(SK_ColorBLUE);
        view->setLinearGradient(colors);
        view->setFlex(1);
        flexboxLayout->addView(view,LayoutParams::makeExactlyLayoutParams(WRAP_CONTENT, WRAP_CONTENT));
    }

    {
        auto view = new View();
        view->setConfig(flexboxLayout->config);
        view->setBackgroundColor(SK_ColorBLUE);
        view->setFlex(2);
        auto lp = LayoutParams::makeExactlyLayoutParams(WRAP_CONTENT, WRAP_CONTENT);
        lp->setMargin({0, 50, 0, 50});
        flexboxLayout->addView(view, lp);
    }
}


void FlexboxLayoutTest::testNested(int drawCount, ViewGroup *root, int width, int height) {
    auto flexboxLayout = new FlexboxLayout();
    config = YGConfigNew();
    flexboxLayout->setConfig(config);
    flexboxLayout->setFlexWrap(YGWrapNoWrap);
    flexboxLayout->setFlexDirection(YGFlexDirectionColumn);
    flexboxLayout->setJustifyContent(YGJustifyFlexStart);
    flexboxLayout->setAlignItems(YGAlignCenter);
    flexboxLayout->setAlignContent(YGAlignCenter);
    flexboxLayout->setStyle(SkPaint::kFill_Style);
    flexboxLayout->setBackgroundColor(SK_ColorWHITE);
    root->addView(flexboxLayout, LayoutParams::makeExactlyLayoutParams(width, height));

    {
        auto container = new FlexboxLayout();
        container->setConfig(flexboxLayout->config);
        container->setFlexDirection(YGFlexDirectionRow);
        container->setBackgroundColor(SkColorSetARGB(0x66, 0xFF, 0xFF, 0x00));
        flexboxLayout->addView(container, LayoutParams::makeExactlyLayoutParams(width, 200));
        {
            for (int i = 0; i < 3; ++i) {
                {
                    auto view = new View();
                    view->setConfig(flexboxLayout->config);
                    view->setBackgroundColor(SK_ColorBLUE);
                    auto lp = LayoutParams::makeExactlyLayoutParams(200, 200);
                    lp->setMargin({20, 0, 0, 0});
                    container->addView(view, lp);
                }
            }
        }
    }

    {
        auto container = new FlexboxLayout();
        container->setConfig(flexboxLayout->config);
        container->setFlexDirection(YGFlexDirectionRow);
        container->setBackgroundColor(SkColorSetARGB(0x66, 0x00, 0xFF, 0xFF));
        flexboxLayout->addView(container, LayoutParams::makeExactlyLayoutParams(width, height - 200));
        {
            auto left = new FlexboxLayout();
            left->setConfig(flexboxLayout->config);
            left->setFlexDirection(YGFlexDirectionColumn);
            left->setBackgroundColor(SK_ColorBLACK);
            container->addView(left, LayoutParams::makeExactlyLayoutParams(500, height - 200));
        }

        {
            auto right = new FlexboxLayout();
            right->setConfig(flexboxLayout->config);
            right->setFlexDirection(YGFlexDirectionColumn);
            right->setBackgroundColor(SK_ColorRED);
            container->addView(right, LayoutParams::makeExactlyLayoutParams(width - 500, height - 200));
        }
    }

}

Page *FlexboxLayoutTest::initPage(int width, int height) {
    auto page = new Page();
    config = YGConfigNew();
    page->setConfig(config);
    page->setLayoutParams(LayoutParams::makeExactlyLayoutParams(width, height));
    page->setStyle(SkPaint::kFill_Style);
    page->setBackgroundColor(SK_ColorTRANSPARENT);
    return page;
}

View *FlexboxLayoutTest::getRootView() {
    auto page = PageStackManager::getInstance()->back();
    SkASSERT(page != nullptr && page->children.size() == 1);
    return page->children[0];
}
