#pragma once

#include "ViewGroup.h"

namespace HYSkiaUI {

class FlexboxLayout : public ViewGroup {

public:

    FlexboxLayout();

    ~FlexboxLayout();

    const char *name() override;

    virtual void setFlexDirection(YGFlexDirection direction) override;

    void measure() override;

    virtual void layout(int l, int t, int r, int b) override;

    YGFlexDirection _direction = YGFlexDirectionColumn;

protected:

    void layoutChildren(int l, int t, int r, int b);

};

}
