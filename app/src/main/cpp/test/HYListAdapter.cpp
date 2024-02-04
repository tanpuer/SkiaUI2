//
// Created by ByteDance on 2022/8/18.
//

#include <TextView.h>
#include "HYListAdapter.h"

HYListAdapter::HYListAdapter() {

}

HYListAdapter::~HYListAdapter() {

}

View *HYListAdapter::createView(int index) {
    auto textView = new TextView();
    textView->setText(SkString("123456abc"));
    textView->setTextColor(SK_ColorBLACK);
    textView->setTextSize(60);
    textView->setBackgroundColor(SK_ColorRED);
    textView->setStyle(SkPaint::kStroke_Style);
    textView->setDecoration(kLineThrough);
    textView->setDecorationStyle(kSolid);
    textView->setDecorationThicknessMultiplier(3);
    textView->setDecorationColor(SK_ColorRED);
    return textView;
}

void HYListAdapter::bindView(View *view, int model) {

}

bool HYListAdapter::canCreateView() {
    return BaseListAdapter::canCreateView();
}
