#include "FlexboxLayoutTest.h"
#include "PageStackManager.h"
#include "Button.h"
#include "ClockView.h"

void FlexboxLayoutTest::doDrawTest(int drawCount, SkCanvas *canvas, int width, int height) {
    if (root == nullptr) {
        ALOGD("doDrawTest %d %d", width, height)
        auto page = initPage(width, height);
        root = page;
        testNested(drawCount, root, width, height);
        PageStackManager::getInstance()->push(page);
        page->enterFromRight(Page::EnterExitInfo(width, 0));
    }
    for (const auto &item: PageStackManager::getInstance()->getPages()) {
        if (!item->getVisibility()) {
            continue;
        }
        item->measure();
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
    flexboxLayout->setWidth(width);
    flexboxLayout->setHeight(height);
    root->addView(flexboxLayout);

    {
        auto view = new View();
        view->setConfig(flexboxLayout->config);
        auto colors = std::vector<SkColor>();
        colors.push_back(SK_ColorYELLOW);
        colors.push_back(SK_ColorBLUE);
        view->setLinearGradient(colors);
        view->setHeight(200);
        view->setAspectRatio(0.5);
        flexboxLayout->addView(view);
    }

    {
        auto view = new View();
        view->setConfig(flexboxLayout->config);
        view->setBackgroundColor(SK_ColorBLUE);
        view->setPositionType(YGPositionTypeAbsolute);
        view->setAlignSelf(YGAlign::YGAlignFlexStart);
        view->setHeight(200);
        view->setAspectRatio(1.0);
        view->setMargin({0, 50, 0, 50});
        flexboxLayout->addView(view);
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
    flexboxLayout->setWidth(width);
    flexboxLayout->setHeight(height);
    root->addView(flexboxLayout);

    for (int i = 0; i < 13; ++i) {
        {
            auto view = new View();
            view->setConfig(flexboxLayout->config);
            view->setBackgroundColor(SK_ColorBLUE);
            view->setWidth(200);
            view->setHeight(200);
            view->setMargin({0, 50, 50, 50});
            flexboxLayout->addView(view);
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
    flexboxLayout->setWidth(width);
    flexboxLayout->setHeight(height);
    root->addView(flexboxLayout);

    {
        auto view = new View();
        view->setConfig(flexboxLayout->config);
        auto colors = std::vector<SkColor>();
        colors.push_back(SK_ColorYELLOW);
        colors.push_back(SK_ColorBLUE);
        view->setLinearGradient(colors);
        view->setWidth(200);
        view->setHeight(200);
        flexboxLayout->addView(view);
    }

    {
        auto view = new View();
        view->setConfig(flexboxLayout->config);
        view->setBackgroundColor(SK_ColorBLUE);
        view->setAlignSelf(YGAlign::YGAlignFlexStart);
        view->setDisplay(YGDisplayNone);
        view->setWidth(200);
        view->setHeight(200);
        view->setMargin({0, 50, 0, 50});
        flexboxLayout->addView(view);
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
    flexboxLayout->setWidth(width);
    flexboxLayout->setHeight(height);
    root->addView(flexboxLayout);

    {
        auto view = new View();
        view->setConfig(flexboxLayout->config);
        auto colors = std::vector<SkColor>();
        colors.push_back(SK_ColorYELLOW);
        colors.push_back(SK_ColorBLUE);
        view->setLinearGradient(colors);
        view->setFlexGrow(0.3);
        flexboxLayout->addView(view);
    }

    {
        auto view = new View();
        view->setConfig(flexboxLayout->config);
        view->setBackgroundColor(SK_ColorBLUE);
        view->setAlignSelf(YGAlign::YGAlignFlexStart);
        view->setDisplay(YGDisplayFlex);
        view->setMargin({50, 50, 50, 50});
        view->setFlexGrow(0.7);
        flexboxLayout->addView(view);
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
    flexboxLayout->setWidth(width);
    flexboxLayout->setHeight(height);
    root->addView(flexboxLayout);

    {
        auto view = new View();
        view->setConfig(flexboxLayout->config);
        auto colors = std::vector<SkColor>();
        colors.push_back(SK_ColorYELLOW);
        colors.push_back(SK_ColorBLUE);
        view->setLinearGradient(colors);
        view->setWidth(200);
        view->setHeight(200);
        flexboxLayout->addView(view);
    }

    {
        auto view = new View();
        view->setConfig(flexboxLayout->config);
        view->setBackgroundColor(SK_ColorBLUE);
        view->setWidth(200);
        view->setHeight(200);
        view->setMargin({0, 50, 0, 50});
        flexboxLayout->addView(view);
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
    flexboxLayout->setWidth(width);
    flexboxLayout->setHeight(height);
    root->addView(flexboxLayout);

    for (int i = 0; i < 18; ++i) {
        {
            auto view = new View();
            view->setConfig(flexboxLayout->config);
            view->setBackgroundColor(SK_ColorBLUE);
            view->setGap(YGGutterAll, 20);
            view->setWidth(200);
            view->setHeight(200);
            flexboxLayout->addView(view);
        }
    }
}

void FlexboxLayoutTest::testSetFlex(int drawCount, ViewGroup *root, int width, int height) {
    auto flexboxLayout = new FlexboxLayout();
    config = YGConfigNew();
    flexboxLayout->setConfig(config);
    flexboxLayout->setFlexWrap(YGWrapWrap);
    flexboxLayout->setFlexDirection(YGFlexDirectionColumn);
    flexboxLayout->setJustifyContent(YGJustifyFlexStart);
    flexboxLayout->setAlignItems(YGAlignCenter);
    flexboxLayout->setAlignContent(YGAlignCenter);
    flexboxLayout->setStyle(SkPaint::kFill_Style);
    flexboxLayout->setBackgroundColor(SK_ColorWHITE);
    flexboxLayout->setPadding({100, 100, 0, 0});
    flexboxLayout->setWidth(width);
    flexboxLayout->setHeight(height);
    root->addView(flexboxLayout);

    {
        auto view = new View();
        view->setConfig(flexboxLayout->config);
        auto colors = std::vector<SkColor>();
        colors.push_back(SK_ColorYELLOW);
        colors.push_back(SK_ColorBLUE);
        view->setLinearGradient(colors);
        view->setFlex(1);
        view->setWidth(width);
        flexboxLayout->addView(view);
    }

    {
        auto view = new View();
        view->setConfig(flexboxLayout->config);
        view->setBackgroundColor(SK_ColorBLUE);
        view->setFlex(2);
        view->setMargin({0, 50, 0, 50});
        view->setWidth(width);
        flexboxLayout->addView(view);
    }
}


void FlexboxLayoutTest::testNested(int drawCount, ViewGroup *root, int width, int height) {
    auto flexboxLayout = new FlexboxLayout();
    config = YGConfigNew();
    flexboxLayout->setConfig(config);
    flexboxLayout->setFlexWrap(YGWrapNoWrap);
    flexboxLayout->setFlexDirection(YGFlexDirectionColumn);
    flexboxLayout->setJustifyContent(YGJustifyFlexStart);
//    flexboxLayout->setAlignItems(YGAlignCenter);
//    flexboxLayout->setAlignContent(YGAlignCenter);
    flexboxLayout->setStyle(SkPaint::kFill_Style);
    flexboxLayout->setBackgroundColor(SK_ColorWHITE);
    flexboxLayout->setWidth(width);
    flexboxLayout->setHeight(height);
    root->addView(flexboxLayout);

    {
        auto container = new FlexboxLayout();
        container->setConfig(flexboxLayout->config);
        container->setFlexDirection(YGFlexDirectionRow);
        container->setBackgroundColor(SkColorSetARGB(0x66, 0xFF, 0xFF, 0x00));
//        container->setWidth(width);
        container->setHeight(200);
        flexboxLayout->addView(container);
        {
            for (int i = 0; i < 3; ++i) {
                {
                    auto view = new View();
                    view->setConfig(flexboxLayout->config);
                    view->setBackgroundColor(SK_ColorBLUE);
                    view->setWidth(200);
                    view->setHeight(200);
                    view->setMargin({20, 0, 0, 0});
                    container->addView(view);
                }
            }
        }
    }

    {
        auto container = new FlexboxLayout();
        container->setConfig(flexboxLayout->config);
        container->setFlexDirection(YGFlexDirectionRow);
        container->setBackgroundColor(SkColorSetARGB(0x33, 0x00, 0xFF, 0xFF));
        container->setWidth(width);
        container->setHeight(height - 200);
        flexboxLayout->addView(container);
        {
            auto left = new FlexboxLayout();
            left->setConfig(flexboxLayout->config);
            left->setFlexDirection(YGFlexDirectionColumn);
            left->setBackgroundColor(SK_ColorWHITE);
            left->setFlex(1);
            container->addView(left);

            {
                auto button = new Button();
                button->setConfig(flexboxLayout->config);
                button->setText(SkString("Button"));
                button->setTextSize(60);
                button->setCornerRadius(20);
                button->setBackgroundColor(SK_ColorRED);
                button->setTextColor(SK_ColorBLACK);
                button->addShadow(SK_ColorRED, {2.0, 2.0}, 1.0f);
                button->setMargin({50, 50, 50, 50});
                button->setWidth(260);
                button->setHeight(100);
                left->addView(button);
                button->setOnClickListener([](View *view) {
                    ALOGD("setOnClickListener perform %s", view->name())
                });
            }
        }

        {
            auto right = new FlexboxLayout();
            right->setConfig(flexboxLayout->config);
            right->setFlexDirection(YGFlexDirectionColumn);
            right->setBackgroundColor(SkColorSetARGB(0x55, 0xFF, 0x00, 0x00));
            right->setFlex(2);
            container->addView(right);

            {
                auto clockView = new ClockView();
                clockView->setConfig(flexboxLayout->config);
                clockView->setWidth(500);
                clockView->setHeight(500);
                clockView->setMargin({50, 50, 0, 0});
                right->addView(clockView);
            }
        }
    }

}

Page *FlexboxLayoutTest::initPage(int width, int height) {
    auto page = new Page();
    config = YGConfigNew();
    page->setConfig(config);
    page->setWidth(width);
    page->setHeight(height);
    page->setStyle(SkPaint::kFill_Style);
    page->setBackgroundColor(SK_ColorTRANSPARENT);
    return page;
}

View *FlexboxLayoutTest::getRootView() {
    auto page = PageStackManager::getInstance()->back();
    SkASSERT(page != nullptr && page->children.size() == 1);
    return page->children[0];
}
