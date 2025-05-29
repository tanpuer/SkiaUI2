#pragma once

#include "FlexboxLayout.h"

namespace HYSkiaUI {

class GridLayout : public FlexboxLayout {

public:

    GridLayout();

    ~GridLayout() override;

    void setContext(std::shared_ptr<SkiaUIContext> &context) override;

    void measure() override;

    void layout(int l, int t, int r, int b) override;

    void setRowCount(int count);

private:

    int count = 2;

};

}
