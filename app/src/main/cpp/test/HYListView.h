//
// Created by ByteDance on 2022/8/18.
//

#ifndef SKIAUI_HYLISTVIEW_H
#define SKIAUI_HYLISTVIEW_H


#include <BaseListView.h>

/**
 * 最简单的测试ListView
 */
class HYListView : public BaseListView<int> {

public:

    HYListView();

    virtual ~HYListView();

    View *createView(int index) override;

    void setAdapter(BaseListAdapter<int> *adapter) override;

    void attachChild(View *view) override;

};


#endif //SKIAUI_HYLISTVIEW_H
