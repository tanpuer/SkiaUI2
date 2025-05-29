#pragma once

#include "View.h"

namespace HYSkiaUI {

class LoadingView : public View {

public:

    LoadingView();

    ~LoadingView() override;

    void draw(SkCanvas *canvas) override;

    void setLoadingColor(SkColor color);

    void setLoadingWidth(int width);

    void setSize(int size);

    void setLoadingHeight(int height);

    void setLoadingDuration(int duration);

    const char *name() override;

private:

    int loadingWidth = 10;

    int loadingHeight = 30;

    int size = 5;

    int loadingMargin = 10;

    std::unique_ptr<SkPaint> paint;

    int duration = 800; //ms
};

}
