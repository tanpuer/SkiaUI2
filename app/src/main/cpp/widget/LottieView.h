#pragma once

#include "View.h"
#include <modules/skottie/include/Skottie.h>

using namespace skottie;

class LottieView : public View {

public:

    LottieView();

    ~LottieView();

    void setSource(const char *path);

    void draw(SkCanvas *canvas) override;

    void layout(int l, int t, int r, int b) override;

private:

    sk_sp<Animation> lottieAnimation = nullptr;

    SkRect lottieRect;

    double start = 0.;

};

