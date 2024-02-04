//
// Created by ByteDance on 2022/12/7.
//

#ifndef SKIAUI_HYRECYCLERVIEW_H
#define SKIAUI_HYRECYCLERVIEW_H


#include "../widget/RecyclerView.h"
#include "TextView.h"
#include "ProgressBar.h"
#include "ImageView.h"

struct TestModel {

    int a;
    SkString b;

    TestModel(int a, SkString b) {
        this->a = a;
        this->b = b;
    }

};

class HYRecyclerView : public RecyclerView<TestModel *> {

public:

    HYRecyclerView() {
        auto adapter = new HYRecyclerAdapter();
        auto data = std::vector<TestModel *>();
        for (int i = 0; i < 200; ++i) {
            auto string = "abc" + std::to_string(i);
            data.emplace_back(new TestModel(i, SkString(string)));
        }
        adapter->setData(data);
        setAdapter(adapter);
    }

    virtual ~HYRecyclerView() {

    }

    class HYRecyclerAdapter : public RecyclerViewAdapter<TestModel *> {

        RecyclerViewHolder<TestModel *> *onCreateViewHolder(int itemType) override {
            switch (itemType) {
                case 1: {
                    auto flexboxLayout = new FlexboxLayout();
                    flexboxLayout->setConfig(config);
                    flexboxLayout->setFlexWrap(YGWrapNoWrap);
                    flexboxLayout->setFlexDirection(YGFlexDirectionColumn);
                    flexboxLayout->setJustifyContent(YGJustifyFlexStart);
                    flexboxLayout->setAlignItems(YGAlignCenter);
                    flexboxLayout->setAlignContent(YGAlignCenter);
                    flexboxLayout->setStyle(SkPaint::kStroke_Style);
                    auto vh = new HYRecyclerVH2(flexboxLayout);
                    {
                        auto progressBar = new ProgressBar();
                        progressBar->setConfig(config);
                        progressBar->setBarColor(SK_ColorRED);
                        progressBar->setBackgroundColor(SK_ColorGRAY);
                        progressBar->setStrokeWidth(10.0);
                        progressBar->setStyle(SkPaint::kStroke_Style);
                        flexboxLayout->addView(progressBar,
                                               LayoutParams::makeExactlyLayoutParams(200, 200));
                    }
                    return vh;
                }
                case 2: {
                    auto flexboxLayout = new FlexboxLayout();
                    flexboxLayout->setConfig(config);
                    flexboxLayout->setFlexWrap(YGWrapNoWrap);
                    flexboxLayout->setFlexDirection(YGFlexDirectionColumn);
                    flexboxLayout->setJustifyContent(YGJustifyFlexStart);
                    flexboxLayout->setAlignItems(YGAlignCenter);
                    flexboxLayout->setAlignContent(YGAlignCenter);
                    flexboxLayout->setStyle(SkPaint::kStroke_Style);
                    auto vh = new HYRecyclerVH3(flexboxLayout);
                    {
                        auto imageView = new ImageView();
                        imageView->setConfig(config);
                        imageView->setSource("/sdcard/cowboy.png");
                        imageView->setCornerRadius(100);
                        imageView->setScaleType(ImageView::ScaleType::FitCenter);
                        imageView->setStyle(SkPaint::kStroke_Style);
                        imageView->setBackgroundColor(SK_ColorRED);
                        imageView->setStrokeWidth(2);
                        auto lp = LayoutParams::makeExactlyLayoutParams(200, 200);
                        lp->setMargin({0, 100, 0, 100});
                        flexboxLayout->addView(imageView, lp);
                    }
                    return vh;
                }
                default: {
                    auto textView = new TextView();
                    textView->setConfig(config);
                    textView->setText(SkString("abc"));
                    textView->setTextColor(SK_ColorBLACK);
                    textView->setTextSize(60);
                    textView->setBackgroundColor(SK_ColorRED);
                    textView->setStyle(SkPaint::kStroke_Style);
                    textView->setDecoration(kLineThrough);
                    textView->setDecorationStyle(kSolid);
                    textView->setDecorationThicknessMultiplier(3);
                    textView->setDecorationColor(SK_ColorRED);
                    auto vh = new HYRecyclerVH(textView);
                    return vh;
                }
            }
        }

        void onBindViewHolder(RecyclerViewHolder<TestModel *> *viewHolder, int index,
                              TestModel *item) override {
            viewHolder->updateView(item);
            if (this->itemClickListener != nullptr) {
                viewHolder->getItemView()->setOnClickListener([this, item](View *view) {
                    this->itemClickListener(item, view);
                });
            }
            ALOGD("RecyclerView onBindViewHolder %d %s", index, item->b.c_str())
        }

        void
        onRecycleViewHolder(RecyclerViewHolder<TestModel *> *viewHolder, TestModel *item) override {
//            ALOGD("RecyclerView onRecycleViewHolder %s", item->b.c_str())
        }

        int getItemType(int index) override {
            return index % 3;
//            return 0;
        }

        class HYRecyclerVH : public RecyclerViewHolder<TestModel *> {

        public:

            HYRecyclerVH(View *itemView) : RecyclerViewHolder<TestModel *>(itemView) {

            }

            HYRecyclerVH() = delete;

            void updateView(TestModel *item) override {
                auto textView = dynamic_cast<TextView *>(itemView);
                if (textView != nullptr) {
                    textView->setText(item->b);
                }
            }

        };

        class HYRecyclerVH2 : public RecyclerViewHolder<TestModel *> {

        public:

            HYRecyclerVH2(View *itemView) : RecyclerViewHolder<TestModel *>(itemView) {

            }

            HYRecyclerVH2() = delete;

            void updateView(TestModel *item) override {

            }

        };

        class HYRecyclerVH3 : public RecyclerViewHolder<TestModel *> {

        public:
            HYRecyclerVH3(View *itemView) : RecyclerViewHolder<TestModel *>(itemView) {

            }

            HYRecyclerVH3() = delete;

            void updateView(TestModel *item) override {

            }
        };

    };

};

#endif //SKIAUI_HYRECYCLERVIEW_H
