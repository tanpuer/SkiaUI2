//
// Created by ByteDance on 2022/8/18.
//

#ifndef SKIAUI_HYLISTADAPTER_H
#define SKIAUI_HYLISTADAPTER_H


#include <BaseListAdapter.h>

class HYListAdapter : public BaseListAdapter<int> {

public:

    HYListAdapter();

    virtual ~HYListAdapter();

    virtual View * createView(int index) override;

    void bindView(View *view, int model) override;

    virtual bool canCreateView() override;

};


#endif //SKIAUI_HYLISTADAPTER_H
