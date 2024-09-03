#pragma once

#include "Page.h"
#include "AudioFFTView.h"
#include "ProgressBar.h"
#include "ImageView.h"
#include "LinearAnimator.h"
#include "LyricScrollView.h"

class QQMusicPage : public Page {

public:

    void init(std::shared_ptr<SkiaUIContext> &context, int width, int height) override;

    void drawOnFrame(int drawCount) override;

private:

    float progress = 0.0;

    LyricScrollView *lyricView = nullptr;

    AudioFFTView *fftView = nullptr;

    ProgressBar *progressBar = nullptr;

    ImageView *playImage = nullptr;

    ImageView *armView = nullptr;

    void updateArmView(bool play);

    LinearAnimator *recordOutAnimator = nullptr;

    LinearAnimator *recordInnerAnimator = nullptr;

};
