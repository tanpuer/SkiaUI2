#pragma once

#include "Page.h"
#include "RecyclerView.h"
#include "TextView.h"

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
            auto textView = new TextView();
            textView->setContext(context);
            textView->setTextColor(SK_ColorGREEN);
            textView->setHeight(100);
            textView->setWidth(width);
            textView->setTextSize(100);
            textView->setTextAlign(skia::textlayout::TextAlign::kCenter);
            textView->setText(std::to_string(index).c_str());
            textView->setMargin({0, 0, 0, 50});
            return textView;
        }

        void onBindView(uint32_t index, HYSkiaUI::View *view) override {
            auto textView = reinterpret_cast<TextView *>(view);
            if (textView != nullptr) {
                textView->setText(std::to_string(index).c_str());
            }
        }

    };

public:

    void init(std::shared_ptr<SkiaUIContext> &context, int width, int height) override;

private:

    void initChildren(ViewGroup *root, int width, int height);

};

}
