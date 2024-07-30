#pragma once

#include "View.h"
#include <modules/skottie/include/Skottie.h>

using namespace skottie;

class LottieView : public View {

public:

    LottieView();

    ~LottieView();

    void setSource(const char *path);

    const char* getSource();

    void draw(SkCanvas *canvas) override;

    void layout(int l, int t, int r, int b) override;

    const char *name() override;

    void start();

    void pause();

    void setRepeat(bool repeat);

private:

    sk_sp<Animation> lottieAnimation = nullptr;

    SkRect lottieRect;

    double startTime = 0.;

    bool autoPlay = true;

    bool repeat = true;

    std::string source;

};

