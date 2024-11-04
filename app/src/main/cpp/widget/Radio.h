#pragma once

#include "View.h"

class Radio : public View {

public:

    Radio();

    ~Radio();

    void setOnChangeListener(std::function<void(bool)> &&changeFunc);

    void layout(int l, int t, int r, int b) override;

    void draw(SkCanvas *canvas) override;

    void setSelected(bool selected);

private:

    std::function<void(bool)> changeFunc = nullptr;

    bool selected = false;

    std::unique_ptr<SkPaint> radioPaint = nullptr;

    void setOnClickListener(std::function<void(View *)> clickListener) override;

};
