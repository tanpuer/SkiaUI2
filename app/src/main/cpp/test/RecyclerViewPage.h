#pragma once

#include "Page.h"
#include "RecyclerView.h"
#include "TextView.h"
#include "LottieView.h"
#include "LoadingView.h"

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
                auto lottieView = new LottieView();
                lottieView->setContext(this->context);
                lottieView->setWidth(375);
                lottieView->setHeight(240);
                lottieView->setSource("WorkspacePlanet.json");
                lottieView->setStyle(SkPaint::kStroke_Style);
                lottieView->setBackgroundColor(SK_ColorRED);
                lottieView->setStrokeWidth(2);
                lottieView->setMargin({0, 25, 0, 25});
                return lottieView;
            } else {
                auto loadingView = new LoadingView();
                loadingView->setContext(this->context);
                loadingView->setWidth(1080);
                loadingView->setHeight(200);
                loadingView->setMargin({0, 25, 0, 25});
                loadingView->setStyle(SkPaint::kStroke_Style);
                return loadingView;
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
                auto lottieView = reinterpret_cast<LottieView *>(view);
                if (lottieView != nullptr) {

                }
            }
        }

        int getViewType(uint32_t index) override {
            return index % 3;
        }

    };

public:

    void init(std::shared_ptr<SkiaUIContext> &context, int width, int height) override;

private:

    void initChildren(ViewGroup *root, int width, int height);

};

}
