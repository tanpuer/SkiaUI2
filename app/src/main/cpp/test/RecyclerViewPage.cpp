#include "RecyclerViewPage.h"
#include "TextView.h"

namespace HYSkiaUI {

void
HYSkiaUI::RecyclerViewPage::init(std::shared_ptr<SkiaUIContext> &context, int width, int height) {
    setContext(context);
    setWidth(width);
    setHeight(height);
    setFlexWrap(YGWrapWrap);
    setFlexDirection(YGFlexDirectionColumn);
    setJustifyContent(YGJustifyCenter);
    setAlignItems(YGAlignCenter);
    setAlignContent(YGAlignCenter);
    setStyle(SkPaint::kFill_Style);
    setBackgroundColor(SK_ColorTRANSPARENT);
    initRecyclerView(this, width, height);
}

void RecyclerViewPage::initRecyclerView(ViewGroup *root, int width, int height) {
    auto recyclerView = new TestRecyclerView();
    config = YGConfigNew();
    recyclerView->setContext(this->context);
    recyclerView->setWidth(width);
    recyclerView->setHeight(height);
    recyclerView->setFlexDirection(YGFlexDirectionColumn);
    recyclerView->setStyle(SkPaint::kFill_Style);
    recyclerView->setBackgroundColor(SK_ColorGREEN);
    root->addView(recyclerView);

    auto adapter = new TestAdapter(this, getContext());
    std::vector<RecyclerData> result;
    for (int i = 0; i < 50; ++i) {
        RecyclerData data;
        data.index = i;
        result.emplace_back(data);
    }
    adapter->setData(result);
    recyclerView->setAdapter(adapter);
}

void TestViewHolder::updateView(RecyclerData item) {
    auto flexboxLayout = dynamic_cast<FlexboxLayout *>(itemView);
    auto textView = dynamic_cast<TextView *>(flexboxLayout->children[0]);
    textView->setTextGradient({}, {});
    textView->setMaxLines(1);
    textView->setText(std::to_string(item.index).c_str());
    textView->setTextSize(60);
}

RecyclerViewHolder<RecyclerData> *TestAdapter::onCreateViewHolder(int viewType) {
    auto flexLayout = new FlexboxLayout();
    flexLayout->setContext(this->context);
    flexLayout->setAlignItems(YGAlignCenter);
    flexLayout->setJustifyContent(YGJustifyCenter);
    flexLayout->setBackgroundColor(SK_ColorTRANSPARENT);
    flexLayout->setWidth(getRecyclerView()->getWidth());
    auto textView = new TextView();
    textView->setContext(this->context);
    textView->setTextSize(60);
    textView->setTextColor(SK_ColorWHITE);
    textView->setStrokeWidth(0);
    textView->setBackgroundColor(SK_ColorTRANSPARENT);
    textView->setStyle(SkPaint::kStroke_Style);
    textView->setMaxLines(1);
    flexLayout->addView(textView);
    auto vh = new TestViewHolder(flexLayout);
    return vh;
}

void TestAdapter::onBindViewHolder(RecyclerViewHolder<RecyclerData>
                                   *viewHolder,
                                   int index,
                                   RecyclerData
                                   item) {
    viewHolder->updateView(item);
}

void
TestAdapter::onRecycleViewHolder(RecyclerViewHolder<RecyclerData> *viewHolder, RecyclerData item) {

}

}
