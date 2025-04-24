#pragma once

#include "Page.h"
#include "RecyclerView.h"
#include "TextView.h"
#include "LottieView.h"
#include "LoadingView.h"
#include "AndroidImageView.h"

namespace HYSkiaUI {

class RecyclerViewPage : public Page {

public:
    class MyRecyclerView : public RecyclerView {

    public:

        MyRecyclerView() {

        }

        ~MyRecyclerView() {

        }

        View *onCreateView(uint32_t index) override {
            auto type = getViewType(index);
            if (type == 0) {
                auto textView = new TextView();
                textView->setContext(context);
                textView->setTextColor(SK_ColorGREEN);
                textView->setHeight(100);
                textView->setWidth(width);
                textView->setTextSize(100);
                textView->setTextAlign(skia::textlayout::TextAlign::kCenter);
                textView->setText(std::to_string(index).c_str());
                textView->setMargin({0, 25, 0, 25});
                return textView;
            } else if (type == 1) {
                auto flexboxLayout = new FlexboxLayout();
                flexboxLayout->setContext(this->context);
                flexboxLayout->setWidth(width);
                flexboxLayout->setStyle(SkPaint::kStroke_Style);
                flexboxLayout->setBackgroundColor(SK_ColorTRANSPARENT);
                flexboxLayout->setStrokeWidth(0);
                flexboxLayout->setMargin({0, 0, 0, 50});
                flexboxLayout->setFlexDirection(YGFlexDirection::YGFlexDirectionRow);
                flexboxLayout->setJustifyContent(YGJustify::YGJustifyCenter);
                flexboxLayout->setAlignItems(YGAlign::YGAlignCenter);

                auto lottieView = new LottieView();
                lottieView->setContext(this->context);
                lottieView->setWidth(375);
                lottieView->setHeight(240);
                lottieView->setSource("WorkspacePlanet.json");
                lottieView->setStyle(SkPaint::kStroke_Style);
                lottieView->setBackgroundColor(SK_ColorRED);
                lottieView->setStrokeWidth(2);
                flexboxLayout->addView(lottieView);

                auto textView = new TextView();
                textView->setContext(this->context);
                textView->setText(SkString("test recyclerview"));
                textView->setTextColor(SK_ColorGREEN);
                textView->setTextSize(50);
                textView->setMargin({50, 0, 0, 0});
                textView->setBackgroundColor(SK_ColorTRANSPARENT);
                textView->setStyle(SkPaint::kStroke_Style);
                flexboxLayout->addView(textView);

                return flexboxLayout;
            } else if (type == 2) {
                auto loadingView = new LoadingView();
                loadingView->setContext(this->context);
                loadingView->setWidth(1080);
                loadingView->setHeight(200);
                loadingView->setMargin({0, 25, 0, 25});
                loadingView->setStyle(SkPaint::kStroke_Style);
                return loadingView;
            } else {
                auto imageView = new AndroidImageView();
                imageView->setContext(this->context);
                imageView->setWidth(300);
                imageView->setHeight(300);
                imageView->setMargin({0, 25, 0, 25});
                if ((index + 1) % 8 == 0) {
                    imageView->setSource("bird.gif");
                } else {
                    imageView->setSource("raining.png");
                }
                return imageView;
            }
        }

        void onBindView(uint32_t index, HYSkiaUI::View *view) override {
            auto type = getViewType(index);
            if (type == 0) {
                auto textView = reinterpret_cast<TextView *>(view);
                if (textView != nullptr) {
                    textView->setText(std::to_string(index).c_str());
                }
            } else if (type == 1) {
                auto flexboxLayout = reinterpret_cast<FlexboxLayout *>(view);
                if (flexboxLayout != nullptr) {

                }
            } else if (type == 2) {
                auto loadingView = reinterpret_cast<LoadingView *>(view);
                if (loadingView != nullptr) {

                }
            } else {
                auto imageView = reinterpret_cast<AndroidImageView *>(view);
                if (imageView != nullptr) {
                    if ((index + 1) % 8 == 0) {
                        imageView->setSource("bird.gif");
                    } else {
                        imageView->setSource("raining.png");
                    }
                }
            }
        }

        uint32_t getViewType(uint32_t index) override {
            return index % 4;
        }

    };

public:

    void init(std::shared_ptr<SkiaUIContext> &context, int width, int height) override;

private:

    void initChildren(ViewGroup *root, int width, int height);

};

}
